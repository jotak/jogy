#ifndef _GUI_COMBOBOX_H
#define _GUI_COMBOBOX_H

#include "Container.h"
#include "Button.h"

namespace jogy {

class ComboBox : public Component, public ComponentOwnerInterface
{
public:
    // Constructor / destructor
    ComboBox();
    virtual ~ComboBox();

    // Inherited functions
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functions
    virtual Object * onButtonEvent(ButtonAction * pEvent);

    // Events
    virtual void onFocusLost();
    virtual bool onButtonEvent(ButtonAction * pEvent, Component * pCpnt);

    // Size and position
    virtual bool isAt(int xPxl, int yPxl);
    virtual void onResize(int iOldWidth, int iOldHeight);
    void moveTo(int xPxl, int yPxl);
    void moveBy(int xPxl, int yPxl);
    virtual void setWidth(int iWidth);
    virtual void setDimensions(int iWidth, int iHeight);

    // List elements
    void clearList();
    Button * addString(string sText, string sId);
    string getText() { return m_pLabel->getText(); };
    void setItem(int id);
    Button * getItem(string sId);
    Button * getItem(u16 uId);
    Button * getSelectedItem();
    int getSelectedItemId();
    u16 getItemsCount();

    // Builder
    virtual ComboBox * build();
    ComboBox * withMainGeometry(ITexture ** pTex, IGeometryQuads * pGeo);
    ComboBox * withLabelInfo(Color textColor, fontid fontId, IGeometryText * pLabelGeo) {
    	m_pLabel->withText("", fontId, textColor)->withGeometry(pLabelGeo);
    	return this;
    }
    ComboBox * withMaxListDimensions(int w, int h) {
    	m_pList->withMaxDimensions(w, h); return this;
    }
    ComboBox * withListGeometries(ITexture ** pTex, IGeometryQuads * pGeo, ITexture * pDocTex, IGeometryQuads * pDocGeo) {
    	m_pList->withGeometry(pTex, pGeo);
    	m_pList->getDocument()->withGeometry(pDocTex, pDocGeo);
    	return this;
    }
    ComboBox * withListButtonHover(InputBehaviour opt) {
    	m_pListButtonTemplate->withInputBehaviour(None, opt); return this;
    }
    ComboBox * withListButtonGeometry(ITexture * pTex, IGeometryQuads * pGeo) {
    	m_pListButtonTemplate->withBaseGeometry(pTex, pGeo); return this;
    }
    ComboBox * withListButtonLabelInfo(Color textColor, fontid fontId, IGeometryText * pLabelGeo) {
    	m_pListButtonTemplate->withLabel("", fontId, textColor, pLabelGeo);
    	return this;
    }
    ComboBox * withListButtonHoverGeometry(ITexture * pTex, IGeometryQuads * pGeo) {
    	m_pListButtonTemplate->withHoverGeometry(pTex, pGeo); return this;
    }
    ComboBox * withListButtonHeight(int h) {
    	m_pListButtonTemplate->withDimensions(0, h); return this;
    }

protected:
    void centerLabel();

    Label * m_pLabel;
    Container * m_pList;
    Button * m_pListButtonTemplate;
    double m_dListPos;
    IGeometryQuads * m_pMainGeometry;

private:
    void rebuildGeometry();
};
}

#endif
