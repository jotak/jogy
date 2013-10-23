#include "FrameIntro.h"
#include "../jogy.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : FrameIntro
//  Constructor
// -----------------------------------------------------------------
FrameIntro::FrameIntro(u16 uEffectId, float fIntroTime) : FrameEffect(uEffectId, EFFECT_ACTIVATE_ON_FINISHED)
{
    m_fTimer = m_fTotalTime = fIntroTime;
}

// -----------------------------------------------------------------
// Name : ~FrameIntro
//  Destructor
// -----------------------------------------------------------------
FrameIntro::~FrameIntro()
{
}

// -----------------------------------------------------------------
// Name : onBeginDisplay
// -----------------------------------------------------------------
void FrameIntro::onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor)
{
    float coef = max(1 - m_fTimer / m_fTotalTime, 0.001f); // must not be 0

    // Scaling
    Vertex fCenter = Jogy::interface->screenTransform(
                           iXOffset + m_pFrame->getXPos() + m_pFrame->getWidth() / 2,
                           iYOffset + m_pFrame->getYPos() + m_pFrame->getHeight() / 2);
    Jogy::interface->pushMatrix();
    Jogy::interface->translate(fCenter.x * (1 - coef), fCenter.y * (1 - coef), 0.0f);
    Jogy::interface->scale(coef, coef, 1.0f);
}

// -----------------------------------------------------------------
// Name : onEndDisplay
// -----------------------------------------------------------------
void FrameIntro::onEndDisplay()
{
	Jogy::interface->popMatrix();
}

// -----------------------------------------------------------------
// Name : onUpdate
// -----------------------------------------------------------------
void FrameIntro::onUpdate(double delta)
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
FrameIntro * FrameIntro::clone()
{
    FrameIntro * pClone = new FrameIntro(m_uEffectId, m_fTotalTime);
    return pClone;
}

// -----------------------------------------------------------------
// Name : reset
// -----------------------------------------------------------------
void FrameIntro::reset()
{
    FrameEffect::reset();
    m_fTimer = m_fTotalTime;
}
}
