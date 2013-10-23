#ifndef _GUI_SMART_SLIDER_H
#define _GUI_SMART_SLIDER_H

#include "guiLabel.h"
#include "Geometries/IGeometryQuads.h"

class guiSliderItem : public BaseObject
{
public:
    virtual string getInfo() { return m_sName; };
    ITexture * m_pTex;
    string m_sName;
    bool m_bEnabled;
    string m_sDisabledReason;
};

class guiSmartSlider : public guiComponent
{
public:
    // Constructor / destructor
    guiSmartSlider();
    virtual ~guiSmartSlider();

    // Inherited functions
    virtual u32 getType() { return GOTYPE_SMARTSLIDER; };
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functionsDisabled
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);
    virtual guiObject * onCursorMoveEvent(int xPxl, int yPxl);

    // Member access
    int getSpacing() { return m_iSpacing; };
    void setSpacing(int iSpacing) { m_iSpacing = iSpacing; };
    guiSliderItem * getSelectedItem() { return m_pSelectedItem; };

    // Specific functions
    void addItem(guiSliderItem * pItem, bool bFirst = false);
    void loadGeometry();
    void deleteItems();

    // Builder
    virtual guiSmartSlider * build();
    guiSmartSlider * withMainGeometry(IGeometryQuads * pGeo);
    guiSmartSlider * withSelectorGeometry(ITexture * pTex, IGeometryQuads * pGeo);
    guiSmartSlider * withDisabledGeometry(ITexture * pTex, IGeometryQuads * pGeo);
    guiSmartSlider * withItemSize(int size, int spacing) {
    	m_iItemSize = size; m_iSpacing = spacing; return this;
    };
    guiSmartSlider * withLabelInfo(Color textColor, fontid fontId, IGeometryText * pLabelGeo);

protected:
    list<guiSliderItem*> m_pItems;
    int m_iSpacing;
    int m_iSliderPos;
    int m_iTheoricSize;
    guiSliderItem * m_pSelectedItem;
    IGeometryQuads * m_pMainGeometry;
    IGeometryQuads * m_pDisabledGeometry;
    IGeometryQuads * m_pSelectorGeometry;
    int m_iSelectorPos;
    guiLabel * m_pLabel;
    guiLabel * m_pDisableReasonLabel;
    int m_iItemSize;
};

#endif
