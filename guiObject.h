#ifndef _GUI_OBJECT_H
#define _GUI_OBJECT_H

#include "Utils/BaseObject.h"
#include "Utils/ButtonAction.h"

#define GOTYPE_NOTHING      0x00000000
#define GOTYPE_CONTAINER    0x00000001
#define GOTYPE_FRAME        0x00000002
#define GOTYPE_TBDFRAME     0x00000004
#define GOTYPE_BUTTON       0x00000008
#define GOTYPE_LABEL        0x00000010
#define GOTYPE_TOGGLEBUTTON 0x00000020
#define GOTYPE_DOCUMENT     0x00000040
#define GOTYPE_EDITBOX      0x00000080
#define GOTYPE_SMARTSLIDER  0x00000100


#define MAX_FONTS    5

typedef unsigned char fontid;

class guiObject : public BaseObject
{
public:
    guiObject();
    virtual ~guiObject();

    // Update / display
    virtual void update(double delta) {};
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor) = 0;

    // Input functions
    virtual guiObject * onButtonEvent(ButtonAction * pEvent) { return NULL; };
    virtual guiObject * onCursorMoveEvent(int xPxl, int yPxl) { return this; };
    virtual void onCursorMoveOutEvent() {};

    // Size and position
    int getXPos() { return m_iXPxl; };
    int getYPos() { return m_iYPxl; };
    int getWidth() { return m_iWidth; };
    int getHeight() { return m_iHeight; };
    virtual void setXPos(int iXPxl) { m_iXPxl = iXPxl; };
    virtual void setYPos(int iYPxl) { m_iYPxl = iYPxl; };
    virtual void setWidth(int iWidth);
    virtual void setHeight(int iHeight);
    virtual void setDimensions(int iWidth, int iHeight);
    virtual bool isAt(int xPxl, int yPxl);
    virtual void moveTo(int xPxl, int yPxl);
    virtual void moveBy(int xPxl, int yPxl);
    virtual void onResize(int iOldWidth, int iOldHeight) {};
    void centerOn(guiObject * pOther);
    void vCenterOn(guiObject * pOther);
    void hCenterOn(guiObject * pOther);

    // Tooltip text
    string getTooltipText() { return m_sTooltip; };
    void setTooltipText(string sTooltip) { m_sTooltip = sTooltip; };

    // Other
    Color getDiffuseColor() { return m_DiffuseColor; };
    void setDiffuseColor(Color color) { m_DiffuseColor = color; };
    virtual u32 getType() = 0;

    // Builder
    virtual guiObject * build() { return this; };
    guiObject * withPosition(int x, int y) {
    	m_iXPxl = x; m_iYPxl = y; return this;
    }
    guiObject * withDimensions(int w, int h) {
    	m_iWidth = w; m_iHeight = h; return this;
    }

protected:
    int m_iXPxl;
    int m_iYPxl;
    int m_iWidth;
    int m_iHeight;
    string m_sTooltip;
    Color m_DiffuseColor;
};

#endif
