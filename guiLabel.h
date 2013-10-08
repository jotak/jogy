#ifndef _GUI_LABEL_H
#define _GUI_LABEL_H

#include "guiComponent.h"

class guiLabel : public guiComponent
{
public:
    guiLabel();
    virtual ~guiLabel();

    // Inherited functions
    virtual u32 getType() { return guiComponent::getType() | GOTYPE_LABEL; };
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);

    // Other functions
    fontid getFontId() { return m_FontId; };
    void setFontId(fontid id) { m_FontId = id; };
    int getBoxWidth() { return m_iBoxWidth; };
    void setBoxWidth(int iWidth);
    void setText(string sText);
    string getText() { return m_sText; };
    void setCatchClicks(bool b) { m_bCatchClicks = b; };
    void setComponentOwner(guiComponent * pOwner) { m_pComponentOwner = pOwner; };

    // Builder
    virtual guiLabel * build();
    guiLabel * withText(string sText, fontid fontId, Color textColor) {
    	m_sText = sText; m_FontId = fontId; setDiffuseColor(textColor); return this;
    }

protected:
    void computeGeometry();

    string m_sText;
    fontid m_FontId;
    int m_iBoxWidth;
    bool m_bCatchClicks;
    guiComponent * m_pComponentOwner;
};

#endif
