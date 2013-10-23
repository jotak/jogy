#ifndef _GUI_FRAME_INTRO_H
#define _GUI_FRAME_INTRO_H

#include "FrameEffect.h"

namespace jogy {

class FrameIntro : public FrameEffect
{
public:
    FrameIntro(u16 uEffectId, float fIntroTime);
    virtual ~FrameIntro();

    virtual void onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor);
    virtual void onEndDisplay();
    virtual void onUpdate(double delta);
    virtual void reset();
    virtual FrameIntro * clone();

protected:
    float m_fTimer, m_fTotalTime;
};
}

#endif
