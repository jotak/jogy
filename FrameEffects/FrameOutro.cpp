#include "FrameOutro.h"
#include "../jogy.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : FrameOutro
//  Constructor
// -----------------------------------------------------------------
FrameOutro::FrameOutro(u16 uEffectId, float fOutroTime, u8 onFinished) : FrameEffect(uEffectId, onFinished)
{
    m_fTimer = m_fTotalTime = fOutroTime;
}

// -----------------------------------------------------------------
// Name : ~FrameOutro
//  Destructor
// -----------------------------------------------------------------
FrameOutro::~FrameOutro()
{
}

// -----------------------------------------------------------------
// Name : onBeginDisplay
// -----------------------------------------------------------------
void FrameOutro::onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor)
{
    float coef = max(m_fTimer / m_fTotalTime, 0.001f); // must not be 0

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
void FrameOutro::onEndDisplay()
{
	Jogy::interface->popMatrix();
}

// -----------------------------------------------------------------
// Name : onUpdate
// -----------------------------------------------------------------
void FrameOutro::onUpdate(double delta)
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
FrameOutro * FrameOutro::clone()
{
    FrameOutro * pClone = new FrameOutro(m_uEffectId, m_fTotalTime);
    return pClone;
}

// -----------------------------------------------------------------
// Name : reset
// -----------------------------------------------------------------
void FrameOutro::reset()
{
    FrameEffect::reset();
    m_fTimer = m_fTotalTime;
    m_pFrame->setEnabled(false);
    m_pFrame->getDocument()->setEnabled(false);
}
}
