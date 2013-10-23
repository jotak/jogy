#ifndef _GUI_LABEL_H
#define _GUI_LABEL_H

#include "Component.h"
#include "../Geometries/IGeometryText.h"


namespace jogy {

class Label : public Component
{
public:
    Label();
    virtual ~Label();

    // Inherited functions
    virtual u32 getType() { return GOTYPE_LABEL; };
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);
    virtual Object * onButtonEvent(ButtonAction * pEvent);

    // Other functions
    fontid getFontId() { return m_FontId; };
    void setFontId(fontid id) { m_FontId = id; };
    int getBoxWidth() { return m_iBoxWidth; };
    void setBoxWidth(int iWidth);
    void setText(string sText);
    string getText() { return m_sText; };
    void setCatchClicks(bool b) { m_bCatchClicks = b; };
    void setComponentOwner(Component * pOwner) { m_pComponentOwner = pOwner; };
    IGeometryText * getGeometry() { return m_pGeometry; };

    // Builder
    virtual Label * build();
    Label * withText(string sText, fontid fontId, Color textColor) {
    	m_sText = sText; m_FontId = fontId; setDiffuseColor(textColor); return this;
    }
    Label * withGeometry(IGeometryText * pGeo) {
    	m_pGeometry = pGeo; return this;
    }

protected:
    void computeGeometry();

    string m_sText;
    fontid m_FontId;
    int m_iBoxWidth;
    bool m_bCatchClicks;
    Component * m_pComponentOwner;
    IGeometryText * m_pGeometry;
};
}

#endif
