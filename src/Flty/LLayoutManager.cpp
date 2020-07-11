#include "LLayoutManager.h"

#include "LLayerContext.h"
#include "LRenderNode.hpp"
#include "LStyleSheet.h"
#include "LWidget.h"

LLayoutManager::LLayoutManager(const lshared_ptr<LLayerContext>& layer) :
    m_LayerContext{ layer }
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
    auto &&rt = parent->m_Style.boundingRect();
    auto &children = parent->m_Children;
    for (auto& node : children) {
        updateBounding(rt, node->m_Style);
        node->setLayoutChanged(false);

        if (node->m_Children.size() > 0) {
            doLayout(node);
        }
    }
}

void LLayoutManager::computeHorizontalStyle(lshared_ptr<LRenderNode>& parent)
{
    auto &parentStyle = parent->m_Style;
    int limitWidth = parentStyle.width();
    const int limitHeight = parentStyle.height();

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

        // height
        int height = style.height();
        auto&& heightPolicy = style.heightPolicy();
        if (style.isHeightFixed()) {
            height = std::min(limitHeight, height);
        }
        else if (heightPolicy == LSizePolicy::Expanding) {
            height = std::min(limitHeight, style.maxSize().height());
        }
        else if (heightPolicy == LSizePolicy::Hint) {
            height = std::max(style.minSize().height(), height);
            height = std::min(limitHeight, width);
        }

        style.setSize(SkISize::Make(width, height));
        limitWidth -= width;
    }

    // adjust expanding
    int expandingNums = expandingNodes.size();
    if (expandingNums > 0) {
        int width = 0;
        for (auto& node : expandingNodes) {
            auto &style = node->m_Style;

            width = std::min(limitWidth / expandingNums, style.maxSize().fWidth);
            if (width < style.minSize().fWidth) {
                // TODO: warning ignore min val
            }
            style.setSize(SkISize::Make(width, style.height()));

            limitWidth -= width;
            expandingNums--;

            if (limitWidth <= 0) {
                limitWidth = 0;
            }
        }
    }

    // adjust position
    int startOff = 0;
    auto &&rt = parentStyle.boundingRect();
    int adjustWidth = limitWidth / children.size();
    for (auto& node : children) {
        auto &style = node->m_Style;
        style.setPos(SkIPoint::Make(startOff + adjustWidth / 2, (limitHeight - style.height()) / 2));
        updateBounding(rt, style);

        doLayout(node);
        startOff += adjustWidth + style.width();
    }
}

void LLayoutManager::computeVerticalStyle(lshared_ptr<LRenderNode>& parent)
{
    auto &parentStyle = parent->m_Style;
    const int limitWidth = parentStyle.width();
    int limitHeight = parentStyle.height();

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

        // width
        int width = style.width();
        auto&& widthPolicy = style.widthPolicy();
        if (style.isWidthFixed()) {
            width = std::min(limitWidth, width);
        }
        else if (widthPolicy == LSizePolicy::Expanding) {
            width = std::min(limitWidth, style.maxSize().fWidth);
        }
        else if (widthPolicy == LSizePolicy::Hint) {
            width = std::max(style.minSize().fWidth, width);
            width = std::min(limitWidth, width);
        }

        style.setSize(SkISize::Make(width, height));
        limitHeight -= height;
    }

    // adjust expanding
    int expandingNums = expandingNodes.size();
    if (expandingNums > 0) {
        int height = 0;
        for (auto& node : expandingNodes) {
            auto &style = node->m_Style;

            height = std::min(limitHeight / expandingNums, style.maxSize().fHeight);
            if (height < style.minSize().fHeight) {
                // TODO: warning ignore min val
            }
            style.setSize(SkISize::Make(style.width(), height));

            limitHeight -= height;
            expandingNums--;

            if (limitHeight <= 0) {
                limitHeight = 0;
            }
        }
    }

    // adjust position
    int startOff = 0;
    auto &&rt = parentStyle.boundingRect();
    int adjustHeight = limitHeight / children.size();
    for (auto& node : children) {
        auto &style = node->m_Style;
        style.setPos(SkIPoint::Make((limitWidth - style.width()) / 2, startOff + adjustHeight / 2));
        updateBounding(rt, style);

        doLayout(node);
        startOff += adjustHeight + style.height();
    }
}

void LLayoutManager::updateBounding(const SkIRect& parentBounding, LStyleSheet& style)
{
    SkIPoint offset = SkIPoint::Make(parentBounding.x(), parentBounding.y());
    style.updateBoundingRectByOffset(offset);
}