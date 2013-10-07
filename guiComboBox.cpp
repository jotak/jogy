#include "guiComboBox.h"
#include "../Display/DisplayEngine.h"
#include "../Display/TextureEngine.h"
#include "../Managers/InterfaceManager.h"

// -----------------------------------------------------------------
// Name : guiComboBox
//  Constructor
// -----------------------------------------------------------------
guiComboBox::guiComboBox() : guiComponent()
{
    m_pLabel = new guiLabel();
    m_pList = new guiContainer();
    m_pList->setDocument(new guiDocument());
    m_pList->setVisible(false);
    m_pListButtonTemplate = new guiButton();
    m_dListPos = -1;
}

// -----------------------------------------------------------------
// Name : ~guiComboBox
//  Destructor
// -----------------------------------------------------------------
guiComboBox::~guiComboBox()
{
    _interface->cancelTopDisplay(m_pList);
    delete m_pList;
    delete m_pLabel;
    delete m_pListButtonTemplate;
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void guiComboBox::init(Texture ** pMainTex, Texture * pDocTex, Color textColor, FontId fontId, FrameFitBehavior wfit, FrameFitBehavior hfit, int iMaxWidth, int iMaxHeight, Texture * btnTex1, Texture * btnTex2, BtnClickOptions btnClickOpt, int btnHeight, Texture ** frameTexs, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl)
{
    guiComponent::init(sCpntId, xPxl, yPxl, wPxl, hPxl);
    m_pLabel->init("", fontId, textColor, "", xPxl + 5, yPxl + 3, 0, 0);
    m_pList->init(wfit, hfit, 0, 0, iMaxWidth, iMaxHeight, frameTexs, "ComboContainer", xPxl, yPxl + hPxl, wPxl, iMaxHeight);
    m_pList->getDocument()->init("ComboDoc", pDocTex, 0, 0, m_pList->getInnerWidth(), m_pList->getInnerHeight());
    m_pListButtonTemplate->init("", fontId, textColor, NULL, BCO_None, btnTex2, btnClickOpt, btnTex1, "TemplateComboButton", 0, 0, m_pList->getInnerWidth(), btnHeight);

    QuadData ** pQuads;
    int nQuads = computeQuadsList(&pQuads, pMainTex);
    m_pGeometry = new GeometryQuads(nQuads, pQuads, VB_Static);
    QuadData::releaseQuads(nQuads, pQuads);
}

// -----------------------------------------------------------------
// Name : clone
// -----------------------------------------------------------------
guiObject * guiComboBox::clone()
{
    Texture * texlist[3] = { ((GeometryQuads*)m_pGeometry)->getTexture(0), ((GeometryQuads*)m_pGeometry)->getTexture(1), ((GeometryQuads*)m_pGeometry)->getTexture(2) };
    GeometryQuads * pListGeo = (GeometryQuads*) m_pList->getGeometry();
    Texture * lsttexlist[8] = { pListGeo->getTexture(0), pListGeo->getTexture(1), pListGeo->getTexture(2), pListGeo->getTexture(3), pListGeo->getTexture(4), pListGeo->getTexture(5), pListGeo->getTexture(6), pListGeo->getTexture(7) };
    guiComboBox * pObj = new guiComboBox();
    pObj->init(
        texlist,
        ((GeometryQuads*)m_pList->getDocument()->getGeometry())->getTexture(),
        m_pLabel->getDiffuseColor(),
        m_pLabel->getFontId(),
        m_pList->getWidthFitBehavior(),
        m_pList->getHeightFitBehavior(),
        m_pList->getMaxWidth(),
        m_pList->getMaxHeight(),
        m_pListButtonTemplate->getNormalGeometry()->getTexture(),
        m_pListButtonTemplate->getClickedGeometry()->getTexture(),
        m_pListButtonTemplate->getClickOption(),
        m_pListButtonTemplate->getHeight(),
        lsttexlist,
        m_sCpntId, m_iXPxl, m_iYPxl, m_iWidth, m_iHeight);
    return pObj;
}

// -----------------------------------------------------------------
// Name : computeQuadsList
// -----------------------------------------------------------------
int guiComboBox::computeQuadsList(QuadData *** pQuads, Texture ** pTextures)
{
    *pQuads = new QuadData*[3];
    int xPxlMiddleStart = pTextures[0]->getWidth();
    int xPxlMiddleEnd = m_iWidth - pTextures[2]->getWidth();
    (*pQuads)[0] = new QuadData(0, xPxlMiddleStart, 0, m_iHeight, pTextures[0]);
    (*pQuads)[1] = new QuadData(xPxlMiddleStart, xPxlMiddleEnd, 0, m_iHeight, pTextures[1]);
    (*pQuads)[2] = new QuadData(xPxlMiddleEnd, m_iWidth, 0, m_iHeight, pTextures[2]);
    return 3;
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void guiComboBox::update(double delta)
{
    guiComponent::update(delta);
    if (m_pList->isVisible()) {
        m_pList->update(delta);
    }
    if (m_dListPos >= 0)
    {
        m_dListPos += 2*delta;
        if (m_dListPos >= 1) {
            m_dListPos = -1;
        }
    }
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiComboBox::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    CoordsScreen coords = CoordsScreen(m_iXPxl + iXOffset, m_iYPxl + iYOffset, GUIPLANE);
    m_pGeometry->display(coords, cpntColor);
    m_pLabel->displayAt(iXOffset, iYOffset, cpntColor, docColor);

    if (m_pList->isVisible())
    {
        if (m_dListPos >= 0)
        {
        	Color c(1.0f, 1.0f, 1.0f, m_dListPos);
            cpntColor.multiply(&c);
            docColor.multiply(&c);
        }
        if (docColor.a >= 0) {
            docColor.a = (docColor.a + 3) / 4;
        }
        _interface->topDisplay(m_pList, iXOffset, iYOffset, &cpntColor, &docColor);
    }
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiComboBox::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible) {
        return NULL;
    }
    if (pEvent->eButton == Button1 && pEvent->eEvent == Event_Down)
    {
        m_pOwner->bringAbove(this);
        if (m_pList->isAt(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset)) {
            return m_pList->onButtonEvent(pEvent);
        } else {
            if (m_pList->isVisible())
            {
                m_pList->setVisible(false);
                m_dListPos = -1;
            }
            else
            {
                m_pList->setVisible(true);
                m_dListPos = 0;
            }
        }
        return this;
    } else if ((pEvent->eButton == ButtonZ || pEvent->eButton == ButtonX) && m_pList->isVisible()) {
        return m_pList->onButtonEvent(pEvent);
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : onFocusLost
// -----------------------------------------------------------------
void guiComboBox::onFocusLost()
{
    m_pList->setVisible(false);
}

// -----------------------------------------------------------------
// Name : onButtonEvent
//  This function will be called when a list button is clicked.
// -----------------------------------------------------------------
bool guiComboBox::onButtonEvent(ButtonAction * pEvent, guiComponent * pCpnt)
{
    m_pList->setVisible(false);
    m_pLabel->setText(((guiButton*)pCpnt)->getText());
    centerLabel();
    return m_pOwner->onButtonEvent(pEvent, pCpnt);
}

// -----------------------------------------------------------------
// Name : isAt
// -----------------------------------------------------------------
bool guiComboBox::isAt(int xPxl, int yPxl)
{
    if (!m_bVisible) {
        return false;
    }

    if (m_pList->isVisible()) {
        return guiComponent::isAt(xPxl, yPxl) || m_pList->isAt(xPxl, yPxl);
    } else {
        return guiComponent::isAt(xPxl, yPxl);
    }
}

// -----------------------------------------------------------------
// Name : addString
// -----------------------------------------------------------------
guiButton * guiComboBox::addString(string sText, string sId)
{
    // Create button
    guiButton * pBtn = (guiButton*) m_pListButtonTemplate->clone();
    pBtn->setId(sId);
    pBtn->setText(sText);

    // Find lowest existing button in list
    int yPxl = 0;
	for (guiComponent* &pCpnt : m_pList->getDocument()->getComponents()) {
        yPxl += pCpnt->getHeight();
    }

    // Update new button info and add to document
    pBtn->moveBy(0, yPxl);
    m_pList->getDocument()->addComponent(pBtn);
    pBtn->setOwner(this); // change button owner to catch clicks
    m_pList->getDocument()->setHeight(yPxl + pBtn->getHeight());

    return pBtn;
}

// -----------------------------------------------------------------
// Name : setItem
// -----------------------------------------------------------------
void guiComboBox::setItem(int id)
{
    if (id < 0) {
        m_pLabel->setText("");
    } else {
    	guiButton * pBtn = getItem(id);
        if (pBtn != NULL) {
            m_pLabel->setText(pBtn->getText());
            centerLabel();
        } else {
        	m_pLabel->setText("");
        }
    }
}

// -----------------------------------------------------------------
// Name : getItem
// -----------------------------------------------------------------
guiButton * guiComboBox::getItem(string sId)
{
    return (guiButton*) m_pList->getDocument()->getComponent(sId);
}

// -----------------------------------------------------------------
// Name : getItem
// -----------------------------------------------------------------
guiButton * guiComboBox::getItem(u16 uId)
{
	u16 counter = 0;
	for (guiComponent* &pCpnt : m_pList->getDocument()->getComponents()) {
		if (counter == uId) {
			if (pCpnt->getType() & GOTYPE_BUTTON) {
				return (guiButton*) pCpnt;
			} else {
				return NULL;
			}
		}
		counter++;
	}
    return NULL;
}

// -----------------------------------------------------------------
// Name : getSelectedItem
// -----------------------------------------------------------------
guiButton * guiComboBox::getSelectedItem()
{
//    guiButton * pBtn = (guiButton*) m_pList->getDocument()->getFirstComponent();
//    while (pBtn != NULL)
//    {
//        if (strcmp(pBtn->getText(), m_pLabel->getText()) == 0)
//            return pBtn;
//        pBtn = (guiButton*) m_pList->getDocument()->getNextComponent();
//    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : getSelectedItemId
// -----------------------------------------------------------------
int guiComboBox::getSelectedItemId()
{
//    int id = 0;
//    guiButton * pBtn = (guiButton*) m_pList->getDocument()->getFirstComponent();
//    while (pBtn != NULL)
//    {
//        if (strcmp(pBtn->getText(), m_pLabel->getText()) == 0)
//            return id;
//        id++;
//        pBtn = (guiButton*) m_pList->getDocument()->getNextComponent();
//    }
    return -1;
}

// -----------------------------------------------------------------
// Name : getItemsCount
// -----------------------------------------------------------------
u16 guiComboBox::getItemsCount()
{
    return (u16) m_pList->getDocument()->getComponents().size();
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void guiComboBox::onResize(int iOldWidth, int iOldHeight)
{
    guiComponent::onResize(iOldWidth, iOldHeight);
    if (m_iWidth == iOldWidth && m_iHeight == iOldHeight)
        return;
    m_pList->setWidth(m_iWidth);
    centerLabel();
    if (m_pGeometry != NULL)
    {
    	Texture * texlist[3] = { ((GeometryQuads*)m_pGeometry)->getTexture(0), ((GeometryQuads*)m_pGeometry)->getTexture(1), ((GeometryQuads*)m_pGeometry)->getTexture(2) };
        QuadData ** pQuads;
        int nQuads = computeQuadsList(&pQuads, texlist);
        ((GeometryQuads*)m_pGeometry)->modify(nQuads, pQuads);
        QuadData::releaseQuads(nQuads, pQuads);
    }
}

// -----------------------------------------------------------------
// Name : centerLabel
// -----------------------------------------------------------------
void guiComboBox::centerLabel()
{
    if (m_pGeometry != NULL)
    {
        int w1 = ((GeometryQuads*)m_pGeometry)->getTexture(0)->getWidth();
        int w2 = getWidth() - ((GeometryQuads*)m_pGeometry)->getTexture(2)->getWidth() - w1;
        m_pLabel->moveTo(getXPos() + w1 + w2 / 2 - m_pLabel->getWidth() / 2, getYPos() + getHeight() / 2 - m_pLabel->getHeight() / 2);
    }
}

// -----------------------------------------------------------------
// Name : setWidth
// -----------------------------------------------------------------
void guiComboBox::setWidth(int iWidth)
{
    guiComponent::setWidth(iWidth);
    if (iWidth > m_pList->getMaxWidth()) {
        m_pList->setMaxWidth(iWidth);
    }
    m_pList->setWidth(iWidth);
    m_pListButtonTemplate->setWidth(m_pList->getInnerWidth());
}

// -----------------------------------------------------------------
// Name : setDimensions
// -----------------------------------------------------------------
void guiComboBox::setDimensions(int iWidth, int iHeight)
{
    guiComponent::setDimensions(iWidth, iHeight);
    if (iWidth > m_pList->getMaxWidth()) {
        m_pList->setMaxWidth(iWidth);
    }
    m_pList->setWidth(iWidth);
    m_pListButtonTemplate->setWidth(m_pList->getInnerWidth());
}

// -----------------------------------------------------------------
// Name : moveTo
// -----------------------------------------------------------------
void guiComboBox::moveTo(int xPxl, int yPxl)
{
    guiComponent::moveTo(xPxl, yPxl);
    m_pList->moveTo(xPxl, getHeight() + yPxl);
    centerLabel();
}

// -----------------------------------------------------------------
// Name : moveBy
// -----------------------------------------------------------------
void guiComboBox::moveBy(int xPxl, int yPxl)
{
    guiComponent::moveBy(xPxl, yPxl);
    m_pList->moveBy(xPxl, yPxl);
    centerLabel();
}

// -----------------------------------------------------------------
// Name : clearList
// -----------------------------------------------------------------
void guiComboBox::clearList()
{
    m_pList->getDocument()->deleteAllComponents();
    m_pLabel->setText("");
}

// -----------------------------------------------------------------
// Name : createDefaultComboBox
//  Static default constructor
//  Use it to avoid passing always the same 3591218 arguments to "init"
// -----------------------------------------------------------------
guiComboBox * guiComboBox::createDefaultComboBox(string sId)
{
    guiComboBox * pCombo = new guiComboBox();
    Texture * maintexs[3];
    maintexs[0] = _tex->findTexture("gui/interface:ComboLeft");
    maintexs[1] = _tex->findTexture("gui/interface:ComboMiddle");
    maintexs[2] = _tex->findTexture("gui/interface:ComboRight");
    Texture * frmtex[8];
    frmtex[0] = _tex->findTexture("gui/interface:ComboListBorder");
    frmtex[1] = _tex->findTexture("gui/interface:ComboListBorder");
    frmtex[2] = _tex->findTexture("gui/interface:ComboListBorder");
    frmtex[3] = _tex->findTexture("gui/interface:ComboListBorder");
    frmtex[4] = _tex->findTexture("gui/interface:ComboListBorder");
    frmtex[5] = _tex->findTexture("gui/interface:ComboListBorder");
    frmtex[6] = _tex->findTexture("gui/interface:ComboListBorder");
    frmtex[7] = _tex->findTexture("gui/interface:ComboListBorder");
    pCombo->init(maintexs,
    		_tex->findTexture("gui/interface:ComboListBg"),
    		TEXT_COLOR, TEXT_FONT, FB_FitDocumentToFrame, FB_FitFrameToDocumentWhenSmaller,
    		0, 200,
    		_tex->findTexture("gui/interface:Transparent"),
    		_tex->findTexture("gui/interface:Transparent"),
    		BCO_Enlight, 16,
    		frmtex, sId, 0, 0, 200, 28);
    return pCombo;
}
