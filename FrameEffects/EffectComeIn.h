#ifndef _EFFECT_COMEIN_H
#define _EFFECT_COMEIN_H

#include "FrameEffect.h"

namespace jogy {

class EffectComeIn : public FrameEffect
{
public:
    EffectComeIn(u16 uEffectId, float fDuration);
    virtual ~EffectComeIn();

    virtual void onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor);
    virtual void onEndDisplay();
    virtual void onUpdate(double delta);
    virtual void reset();
    virtual EffectComeIn * clone();

protected:
    float m_fTimer, m_fTotalTime;
};
}

#endif
