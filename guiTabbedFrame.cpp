#include "guiTabbedFrame.h"
#include "guiLabel.h"
#include "../Display/TextureEngine.h"
#include "../Display/DisplayEngine.h"
#include "../Input/InputEngine.h"

// -----------------------------------------------------------------
// Name : guiTabbedFrame
//  Constructor
// -----------------------------------------------------------------
guiTabbedFrame::guiTabbedFrame() : guiFrame()
{
    m_pTabsGeometry = NULL;
    m_iXPanelDecal = 0;
    m_FontId = (FontId)0;
    for (int i = 0; i < 6; i++) {
        m_pTexList[i] = NULL;
    }
}

// -----------------------------------------------------------------
// Name : ~guiTabbedFrame
//  Destructor
// -----------------------------------------------------------------
guiTabbedFrame::~guiTabbedFrame()
{
    m_pDoc = NULL;   // it's going to be deleted below
    // We need to explicitly delete all guiDocuments as they won't be deleted from guiTabbedFrame_Document destructor
	for (guiTabbedFrame_Document* &pDoc : m_pDocumentsList) {
        delete pDoc->m_pDoc;
    }
    // Now we can delete the list
    FREEVEC(m_pDocumentsList);
    FREE(m_pTabsGeometry);
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void guiTabbedFrame::init(Texture ** pTabTexs, FontId fontId, int xdecal, FramePosition positionType, FrameFitBehavior widthFit, FrameFitBehavior heightFit, int iMaxWidth, int iMaxHeight, Texture ** pMainTexs, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl)
{
    int iSelTabHeight = pTabTexs[0]->getHeight();
    guiFrame::init(positionType, widthFit, heightFit, 0, iSelTabHeight, iMaxWidth, iMaxHeight, pMainTexs, sCpntId, xPxl, yPxl, wPxl, hPxl);
    memcpy(m_pTexList, pTabTexs, 6 * sizeof(Texture*));
    m_FontId = fontId;
    m_iXPanelDecal = xdecal;
    computeGeometry();
}

// -----------------------------------------------------------------
// Name : clone
// -----------------------------------------------------------------
guiObject * guiTabbedFrame::clone()
{
	Texture * frmtexs[8] = { ((GeometryQuads*)m_pGeometry)->getTexture(0), ((GeometryQuads*)m_pGeometry)->getTexture(1), ((GeometryQuads*)m_pGeometry)->getTexture(2), ((GeometryQuads*)m_pGeometry)->getTexture(3), ((GeometryQuads*)m_pGeometry)->getTexture(4), ((GeometryQuads*)m_pGeometry)->getTexture(5), ((GeometryQuads*)m_pGeometry)->getTexture(6), ((GeometryQuads*)m_pGeometry)->getTexture(7) };
    guiTabbedFrame * pObj = new guiTabbedFrame();
    pObj->init(m_pTexList, m_FontId, m_iXPanelDecal, m_PositionType, m_WidthFit, m_HeightFit, m_iMaxWidth, m_iMaxHeight, frmtexs, m_sCpntId, m_iXPxl, m_iYPxl, m_iWidth, m_iHeight);
    return pObj;
}

// -----------------------------------------------------------------
// Name : computeGeometry
// -----------------------------------------------------------------
void guiTabbedFrame::computeGeometry()
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
    yend = m_pTexList[0]->getHeight();
    int boxWidth = (m_iWidth - 2 * m_iXPanelDecal) / size;
    int i = 0;
    int iTexBase;
	for (guiTabbedFrame_Document* &pDoc : m_pDocumentsList) {
        if (pDoc->m_pDoc == m_pDoc)
        {
            ystart = 0;
            iTexBase = 0;
            pDoc->m_pLabel->setDiffuseColor(Color(0.0f, 0.0f, 0.0f));
        }
        else
        {
            ystart = yend - m_pTexList[3]->getHeight();
            iTexBase = 3;
            if (pDoc->m_pDoc->didContentChange()) {
                pDoc->m_pLabel->setDiffuseColor(Color(0.2f, 0.2f, 1.0f));
            } else {
                pDoc->m_pLabel->setDiffuseColor(Color(0.5f, 0.5f, 0.5f));
            }
        }

        xend = xstart + m_pTexList[0 + iTexBase]->getWidth();
        pQuads[i++] = new QuadData(xstart, xend, ystart, yend, m_pTexList[0 + iTexBase]);
        xstart = xend;
        xend += boxWidth - m_pTexList[2 + iTexBase]->getWidth() - m_pTexList[0 + iTexBase]->getWidth();
        pQuads[i++] = new QuadData(xstart, xend, ystart, yend, m_pTexList[1 + iTexBase]);
        xstart = xend;
        xend += m_pTexList[2 + iTexBase]->getWidth();
        pQuads[i++] = new QuadData(xstart, xend, ystart, yend, m_pTexList[2 + iTexBase]);
        xstart = xend;
    }

    if (m_pTabsGeometry == NULL) {
        m_pTabsGeometry = new GeometryQuads(nQuads, pQuads, VB_Static);
    } else {
        m_pTabsGeometry->modify(nQuads, pQuads);
    }
    QuadData::releaseQuads(nQuads, pQuads);
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void guiTabbedFrame::update(double delta)
{
    // Update frame and active document
    guiFrame::update(delta);

    // Update other documents
	for (guiTabbedFrame_Document* &pDoc : m_pDocumentsList) {
        if (pDoc->m_pDoc != m_pDoc) {
        	pDoc->m_pDoc->update(delta);
        }
    }
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiTabbedFrame::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    guiFrame::displayAt(iXOffset, iYOffset, cpntColor, docColor);
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
    int boxSize = (m_iWidth - 2 * m_iXPanelDecal) / size;
    if (boxSize < 50)
    {
        boxSize = 50;
        // display with arrows
    }
    else
    {
        CoordsScreen coords = CoordsScreen(iXOffset + m_iXPxl + m_iXPanelDecal, iYOffset + m_iYPxl, GUIPLANE);
        m_pTabsGeometry->display(coords, cpntColor);
        int boxWidth = (m_iWidth - 2 * m_iXPanelDecal) / size;
        int i = 0;
    	for (guiTabbedFrame_Document* &pDoc : m_pDocumentsList) {
            if (pDoc->m_pDoc == m_pDoc) {
            	pDoc->m_pLabel->setDiffuseColor(Color(0.0f, 0.0f, 0.0f));
            } else {
                if (pDoc->m_pDoc->didContentChange()) {
                	pDoc->m_pLabel->setDiffuseColor(Color(0.2f, 0.2f, 1.0f));
                } else {
                	pDoc->m_pLabel->setDiffuseColor(Color(0.5f, 0.5f, 0.5f));
                }
            }

            int xPxl = m_iXPxl + m_iXPanelDecal + i * boxWidth + (boxWidth - pDoc->m_pLabel->getWidth()) / 2;
            int yPxl = m_iYPxl + 3 + (m_pTexList[0]->getHeight() - pDoc->m_pLabel->getHeight()) / 2;
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
guiObject * guiTabbedFrame::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible || m_pDoc == NULL) {
        return NULL;
    }

    int iPanel = getIPanel(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset);
    if (iPanel < 0) {
        return guiFrame::onButtonEvent(pEvent);
    }
    if (pEvent->eButton != Button1 || pEvent->eEvent != Event_Down) {
        return NULL;
    }

//    int boxSize = (m_iWidth - 2 * m_iXPanelDecal) / m_pDocumentsList->size;
//    if (boxSize < 50) {
//        boxSize = 50;
//        // special case TODO
//    } else {
//        setMainDocument(((guiTabbedFrame_Document*) ((*m_pDocumentsList)[iPanel]))->m_pDoc);
//    }
    return NULL;  // We don't need to catch future drag or dbclick events
}

// -----------------------------------------------------------------
// Name : attachDocument
// -----------------------------------------------------------------
void guiTabbedFrame::attachDocument(guiDocument * pDoc, FrameFitBehavior OldWidthFit, FrameFitBehavior OldHeightFit)
{
    guiFrame::setDocument(pDoc);
    m_pDocumentsList.push_front(new guiTabbedFrame_Document(pDoc, OldWidthFit, OldHeightFit, m_FontId));
    computeGeometry();
}

// -----------------------------------------------------------------
// Name : detachDocument
// -----------------------------------------------------------------
guiTabbedFrame_Document * guiTabbedFrame::detachDocument()
{
    guiDocument * pDoc = guiFrame::unsetDocument();
    if (pDoc == NULL) {
        return NULL;
    }

    int size = m_pDocumentsList.size();
    if (size < 2)
    {
        setDocument(pDoc);
        return NULL;
    }

    guiTabbedFrame_Document * ret = NULL;
    for (list<guiTabbedFrame_Document*>::iterator it = m_pDocumentsList.begin(); it != m_pDocumentsList.end(); ++it) {
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
void guiTabbedFrame::setMainDocument(guiDocument * pDoc)
{
    if (m_pDoc != pDoc)
    {
        m_pDoc = pDoc;
        computeGeometry();
    }
}

// -----------------------------------------------------------------
// Name : isPanelAt
// -----------------------------------------------------------------
bool guiTabbedFrame::isPanelAt(int xPxl, int yPxl)
{
    return (xPxl >= m_iXPxl + m_iXPanelDecal && xPxl <= m_iXPxl + m_iWidth - m_iXPanelDecal && yPxl >= m_iYPxl && yPxl <= m_iYPxl + (int)m_pTexList[0]->getHeight());
}

// -----------------------------------------------------------------
// Name : getIPanel
// -----------------------------------------------------------------
short guiTabbedFrame::getIPanel(int xPxl, int yPxl)
{
    if (!isPanelAt(xPxl, yPxl)) {
        return -1;
    }
    return ((xPxl - m_iXPxl - m_iXPanelDecal) * m_pDocumentsList.size()) / (m_iWidth - 2 * m_iXPanelDecal);
}

// -----------------------------------------------------------------
// Name : isAt
// -----------------------------------------------------------------
bool guiTabbedFrame::isAt(int xPxl, int yPxl)
{
    if (m_bVisible && isPanelAt(xPxl, yPxl)) {
        return true;
    }
    return guiFrame::isAt(xPxl, yPxl);
}

// -----------------------------------------------------------------
// Name : moveTo
// -----------------------------------------------------------------
void guiTabbedFrame::moveTo(int xPxl, int yPxl)
{
    guiFrame::moveTo(xPxl, yPxl);
}

// -----------------------------------------------------------------
// Name : createDefaultTabbedFrame
//  Static default constructor
// -----------------------------------------------------------------
guiTabbedFrame * guiTabbedFrame::createDefaultTabbedFrame(FrameFitBehavior widthFit, FrameFitBehavior heightFit, int width, int height, bool bAlpha, string sId)
{
    guiTabbedFrame * pFrame = new guiTabbedFrame();
    Texture * iTabs[6];
    iTabs[0] = _tex->findTexture("gui/interface:TabSelLeft");
    iTabs[1] = _tex->findTexture("gui/interface:TabSelMiddle");
    iTabs[2] = _tex->findTexture("gui/interface:TabSelRight");
    iTabs[3] = _tex->findTexture("gui/interface:TabLeft");
    iTabs[4] = _tex->findTexture("gui/interface:TabMiddle");
    iTabs[5] = _tex->findTexture("gui/interface:TabRight");
    Texture * iTexs[8];
    iTexs[0] = _tex->findTexture("gui/interface:FrmTL");
    iTexs[1] = _tex->findTexture("gui/interface:FrmTC");
    iTexs[2] = _tex->findTexture("gui/interface:FrmTR");
    iTexs[3] = _tex->findTexture("gui/interface:FrmCL");
    iTexs[4] = _tex->findTexture("gui/interface:FrmCR");
    iTexs[5] = _tex->findTexture("gui/interface:FrmBL");
    iTexs[6] = _tex->findTexture("gui/interface:FrmBC");
    iTexs[7] = _tex->findTexture("gui/interface:FrmBR");
    pFrame->init(iTabs, Bookantiqua_wh_16,
                 3, FP_Floating, widthFit, heightFit, 0, 0,
                 iTexs, sId, 0, 0, width, height);
    return pFrame;
}







// -----------------------------------------------------------------
// Name : guiTabbedFrame_Document
//  Default constructor for guiTabbedFrame_Document
// -----------------------------------------------------------------
guiTabbedFrame_Document::guiTabbedFrame_Document(guiDocument * pDoc, FrameFitBehavior OldWidthFit, FrameFitBehavior OldHeightFit, FontId fontId)
{
    m_pDoc = pDoc;
    m_OldWidthFit = OldWidthFit;
    m_OldHeightFit = OldHeightFit;
    m_pLabel = new guiLabel();
    m_pLabel->init(pDoc->getTitle(), fontId, Color(0,0,0), "", 0, 0, 0, 0);
}

// -----------------------------------------------------------------
// Name : ~guiTabbedFrame_Document
//  Default destructor for guiTabbedFrame_Document
// -----------------------------------------------------------------
guiTabbedFrame_Document::~guiTabbedFrame_Document()
{
    // Do not delete m_pDoc as we may want to delete a guiTabbedFrame_Document but keep the document
    delete m_pLabel;
}
