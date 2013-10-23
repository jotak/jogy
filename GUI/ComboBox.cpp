#include "ComboBox.h"
#include "../jogy.h"

#define LABEL_XOFFSET	5
#define LABEL_YOFFSET	3

namespace jogy {

// -----------------------------------------------------------------
// Name : ComboBox
//  Constructor
// ---------------------------------------------
ComboBox::ComboBox() : Component()
{
	m_pMainGeometry = NULL;
    m_pLabel = new Label();
    m_pList = new Container();
    m_pList->setDocument(new Document());
    m_pList->setVisible(false);
    m_pListButtonTemplate = new Button();
    m_dListPos = -1;
}

// -----------------------------------------------------------------
// Name : ~ComboBox
//  Destructor
// -----------------------------------------------------------------
ComboBox::~ComboBox()
{
    Jogy::interface->cancelDisplayFront(m_pList);
    delete m_pList;
    delete m_pLabel;
    delete m_pListButtonTemplate;
    FREE(m_pMainGeometry);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
ComboBox * ComboBox::build()
{
    Component::build();

    // Build label
    m_pLabel->withPosition(m_iXPxl + LABEL_XOFFSET, m_iYPxl + LABEL_YOFFSET);
    m_pLabel->build();

    // Build inner list
    m_pList->withFrameFitBehavior(FB_FitDocumentToFrame, FB_FitFrameToDocumentWhenSmaller)
    		->withPosition(m_iXPxl, m_iYPxl + m_iHeight)
    		->withDimensions(m_iWidth, m_pList->getMaxHeight());
    m_pList->build();

    // Build list's document
    m_pList->getDocument()->withDimensions(m_pList->getInnerWidth(), m_pList->getInnerHeight());
    m_pList->getDocument()->build();

    // Configure list buttons
    m_pListButtonTemplate->withDimensions(m_pList->getInnerWidth(), m_pListButtonTemplate->getHeight());
    m_pListButtonTemplate->withInputBehaviour(None, m_pListButtonTemplate->getHoverOption());

    rebuildGeometry();

    return this;
}

// -----------------------------------------------------------------
// Name : withMainGeometry
// -----------------------------------------------------------------
ComboBox * ComboBox::withMainGeometry(ITexture ** pTex, IGeometryQuads * pGeo)
{
    m_pMainGeometry = pGeo;
    QuadData ** pQuads = new QuadData*[3];
    pQuads[0] = new QuadData(0, 1, 0, 3, pTex[0]);
    pQuads[1] = new QuadData(1, 2, 0, 3, pTex[1]);
    pQuads[2] = new QuadData(2, 3, 0, 3, pTex[2]);
    m_pMainGeometry->build(3, pQuads);
    QuadData::releaseQuads(3, pQuads);
	return this;
}

// -----------------------------------------------------------------
// Name : rebuildGeometry
// -----------------------------------------------------------------
void ComboBox::rebuildGeometry()
{
    QuadData ** pQuads = new QuadData*[3];
    int xPxlMiddleStart = m_pMainGeometry->getTexture(0)->getWidth();
    int xPxlMiddleEnd = m_iWidth - m_pMainGeometry->getTexture(2)->getWidth();
    pQuads[0] = new QuadData(0, xPxlMiddleStart, 0, m_iHeight, m_pMainGeometry->getTexture(0));
    pQuads[1] = new QuadData(xPxlMiddleStart, xPxlMiddleEnd, 0, m_iHeight, m_pMainGeometry->getTexture(1));
    pQuads[2] = new QuadData(xPxlMiddleEnd, m_iWidth, 0, m_iHeight, m_pMainGeometry->getTexture(2));
    m_pMainGeometry->build(3, pQuads);
    QuadData::releaseQuads(3, pQuads);
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void ComboBox::update(double delta)
{
    Component::update(delta);
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
void ComboBox::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }
    m_pMainGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor);
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
        Jogy::interface->displayFront(m_pList, iXOffset, iYOffset, cpntColor, docColor);
    }
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
Object * ComboBox::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible) {
        return NULL;
    }
    if (pEvent->eButton == Button1 && pEvent->eEvent == EventDown)
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
void ComboBox::onFocusLost()
{
    m_pList->setVisible(false);
}

// -----------------------------------------------------------------
// Name : onButtonEvent
//  This function will be called when a list button is clicked.
// -----------------------------------------------------------------
bool ComboBox::onButtonEvent(ButtonAction * pEvent, Component * pCpnt)
{
    m_pList->setVisible(false);
    m_pLabel->setText(((Button*)pCpnt)->getText());
    centerLabel();
    return m_pOwner->onButtonEvent(pEvent, pCpnt);
}

// -----------------------------------------------------------------
// Name : isAt
// -----------------------------------------------------------------
bool ComboBox::isAt(int xPxl, int yPxl)
{
    if (!m_bVisible) {
        return false;
    }

    if (m_pList->isVisible()) {
        return Component::isAt(xPxl, yPxl) || m_pList->isAt(xPxl, yPxl);
    } else {
        return Component::isAt(xPxl, yPxl);
    }
}

// -----------------------------------------------------------------
// Name : addString
// -----------------------------------------------------------------
Button * ComboBox::addString(string sText, string sId)
{
    // Create button
    Button * pBtn = new Button();
    pBtn->withInputBehaviour(m_pListButtonTemplate->getClickedOption(), m_pListButtonTemplate->getHoverOption())
    		->withLabel(
    				sText,
    				m_pListButtonTemplate->getLabel()->getFontId(),
    				m_pListButtonTemplate->getLabel()->getDiffuseColor(),
    				(IGeometryText*) m_pListButtonTemplate->getLabel()->getGeometry()->clone())
    		->withHoverGeometry(m_pListButtonTemplate->getHoverGeometry()->getTexture(),
    				(IGeometryQuads*) m_pListButtonTemplate->getHoverGeometry()->clone())
    		->withCpntId(sId)
    		->withDimensions(m_pListButtonTemplate->getWidth(), m_pListButtonTemplate->getHeight());

    // Find lowest existing button in list
    int yPxl = 0;
	for (Component* &pCpnt : m_pList->getDocument()->getComponents()) {
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
void ComboBox::setItem(int id)
{
    if (id < 0) {
        m_pLabel->setText("");
    } else {
    	Button * pBtn = getItem(id);
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
Button * ComboBox::getItem(string sId)
{
    return (Button*) m_pList->getDocument()->getComponent(sId);
}

// -----------------------------------------------------------------
// Name : getItem
// -----------------------------------------------------------------
Button * ComboBox::getItem(u16 uId)
{
	u16 counter = 0;
	for (Component* &pCpnt : m_pList->getDocument()->getComponents()) {
		if (counter == uId) {
			if (pCpnt->getType() & GOTYPE_BUTTON) {
				return (Button*) pCpnt;
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
Button * ComboBox::getSelectedItem()
{
//    Button * pBtn = (Button*) m_pList->getDocument()->getFirstComponent();
//    while (pBtn != NULL)
//    {
//        if (strcmp(pBtn->getText(), m_pLabel->getText()) == 0)
//            return pBtn;
//        pBtn = (Button*) m_pList->getDocument()->getNextComponent();
//    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : getSelectedItemId
// -----------------------------------------------------------------
int ComboBox::getSelectedItemId()
{
//    int id = 0;
//    Button * pBtn = (Button*) m_pList->getDocument()->getFirstComponent();
//    while (pBtn != NULL)
//    {
//        if (strcmp(pBtn->getText(), m_pLabel->getText()) == 0)
//            return id;
//        id++;
//        pBtn = (Button*) m_pList->getDocument()->getNextComponent();
//    }
    return -1;
}

// -----------------------------------------------------------------
// Name : getItemsCount
// -----------------------------------------------------------------
u16 ComboBox::getItemsCount()
{
    return (u16) m_pList->getDocument()->getComponents().size();
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void ComboBox::onResize(int iOldWidth, int iOldHeight)
{
    Component::onResize(iOldWidth, iOldHeight);
    if (m_iWidth == iOldWidth && m_iHeight == iOldHeight)
        return;
    m_pList->setWidth(m_iWidth);
    centerLabel();
    if (m_pMainGeometry != NULL) {
        rebuildGeometry();
    }
}

// -----------------------------------------------------------------
// Name : centerLabel
// -----------------------------------------------------------------
void ComboBox::centerLabel()
{
    if (m_pMainGeometry != NULL)
    {
        int w1 = m_pMainGeometry->getTexture(0)->getWidth();
        int w2 = getWidth() - m_pMainGeometry->getTexture(2)->getWidth() - w1;
        m_pLabel->moveTo(getXPos() + w1 + w2 / 2 - m_pLabel->getWidth() / 2, getYPos() + getHeight() / 2 - m_pLabel->getHeight() / 2);
    }
}

// -----------------------------------------------------------------
// Name : setWidth
// -----------------------------------------------------------------
void ComboBox::setWidth(int iWidth)
{
    Component::setWidth(iWidth);
    if (iWidth > m_pList->getMaxWidth()) {
        m_pList->setMaxWidth(iWidth);
    }
    m_pList->setWidth(iWidth);
    m_pListButtonTemplate->setWidth(m_pList->getInnerWidth());
}

// -----------------------------------------------------------------
// Name : setDimensions
// -----------------------------------------------------------------
void ComboBox::setDimensions(int iWidth, int iHeight)
{
    Component::setDimensions(iWidth, iHeight);
    if (iWidth > m_pList->getMaxWidth()) {
        m_pList->setMaxWidth(iWidth);
    }
    m_pList->setWidth(iWidth);
    m_pListButtonTemplate->setWidth(m_pList->getInnerWidth());
}

// -----------------------------------------------------------------
// Name : moveTo
// -----------------------------------------------------------------
void ComboBox::moveTo(int xPxl, int yPxl)
{
    Component::moveTo(xPxl, yPxl);
    m_pList->moveTo(xPxl, getHeight() + yPxl);
    centerLabel();
}

// -----------------------------------------------------------------
// Name : moveBy
// -----------------------------------------------------------------
void ComboBox::moveBy(int xPxl, int yPxl)
{
    Component::moveBy(xPxl, yPxl);
    m_pList->moveBy(xPxl, yPxl);
    centerLabel();
}

// -----------------------------------------------------------------
// Name : clearList
// -----------------------------------------------------------------
void ComboBox::clearList()
{
    m_pList->getDocument()->deleteAllComponents();
    m_pLabel->setText("");
}
}
