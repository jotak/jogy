#include "Popup.h"
#include "../FrameEffects/EffectComeIn.h"
#include "../FrameEffects/FrameOutro.h"
#include "../FrameEffects/FrameMouseFocus.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : Popup
//  Constructor ; do not use (private)
// -----------------------------------------------------------------
Popup::Popup() : Frame()
{
    m_fTimer = -1;
    m_bAutoClose = false;
}

// -----------------------------------------------------------------
// Name : ~Popup
//  Destructor
// -----------------------------------------------------------------
Popup::~Popup()
{
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void Popup::update(double delta)
{
    if (!m_bVisible) {
        return;
    }
    Frame::update(delta);
    if (m_fTimer >= 1)
    {
        m_fTimer -= delta;
        if (m_fTimer < 1)
        {
            FrameOutro * pEffect = new FrameOutro(POPUP_EFFECT_OUTRO, 1.0f, EFFECT_DELFRM_ON_FINISHED);
            addEffect(pEffect);
            pEffect->setActive(true);
        }
    }
    if (m_bAutoClose)
    {
        Component * pCpnt = getClickedComponent();
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
Button * Popup::getButton(int iButton)
{
    char str[64] = "0";
    str[0] = iButton - '0';
    return (Button*) getDocument()->getComponent(str);
}
}
