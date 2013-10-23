#include "Container.h"
#include "../JogyResources.h"

#define SCROLLSTEP          30    // in pixels
#define SCROLLFIRSTDELTA    0.4f  // seconds
#define SCROLLNEXTDELTA     0.12f; // seconds

namespace jogy {

// -----------------------------------------------------------------
// Name : Container
//  Fixed-sized constructor
// -----------------------------------------------------------------
Container::Container() : Component()
{
    m_iMaxWidth = m_iMaxHeight = m_iInnerXPxl = m_iInnerYPxl = m_iInnerWidth = m_iInnerHeight = 0;
    m_pDoc = NULL;
    m_pStencilGeometry = NULL;
    m_WidthFit = m_HeightFit = FB_NoFit;
    m_iXOffset = m_iYOffset = 0;
    m_iClickedScroll = -1;
    m_fScrollDelta = 0.0f;
    m_pGeometry = NULL;
}

// -----------------------------------------------------------------
// Name : ~Container
//  Destructor
// -----------------------------------------------------------------
Container::~Container()
{
    FREE(m_pDoc);
    FREE(m_pStencilGeometry);
	FREE(m_pGeometry);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
Container * Container::build()
{
    Component::build();

    ITexture * pTLTex = m_pGeometry->getTexture(0);
    ITexture * pBRTex = m_pGeometry->getTexture(7);
    m_iInnerXPxl = m_iXPxl + m_iXOffset + pTLTex->getWidth();
    m_iInnerYPxl = m_iYPxl + m_iYOffset + pTLTex->getHeight();
    m_iInnerWidth = m_iWidth - pTLTex->getWidth() - pBRTex->getWidth() - m_iXOffset;
    m_iInnerHeight = m_iHeight - pTLTex->getHeight() - pBRTex->getHeight() - m_iYOffset;

    rebuildGeometry();
    m_pStencilGeometry->build(m_iInnerWidth, m_iInnerHeight);

    // Scroll buttons positions
    m_ScrollButtonsData[0].x = m_iInnerXPxl + m_iInnerWidth - Resources::iScrollButtonWidth;
    m_ScrollButtonsData[0].y = m_iInnerYPxl;
    m_ScrollButtonsData[1].x = m_iInnerXPxl + m_iInnerWidth - Resources::iScrollButtonWidth;
    m_ScrollButtonsData[1].y = m_iInnerYPxl + m_iInnerHeight - Resources::iScrollButtonHeight;
    m_ScrollButtonsData[2].x = m_iInnerXPxl;
    m_ScrollButtonsData[2].y = m_iInnerYPxl + m_iInnerHeight - Resources::iScrollButtonWidth;
    m_ScrollButtonsData[3].x = m_iInnerXPxl + m_iInnerWidth - Resources::iScrollButtonHeight;
    m_ScrollButtonsData[3].y = m_iInnerYPxl + m_iInnerHeight - Resources::iScrollButtonWidth;

    return this;
}

// -----------------------------------------------------------------
// Name : withGeometry
// -----------------------------------------------------------------
Container * Container::withGeometry(ITexture ** pTex, IGeometryQuads * pGeo)
{
    m_pGeometry = pGeo;
	// 8 quads
    QuadData ** pQuads = new QuadData*[8];
    pQuads[0] = new QuadData(0, 1, 2, 3, pTex[0]);
    pQuads[1] = new QuadData(1, 2, 2, 3, pTex[1]);
    pQuads[2] = new QuadData(2, 3, 2, 3, pTex[2]);
    pQuads[3] = new QuadData(0, 1, 1, 2, pTex[3]);
    pQuads[4] = new QuadData(2, 3, 1, 2, pTex[4]);
    pQuads[5] = new QuadData(0, 1, 0, 1, pTex[5]);
    pQuads[6] = new QuadData(1, 2, 0, 1, pTex[6]);
    pQuads[7] = new QuadData(2, 3, 0, 1, pTex[7]);
    m_pGeometry->build(8, pQuads);
    QuadData::releaseQuads(8, pQuads);
	return this;
}

// -----------------------------------------------------------------
// Name : rebuildGeometry
// -----------------------------------------------------------------
void Container::rebuildGeometry()
{
	// 8 quads
    QuadData ** pQuads = new QuadData*[8];
    int xPxlMiddleStart = m_pGeometry->getTexture(0)->getWidth();
    int xPxlMiddleEnd = m_iWidth - m_iXOffset - m_pGeometry->getTexture(2)->getWidth();
    int yPxlMiddleStart = m_pGeometry->getTexture(0)->getHeight();
    int yPxlMiddleEnd = m_iHeight - m_iYOffset - m_pGeometry->getTexture(5)->getHeight();
    pQuads[0] = new QuadData(0,               xPxlMiddleStart,       yPxlMiddleEnd,   m_iHeight - m_iYOffset, m_pGeometry->getTexture(0));
    pQuads[1] = new QuadData(xPxlMiddleStart, xPxlMiddleEnd,         yPxlMiddleEnd,   m_iHeight - m_iYOffset, m_pGeometry->getTexture(1));
    pQuads[2] = new QuadData(xPxlMiddleEnd,   m_iWidth - m_iXOffset, yPxlMiddleEnd,   m_iHeight - m_iYOffset, m_pGeometry->getTexture(2));
    pQuads[3] = new QuadData(0,               xPxlMiddleStart,       yPxlMiddleStart, yPxlMiddleEnd,          m_pGeometry->getTexture(3));
    pQuads[4] = new QuadData(xPxlMiddleEnd,   m_iWidth - m_iXOffset, yPxlMiddleStart, yPxlMiddleEnd,          m_pGeometry->getTexture(4));
    pQuads[5] = new QuadData(0,               xPxlMiddleStart,       0,               yPxlMiddleStart,        m_pGeometry->getTexture(5));
    pQuads[6] = new QuadData(xPxlMiddleStart, xPxlMiddleEnd,         0,               yPxlMiddleStart,        m_pGeometry->getTexture(6));
    pQuads[7] = new QuadData(xPxlMiddleEnd,   m_iWidth - m_iXOffset, 0,               yPxlMiddleStart,        m_pGeometry->getTexture(7));

    m_pGeometry->build(8, pQuads);
    QuadData::releaseQuads(8, pQuads);
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void Container::update(double delta)
{
    if (!m_bVisible) {
        return;
    }
    if (m_pDoc != NULL)
    {
        if (m_pDoc->doesNeedDestroy())
        {
            delete m_pDoc;
            m_pDoc = NULL;
        }
        else
        {
            if (m_bEnabled != m_pDoc->isEnabled()) {
                setEnabled(m_pDoc->isEnabled());
            }
            updateSizeFit();
            m_pDoc->update(delta);

            // Scroll?
            if (m_iClickedScroll >= 0)
            {
                m_fScrollDelta -= delta;
                if (m_fScrollDelta <= 0)
                {
                    m_fScrollDelta = SCROLLNEXTDELTA;
                    stepScroll(m_iClickedScroll);
                }
            }
            // Check if it can scroll
            int iDocX = m_pDoc->getXPos();
            int iDocY = m_pDoc->getYPos();
            m_ScrollButtonsData[0].show = (iDocY < 0);
            m_ScrollButtonsData[1].show = (iDocY + m_pDoc->getHeight() > m_iInnerHeight);
            m_ScrollButtonsData[2].show = (iDocX < 0);
            m_ScrollButtonsData[3].show = (iDocX + m_pDoc->getWidth() > m_iInnerWidth);
        }
    }
    Component::update(delta);
}

// -----------------------------------------------------------------
// Name : updateSizeFit
// -----------------------------------------------------------------
void Container::updateSizeFit()
{
    ITexture * pTLTex = m_pGeometry->getTexture(0);
    ITexture * pBRTex = m_pGeometry->getTexture(7);

    if (m_pDoc->getWidth() != m_iInnerWidth)
    {
        switch (m_WidthFit)
        {
        case FB_FitDocumentToFrame:
            m_pDoc->setWidth(m_iInnerWidth);
            break;
        case FB_FitDocumentToFrameWhenSmaller:
        {
            if (m_pDoc->getWidth() < m_iInnerWidth)
                m_pDoc->setWidth(m_iInnerWidth);
            break;
        }
        case FB_FitFrameToDocument:
            m_iInnerWidth = m_pDoc->getWidth();
            setWidth(m_iInnerWidth + pTLTex->getWidth() + pBRTex->getWidth() + m_iXOffset);
            break;
        case FB_FitFrameToDocumentWhenSmaller:
        {
            int maxInnerW = m_iMaxWidth - pTLTex->getWidth() - pBRTex->getWidth() - m_iXOffset;
            if (m_pDoc->getWidth() < maxInnerW)
            {
                m_iInnerWidth = m_pDoc->getWidth();
                setWidth(m_iInnerWidth + pTLTex->getWidth() + pBRTex->getWidth() + m_iXOffset);
            }
            else if (maxInnerW != m_iInnerWidth)
            {
                m_iInnerWidth = maxInnerW;
                setWidth(m_iInnerWidth + pTLTex->getWidth() + pBRTex->getWidth() + m_iXOffset);
            }
            break;
        }
        default:
            break;
        }
    }
    if (m_pDoc->getHeight() != m_iInnerHeight)
    {
        switch (m_HeightFit)
        {
        case FB_FitDocumentToFrame:
            m_pDoc->setHeight(m_iInnerHeight);
            break;
        case FB_FitDocumentToFrameWhenSmaller:
        {
            if (m_pDoc->getHeight() < m_iInnerHeight)
                m_pDoc->setHeight(m_iInnerHeight);
            break;
        }
        case FB_FitFrameToDocument:
            m_iInnerHeight = m_pDoc->getHeight();
            setHeight(m_iInnerHeight + pTLTex->getHeight() + pBRTex->getHeight() + m_iYOffset);
            break;
        case FB_FitFrameToDocumentWhenSmaller:
        {
            int maxInnerH = m_iMaxHeight - pTLTex->getHeight() - pBRTex->getHeight() - m_iYOffset;
            if (m_pDoc->getHeight() < maxInnerH)
            {
                m_iInnerHeight = m_pDoc->getHeight();
                setHeight(m_iInnerHeight + pTLTex->getHeight() + pBRTex->getHeight() + m_iYOffset);
            }
            else if (maxInnerH != m_iInnerHeight)
            {
                m_iInnerHeight = maxInnerH;
                setHeight(m_iInnerHeight + pTLTex->getHeight() + pBRTex->getHeight() + m_iYOffset);
            }
            break;
        }
        default:
            break;
        }
    }

    // Scroll buttons positions
    m_ScrollButtonsData[0].x = m_iInnerXPxl + m_iInnerWidth - Resources::iScrollButtonWidth;
    m_ScrollButtonsData[0].y = m_iInnerYPxl;
    m_ScrollButtonsData[1].x = m_iInnerXPxl + m_iInnerWidth - Resources::iScrollButtonWidth;
    m_ScrollButtonsData[1].y = m_iInnerYPxl + m_iInnerHeight - Resources::iScrollButtonHeight;
    m_ScrollButtonsData[2].x = m_iInnerXPxl;
    m_ScrollButtonsData[2].y = m_iInnerYPxl + m_iInnerHeight - Resources::iScrollButtonWidth;
    m_ScrollButtonsData[3].x = m_iInnerXPxl + m_iInnerWidth - Resources::iScrollButtonHeight;
    m_ScrollButtonsData[3].y = m_iInnerYPxl + m_iInnerHeight - Resources::iScrollButtonWidth;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void Container::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    if (m_pDoc != NULL)
    {
        m_pStencilGeometry->fillStencil(m_iInnerXPxl + iXOffset, m_iInnerYPxl + iYOffset, true);
//        int iPreviousState = _display->setStencilState(2);
        docColor = m_pDoc->isEnabled() ? docColor : Color(0.5f, 0.5f, 0.5f, 0.5f);
        cpntColor = m_pDoc->isEnabled() ? cpntColor : Color(0.5f, 0.5f, 0.5f, 0.5f);
        m_pDoc->displayAt(m_iInnerXPxl + iXOffset, m_iInnerYPxl + iYOffset, cpntColor, docColor);
        m_pStencilGeometry->fillStencil(m_iInnerXPxl + iXOffset, m_iInnerYPxl + iYOffset, false);
//        _display->setStencilState(iPreviousState);
    }
    m_pGeometry->display(m_iXPxl + iXOffset + m_iXOffset, m_iYPxl + iYOffset + m_iYOffset, docColor);
    // Draw scroll buttons
    cpntColor.a *= 0.5f;
    for (int i = 0; i < 4; i++)
    {
        if (m_ScrollButtonsData[i].show)
        {
        	Resources::pScrollButtons[i]->display(m_ScrollButtonsData[i].x + iXOffset,
            		m_ScrollButtonsData[i].y + iYOffset,
            		cpntColor);
        }
    }
}

// -----------------------------------------------------------------
// Name : checkDocumentPosition
// -----------------------------------------------------------------
void Container::checkDocumentPosition()
{
    int iDocX = m_pDoc->getXPos();
    int iDocY = m_pDoc->getYPos();
    if (iDocX < m_iInnerWidth - m_pDoc->getWidth())
    {
        iDocX = m_iInnerWidth - m_pDoc->getWidth();
        m_pDoc->moveTo(iDocX, iDocY);
    }
    if (iDocY < m_iInnerHeight - m_pDoc->getHeight())
    {
        iDocY = m_iInnerHeight - m_pDoc->getHeight();
        m_pDoc->moveTo(iDocX, iDocY);
    }
    if (iDocX > 0)
    {
        iDocX = 0;
        m_pDoc->moveTo(0, iDocY);
    }
    if (iDocY > 0)
    {
        iDocY = 0;
        m_pDoc->moveTo(iDocX, 0);
    }
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
Object * Container::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible || m_pDoc == NULL) {
        return NULL;
    }

    int xPxl = pEvent->xPos - pEvent->xOffset;
    int yPxl = pEvent->yPos - pEvent->yOffset;
    if (pEvent->eEvent == EventDown && pEvent->eButton == Button1)
    {
        // Check click on scroll button
        for (int i = 0; i < 4; i++)
        {
            if (m_ScrollButtonsData[i].show && xPxl >= m_ScrollButtonsData[i].x
            		&& yPxl >= m_ScrollButtonsData[i].y
            		&& xPxl <= m_ScrollButtonsData[i].x + (i < 2 ? Resources::iScrollButtonWidth : Resources::iScrollButtonHeight)
            		&& yPxl <= m_ScrollButtonsData[i].y + (i < 2 ? Resources::iScrollButtonHeight : Resources::iScrollButtonWidth))
            {
                m_iClickedScroll = i;
                stepScroll(i);
                m_fScrollDelta = SCROLLFIRSTDELTA;
                return this;
            }
        }
    }
    else if (m_iClickedScroll >= 0 && pEvent->eButton == Button1)
    {
        if (pEvent->eEvent == EventUp) {
            m_iClickedScroll = -1;
        }
        return this;
    }
    if (pEvent->eEvent == EventDown && isDocumentAt(xPxl, yPxl))
    {
        pEvent->xOffset += m_iInnerXPxl + m_pDoc->getXPos();
        pEvent->yOffset += m_iInnerYPxl + m_pDoc->getYPos();
        Object * pObj = m_pDoc->onButtonEvent(pEvent);
        if (pObj != NULL) {
            return pObj;
        }
    }
    if (pEvent->eButton == ButtonX)
    {
        m_pDoc->moveBy(0, -SCROLLSTEP);
        checkDocumentPosition();
        return this;
    }
    if (pEvent->eButton == ButtonZ)
    {
        m_pDoc->moveBy(0, SCROLLSTEP);
        checkDocumentPosition();
        return this;
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : onCursorMoveEvent
// -----------------------------------------------------------------
Object * Container::onCursorMoveEvent(int xPxl, int yPxl)
{
    if (!m_bVisible) {
        return NULL;
    }
    if (m_pDoc != NULL)
    {
        if (m_pDoc->isEnabled() && isDocumentAt(xPxl, yPxl)) {
            return m_pDoc->onCursorMoveEvent(xPxl - m_iInnerXPxl - m_pDoc->getXPos(), yPxl - m_iInnerYPxl - m_pDoc->getYPos());
        }
    }
    return this;
}

// -----------------------------------------------------------------
// Name : isDocumentAt
// -----------------------------------------------------------------
bool Container::isDocumentAt(int xPxl, int yPxl)
{
    return (m_bVisible && xPxl >= m_iInnerXPxl && yPxl >= m_iInnerYPxl && xPxl <= m_iInnerXPxl + m_iInnerWidth && yPxl <= m_iInnerYPxl + m_iInnerHeight);
}

// -----------------------------------------------------------------
// Name : moveTo
// -----------------------------------------------------------------
void Container::moveTo(int xPxl, int yPxl)
{
    m_iInnerXPxl += xPxl - m_iXPxl;
    m_iInnerYPxl += yPxl - m_iYPxl;
    Component::moveTo(xPxl, yPxl);
}

// -----------------------------------------------------------------
// Name : moveBy
// -----------------------------------------------------------------
void Container::moveBy(int xPxl, int yPxl)
{
    Component::moveBy(xPxl, yPxl);
    m_iInnerXPxl += xPxl;
    m_iInnerYPxl += yPxl;
}

// -----------------------------------------------------------------
// Name : scrollToTop
// -----------------------------------------------------------------
void Container::scrollToTop()
{
    m_pDoc->moveTo(m_pDoc->getXPos(), 0);
}

// -----------------------------------------------------------------
// Name : scrollToBottom
// -----------------------------------------------------------------
void Container::scrollToBottom()
{
    m_pDoc->moveTo(m_pDoc->getXPos(), m_iInnerHeight - m_pDoc->getHeight());
}

// -----------------------------------------------------------------
// Name : scrollToLeft
// -----------------------------------------------------------------
void Container::scrollToLeft()
{
    m_pDoc->moveTo(0, m_pDoc->getYPos());
}

// -----------------------------------------------------------------
// Name : scrollToRight
// -----------------------------------------------------------------
void Container::scrollToRight()
{
    m_pDoc->moveTo(m_iInnerWidth - m_pDoc->getWidth(), m_pDoc->getYPos());
}

// -----------------------------------------------------------------
// Name : setDocument
// -----------------------------------------------------------------
void Container::setDocument(Document * pDoc)
{
    m_pDoc = pDoc;
    pDoc->setOwner(this);
    pDoc->moveTo(0, 0);
    if (m_pGeometry != NULL) {
        updateSizeFit();
    }
}

// -----------------------------------------------------------------
// Name : unsetDocument
// -----------------------------------------------------------------
Document * Container::unsetDocument()
{
    if (m_pDoc == NULL) {
        return NULL;
    }
    Document * pDoc = m_pDoc;
    m_pDoc = NULL;
    return pDoc;
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void Container::onResize(int iOldWidth, int iOldHeight)
{
    Component::onResize(iOldWidth, iOldHeight);
    if (m_iWidth == iOldWidth && m_iHeight == iOldHeight)
        return;
    if (m_pGeometry != NULL)
    {
        m_iInnerWidth = m_iWidth - m_iXOffset - m_pGeometry->getTexture(0)->getWidth() - m_pGeometry->getTexture(7)->getWidth();
        m_iInnerHeight = m_iHeight - m_iYOffset - m_pGeometry->getTexture(0)->getHeight() - m_pGeometry->getTexture(7)->getHeight();
        rebuildGeometry();
        m_pStencilGeometry->build(m_iInnerWidth, m_iInnerHeight);
    }

    // Scroll buttons positions
    m_ScrollButtonsData[0].x = m_iInnerXPxl + m_iInnerWidth - Resources::iScrollButtonWidth;
    m_ScrollButtonsData[0].y = m_iInnerYPxl;
    m_ScrollButtonsData[1].x = m_iInnerXPxl + m_iInnerWidth - Resources::iScrollButtonWidth;
    m_ScrollButtonsData[1].y = m_iInnerYPxl + m_iInnerHeight - Resources::iScrollButtonHeight;
    m_ScrollButtonsData[2].x = m_iInnerXPxl;
    m_ScrollButtonsData[2].y = m_iInnerYPxl + m_iInnerHeight - Resources::iScrollButtonWidth;
    m_ScrollButtonsData[3].x = m_iInnerXPxl + m_iInnerWidth - Resources::iScrollButtonHeight;
    m_ScrollButtonsData[3].y = m_iInnerYPxl + m_iInnerHeight - Resources::iScrollButtonWidth;
}

// -----------------------------------------------------------------
// Name : setVisible
// -----------------------------------------------------------------
void Container::setVisible(bool bVisible)
{
    bool bWasVisible = isVisible();
    Component::setVisible(bVisible);
    if (m_pDoc != NULL && bVisible && !bWasVisible)
        m_pDoc->onShow();
    else if (m_pDoc != NULL && !bVisible && bWasVisible)
        m_pDoc->onHide();
}

// -----------------------------------------------------------------
// Name : stepScroll
// -----------------------------------------------------------------
void Container::stepScroll(int iDir)
{
    switch (iDir)
    {
    case 0: // top
        m_pDoc->moveBy(0, SCROLLSTEP);
        checkDocumentPosition();
        break;
    case 1: // bottom
        m_pDoc->moveBy(0, -SCROLLSTEP);
        checkDocumentPosition();
        break;
    case 2: // left
        m_pDoc->moveBy(SCROLLSTEP, 0);
        checkDocumentPosition();
        break;
    case 3: // right
        m_pDoc->moveBy(-SCROLLSTEP, 0);
        checkDocumentPosition();
        break;
    }
}

//// -----------------------------------------------------------------
//// Name : createDefaultPanel
////  Static default constructor
////  Use it to avoid passing always the same 3591218 arguments to "init"
//// -----------------------------------------------------------------
//Container * Container::createDefaultPanel(int width, int height, string sId)
//{
//    Container * pPanel = new Container();
//    ITexture * frmtex[8];
//    frmtex[0] = _tex->findTexture("gui/interface:LstTL");
//    frmtex[1] = _tex->findTexture("gui/interface:LstTC");
//    frmtex[2] = _tex->findTexture("gui/interface:LstTR");
//    frmtex[3] = _tex->findTexture("gui/interface:LstCL");
//    frmtex[4] = _tex->findTexture("gui/interface:LstCR");
//    frmtex[5] = _tex->findTexture("gui/interface:LstBL");
//    frmtex[6] = _tex->findTexture("gui/interface:LstBC");
//    frmtex[7] = _tex->findTexture("gui/interface:LstBR");
//    pPanel->init(
//        FB_FitDocumentToFrameWhenSmaller,
//        FB_FitDocumentToFrameWhenSmaller,
//        0, 0, 0, 0, frmtex, sId, 0, 0, width, height);
//
//    // Attach document
//    Document * pDoc = new Document();
//    pDoc->init(
//        "",
//        _tex->findTexture("gui/interface:WinBg"),
//        0, 0, 1, 1);
//    pPanel->setDocument(pDoc);
//
//    return pPanel;
//}
}
