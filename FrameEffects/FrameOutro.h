#ifndef _GUI_FRAME_OUTRO_H
#define _GUI_FRAME_OUTRO_H

#include "FrameEffect.h"

namespace jogy {

class FrameOutro : public FrameEffect
{
public:
    FrameOutro(u16 uEffectId, float fOutroTime, u8 onFinished = EFFECT_HIDE_ON_FINISHED);
    virtual ~FrameOutro();

    virtual void onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor);
    virtual void onEndDisplay();
    virtual void onUpdate(double delta);
    virtual void reset();
    virtual FrameOutro * clone();

protected:
    float m_fTimer, m_fTotalTime;
};
}

#endif
