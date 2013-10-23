#ifndef _GUI_TABBEDFRAME_H
#define _GUI_TABBEDFRAME_H

#include "Frame.h"
#include "Label.h"


namespace jogy {

class TabbedFrame_Document : public BaseObject
{
public:
    TabbedFrame_Document(Document*, FrameFitBehavior, FrameFitBehavior, Label*);
    virtual ~TabbedFrame_Document();
    Document * m_pDoc;
    Label * m_pLabel;
    FrameFitBehavior m_OldWidthFit;
    FrameFitBehavior m_OldHeightFit;
};

class TabbedFrame : public Frame
{
public:
    TabbedFrame();
    virtual ~TabbedFrame();

    // Inherited functions
    virtual u32 getType() { return Frame::getType() | GOTYPE_TBDFRAME; };
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functions
    virtual Object * onButtonEvent(ButtonAction * pEvent);

    // Document management
    virtual void attachDocument(Document * pDoc, FrameFitBehavior oldWidthFit, FrameFitBehavior oldHeightFit);
    virtual TabbedFrame_Document * detachDocument();
    void setMainDocument(Document * pDoc);

    // Panel functions
    bool isPanelAt(int xPxl, int yPxl);
    short getIPanel(int xPxl, int yPxl);

    // Size and position
    virtual bool isAt(int xPxl, int yPxl);
    virtual void moveTo(int xPxl, int yPxl);

    // Member access
    void setPanelXDecal(int xspace) { m_iXSpace = xspace; };

    // Builder
    virtual TabbedFrame * build();
    TabbedFrame * withTabsGeometry(ITexture ** pTex, IGeometryQuads * pGeo, ITexture ** pSelTex, IGeometryQuads * pSelGeo);
    TabbedFrame * withXSpace(int xspace) {
    	m_iXSpace = xspace; return this;
    }
    TabbedFrame * withLabelInfo(fontid fontId, IGeometryText * pLabelGeo);

protected:
    Label * m_pLabelTemplate;
    list<TabbedFrame_Document*> m_pDocumentsList;
    IGeometryQuads * m_pTabsGeometry;
    IGeometryQuads * m_pUnselectedTabGeometry;
    IGeometryQuads * m_pSelectedTabGeometry;
    int m_iXSpace;

private:
    void rebuildGeometry();
};
}

#endif
