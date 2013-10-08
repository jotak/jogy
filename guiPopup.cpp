#include "guiPopup.h"
#include "FrameEffects/EffectComeIn.h"
#include "FrameEffects/guiFrameOutro.h"
#include "FrameEffects/guiFrameMouseFocus.h"

// -----------------------------------------------------------------
// Name : guiPopup
//  Constructor ; do not use (private)
// -----------------------------------------------------------------
guiPopup::guiPopup() : guiFrame()
{
    m_fTimer = -1;
    m_bAutoClose = false;
}

// -----------------------------------------------------------------
// Name : ~guiPopup
//  Destructor
// -----------------------------------------------------------------
guiPopup::~guiPopup()
{
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void guiPopup::update(double delta)
{
    if (!m_bVisible) {
        return;
    }
    guiFrame::update(delta);
    if (m_fTimer >= 1)
    {
        m_fTimer -= delta;
        if (m_fTimer < 1)
        {
            guiFrameOutro * pEffect = new guiFrameOutro(POPUP_EFFECT_OUTRO, 1.0f, EFFECT_DELFRM_ON_FINISHED);
            addEffect(pEffect);
            pEffect->setActive(true);
        }
    }
    if (m_bAutoClose)
    {
        guiComponent * pCpnt = getClickedComponent();
        if (pCpnt != NULL)
        {
            if (pCpnt->getId() == "OkButton")
            {
                getDocument()->close();
                m_bAutoClose = false;
            }
        }
    }
}

// -----------------------------------------------------------------
// Name : getButton
// -----------------------------------------------------------------
guiButton * guiPopup::getButton(int iButton)
{
    char str[64] = "0";
    str[0] = iButton - '0';
    return (guiButton*) getDocument()->getComponent(str);
}
