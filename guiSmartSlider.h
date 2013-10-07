#ifndef _GUI_SMART_SLIDER_H
#define _GUI_SMART_SLIDER_H

#include "guiLabel.h"
#include "../Geometries/GeometryQuads.h"

class guiSliderItem : public BaseObject
{
public:
    virtual string getInfo() { return m_sName; };
    Texture * m_pTex;
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
    virtual void init(int iItemSize, int iSpacing, FontId fontId, Color textColor, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl);
    virtual u32 getType() { return guiComponent::getType() | GOTYPE_SMARTSLIDER; };
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functions
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

protected:
    list<guiSliderItem*> m_pItems;
    int m_iSpacing;
    int m_iSliderPos;
    int m_iTheoricSize;
    guiSliderItem * m_pSelectedItem;
    GeometryQuads * m_pDisabledGeometry;
    GeometryQuads * m_pSelectorGeometry;
    int m_iSelectorPos;
    guiLabel * m_pLabel;
    guiLabel * m_pDisableReasonLabel;
    int m_iItemSize;
};

#endif
