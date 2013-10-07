#ifndef _GUI_OBJECT_H
#define _GUI_OBJECT_H

#include "Utils/GraphicObject.h"

#define MAX_FONTS    5

typedef unsigned char fontid;

class ButtonAction;

class guiObject : public GraphicObject
{
public:
    guiObject();
    virtual ~guiObject();

    // Texture(s) registration
    static void registerTextures(const fontid * pFonts, unsigned int nbFonts);

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

    // Clone and init
    virtual void init(int xPxl, int yPxl, int wPxl, int hPxl, IGeometry * pGeometry);

    // Tooltip text
    string getTooltipText() { return m_sTooltip; };
    void setTooltipText(string sTooltip) { m_sTooltip = sTooltip; };

    // Other
    Color getDiffuseColor() { return m_DiffuseColor; };
    void setDiffuseColor(Color color) { m_DiffuseColor = color; };

protected:
    int m_iXPxl;
    int m_iYPxl;
    int m_iWidth;
    int m_iHeight;
    string m_sTooltip;
    Color m_DiffuseColor;

    static fontid m_aiAllFonts[MAX_FONTS];
};

#endif
