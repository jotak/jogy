#include "ToggleButton.h"
#include "../ComponentOwnerInterface.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : ToggleButton
//  Constructor
// -----------------------------------------------------------------
ToggleButton::ToggleButton() : Button()
{
}

// -----------------------------------------------------------------
// Name : ~ToggleButton
//  Destructor
// -----------------------------------------------------------------
ToggleButton::~ToggleButton()
{
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
Object * ToggleButton::onButtonEvent(ButtonAction * pEvent)
{
    if ((pEvent->eButton != Button1 && (pEvent->eButton != Button2 || !m_bCatchButton2Events)) || !m_bEnabled || !m_bVisible) {
        return NULL;
    }
    bool bClickState = m_bClickState;
    bool bMouseOver = m_bMouseOver;
    Object * pObj = (pEvent->eEvent != EventUp) ? Button::onButtonEvent(pEvent) : this;
    if (pEvent->eEvent == EventDown)
    {
        m_bClickState = !bClickState;
        return this;
    }
    else if (pEvent->eEvent == EventDrag)
    {
        m_bClickState = m_bMouseOver == bMouseOver ? bClickState : !bClickState;
        return this;
    }
    else if (pEvent->eEvent == EventUp)
    {
        m_bMouseDown = false;
        if (m_bMouseOver && m_pOwner != NULL)
            m_pOwner->onButtonEvent(pEvent, this);
        return this;
    }
    return pObj;
}
}
