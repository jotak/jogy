#include "guiToggleButton.h"
#include "ComponentOwnerInterface.h"

// -----------------------------------------------------------------
// Name : guiToggleButton
//  Constructor
// -----------------------------------------------------------------
guiToggleButton::guiToggleButton() : guiButton()
{
}

// -----------------------------------------------------------------
// Name : ~guiToggleButton
//  Destructor
// -----------------------------------------------------------------
guiToggleButton::~guiToggleButton()
{
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiToggleButton::onButtonEvent(ButtonAction * pEvent)
{
    if ((pEvent->eButton != Button1 && (pEvent->eButton != Button2 || !m_bCatchButton2Events)) || !m_bEnabled || !m_bVisible) {
        return NULL;
    }
    bool bClickState = m_bClickState;
    bool bMouseOver = m_bMouseOver;
    guiObject * pObj = (pEvent->eEvent != Event_Up) ? guiButton::onButtonEvent(pEvent) : this;
    if (pEvent->eEvent == Event_Down)
    {
        m_bClickState = !bClickState;
        return this;
    }
    else if (pEvent->eEvent == Event_Drag)
    {
        m_bClickState = m_bMouseOver == bMouseOver ? bClickState : !bClickState;
        return this;
    }
    else if (pEvent->eEvent == Event_Up)
    {
        m_bMouseDown = false;
        if (m_bMouseOver && m_pOwner != NULL)
            m_pOwner->onButtonEvent(pEvent, this);
        return this;
    }
    return pObj;
}
