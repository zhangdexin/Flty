#include "LLayoutManager.h"

#include "LLayerContext.h"
#include "LRenderNode.hpp"
#include "LStyleSheet.h"
#include "LWidget.h"

LLayoutManager::LLayoutManager(const lshared_ptr<LLayerContext>& layer, WidgetSizeChangedFunc&& f) :
    m_LayerContext{ layer },
    m_sizeChangedCb(std::move(f))
{}

void LLayoutManager::needLayout(const lwidget_sptr& widget)
{
    auto nodePtr = m_LayerContext->node(widget->m_WidgetId);
    if (!nodePtr) {
        return;
    }

    auto &&node = nodePtr->get();
    auto &style = node->m_Style;

    node->setLayoutChanged(true)
        .setGraphicChangedByLayout(true);

    auto &queue = widget->m_StyledChangedQueue;
    while (!queue->empty()) {
        lstyleTask task;
        queue->read(task);
        task(style);
    }
}

void LLayoutManager::layout()
{
    auto& rootNode = m_LayerContext->m_RootNodePtr;
    if (rootNode->m_LayoutChanged) {
        rootNode->m_Style.updateBoundingRect();
        rootNode->m_Style.updateContentRect();
        m_sizeChangedCb(rootNode->m_Style, rootNode->m_Id);
    }

    auto& vct = rootNode->m_Children;
    if (vct.size() > 0) {
        doLayout(rootNode);
    }
}

void LLayoutManager::doLayout(lshared_ptr<LRenderNode>& parent)
{
    const auto&& boxType = parent->m_Style.boxType();
    if (boxType == LBoxType::None) {
        computeNoneBoxStyle(parent);
    }
    else if (boxType == LBoxType::Horizontal) {
        computeHorizontalStyle(parent);
    }
    else if (boxType == LBoxType::Vertical) {
        computeVerticalStyle(parent);
    }
}

void LLayoutManager::computeNoneBoxStyle(lshared_ptr<LRenderNode>& parent)
{
    auto &&rt = parent->m_Style.contentRect();
    auto &children = parent->m_Children;
    for (auto& node : children) {
        updateBounding(rt, node->m_Style);
        node->m_Style.updateContentRect();
        m_sizeChangedCb(node->m_Style, node->m_Id);

        node->setLayoutChanged(false);
        if (node->m_Children.size() > 0) {
            doLayout(node);
        }
    }
}

void LLayoutManager::computeHorizontalStyle(lshared_ptr<LRenderNode>& parent)
{
    auto &&parentRect = parent->m_Style.contentRect();
    int limitWidth = parentRect.width();
    const int limitHeight = parentRect.height();

    auto &children = parent->m_Children;
    lset<lshared_ptr<LRenderNode>> expandingNodes;
    for (auto& node : children) {   // first layout
        node->setLayoutChanged(false);

        // width
        if (limitWidth <= 0) {
            // TODO: warning
            break;
        }

        auto &style = node->m_Style;
        int width = 0;
        if (style.isWidthFixed()) {
            width = std::min(style.width(), limitWidth);
        }
        else if (style.widthPolicy() == LSizePolicy::Hint) {
            width = std::max(style.width(), style.minSize().fWidth);
            width = std::min(width, limitWidth);
        }
        else {
            expandingNodes.insert(node);
        }
        auto mg = style.margin();
        limitWidth -= width + mg.m_Left + mg.m_Right;

        // height
        int height = calcHorizontalBoxHeight(style, limitHeight);

        style.setSize(SkISize::Make(width, height));
        m_sizeChangedCb(style, node->m_Id);
    }

    // adjust expanding
    int expandingNums = expandingNodes.size();
    if (expandingNums > 0) {
        int width = 0;
        for (auto& node : expandingNodes) {
            auto &style = node->m_Style;

            auto mg = style.margin();
            auto mgHor = mg.m_Left + mg.m_Right;

            width = std::min(limitWidth / expandingNums, style.maxSize().fWidth);
            width = std::max(width - mgHor, 0);
            if (width < style.minSize().fWidth) {
                // TODO: warning ignore min val
            }
            style.setSize(SkISize::Make(width, style.height()));
            m_sizeChangedCb(style, node->m_Id);

            limitWidth -= width + mgHor;
            expandingNums--;

            if (limitWidth <= 0) {
                limitWidth = 0;
            }
        }
    }

    // adjust position
    int startOff = 0;
    int adjustWidth = limitWidth / children.size();
    for (auto& node : children) {
        auto &style = node->m_Style;

        int top = 0; 
        auto margin = style.margin();
        auto marginVer = margin.m_Top + margin.m_Bottom;
        if (limitHeight - style.height() >= marginVer) {
            if (margin.m_Bottom > margin.m_Top) {
                top = limitHeight - style.height() - margin.m_Top;
            }
        }
        else {
            top = (limitHeight - style.height()) / 2;
        }

        style.setPos(SkIPoint::Make(startOff + margin.m_Left + adjustWidth / 2, top));
        updateBounding(parentRect, style);
        m_sizeChangedCb(style, node->m_Id);

        doLayout(node);
        startOff += adjustWidth + style.width() + margin.m_Left + margin.m_Right;
    }
}

int LLayoutManager::calcHorizontalBoxHeight(const LStyleSheet& style, int limitHeight)
{
    int height = style.height();
    auto&& heightPolicy = style.heightPolicy();
    if (style.isHeightFixed()) {
        height = std::min(limitHeight, height);
        return height;
    }

    auto verMg = style.margin().m_Top + style.margin().m_Bottom;
    if (heightPolicy == LSizePolicy::Expanding) {
        height = std::min(limitHeight, style.maxSize().height());
    }
    else if (heightPolicy == LSizePolicy::Hint) {
        height = std::max(style.minSize().height(), height);
        height = std::min(limitHeight, height);
    }

    if (limitHeight - height >= verMg) {
        return height;
    }

    if (limitHeight - style.minSize().height() < verMg) {
        height = style.minSize().height();
    }
    else {
        height = limitHeight - verMg;
    }

    return height;
}

void LLayoutManager::computeVerticalStyle(lshared_ptr<LRenderNode>& parent)
{
    auto &&parentRect = parent->m_Style.contentRect();
    const int limitWidth = parentRect.width();
    int limitHeight = parentRect.height();

    auto &children = parent->m_Children;
    lset<lshared_ptr<LRenderNode>> expandingNodes;
    for (auto& node : children) {   // first layout
        node->setLayoutChanged(false);

        // height
        if (limitHeight <= 0) {
            // TODO: warning
            break;
        }

        auto &style = node->m_Style;
        int height = 0;
        if (style.isHeightFixed()) {
            height = std::min(style.height(), limitHeight);
        }
        else if (style.heightPolicy() == LSizePolicy::Hint) {
            height = std::max(style.height(), style.minSize().fHeight);
            height = std::min(height, limitHeight);
        }
        else {
            expandingNodes.insert(node);
        }
        auto mg = style.margin();
        limitHeight -= height + mg.m_Bottom + mg.m_Top;

        // width
        int width = calcVerticalBoxWidth(style, limitWidth);

        style.setSize(SkISize::Make(width, height));
        m_sizeChangedCb(style, node->m_Id);
    }

    // adjust expanding
    int expandingNums = expandingNodes.size();
    if (expandingNums > 0) {
        int height = 0;
        for (auto& node : expandingNodes) {
            auto &style = node->m_Style;

            auto mg = style.margin();
            auto mgVer = mg.m_Bottom + mg.m_Top;

            height = std::min(limitHeight / expandingNums, style.maxSize().fHeight);
            height = std::max(height - mgVer, 0);
            if (height < style.minSize().fHeight) {
                // TODO: warning ignore min val
            }
            style.setSize(SkISize::Make(style.width(), height));
            m_sizeChangedCb(style, node->m_Id);

            limitHeight -= height + mgVer;
            expandingNums--;

            if (limitHeight <= 0) {
                limitHeight = 0;
            }
        }
    }

    // adjust position
    int startOff = 0;
    int adjustHeight = limitHeight / children.size();
    for (auto& node : children) {
        auto &style = node->m_Style;

        int left = 0;
        auto margin = style.margin();
        auto marginHor = margin.m_Left + margin.m_Right;
        if (limitWidth - style.width() >= marginHor) {
            if (margin.m_Right > margin.m_Left) {
                left = limitWidth - style.width() - margin.m_Left;
            }
        }
        else {
            left = (limitWidth - style.width()) / 2;
        }

        style.setPos(SkIPoint::Make((limitWidth - style.width()) / 2, startOff + margin.m_Top + adjustHeight / 2));
        updateBounding(parentRect, style);
        m_sizeChangedCb(style, node->m_Id);

        doLayout(node);
        startOff += adjustHeight + style.height() + margin.m_Top + margin.m_Bottom;
    }
}

int LLayoutManager::calcVerticalBoxWidth(const LStyleSheet& style, int limitWidth)
{
    int width = style.width();
    auto&& widthPolicy = style.widthPolicy();
    if (style.isWidthFixed()) {
        width = std::min(limitWidth, width);
        return width;
    }

    if (widthPolicy == LSizePolicy::Expanding) {
        width = std::min(limitWidth, style.maxSize().fWidth);
    }
    else if (widthPolicy == LSizePolicy::Hint) {
        width = std::max(style.minSize().fWidth, width);
        width = std::min(limitWidth, width);
    }

    auto horMg = style.margin().m_Left + style.margin().m_Right;
    if (limitWidth - width >= horMg) {
        return width;
    }

    if (limitWidth - style.minSize().width() < horMg) {
        width = style.minSize().width();
    }
    else {
        width = limitWidth - horMg;
    }

    return width;
}

void LLayoutManager::updateBounding(const SkIRect& parentBounding, LStyleSheet& style)
{
    SkIPoint offset = SkIPoint::Make(parentBounding.x(), parentBounding.y());
    style.updateBoundingRectByOffset(offset);
}
