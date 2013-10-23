#ifndef _GUI_FRAME_H
#define _GUI_FRAME_H

#include "Container.h"
#include "../FrameEffects/FrameEffect.h"


namespace jogy {

enum FramePosition
{
    Fixed = 0,
    Floating
};

class Frame : public Container
{
public:
    Frame();
    virtual ~Frame();

    // Container override
    virtual u32 getType() { return Container::getType() | GOTYPE_FRAME; };
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);
    virtual void setEnabled(bool bEnabled);

    // Input override
    virtual Object * onButtonEvent(ButtonAction * pEvent);
    virtual Object * onCursorMoveEvent(int xPxl, int yPxl);

    // Member access
    FramePosition getPositionType() { return m_PositionType; };
    void setPositionType(FramePosition posType) { m_PositionType = posType; };
    bool isPointed() { return m_bIsPointed; };
    bool isFocused() { return m_bFocused; };

    // Effect
    void addEffect(FrameEffect * pEffect);
    void activateEffect(u16 uEffectId);
    void removeEffect(u16 uEffectId);

    // Other
    void checkPositionIfDragged();
    virtual void show();
    virtual void hide();
    void flash(float fFlashTime);
    void setRetractible(u8 uBorder);
    void extract();
    bool isSticked() { return m_bSticked; };
    void setMovable(bool bMovable) { m_bMovable = bMovable; };
    void centerOnScreen();

    // Builder
    virtual Frame * build();
    Frame * withFramePositionType(FramePosition positionType) {
    	m_PositionType = positionType; return this;
    }

protected:
    FramePosition m_PositionType;
    bool m_bFocused;
    int m_iDragXPos;
    int m_iDragYPos;
    list<FrameEffect*> m_pEffects;
    bool m_bIsPointed;
    u8 m_uRetractBorder;
    float m_fRetractTimer;
    s8 m_iRetractState; // -2 => hidden ; -1 => hiding ; 1 => showing ; 2 => shown
    int m_iStickX, m_iStickY;
    bool m_bStickable;
    bool m_bSticked;
    bool m_bMovable;
};
}

#endif
