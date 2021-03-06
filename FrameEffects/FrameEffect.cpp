#include "FrameEffect.h"
#include "../GUI/Frame.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : FrameEffect
//  Constructor
// -----------------------------------------------------------------
FrameEffect::FrameEffect(u16 uEffectId, u8 onFinished)
{
    m_uEffectId = uEffectId;
    m_bActive = false;
    m_pFrame = NULL;
    m_bFinished = false;
    m_uActionOnFinished = onFinished;
}

// -----------------------------------------------------------------
// Name : ~FrameEffect
//  Destructor
// -----------------------------------------------------------------
FrameEffect::~FrameEffect()
{
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void FrameEffect::update(double delta)
{
    if (m_bActive && !m_bFinished)
    {
        onUpdate(delta);
        if (m_bFinished)
        {
            switch (m_uActionOnFinished)
            {
            case EFFECT_ACTIVATE_ON_FINISHED:
                m_pFrame->setEnabled(true);
                m_pFrame->getDocument()->setEnabled(true);
                break;
            case EFFECT_HIDE_ON_FINISHED:
                m_pFrame->setVisible(false);
                break;
            case EFFECT_DELFRM_ON_FINISHED:
                m_pFrame->getDocument()->close();
                break;
            }
        }
    }
}

// -----------------------------------------------------------------
// Name : setActive
// -----------------------------------------------------------------
void FrameEffect::setActive(bool bActive)
{
    m_bActive = bActive;
}

// -----------------------------------------------------------------
// Name : reset
// -----------------------------------------------------------------
void FrameEffect::reset()
{
    if (m_uActionOnFinished == EFFECT_ACTIVATE_ON_FINISHED)
    {
        m_pFrame->setEnabled(false);
        m_pFrame->getDocument()->setEnabled(false);
    }
    m_bActive = true;
    m_bFinished = false;
}
}
