#include "Component.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : Component
//  Constructor
// -----------------------------------------------------------------
Component::Component() : Object()
{
    m_sCpntId = "";
    m_bEnabled = m_bVisible = true;
    m_uHighlight = HIGHLIGHT_TYPE_NONE;
    m_pOwner = NULL;
}

// -----------------------------------------------------------------
// Name : ~Component
//  Destructor
// -----------------------------------------------------------------
Component::~Component()
{
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
Component * Component::build()
{
    Object::build();
    return this;
}

// -----------------------------------------------------------------
// Name : highlight
// -----------------------------------------------------------------
void Component::highlight(u8 type)
{
    m_uHighlight = type;
}

// -----------------------------------------------------------------
// Name : setVisible
// -----------------------------------------------------------------
void Component::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}
}
