#ifndef _GUI_TABBEDFRAME_H
#define _GUI_TABBEDFRAME_H

#include "guiFrame.h"
#include "guiLabel.h"

class guiTabbedFrame_Document : public BaseObject
{
public:
    guiTabbedFrame_Document(guiDocument*, FrameFitBehavior, FrameFitBehavior, guiLabel*);
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
    virtual void attachDocument(guiDocument * pDoc, FrameFitBehavior oldWidthFit, FrameFitBehavior oldHeightFit);
    virtual guiTabbedFrame_Document * detachDocument();
    void setMainDocument(guiDocument * pDoc);

    // Panel functions
    bool isPanelAt(int xPxl, int yPxl);
    short getIPanel(int xPxl, int yPxl);

    // Size and position
    virtual bool isAt(int xPxl, int yPxl);
    virtual void moveTo(int xPxl, int yPxl);

    // Member access
    void setPanelXDecal(int xspace) { m_iXSpace = xspace; };

    // Builder
    virtual guiTabbedFrame * build();
    guiTabbedFrame * withTabsGeometry(ITexture ** pTex, IGeometryQuads * pGeo, ITexture ** pSelTex, IGeometryQuads * pSelGeo);
    guiTabbedFrame * withXSpace(int xspace) {
    	m_iXSpace = xspace; return this;
    }
    guiTabbedFrame * withLabelInfo(fontid fontId, IGeometryText * pLabelGeo);

protected:
    guiLabel * m_pLabelTemplate;
    list<guiTabbedFrame_Document*> m_pDocumentsList;
    IGeometryQuads * m_pTabsGeometry;
    IGeometryQuads * m_pUnselectedTabGeometry;
    IGeometryQuads * m_pSelectedTabGeometry;
    int m_iXSpace;

private:
    void rebuildGeometry();
};

#endif
