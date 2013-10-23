#include "FrameFlash.h"
#include "../GUI/Frame.h"

#define FLASH_SPEED           8.0f       // flashes / sec

namespace jogy {

// -----------------------------------------------------------------
// Name : FrameFlash
//  Constructor
// -----------------------------------------------------------------
FrameFlash::FrameFlash(u16 uEffectId, float fFlashTime) : FrameEffect(uEffectId, EFFECT_REMOVE_ON_FINISHED)
{
    m_fTimer = m_fTotalTime = fFlashTime;
}

// -----------------------------------------------------------------
// Name : ~FrameFlash
//  Destructor
// -----------------------------------------------------------------
FrameFlash::~FrameFlash()
{
}

// -----------------------------------------------------------------
// Name : onBeginDisplay
// -----------------------------------------------------------------
void FrameFlash::onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor)
{
    float timesec = m_fTimer - (int) m_fTimer;
    int i = (int) (FLASH_SPEED * timesec);
    if (i%2)
    {
    	Color c(1.0f, 0.6f, 0.3f, 0.7f);
        cpntColor->multiply(&c);
        docColor->multiply(&c);
    }
}

// -----------------------------------------------------------------
// Name : onEndDisplay
// -----------------------------------------------------------------
void FrameFlash::onEndDisplay()
{
}

// -----------------------------------------------------------------
// Name : onUpdate
// -----------------------------------------------------------------
void FrameFlash::onUpdate(double delta)
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
FrameFlash * FrameFlash::clone()
{
    return new FrameFlash(m_uEffectId, m_fTotalTime);
}

// -----------------------------------------------------------------
// Name : reset
// -----------------------------------------------------------------
void FrameFlash::reset()
{
    FrameEffect::reset();
    m_fTimer = m_fTotalTime;
}
}
