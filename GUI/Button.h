#ifndef _GUI_BUTTON_H
#define _GUI_BUTTON_H

#include "Label.h"
#include "../Geometries/IGeometryQuads.h"
#include "../Utils/ButtonAction.h"


namespace jogy {

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

class Button : public Component
{
public:
    // Constructor / destructor
    Button();
    virtual ~Button();

    // Inherited functions
    virtual u32 getType() { return GOTYPE_BUTTON; };
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);
    virtual void moveTo(int xPxl, int yPxl);
    virtual void moveBy(int xPxl, int yPxl);
    virtual void setEnabled(bool bEnabled);

    // Input functions
    virtual Object * onButtonEvent(ButtonAction * pEvent);
    virtual Object * onCursorMoveEvent(int xPxl, int yPxl);
    virtual void onCursorMoveOutEvent();

    // Member access functions
    string getText() { return m_pLabel->getText(); };
    void setText(string sText);
    void setFontId(fontid id) { m_pLabel->setFontId(id); };
    void setTextColor(Color textColor) { m_pLabel->setDiffuseColor(textColor); };
    InputBehaviour getClickedOption() { return m_ClickedOption; };
    InputBehaviour getHoverOption() { return m_HoverOption; };
    void setClickedOption(InputBehaviour clickedOption) { m_ClickedOption = clickedOption; };
    void setHoverOption(InputBehaviour hoverOption) { m_HoverOption = hoverOption; };
    IGeometryQuads * getClickedGeometry() { return m_pGeometryClicked; };
    IGeometryQuads * getHoverGeometry() { return m_pGeometryHover; };
    IGeometryQuads * getBaseGeometry() { return m_pGeometryBase; };
    void setCatchButton2Events(bool bCatch) { m_bCatchButton2Events = bCatch; };
    void setCatchDoubleClicks(bool bCatch) { m_bCatchDoubleClicks = bCatch; };
    void setMultiClicks(bool bMulti) { m_bMultiClicks = bMulti; };
    void setBaseTexture(ITexture * pTex);
    void setClickCallback(ClickCallback clickCallback) { m_ClickCallback = clickCallback; };
    Label * getLabel() { return m_pLabel; };

    bool doClick(ButtonAction * pEvent);

    // Size & position
    virtual void onResize(int iOldWidth, int iOldHeight);
    void autoPad(int margin);
    void autoPadWidth(int margin, int minWidth = 0);

    // Other
    void attachImage(ITexture * pTex, IGeometryQuads * pImageGeo);

    // Builder
    virtual Button * build();
    Button * withInputBehaviour(InputBehaviour clickedBehaviour, InputBehaviour hoverBehaviour) {
    	m_ClickedOption = clickedBehaviour; m_HoverOption = hoverBehaviour; return this;
    }
    Button * withLabel(string sText, fontid fontId, Color textColor, IGeometryText * pLabelGeo);
    Button * withBaseGeometry(ITexture * pTex, IGeometryQuads * pGeo);
    Button * withClickedGeometry(ITexture * pTex, IGeometryQuads * pGeo);
    Button * withHoverGeometry(ITexture * pTex, IGeometryQuads * pGeo);

protected:
    bool m_bMouseDown;
    bool m_bMouseOver;
    Label * m_pLabel;
    InputBehaviour m_ClickedOption;
    InputBehaviour m_HoverOption;
    IGeometryQuads * m_pGeometryClicked;
    IGeometryQuads * m_pGeometryHover;
    IGeometryQuads * m_pGeometryBase;
    IGeometryQuads * m_pGeometryAttachedImage;
    bool m_bClickState;
    bool m_bCatchButton2Events;
    bool m_bCatchDoubleClicks;
    bool m_bMultiClicks;
    float m_fMultiClicksTimer;
    ButtonAction m_MultiClicksEvent;
    ClickCallback m_ClickCallback;
};
}

#endif
