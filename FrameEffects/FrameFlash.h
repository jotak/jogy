#ifndef _GUI_FRAME_FLASH_H
#define _GUI_FRAME_FLASH_H

#include "FrameEffect.h"

namespace jogy {

class FrameFlash : public FrameEffect
{
public:
    FrameFlash(u16 uEffectId, float fFlashTime);
    virtual ~FrameFlash();

    virtual void onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor);
    virtual void onEndDisplay();
    virtual void onUpdate(double delta);
    virtual void reset();
    virtual FrameFlash * clone();

protected:
    float m_fTimer, m_fTotalTime;
};
}

#endif
