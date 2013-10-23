#ifndef _GUI_SMART_SLIDER_H
#define _GUI_SMART_SLIDER_H

#include "Label.h"
#include "../Geometries/IGeometryQuads.h"


namespace jogy {

class guiSliderItem : public BaseObject
{
public:
    virtual string getInfo() { return m_sName; };
    ITexture * m_pTex;
    string m_sName;
    bool m_bEnabled;
    string m_sDisabledReason;
};

class SmartSlider : public Component
{
public:
    // Constructor / destructor
    SmartSlider();
    virtual ~SmartSlider();

    // Inherited functions
    virtual u32 getType() { return GOTYPE_SMARTSLIDER; };
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functionsDisabled
    virtual Object * onButtonEvent(ButtonAction * pEvent);
    virtual Object * onCursorMoveEvent(int xPxl, int yPxl);

    // Member access
    int getSpacing() { return m_iSpacing; };
    void setSpacing(int iSpacing) { m_iSpacing = iSpacing; };
    guiSliderItem * getSelectedItem() { return m_pSelectedItem; };

    // Specific functions
    void addItem(guiSliderItem * pItem, bool bFirst = false);
    void loadGeometry();
    void deleteItems();

    // Builder
    virtual SmartSlider * build();
    SmartSlider * withMainGeometry(IGeometryQuads * pGeo);
    SmartSlider * withSelectorGeometry(ITexture * pTex, IGeometryQuads * pGeo);
    SmartSlider * withDisabledGeometry(ITexture * pTex, IGeometryQuads * pGeo);
    SmartSlider * withItemSize(int size, int spacing) {
    	m_iItemSize = size; m_iSpacing = spacing; return this;
    };
    SmartSlider * withLabelInfo(Color textColor, fontid fontId, IGeometryText * pLabelGeo);

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
    Label * m_pLabel;
    Label * m_pDisableReasonLabel;
    int m_iItemSize;
};
}

#endif
