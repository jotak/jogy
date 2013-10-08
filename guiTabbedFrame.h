#ifndef _GUI_TABBEDFRAME_H
#define _GUI_TABBEDFRAME_H

#include "guiFrame.h"
#include "guiLabel.h"
#include "../Geometries/IGeometryQuads.h"

class guiTabbedFrame_Document : public BaseObject
{
public:
    guiTabbedFrame_Document(guiDocument * pDoc, FrameFitBehavior OldWidthFit, FrameFitBehavior OldHeightFit, FontId fontId);
    virtual ~guiTabbedFrame_Document();
    guiDocument * m_pDoc;
    guiLabel * m_pLabel;
    FrameFitBehavior m_OldWidthFit;
    FrameFitBehavior m_OldHeightFit;
};

class guiTabbedFrame : public guiFrame
{
public:
    guiTabbedFrame();
    virtual ~guiTabbedFrame();

    // Inherited functions
    virtual u32 getType() { return guiFrame::getType() | GOTYPE_TBDFRAME; };
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functions
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);

    // Document management
    virtual void attachDocument(guiDocument * pDoc, FrameFitBehavior OldWidthFit, FrameFitBehavior OldHeightFit);
    virtual guiTabbedFrame_Document * detachDocument();
    void setMainDocument(guiDocument * pDoc);

    // Panel functions
    bool isPanelAt(int xPxl, int yPxl);
    short getIPanel(int xPxl, int yPxl);

    // Size and position
    virtual bool isAt(int xPxl, int yPxl);
    virtual void moveTo(int xPxl, int yPxl);

    // Member access
    void setFontId(FontId id) { m_FontId = id; };
    void setPanelXDecal(int xdecal) { m_iXPanelDecal = xdecal; };

    // Clone / init
    virtual void init(Texture ** pTabTexs, FontId fontId, int xdecal, FramePosition positionType, FrameFitBehavior widthFit, FrameFitBehavior heightFit, int iMaxWidth, int iMaxHeight, Texture ** pMainTexs, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl);

protected:
    list<guiTabbedFrame_Document*> m_pDocumentsList;
    GeometryQuads * m_pTabsGeometry;
    int m_iXPanelDecal;
    FontId m_FontId;
    Texture * m_pTexList[6];

private:
    void computeGeometry();
};

#endif
