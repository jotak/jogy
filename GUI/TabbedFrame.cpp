#include "TabbedFrame.h"
#include "Label.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : TabbedFrame
//  Constructor
// -----------------------------------------------------------------
TabbedFrame::TabbedFrame() : Frame()
{
	m_pLabelTemplate = new Label();
    m_pUnselectedTabGeometry = NULL;
    m_pSelectedTabGeometry = NULL;
    m_pTabsGeometry = NULL;
    m_iXSpace = 0;
}

// -----------------------------------------------------------------
// Name : ~TabbedFrame
//  Destructor
// -----------------------------------------------------------------
TabbedFrame::~TabbedFrame()
{
    m_pDoc = NULL;   // it's going to be deleted below
    // We need to explicitly delete all Documents as they won't be deleted from TabbedFrame_Document destructor
	for (TabbedFrame_Document* &pDoc : m_pDocumentsList) {
        delete pDoc->m_pDoc;
    }
    // Now we can delete the list
    FREEVEC(m_pDocumentsList);
    FREE(m_pTabsGeometry);
    FREE(m_pUnselectedTabGeometry);
    FREE(m_pSelectedTabGeometry);
    FREE(m_pLabelTemplate);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
TabbedFrame * TabbedFrame::build()
{
	// Frame init:
	withOffset(0, m_pSelectedTabGeometry->getTexture()->getHeight());
    Frame::build();

    rebuildGeometry();

    return this;
}

// -----------------------------------------------------------------
// Name : withTabsGeometry
// -----------------------------------------------------------------
TabbedFrame * TabbedFrame::withTabsGeometry(ITexture ** pTex, IGeometryQuads * pGeo, ITexture ** pSelTex, IGeometryQuads * pSelGeo)
{
	m_pUnselectedTabGeometry = pGeo;
	m_pSelectedTabGeometry = pSelGeo;

	// Each geometry has 3 quads
    QuadData ** pQuads = new QuadData*[3];
    pQuads[0] = new QuadData(0, 1, 0, 3, pTex[0]);
    pQuads[1] = new QuadData(1, 2, 0, 3, pTex[1]);
    pQuads[2] = new QuadData(2, 3, 0, 3, pTex[2]);
    m_pUnselectedTabGeometry->build(3, pQuads);
    QuadData::releaseQuads(3, pQuads);

    pQuads = new QuadData*[3];
    pQuads[0] = new QuadData(0, 1, 0, 3, pSelTex[0]);
    pQuads[1] = new QuadData(1, 2, 0, 3, pSelTex[1]);
    pQuads[2] = new QuadData(2, 3, 0, 3, pSelTex[2]);
    m_pSelectedTabGeometry->build(3, pQuads);
    QuadData::releaseQuads(3, pQuads);
	return this;
}

// -----------------------------------------------------------------
// Name : withLabelInfo
// -----------------------------------------------------------------
TabbedFrame * TabbedFrame::withLabelInfo(fontid fontId, IGeometryText * pLabelGeo)
{
	m_pLabelTemplate->withText("", fontId, Color::black)->withGeometry(pLabelGeo);
	return this;
}

// -----------------------------------------------------------------
// Name : rebuildGeometry
// -----------------------------------------------------------------
void TabbedFrame::rebuildGeometry()
{
	int size = m_pDocumentsList.size();
    // n Quads
    int nQuads = 3 * size;
    if (nQuads == 0)
    {
        FREE(m_pTabsGeometry);
        return;
    }
    QuadData ** pQuads = new QuadData*[nQuads];

    int xstart, xend, ystart, yend;
    xstart = 0;
    yend = m_pSelectedTabGeometry->getTexture(0)->getHeight();
    int boxWidth = (m_iWidth - 2 * m_iXSpace) / size;
    int i = 0;
    IGeometryQuads * pCurrent = NULL;
	for (TabbedFrame_Document* &pDoc : m_pDocumentsList) {
        if (pDoc->m_pDoc == m_pDoc)
        {
            ystart = 0;
            pCurrent = m_pSelectedTabGeometry;
            pDoc->m_pLabel->setDiffuseColor(Color(0.0f, 0.0f, 0.0f));
        }
        else
        {
            ystart = yend - m_pUnselectedTabGeometry->getTexture(0)->getHeight();
            pCurrent = m_pUnselectedTabGeometry;
            if (pDoc->m_pDoc->didContentChange()) {
                pDoc->m_pLabel->setDiffuseColor(Color(0.2f, 0.2f, 1.0f));
            } else {
                pDoc->m_pLabel->setDiffuseColor(Color(0.5f, 0.5f, 0.5f));
            }
        }

        xend = xstart + pCurrent->getTexture(0)->getWidth();
        pQuads[i++] = new QuadData(xstart, xend, ystart, yend, pCurrent->getTexture(0));
        xstart = xend;
        xend += boxWidth - pCurrent->getTexture(2)->getWidth() - pCurrent->getTexture(0)->getWidth();
        pQuads[i++] = new QuadData(xstart, xend, ystart, yend, pCurrent->getTexture(1));
        xstart = xend;
        xend += pCurrent->getTexture(2)->getWidth();
        pQuads[i++] = new QuadData(xstart, xend, ystart, yend, pCurrent->getTexture(2));
        xstart = xend;
    }

    if (m_pTabsGeometry == NULL) {
        m_pTabsGeometry = (IGeometryQuads*) m_pUnselectedTabGeometry->clone();
    }
    m_pTabsGeometry->build(nQuads, pQuads);
    QuadData::releaseQuads(nQuads, pQuads);
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void TabbedFrame::update(double delta)
{
    // Update frame and active document
    Frame::update(delta);

    // Update other documents
	for (TabbedFrame_Document* &pDoc : m_pDocumentsList) {
        if (pDoc->m_pDoc != m_pDoc) {
        	pDoc->m_pDoc->update(delta);
        }
    }
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void TabbedFrame::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    Frame::displayAt(iXOffset, iYOffset, cpntColor, docColor);
    if (m_pDocumentsList.empty()) {
        return;
    }

    //if (m_Effect == FE_FadeAlpha)
    //  blendColor = Color *_MULTIPLY(blendColor, m_EffectColor);
    //else
    //  blendColor = Color *_MULTIPLY(blendColor, m_BlendColor);

//  bool bAlpha = blendColor.a >= 0 && blendColor.a < 1.0f;
//  if (bAlpha)
//    getDisplay()->enableBlending();

    int size = m_pDocumentsList.size();
    int boxSize = (m_iWidth - 2 * m_iXSpace) / size;
    if (boxSize < 50)
    {
        boxSize = 50;
        // display with arrows
    }
    else
    {
        m_pTabsGeometry->display(iXOffset + m_iXPxl + m_iXSpace, iYOffset + m_iYPxl, cpntColor);
        int boxWidth = (m_iWidth - 2 * m_iXSpace) / size;
        int i = 0;
    	for (TabbedFrame_Document* &pDoc : m_pDocumentsList) {
            if (pDoc->m_pDoc == m_pDoc) {
            	pDoc->m_pLabel->setDiffuseColor(Color(0.0f, 0.0f, 0.0f));
            } else {
                if (pDoc->m_pDoc->didContentChange()) {
                	pDoc->m_pLabel->setDiffuseColor(Color(0.2f, 0.2f, 1.0f));
                } else {
                	pDoc->m_pLabel->setDiffuseColor(Color(0.5f, 0.5f, 0.5f));
                }
            }

            int xPxl = m_iXPxl + m_iXSpace + i * boxWidth + (boxWidth - pDoc->m_pLabel->getWidth()) / 2;
            int yPxl = m_iYPxl + 3 + (m_pSelectedTabGeometry->getTexture()->getHeight() - pDoc->m_pLabel->getHeight()) / 2;
            pDoc->m_pLabel->displayAt(iXOffset + xPxl, iYOffset + yPxl, cpntColor, docColor);
            i++;
        }
    }
//  if (bAlpha)
//    getDisplay()->disableBlending();
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
Object * TabbedFrame::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible || m_pDoc == NULL) {
        return NULL;
    }

    int iPanel = getIPanel(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset);
    if (iPanel < 0) {
        return Frame::onButtonEvent(pEvent);
    }
    if (pEvent->eButton != Button1 || pEvent->eEvent != EventDown) {
        return NULL;
    }

//    int boxSize = (m_iWidth - 2 * m_iXPanelDecal) / m_pDocumentsList->size;
//    if (boxSize < 50) {
//        boxSize = 50;
//        // special case TODO
//    } else {
//        setMainDocument(((TabbedFrame_Document*) ((*m_pDocumentsList)[iPanel]))->m_pDoc);
//    }
    return NULL;  // We don't need to catch future drag or dbclick events
}

// -----------------------------------------------------------------
// Name : attachDocument
// -----------------------------------------------------------------
void TabbedFrame::attachDocument(Document * pDoc, FrameFitBehavior oldWidthFit, FrameFitBehavior oldHeightFit)
{
    Frame::setDocument(pDoc);
    m_pDocumentsList.push_front(new TabbedFrame_Document(pDoc, oldWidthFit, oldHeightFit, m_pLabelTemplate));
    rebuildGeometry();
}

// -----------------------------------------------------------------
// Name : detachDocument
// -----------------------------------------------------------------
TabbedFrame_Document * TabbedFrame::detachDocument()
{
    Document * pDoc = Frame::unsetDocument();
    if (pDoc == NULL) {
        return NULL;
    }

    int size = m_pDocumentsList.size();
    if (size < 2)
    {
        setDocument(pDoc);
        return NULL;
    }

    TabbedFrame_Document * ret = NULL;
    for (list<TabbedFrame_Document*>::iterator it = m_pDocumentsList.begin(); it != m_pDocumentsList.end(); ++it) {
        if ((*it)->m_pDoc == pDoc)
        {
        	ret = *it;
        	it = m_pDocumentsList.erase(it);
            break;
        }
    }
    if (m_pDocumentsList.empty()) {
        m_pDoc = NULL;
    } else {
        setMainDocument((*(m_pDocumentsList.begin()))->m_pDoc);
    }

    return ret;
}

// -----------------------------------------------------------------
// Name : setMainDocument
// -----------------------------------------------------------------
void TabbedFrame::setMainDocument(Document * pDoc)
{
    if (m_pDoc != pDoc)
    {
        m_pDoc = pDoc;
        rebuildGeometry();
    }
}

// -----------------------------------------------------------------
// Name : isPanelAt
// -----------------------------------------------------------------
bool TabbedFrame::isPanelAt(int xPxl, int yPxl)
{
    return (xPxl >= m_iXPxl + m_iXSpace && xPxl <= m_iXPxl + m_iWidth - m_iXSpace && yPxl >= m_iYPxl && yPxl <= m_iYPxl + m_pSelectedTabGeometry->getTexture()->getHeight());
}

// -----------------------------------------------------------------
// Name : getIPanel
// -----------------------------------------------------------------
short TabbedFrame::getIPanel(int xPxl, int yPxl)
{
    if (!isPanelAt(xPxl, yPxl)) {
        return -1;
    }
    return ((xPxl - m_iXPxl - m_iXSpace) * m_pDocumentsList.size()) / (m_iWidth - 2 * m_iXSpace);
}

// -----------------------------------------------------------------
// Name : isAt
// -----------------------------------------------------------------
bool TabbedFrame::isAt(int xPxl, int yPxl)
{
    if (m_bVisible && isPanelAt(xPxl, yPxl)) {
        return true;
    }
    return Frame::isAt(xPxl, yPxl);
}

// -----------------------------------------------------------------
// Name : moveTo
// -----------------------------------------------------------------
void TabbedFrame::moveTo(int xPxl, int yPxl)
{
    Frame::moveTo(xPxl, yPxl);
}






// -----------------------------------------------------------------
// Name : TabbedFrame_Document
//  Default constructor for TabbedFrame_Document
// -----------------------------------------------------------------
TabbedFrame_Document::TabbedFrame_Document(Document * pDoc, FrameFitBehavior oldWidthFit, FrameFitBehavior oldHeightFit, Label * tpl)
{
    m_pDoc = pDoc;
    m_OldWidthFit = oldWidthFit;
    m_OldHeightFit = oldHeightFit;
    m_pLabel = new Label();
    m_pLabel->withGeometry((IGeometryText*) tpl->getGeometry()->clone())
    		->withText(pDoc->getTitle(), tpl->getFontId(), tpl->getDiffuseColor())
    		->build();
}

// -----------------------------------------------------------------
// Name : ~TabbedFrame_Document
//  Default destructor for TabbedFrame_Document
// -----------------------------------------------------------------
TabbedFrame_Document::~TabbedFrame_Document()
{
    // Do not delete m_pDoc as we may want to delete a TabbedFrame_Document but keep the document
    delete m_pLabel;
}
}
