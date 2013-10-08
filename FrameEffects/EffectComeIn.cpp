#include "EffectComeIn.h"
#include "../guiFrame.h"
#include "../jogy.h"

// -----------------------------------------------------------------
// Name : EffectComeIn
//  Constructor
// -----------------------------------------------------------------
EffectComeIn::EffectComeIn(u16 uEffectId, float fDuration) : guiFrameEffect(uEffectId, EFFECT_ACTIVATE_ON_FINISHED)
{
    m_fTimer = m_fTotalTime = fDuration;
}

// -----------------------------------------------------------------
// Name : ~EffectComeIn
//  Destructor
// -----------------------------------------------------------------
EffectComeIn::~EffectComeIn()
{
}

// -----------------------------------------------------------------
// Name : onBeginDisplay
// -----------------------------------------------------------------
void EffectComeIn::onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor)
{
    float coef = m_fTimer / m_fTotalTime;
    int yPxl = coef * (iYOffset + m_pFrame->getYPos() + m_pFrame->getHeight());

    // Translate
    Jogy::interface->pushMatrix();
    Jogy::interface->translate(0, -yPxl, 0.0f);

    // Fading
    Color color(1, 1, 1, coef);
    cpntColor->multiply(&color);
    docColor->multiply(&color);
}

// -----------------------------------------------------------------
// Name : onEndDisplay
// -----------------------------------------------------------------
void EffectComeIn::onEndDisplay()
{
    Jogy::interface->popMatrix();
}

// -----------------------------------------------------------------
// Name : onUpdate
// -----------------------------------------------------------------
void EffectComeIn::onUpdate(double delta)
{
    m_fTimer -= delta;
    if (m_fTimer <= 0)
    {
        m_fTimer = 0;
        m_bActive = false;
        m_bFinished = true;
    }
}

// -----------------------------------------------------------------
// Name : clone
// -----------------------------------------------------------------
EffectComeIn * EffectComeIn::clone()
{
    return new EffectComeIn(m_uEffectId, m_fTotalTime);
}

// -----------------------------------------------------------------
// Name : reset
// -----------------------------------------------------------------
void EffectComeIn::reset()
{
    guiFrameEffect::reset();
    m_fTimer = m_fTotalTime;
}
