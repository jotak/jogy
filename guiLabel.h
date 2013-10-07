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

    // Clone / init
    virtual void init(string sText, fontid fontId, Color textColor, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl, IGeometry * pGeometry);

    // static builders
    static guiLabel * createDefaultLabel(string sText, string sId);
    static guiLabel * createDefaultLabel(string sText, string sId, int xPxl, int yPxl);

protected:
    void computeGeometry();

    string m_sText;
    fontid m_FontId;
    int m_iBoxWidth;
    bool m_bCatchClicks;
    guiComponent * m_pComponentOwner;
};

#endif
