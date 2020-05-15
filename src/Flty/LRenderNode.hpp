#ifndef __WIDGET_RENDER_NODE_HPP__
#define __WIDGET_RENDER_NODE_HPP__

#include "Utils.hpp"
#include "Defines.hpp"
#include "LStyleSheet.h"

class LStyleSheet;
class LWidget;
struct LRenderNode
{
    LRenderNode() = default;
    LRenderNode(const LWidgetSPtr& widget, bool layoutChanged = false) :
        m_Widget{ widget },
        m_LayoutChanged{ layoutChanged },
        m_GraphicChangedByLayout{ false },
        m_GraphicChangedBySet { false }
    {}

    PACK_SET_MEMBER(LRenderNode, m_Style, LStyleSheet, setStyle);
    PACK_SET_MEMBER(LRenderNode, m_Widget, LWidgetSPtr, setWidget);
    PACK_SET_MEMBER(LRenderNode, m_LayoutChanged, bool, setLayoutChanged);
    PACK_SET_MEMBER(LRenderNode, m_GraphicChangedByLayout, bool, setGraphicChangedByLayout);
    PACK_SET_MEMBER(LRenderNode, m_GraphicChangedBySet, bool, setGraphicChangedBySet);

    LStyleSheet m_Style;
    LWidgetSPtr m_Widget;
    bool        m_LayoutChanged;
    bool        m_GraphicChangedByLayout;
    bool        m_GraphicChangedBySet;
};

#endif
