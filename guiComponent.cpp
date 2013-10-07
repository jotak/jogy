#include "guiComponent.h"

// -----------------------------------------------------------------
// Name : guiComponent
//  Constructor
// -----------------------------------------------------------------
guiComponent::guiComponent() : guiObject()
{
    m_sCpntId = "";
    m_bEnabled = m_bVisible = true;
    m_uHighlight = HIGHLIGHT_TYPE_NONE;
    m_pOwner = NULL;
}

// -----------------------------------------------------------------
// Name : ~guiComponent
//  Destructor
// -----------------------------------------------------------------
guiComponent::~guiComponent()
{
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void guiComponent::init(string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl, IGeometry * pGeometry)
{
    guiObject::init(xPxl, yPxl, wPxl, hPxl, pGeometry);
    m_sCpntId = sCpntId;
}

// -----------------------------------------------------------------
// Name : highlight
// -----------------------------------------------------------------
void guiComponent::highlight(u8 type)
{
    m_uHighlight = type;
}

// -----------------------------------------------------------------
// Name : setVisible
// -----------------------------------------------------------------
void guiComponent::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}
