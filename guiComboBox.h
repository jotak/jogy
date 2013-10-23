#ifndef _GUI_COMBOBOX_H
#define _GUI_COMBOBOX_H

#include "guiContainer.h"
#include "guiButton.h"

class guiComboBox : public guiComponent, public ComponentOwnerInterface
{
public:
    // Constructor / destructor
    guiComboBox();
    virtual ~guiComboBox();

    // Inherited functions
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functions
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);

    // Events
    virtual void onFocusLost();
    virtual bool onButtonEvent(ButtonAction * pEvent, guiComponent * pCpnt);

    // Size and position
    virtual bool isAt(int xPxl, int yPxl);
    virtual void onResize(int iOldWidth, int iOldHeight);
    void moveTo(int xPxl, int yPxl);
    void moveBy(int xPxl, int yPxl);
    virtual void setWidth(int iWidth);
    virtual void setDimensions(int iWidth, int iHeight);

    // List elements
    void clearList();
    guiButton * addString(string sText, string sId);
    string getText() { return m_pLabel->getText(); };
    void setItem(int id);
    guiButton * getItem(string sId);
    guiButton * getItem(u16 uId);
    guiButton * getSelectedItem();
    int getSelectedItemId();
    u16 getItemsCount();

    // Builder
    virtual guiComboBox * build();
    guiComboBox * withMainGeometry(ITexture ** pTex, IGeometryQuads * pGeo);
    guiComboBox * withLabelInfo(Color textColor, fontid fontId, IGeometryText * pLabelGeo) {
    	m_pLabel->withText("", fontId, textColor)->withGeometry(pLabelGeo);
    	return this;
    }
    guiComboBox * withMaxListDimensions(int w, int h) {
    	m_pList->withMaxDimensions(w, h); return this;
    }
    guiComboBox * withListGeometries(ITexture ** pTex, IGeometryQuads * pGeo, ITexture * pDocTex, IGeometryQuads * pDocGeo) {
    	m_pList->withGeometry(pTex, pGeo);
    	m_pList->getDocument()->withGeometry(pDocTex, pDocGeo);
    	return this;
    }
    guiComboBox * withListButtonHover(InputBehaviour opt) {
    	m_pListButtonTemplate->withInputBehaviour(None, opt); return this;
    }
    guiComboBox * withListButtonGeometry(ITexture * pTex, IGeometryQuads * pGeo) {
    	m_pListButtonTemplate->withBaseGeometry(pTex, pGeo); return this;
    }
    guiComboBox * withListButtonLabelInfo(Color textColor, fontid fontId, IGeometryText * pLabelGeo) {
    	m_pListButtonTemplate->withLabel("", fontId, textColor, pLabelGeo);
    	return this;
    }
    guiComboBox * withListButtonHoverGeometry(ITexture * pTex, IGeometryQuads * pGeo) {
    	m_pListButtonTemplate->withHoverGeometry(pTex, pGeo); return this;
    }
    guiComboBox * withListButtonHeight(int h) {
    	m_pListButtonTemplate->withDimensions(0, h); return this;
    }

protected:
    void centerLabel();

    guiLabel * m_pLabel;
    guiContainer * m_pList;
    guiButton * m_pListButtonTemplate;
    double m_dListPos;
    IGeometryQuads * m_pMainGeometry;

private:
    void rebuildGeometry();
};

#endif
