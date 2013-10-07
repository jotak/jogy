#include "guiFrameFlash.h"
#include "../guiFrame.h"

#define FLASH_SPEED           8.0f       // flashes / sec

// -----------------------------------------------------------------
// Name : guiFrameFlash
//  Constructor
// -----------------------------------------------------------------
guiFrameFlash::guiFrameFlash(u16 uEffectId, float fFlashTime) : guiFrameEffect(uEffectId, EFFECT_REMOVE_ON_FINISHED)
{
    m_fTimer = m_fTotalTime = fFlashTime;
}

// -----------------------------------------------------------------
// Name : ~guiFrameFlash
//  Destructor
// -----------------------------------------------------------------
guiFrameFlash::~guiFrameFlash()
{
}

// -----------------------------------------------------------------
// Name : onBeginDisplay
// -----------------------------------------------------------------
void guiFrameFlash::onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor)
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
void guiFrameFlash::onEndDisplay()
{
}

// -----------------------------------------------------------------
// Name : onUpdate
// -----------------------------------------------------------------
void guiFrameFlash::onUpdate(double delta)
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
guiFrameFlash * guiFrameFlash::clone()
{
    guiFrameFlash * pClone = new guiFrameFlash(m_uEffectId, m_fTotalTime);
    return pClone;
}

// -----------------------------------------------------------------
// Name : reset
// -----------------------------------------------------------------
void guiFrameFlash::reset()
{
    guiFrameEffect::reset();
    m_fTimer = m_fTotalTime;
}
