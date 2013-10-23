#include "FrameMouseFocus.h"
#include "../GUI/Frame.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : FrameMouseFocus
//  Constructor
// -----------------------------------------------------------------
FrameMouseFocus::FrameMouseFocus(u16 uEffectId, float fFadeOutTime) : FrameEffect(uEffectId)
{
    m_fTimer = m_fTotalTime = fFadeOutTime;
}

// -----------------------------------------------------------------
// Name : ~FrameMouseFocus
//  Destructor
// -----------------------------------------------------------------
FrameMouseFocus::~FrameMouseFocus()
{
}

// -----------------------------------------------------------------
// Name : onBeginDisplay
// -----------------------------------------------------------------
void FrameMouseFocus::onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor)
{
    if (!m_pFrame->isEnabled()) {
        return;
    }
    // Fading
    Color color(1, 1, 1, 0.4f + 0.6f * m_fTimer / m_fTotalTime);
    docColor->multiply(&color);
}

// -----------------------------------------------------------------
// Name : onEndDisplay
// -----------------------------------------------------------------
void FrameMouseFocus::onEndDisplay()
{
}

// -----------------------------------------------------------------
// Name : onUpdate
// -----------------------------------------------------------------
void FrameMouseFocus::onUpdate(double delta)
{
//  if (!m_pFrame->isFocused())
//  {
    if (m_pFrame->isPointed())
    {
        m_fTimer += delta;
        if (m_fTimer > m_fTotalTime)
            m_fTimer = m_fTotalTime;
    }
    else
    {
        m_fTimer -= delta;
        if (m_fTimer < 0)
            m_fTimer = 0;
    }
//  }
}

// -----------------------------------------------------------------
// Name : clone
// -----------------------------------------------------------------
FrameMouseFocus * FrameMouseFocus::clone()
{
    return new FrameMouseFocus(m_uEffectId, m_fTotalTime);
}

// -----------------------------------------------------------------
// Name : reset
// -----------------------------------------------------------------
void FrameMouseFocus::reset()
{
    FrameEffect::reset();
    m_fTimer = m_fTotalTime;
}
}
