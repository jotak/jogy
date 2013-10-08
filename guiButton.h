#ifndef _GUI_BUTTON_H
#define _GUI_BUTTON_H

#include "guiImage.h"
#include "guiLabel.h"
#include "Geometries/IGeometryQuads.h"
#include "Utils/ButtonAction.h"

enum InputBehaviour
{
    None = 0,
    ReplaceTex,
    AddTex,
    Decal,
    Scale,
    Enlight
};

// must return false if resulting in a closing event
typedef tr1::function<bool (ComponentOwnerInterface*)> ClickCallback;

class guiButton : public guiImage
{
public:
    // Constructor / destructor
    guiButton();
    virtual ~guiButton();

    // Inherited functions
    virtual u32 getType() { return guiComponent::getType() | GOTYPE_BUTTON; };
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);
    virtual void moveTo(int xPxl, int yPxl);
    virtual void moveBy(int xPxl, int yPxl);
    virtual void setEnabled(bool bEnabled);

    // Input functions
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);
    virtual guiObject * onCursorMoveEvent(int xPxl, int yPxl);
    virtual void onCursorMoveOutEvent();

    // Member access functions
    string getText() { return m_pLabel->getText(); };
    void setText(string sText);
    void setFontId(fontid id) { m_pLabel->setFontId(id); };
    void setTextColor(Color textColor) { m_pLabel->setDiffuseColor(textColor); };
    InputBehaviour getClickOption() { return m_ClickOption; };
    InputBehaviour getOverOption() { return m_OverOption; };
    void setClickOption(InputBehaviour clickOption) { m_ClickOption = clickOption; };
    void setOverOption(InputBehaviour overOption) { m_OverOption = overOption; };
    IGeometryQuads * getClickedGeometry() { return m_pGeometryClicked; };
    IGeometryQuads * getOverGeometry() { return m_pGeometryOver; };
    IGeometryQuads * getNormalGeometry() { return m_pGeometryNormal; };
    void setCatchButton2Events(bool bCatch) { m_bCatchButton2Events = bCatch; };
    void setCatchDoubleClicks(bool bCatch) { m_bCatchDoubleClicks = bCatch; };
    void setMultiClicks(bool bMulti) { m_bMultiClicks = bMulti; };
    void setNormalTexture(ITexture * pTex);
    void setClickCallback(ClickCallback clickCallback) { m_ClickCallback = clickCallback; };
    bool doClick(ButtonAction * pEvent);

    // Size & position
    virtual void onResize(int iOldWidth, int iOldHeight);
    void autoPad(int margin);
    void autoPadWidth(int margin, int minWidth = 0);

    // Other
    void attachImage(ITexture * pTex, IGeometryQuads * pImageGeo);

    // Builder
    virtual guiButton * build(ITexture * pTex);
    guiButton * withInputBehaviour(InputBehaviour clickBehaviour, InputBehaviour overBehaviour) {
    	m_ClickOption = clickBehaviour; m_OverOption = overBehaviour; return this;
    }
    guiButton * withLabel(string sText, fontid fontId, Color textColor, IGeometryQuads * pLabelGeo);
    guiButton * withClickTexture(ITexture * pTex, IGeometryQuads * pGeo);
    guiButton * withOverTexture(ITexture * pTex, IGeometryQuads * pGeo);

protected:
    bool m_bMouseDown;
    bool m_bMouseOver;
    guiLabel * m_pLabel;
    InputBehaviour m_ClickOption;
    InputBehaviour m_OverOption;
    IGeometryQuads * m_pGeometryClicked;
    IGeometryQuads * m_pGeometryOver;
    IGeometryQuads * m_pGeometryNormal;
    IGeometryQuads * m_pGeometryAttachedImage;
    bool m_bClickState;
    bool m_bCatchButton2Events;
    bool m_bCatchDoubleClicks;
    bool m_bMultiClicks;
    float m_fMultiClicksTimer;
    ButtonAction m_MultiClicksEvent;
    ClickCallback m_ClickCallback;
};

#endif
