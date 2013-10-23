#ifndef _GUI_FRAME_MOUSE_FOCUS_H
#define _GUI_FRAME_MOUSE_FOCUS_H

#include "FrameEffect.h"

namespace jogy {

class FrameMouseFocus : public FrameEffect
{
public:
    FrameMouseFocus(u16 uEffectId, float fFadeOutTime);
    virtual ~FrameMouseFocus();

    virtual void onBeginDisplay(int iXOffset, int iYOffset, Color * cpntColor, Color * docColor);
    virtual void onEndDisplay();
    virtual void onUpdate(double delta);
    virtual void reset();
    virtual FrameMouseFocus * clone();

protected:
    float m_fTimer, m_fTotalTime;
};
}

#endif
