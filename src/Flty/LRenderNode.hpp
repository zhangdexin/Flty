#ifndef __WIDGET_RENDER_NODE_HPP__
#define __WIDGET_RENDER_NODE_HPP__

#include "Utils.hpp"
#include "Defines.hpp"
#include "LStyleSheet.h"

class LStyleSheet;
class LWidget;
class LRenderNode
{
public:
    LRenderNode() = default;
    LRenderNode(long long id, const LStyleSheet& style) :
        m_Id{ id },
        m_Style{ style },
        m_LayoutChanged{ true },
        m_GraphicChangedByLayout{ false },
        m_GraphicChangedBySet { false }
    {}

    PACK_SET_MEMBER(LRenderNode, m_Style, LStyleSheet, setStyle);
    PACK_SET_MEMBER(LRenderNode, m_Id, long long, setId);
    PACK_SET_MEMBER(LRenderNode, m_LayoutChanged, bool, setLayoutChanged);
    PACK_SET_MEMBER(LRenderNode, m_GraphicChangedByLayout, bool, setGraphicChangedByLayout);
    PACK_SET_MEMBER(LRenderNode, m_GraphicChangedBySet, bool, setGraphicChangedBySet);

    lshared_ptr<LRenderNode>     m_LeftSibling;
    lshared_ptr<LRenderNode>     m_RightSibling;
    lshared_ptr<LRenderNode>     m_Parent;
    lvct_shared_ptr<LRenderNode> m_Children;

    LStyleSheet m_Style;
    long long   m_Id;
    bool        m_LayoutChanged;
    bool        m_GraphicChangedByLayout;
    bool        m_GraphicChangedBySet;
};

#endif
