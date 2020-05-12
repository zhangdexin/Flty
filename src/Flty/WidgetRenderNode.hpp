#ifndef __WIDGET_RENDER_NODE_HPP__
#define __WIDGET_RENDER_NODE_HPP__

#include "Utils.hpp"
#include "Defines.hpp"
#include "LStyleSheet.h"

class LStyleSheet;
class LWidget;
struct WidgetRenderNode
{
    WidgetRenderNode() = default;
    WidgetRenderNode(const LWidgetSPtr& widget, bool layoutChanged = false) :
        m_Widget{ widget },
        m_LayoutChanged{ layoutChanged },
        m_GraphicChanged{ false }
    {}

    PACK_SET_MEMBER(WidgetRenderNode, m_Style, LStyleSheet, setStyle);
    PACK_SET_MEMBER(WidgetRenderNode, m_Widget, LWidgetSPtr, setWidget);
    PACK_SET_MEMBER(WidgetRenderNode, m_LayoutChanged, bool, setLayoutChanged);
    PACK_SET_MEMBER(WidgetRenderNode, m_GraphicChanged, bool, setGraphicChanged);

    LStyleSheet m_Style;
    LWidgetSPtr m_Widget;
    bool        m_LayoutChanged;
    bool        m_GraphicChanged;
};

#endif
