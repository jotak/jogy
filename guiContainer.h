#ifndef _GUI_CONTAINER_H
#define _GUI_CONTAINER_H

#include "guiComponent.h"
#include "guiDocument.h"
#include "Geometries/IGeometryQuads.h"
#include "Geometries/IStencilGeometry.h"

enum FrameFitBehavior
{
    FB_NoFit = 0,
    FB_FitDocumentToFrame,
    FB_FitDocumentToFrameWhenSmaller,
    FB_FitFrameToDocument,
    FB_FitFrameToDocumentWhenSmaller
};

class ScrollButtonData {
public:
	ScrollButtonData() {
		x = 0;
		y = 0;
		show = false;
	};
	int x;
	int y;
	bool show;
};

class guiContainer : public guiComponent
{
public:
    // Constructor / destructor
    guiContainer();
    virtual ~guiContainer();

    // Inherited functions
    virtual u32 getType() { return guiComponent::getType() | GOTYPE_CONTAINER; };
    virtual void setVisible(bool bVisible);

    // Update / display
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);
    virtual void updateSizeFit();

    // Input functions
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);
    virtual guiObject * onCursorMoveEvent(int xPxl, int yPxl);

    // Size and position
    int getInnerXPos() { return m_iInnerXPxl; };
    int getInnerYPos() { return m_iInnerYPxl; };
    int getInnerWidth() { return m_iInnerWidth; };
    int getInnerHeight() { return m_iInnerHeight; };
    bool isDocumentAt(int xPxl, int yPxl);
    virtual void moveTo(int xPxl, int yPxl);
    virtual void moveBy(int xPxl, int yPxl);
    virtual void onResize(int iOldWidth, int iOldHeight);

    // Scroll functions
    void scrollToTop();
    void scrollToBottom();
    void scrollToLeft();
    void scrollToRight();

    // Document management
    virtual guiDocument * getDocument() { return m_pDoc; };
    virtual void setDocument(guiDocument * pDoc);
    virtual guiDocument * unsetDocument();
    void checkDocumentPosition();

    // Member access
    FrameFitBehavior getWidthFitBehavior() { return m_WidthFit; };
    FrameFitBehavior getHeightFitBehavior() { return m_HeightFit; };
    void setWidthFitBehavior(FrameFitBehavior widthFit) { m_WidthFit = widthFit; };
    void setHeightFitBehavior(FrameFitBehavior heightFit) { m_HeightFit = heightFit; };
    int getMaxWidth() { return m_iMaxWidth; };
    int getMaxHeight() { return m_iMaxHeight; };
    void setMaxWidth(int width) { m_iMaxWidth = width; };
    void setMaxHeight(int height) { m_iMaxHeight = height; };

    // Builder
    virtual guiContainer * build(ITexture ** pMainTexs);
    guiContainer * withFrameFitBehavior(FrameFitBehavior widthFit, FrameFitBehavior heightFit) {
    	m_WidthFit = widthFit; m_HeightFit = heightFit; return this;
    }
    guiContainer * withOffset(int x, int y) {
    	m_iXOffset = x; m_iYOffset = y; return this;
    }
    guiContainer * withMaxDimensions(int w, int h) {
    	m_iMaxWidth = w; m_iMaxHeight = h; return this;
    }
    guiContainer * withStencil(IStencilGeometry * pStencil) {
    	m_pStencilGeometry = pStencil; return this;
    }

    // Static data
    static void initStatic(IGeometryQuads ** scrollGeometries, ITexture ** scrollTextures);
    static void deleteStatic();

protected:
    int m_iMaxWidth;
    int m_iMaxHeight;
    int m_iInnerXPxl;
    int m_iInnerYPxl;
    int m_iInnerWidth;
    int m_iInnerHeight;
    guiDocument * m_pDoc;
    IStencilGeometry * m_pStencilGeometry;
    static IGeometryQuads * m_pScrollButtons[4];   // top, bottom, left, right
    ScrollButtonData m_ScrollButtonsData[4];       // top, bottom, left, right
    int m_iClickedScroll;
    float m_fScrollDelta;
    static int m_iScrollButtonWidth;
    static int m_iScrollButtonHeight;
    FrameFitBehavior m_WidthFit;
    FrameFitBehavior m_HeightFit;
    int m_iXOffset;
    int m_iYOffset;

private:
    int computeQuadsList(QuadData *** pQuads, ITexture ** pTextures);
    void stepScroll(int iDir);  // 0=top, 1=bottom, 2=left, 3=right
};

#endif
