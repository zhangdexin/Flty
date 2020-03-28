/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/gpu/GrTextureProducer.h"

#include "include/private/GrRecordingContext.h"
#include "src/core/SkMipMap.h"
#include "src/core/SkRectPriv.h"
#include "src/gpu/GrClip.h"
#include "src/gpu/GrContextPriv.h"
#include "src/gpu/GrProxyProvider.h"
#include "src/gpu/GrRecordingContextPriv.h"
#include "src/gpu/GrRenderTargetContext.h"
#include "src/gpu/GrTextureProxy.h"
#include "src/gpu/SkGr.h"
#include "src/gpu/effects/GrBicubicEffect.h"
#include "src/gpu/effects/GrTextureDomain.h"
#include "src/gpu/effects/GrTextureEffect.h"

sk_sp<GrTextureProxy> GrTextureProducer::CopyOnGpu(GrRecordingContext* context,
                                                   sk_sp<GrTextureProxy> inputProxy,
                                                   GrColorType colorType,
                                                   const CopyParams& copyParams,
                                                   bool dstWillRequireMipMaps) {
    SkASSERT(context);

    const SkRect dstRect = SkRect::Make(copyParams.fDimensions);
    GrMipMapped mipMapped = dstWillRequireMipMaps ? GrMipMapped::kYes : GrMipMapped::kNo;

    SkRect localRect = inputProxy->getBoundsRect();

    bool resizing = false;
    if (copyParams.fFilter != GrSamplerState::Filter::kNearest) {
        resizing = localRect.width() != dstRect.width() || localRect.height() != dstRect.height();
    }

    if (copyParams.fFilter == GrSamplerState::Filter::kNearest && !resizing &&
        dstWillRequireMipMaps) {
        sk_sp<GrTextureProxy> proxy = GrCopyBaseMipMapToTextureProxy(context, inputProxy.get(),
                                                                     inputProxy->origin(),
                                                                     colorType);
        if (proxy) {
            return proxy;
        }
    }

    auto copyRTC = GrRenderTargetContext::MakeWithFallback(
            context, colorType, nullptr, SkBackingFit::kExact, copyParams.fDimensions, 1,
            mipMapped, inputProxy->isProtected(), inputProxy->origin());
    if (!copyRTC) {
        return nullptr;
    }

    const auto& caps = *context->priv().caps();
    GrPaint paint;

    GrSamplerState sampler(GrSamplerState::WrapMode::kClamp, copyParams.fFilter);
    auto boundsRect = SkIRect::MakeSize(inputProxy->dimensions());
    auto fp = GrTextureEffect::MakeTexelSubset(std::move(inputProxy), kUnknown_SkAlphaType,
                                               SkMatrix::I(), sampler, boundsRect, localRect, caps);
    paint.addColorFragmentProcessor(std::move(fp));
    paint.setPorterDuffXPFactory(SkBlendMode::kSrc);

    copyRTC->fillRectToRect(GrNoClip(), std::move(paint), GrAA::kNo, SkMatrix::I(), dstRect,
                            localRect);
    return copyRTC->asTextureProxyRef();
}

/** Determines whether a texture domain is necessary and if so what domain to use. There are two
 *  rectangles to consider:
 *  - The first is the content area specified by the texture adjuster (i.e., textureContentArea).
 *    We can *never* allow filtering to cause bleed of pixels outside this rectangle.
 *  - The second rectangle is the constraint rectangle (i.e., constraintRect), which is known to
 *    be contained by the content area. The filterConstraint specifies whether we are allowed to
 *    bleed across this rect.
 *
 *  We want to avoid using a domain if possible. We consider the above rectangles, the filter type,
 *  and whether the coords generated by the draw would all fall within the constraint rect. If the
 *  latter is true we only need to consider whether the filter would extend beyond the rects.
 */
GrTextureProducer::DomainMode GrTextureProducer::DetermineDomainMode(
        const SkRect& constraintRect,
        FilterConstraint filterConstraint,
        bool coordsLimitedToConstraintRect,
        GrTextureProxy* proxy,
        const GrSamplerState::Filter* filterModeOrNullForBicubic,
        SkRect* domainRect) {
    const SkIRect proxyBounds = SkIRect::MakeSize(proxy->dimensions());

    SkASSERT(proxyBounds.contains(constraintRect));

    const bool proxyIsExact = proxy->isFunctionallyExact();

    // If the constraint rectangle contains the whole proxy then no need for a domain.
    if (constraintRect.contains(proxyBounds) && proxyIsExact) {
        return kNoDomain_DomainMode;
    }

    bool restrictFilterToRect = (filterConstraint == GrTextureProducer::kYes_FilterConstraint);

    // If we can filter outside the constraint rect, and there is no non-content area of the
    // proxy, and we aren't going to generate sample coords outside the constraint rect then we
    // don't need a domain.
    if (!restrictFilterToRect && proxyIsExact && coordsLimitedToConstraintRect) {
        return kNoDomain_DomainMode;
    }

    // Get the domain inset based on sampling mode (or bail if mipped)
    SkScalar filterHalfWidth = 0.f;
    if (filterModeOrNullForBicubic) {
        switch (*filterModeOrNullForBicubic) {
            case GrSamplerState::Filter::kNearest:
                if (coordsLimitedToConstraintRect) {
                    return kNoDomain_DomainMode;
                } else {
                    filterHalfWidth = 0.f;
                }
                break;
            case GrSamplerState::Filter::kBilerp:
                filterHalfWidth = .5f;
                break;
            case GrSamplerState::Filter::kMipMap:
                if (restrictFilterToRect || !proxyIsExact) {
                    // No domain can save us here.
                    return kTightCopy_DomainMode;
                }
                return kNoDomain_DomainMode;
        }
    } else {
        // bicubic does nearest filtering internally.
        filterHalfWidth = 1.5f;
    }

    // Both bilerp and bicubic use bilinear filtering and so need to be clamped to the center
    // of the edge texel. Pinning to the texel center has no impact on nearest mode and MIP-maps

    static const SkScalar kDomainInset = 0.5f;
    // Figure out the limits of pixels we're allowed to sample from.
    // Unless we know the amount of outset and the texture matrix we have to conservatively enforce
    // the domain.
    if (restrictFilterToRect) {
        *domainRect = constraintRect.makeInset(kDomainInset, kDomainInset);
    } else if (!proxyIsExact) {
        // If we got here then: proxy is not exact, the coords are limited to the
        // constraint rect, and we're allowed to filter across the constraint rect boundary. So
        // we check whether the filter would reach across the edge of the proxy.
        // We will only set the sides that are required.

        *domainRect = SkRectPriv::MakeLargest();
        if (coordsLimitedToConstraintRect) {
            // We may be able to use the fact that the texture coords are limited to the constraint
            // rect in order to avoid having to add a domain.
            bool needContentAreaConstraint = false;
            if (proxyBounds.fRight - filterHalfWidth < constraintRect.fRight) {
                domainRect->fRight = proxyBounds.fRight - kDomainInset;
                needContentAreaConstraint = true;
            }
            if (proxyBounds.fBottom - filterHalfWidth < constraintRect.fBottom) {
                domainRect->fBottom = proxyBounds.fBottom - kDomainInset;
                needContentAreaConstraint = true;
            }
            if (!needContentAreaConstraint) {
                return kNoDomain_DomainMode;
            }
        } else {
            // Our sample coords for the texture are allowed to be outside the constraintRect so we
            // don't consider it when computing the domain.
            domainRect->fRight = proxyBounds.fRight - kDomainInset;
            domainRect->fBottom = proxyBounds.fBottom - kDomainInset;
        }
    } else {
        return kNoDomain_DomainMode;
    }

    if (domainRect->fLeft > domainRect->fRight) {
        domainRect->fLeft = domainRect->fRight = SkScalarAve(domainRect->fLeft, domainRect->fRight);
    }
    if (domainRect->fTop > domainRect->fBottom) {
        domainRect->fTop = domainRect->fBottom = SkScalarAve(domainRect->fTop, domainRect->fBottom);
    }
    return kDomain_DomainMode;
}

std::unique_ptr<GrFragmentProcessor> GrTextureProducer::createFragmentProcessorForDomainAndFilter(
        sk_sp<GrTextureProxy> proxy,
        const SkMatrix& textureMatrix,
        DomainMode domainMode,
        const SkRect& domain,
        const GrSamplerState::Filter* filterOrNullForBicubic) {
    SkASSERT(kTightCopy_DomainMode != domainMode);
    const auto& caps = *fContext->priv().caps();
    SkAlphaType srcAlphaType = this->alphaType();
    if (filterOrNullForBicubic) {
        GrSamplerState::WrapMode wrapMode = fDomainNeedsDecal
                                                    ? GrSamplerState::WrapMode::kClampToBorder
                                                    : GrSamplerState::WrapMode::kClamp;
        GrSamplerState samplerState(wrapMode, *filterOrNullForBicubic);
        if (kNoDomain_DomainMode == domainMode) {
            return GrTextureEffect::Make(std::move(proxy), srcAlphaType, textureMatrix,
                                         samplerState, caps);
        }
        return GrTextureEffect::MakeSubset(std::move(proxy), srcAlphaType, textureMatrix,
                                           samplerState, domain, caps);
    } else {
        static const GrSamplerState::WrapMode kClampClamp[] = {
                GrSamplerState::WrapMode::kClamp, GrSamplerState::WrapMode::kClamp};
        static const GrSamplerState::WrapMode kDecalDecal[] = {
                GrSamplerState::WrapMode::kClampToBorder, GrSamplerState::WrapMode::kClampToBorder};

        static constexpr auto kDir = GrBicubicEffect::Direction::kXY;
        bool clampToBorderSupport = caps.clampToBorderSupport();
        if (kDomain_DomainMode == domainMode || (fDomainNeedsDecal && !clampToBorderSupport)) {
            GrTextureDomain::Mode wrapMode = fDomainNeedsDecal ? GrTextureDomain::kDecal_Mode
                                         : GrTextureDomain::kClamp_Mode;
            return GrBicubicEffect::Make(std::move(proxy), textureMatrix, kClampClamp, wrapMode,
                                         wrapMode, kDir, srcAlphaType,
                                         kDomain_DomainMode == domainMode ? &domain : nullptr);
        } else {
            return GrBicubicEffect::Make(std::move(proxy), textureMatrix,
                                         fDomainNeedsDecal ? kDecalDecal : kClampClamp, kDir,
                                         srcAlphaType);
        }
    }
}

sk_sp<GrTextureProxy> GrTextureProducer::refTextureProxyForParams(
        const GrSamplerState::Filter* filterOrNullForBicubic,
        SkScalar scaleAdjust[2]) {
    GrSamplerState sampler; // Default is nearest + clamp
    if (filterOrNullForBicubic) {
        sampler.setFilterMode(*filterOrNullForBicubic);
    }
    if (fDomainNeedsDecal) {
        // Assuming hardware support, switch to clamp-to-border instead of clamp
        if (fContext->priv().caps()->clampToBorderSupport()) {
            sampler.setWrapModeX(GrSamplerState::WrapMode::kClampToBorder);
            sampler.setWrapModeY(GrSamplerState::WrapMode::kClampToBorder);
        }
    }
    return this->refTextureProxyForParams(sampler, scaleAdjust);
}

sk_sp<GrTextureProxy> GrTextureProducer::refTextureProxyForParams(GrSamplerState sampler,
                                                                  SkScalar scaleAdjust[2]) {
    // Check that the caller pre-initialized scaleAdjust
    SkASSERT(!scaleAdjust || (scaleAdjust[0] == 1 && scaleAdjust[1] == 1));

    const GrCaps* caps = this->context()->priv().caps();

    int mipCount = SkMipMap::ComputeLevelCount(this->width(), this->height());
    bool willBeMipped = GrSamplerState::Filter::kMipMap == sampler.filter() && mipCount &&
                        caps->mipMapSupport();

    auto result = this->onRefTextureProxyForParams(sampler, willBeMipped, scaleAdjust);

    // Check to make sure that if we say the texture willBeMipped that the returned texture has mip
    // maps, unless the config is not copyable.
    SkASSERT(!result || !willBeMipped || result->mipMapped() == GrMipMapped::kYes ||
             !caps->isFormatCopyable(result->backendFormat()));

    SkDEBUGCODE(bool expectNoScale = (sampler.filter() != GrSamplerState::Filter::kMipMap &&
                                      !sampler.isRepeated()));
    // Check that the "no scaling expected" case always returns a proxy of the same size as the
    // producer.
    SkASSERT(!result || !expectNoScale || result->dimensions() == this->dimensions());

    return result;
}

std::pair<sk_sp<GrTextureProxy>, GrColorType> GrTextureProducer::refTextureProxy(
        GrMipMapped willNeedMips) {
    GrSamplerState::Filter filter =
            GrMipMapped::kNo == willNeedMips ? GrSamplerState::Filter::kNearest
                                             : GrSamplerState::Filter::kMipMap;
    GrSamplerState sampler(GrSamplerState::WrapMode::kClamp, filter);

    auto result = this->refTextureProxyForParams(sampler, nullptr);

#ifdef SK_DEBUG
    const GrCaps* caps = this->context()->priv().caps();
    // Check that the resulting proxy format is compatible with the GrColorType of this producer
    SkASSERT(!result ||
             result->isFormatCompressed(caps) ||
             caps->areColorTypeAndFormatCompatible(this->colorType(), result->backendFormat()));
#endif

    return {result, this->colorType()};
}
