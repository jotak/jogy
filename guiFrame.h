#ifndef _GUI_FRAME_H
#define _GUI_FRAME_H

#include "guiContainer.h"
#include "FrameEffects/guiFrameEffect.h"

enum FramePosition
{
    Fixed = 0,
    Floating
};

class guiFrame : public guiContainer
{
public:
    guiFrame();
    virtual ~guiFrame();

    // guiContainer override
    virtual u32 getType() { return guiContainer::getType() | GOTYPE_FRAME; };
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);
    virtual void setEnabled(bool bEnabled);

    // Input override
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);
    virtual guiObject * onCursorMoveEvent(int xPxl, int yPxl);

    // Member access
    FramePosition getPositionType() { return m_PositionType; };
    void setPositionType(FramePosition posType) { m_PositionType = posType; };
    bool isPointed() { return m_bIsPointed; };
    bool isFocused() { return m_bFocused; };

    // Effect
    void addEffect(guiFrameEffect * pEffect);
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
    virtual guiFrame * build(ITexture ** pMainTexs);
    guiFrame * withFramePositionType(FramePosition positionType) {
    	m_PositionType = positionType; return this;
    }

protected:
    FramePosition m_PositionType;
    bool m_bFocused;
    int m_iDragXPos;
    int m_iDragYPos;
    list<guiFrameEffect*> m_pEffects;
    bool m_bIsPointed;
    u8 m_uRetractBorder;
    float m_fRetractTimer;
    s8 m_iRetractState; // -2 => hidden ; -1 => hiding ; 1 => showing ; 2 => shown
    int m_iStickX, m_iStickY;
    bool m_bStickable;
    bool m_bSticked;
    bool m_bMovable;
};

#endif
