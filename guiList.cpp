#include "guiList.h"

// -----------------------------------------------------------------
// Name : guiList
// -----------------------------------------------------------------
guiList::guiList() : EventListener(0), m_TextColor(1,1,1,1)
{
    m_pSelectionGeometry = NULL;
    m_FontId = 0;
    m_bHasFocus = false;
    m_pLastSelectedLabel = NULL;
    m_bCatchMouseUp = false;
    m_ActionOnSelection = (InputButton)0;
}

// -----------------------------------------------------------------
// Name : ~guiList
//  Destructor
// -----------------------------------------------------------------
guiList::~guiList()
{
    if (m_bHasFocus) {
        onFocusLost();
    }
    FREE(m_pSelectionGeometry);
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void guiList::init(fontid fontId, Color textColor, ITexture ** pMainTexs, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl)
{
    guiContainer::init(FB_FitDocumentToFrameWhenSmaller, FB_FitDocumentToFrameWhenSmaller, 0, 0, 0, 0, pMainTexs, sCpntId, xPxl, yPxl, wPxl, hPxl);
    m_FontId = fontId;
    m_TextColor = textColor;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiList::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }

    guiContainer::displayAt(iXOffset, iYOffset, cpntColor, docColor);

    if (m_pSelectionGeometry != NULL)
    {
        // display selection
        m_pStencilGeometry->fillStencil(m_iInnerXPxl + iXOffset, m_iInnerYPxl + iYOffset, true);
//        int iPreviousState = _display->setStencilState(2);
        m_pSelectionGeometry->display(
        		m_iInnerXPxl + iXOffset + getDocument()->getXPos(),
        		m_iInnerYPxl + iYOffset + getDocument()->getYPos(),
        		cpntColor * Color(0.4, 0.4, 0.4, 0.5));
        m_pStencilGeometry->fillStencil(m_iInnerXPxl + iXOffset, m_iInnerYPxl + iYOffset, false);
//        _display->setStencilState(iPreviousState);
    }
}

// -----------------------------------------------------------------
// Name : onCatchButtonEvent
//  Cursor-independant buttons (directly called from inputs)
// -----------------------------------------------------------------
bool guiList::onCatchButtonEvent(ButtonAction * pEvent)
{
	list<guiComponent*> lstCpnt = getDocument()->getComponents();
    if ((pEvent->eEvent != Event_Down && pEvent->eEvent != Event_DoubleClick) || lstCpnt.empty()) {
        return false;
    }
    switch (pEvent->eButton)
    {
    case ButtonUp:
    case ButtonDown:
    {
        guiListLabel * pClicked = NULL;
        if (m_pLastSelectedLabel == NULL) {
            pClicked = (guiListLabel*) *(lstCpnt.begin());
        } else {
        	// Find previous selection
        	if (pEvent->eButton == ButtonUp) {
        		for (list<guiComponent*>::reverse_iterator it = lstCpnt.rbegin(); it != lstCpnt.rend(); ++it) {
        			if (*it == m_pLastSelectedLabel) {
        				pClicked = (guiListLabel*) *(++it);
        				break;
        			}
        		}
        	} else {
        		// ButtonDown
        		for (list<guiComponent*>::iterator it = lstCpnt.begin(); it != lstCpnt.end(); ++it) {
        			if (*it == m_pLastSelectedLabel) {
        				pClicked = (guiListLabel*) *(++it);
        				break;
        			}
        		}
        	}
        }
        if (pClicked != NULL)
        {
            if (_input->isShiftPressed()) {
                shiftSelect(pClicked);
            } else {
        		for (list<guiComponent*>::iterator it = lstCpnt.begin(); it != lstCpnt.end(); ++it) {
                    ((guiListLabel*)*it)->setSelected(*it == pClicked);
                }
                updateSelectionGeometry();
            }
            m_pLastSelectedLabel = pClicked;
        }
        break;
    }
    default:
    {
        if (pEvent->eEvent == Event_Down) {
            m_ActionOnSelection = pEvent->eButton;
        }
        break;
    }
    }
    return false;
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiList::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible || m_pDoc == NULL) {
        return NULL;
    }

    if (pEvent->eButton == Button1 && pEvent->eEvent == Event_Down)
    {
        // Get focus
        setFocus();
        m_bCatchMouseUp = false;
        guiObject * pObj = guiContainer::onButtonEvent(pEvent);
        if (pObj == NULL)
        {
            // Deselect all
    		for (guiComponent* &pCpnt : getDocument()->getComponents()) {
                ((guiListLabel*)pCpnt)->setSelected(false);
            }
            m_pLastSelectedLabel = NULL;
            updateSelectionGeometry();
        }
        return pObj;
    }
    return guiContainer::onButtonEvent(pEvent);
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
bool guiList::onButtonEvent(ButtonAction * pEvent, guiComponent * pCpnt)
{
    if (pEvent->eEvent == Event_Down)
    {
        if (pEvent->eButton == Button2) {
            return false;
        }

        guiListLabel * pClickedLabel = (guiListLabel*) pCpnt;
        if (pEvent->eButton == Button1 && _input->isShiftPressed()) {
            shiftSelect(pClickedLabel);
        } else if ((pEvent->eButton == Button1 && _input->isCtrlPressed()) || pEvent->eButton == Button2) {
            // Add/remove current label to/from selection
            pClickedLabel->setSelected(!pClickedLabel->isSelected());
            updateSelectionGeometry();
        }
        else
        {
            if (!pClickedLabel->isSelected())
            {
        		for (guiComponent* &pCpnt : getDocument()->getComponents()) {
                    ((guiListLabel*)pCpnt)->setSelected(pCpnt == pClickedLabel);
                }
                updateSelectionGeometry();
            } else { // We'll do deselection on mouse up to allow dragging
                m_bCatchMouseUp = true;
            }
        }
        m_pLastSelectedLabel = pClickedLabel;
        return true;
    }
    else if (pEvent->eEvent == Event_Drag)
    {
        m_bCatchMouseUp = false;
        return m_pOwner->onButtonEvent(pEvent, this);
    }
    else if (pEvent->eEvent == Event_Up)
    {
        bool bReturn = true;
        if (m_bCatchMouseUp && m_pLastSelectedLabel != NULL)
        {
            // Clear current selection and add label to it
    		for (guiComponent* &pCpnt : getDocument()->getComponents()) {
                ((guiListLabel*)pCpnt)->setSelected(pCpnt == m_pLastSelectedLabel);
            }
            updateSelectionGeometry();
        } else if (!m_bCatchMouseUp) {
            bReturn = m_pOwner->onButtonEvent(pEvent, this);
        }
        m_bCatchMouseUp = false;
        return bReturn;
    }
    return true;
}

// -----------------------------------------------------------------
// Name : shiftSelect
// -----------------------------------------------------------------
void guiList::shiftSelect(guiListLabel * pClickedLabel)
{
    if (m_pLastSelectedLabel == NULL)
    {
        // Nothing selected yet ; only select clicked label
        pClickedLabel->setSelected(true);
    }
    else
    {
        // Select or deselect all labels between last selected label and current label
        bool bSelect = !pClickedLabel->isSelected();
        pClickedLabel->setSelected(bSelect);
        m_pLastSelectedLabel->setSelected(bSelect);
        if (pClickedLabel != m_pLastSelectedLabel)
        {
            bool bInBlock = false;
    		for (guiComponent* &pCpnt : getDocument()->getComponents()) {
                if (!bInBlock && (pCpnt == pClickedLabel || pCpnt == m_pLastSelectedLabel)) {
                    bInBlock = true;
                } else if (bInBlock) {
                    if (pCpnt == pClickedLabel || pCpnt == m_pLastSelectedLabel) {
                        break;
                    }
                    ((guiListLabel*)pCpnt)->setSelected(bSelect);
                }
            }
        }
    }
    updateSelectionGeometry();
}

// -----------------------------------------------------------------
// Name : onFocusLost
// -----------------------------------------------------------------
void guiList::onFocusLost()
{
    m_bHasFocus = false;
    _input->unsetKeyboardListener(this);
    EventListener * p = _input->popUncursoredEventListener();
    if (p != this && p != NULL) { // oops
    	_input->pushUncursoredEventListener(p);
    }
}

// -----------------------------------------------------------------
// Name : onKeyDown
//  Catch some events like "Ctrl+a" to select all
//  See also guiList::onSpecialKeyDown
// -----------------------------------------------------------------
bool guiList::onKeyDown(unsigned char c)
{
    if (m_bHasFocus)
    {
        // ctrl+A = select all
        if (c == 1)  // It looks like ascii 1 is "ctrl+a". TODO : Must be tested on different systems!
        {
    		for (guiComponent* &pCpnt : getDocument()->getComponents()) {
                ((guiListLabel*)pCpnt)->setSelected(true);
            }
            updateSelectionGeometry();
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------
// Name : updateSelectionGeometry
// -----------------------------------------------------------------
void guiList::updateSelectionGeometry()
{
    // First, count number of selected items
    int nSelLabels = 0;
	for (guiComponent* &pCpnt : getDocument()->getComponents()) {
        if (((guiListLabel*)pCpnt)->isSelected()) {
        	nSelLabels++;
        }
    }

    // Get or load texture
    Texture * texture = (m_pSelectionGeometry == NULL) ? _tex->loadTexture("EmptyWhiteSquare") : m_pSelectionGeometry->getTexture();
    if (nSelLabels == 0)
    {
        // Exit
        FREE(m_pSelectionGeometry);
        m_pLastSelectedLabel = NULL;
        return;
    }

    // Loop through labels again to build geometry
    QuadData ** pQuads = new QuadData*[nSelLabels];
    int iQuad = 0;
	for (guiComponent* &pCpnt : getDocument()->getComponents()) {
		guiListLabel* pLbl = (guiListLabel*) pCpnt;
        if (pLbl->isSelected()) {
            pQuads[iQuad++] = new QuadData(0, m_iInnerWidth, pLbl->getYPos(), pLbl->getYPos() + pLbl->getHeight(), texture);
        }
    }

    if (m_pSelectionGeometry == NULL) {
        m_pSelectionGeometry = new GeometryQuads(nSelLabels, pQuads, VB_Static);
    } else {
        m_pSelectionGeometry->modify(nSelLabels, pQuads);
    }
    QuadData::releaseQuads(nSelLabels, pQuads);
}

// -----------------------------------------------------------------
// Name : setFocus
// -----------------------------------------------------------------
void guiList::setFocus()
{
    m_bHasFocus = true;
    _input->setKeyboardListener(this);
    _input->pushUncursoredEventListener(this);
}

// -----------------------------------------------------------------
// Name : clear
// -----------------------------------------------------------------
void guiList::clear()
{
    getDocument()->deleteAllComponents();
    updateSelectionGeometry();
}

// -----------------------------------------------------------------
// Name : sort
// -----------------------------------------------------------------
void guiList::sort()
{
//    SortInterface::sort(getDocument()->getComponentsList());
//    int yPxl = 0;
//    guiComponent * pCpnt = getDocument()->getFirstComponent();
//    while (pCpnt != NULL)
//    {
//        pCpnt->moveTo(pCpnt->getXPos(), yPxl);
//        yPxl += pCpnt->getHeight();
//        pCpnt = getDocument()->getNextComponent();
//    }
}

// -----------------------------------------------------------------
// Name : sortCompare
// -----------------------------------------------------------------
bool guiList::sortCompare(BaseObject * A, BaseObject * B)
{
    guiListLabel * pLblA = (guiListLabel*) A;
    guiListLabel * pLblB = (guiListLabel*) B;
    return pLblA->getText().compare(pLblB->getText()) > 0;
}

// -----------------------------------------------------------------
// Name : addItem
// -----------------------------------------------------------------
guiList::guiListLabel * guiList::addItem(string sText, string sId)
{
    guiComponent * pCpnt = *(getDocument()->getComponents().rbegin());
    int yPxl = (pCpnt == NULL) ? 0 : pCpnt->getYPos() + pCpnt->getHeight();
    guiListLabel * pLbl = new guiListLabel();
    pLbl->init(sText, m_FontId, m_TextColor, sId, 5, yPxl, m_iInnerWidth - 10, 0);
    getDocument()->addComponent(pLbl);
    getDocument()->setHeight(yPxl + pLbl->getHeight());
    return pLbl;
}

// -----------------------------------------------------------------
// Name : removeSelection
// -----------------------------------------------------------------
void guiList::removeSelection()
{
    int yPxl = 0;
	for (list<guiComponent*>::iterator it = getDocument()->getComponents().begin(); it != getDocument()->getComponents().end(); ++it) {
		guiListLabel * pLbl = (guiListLabel*) *it;
        if (pLbl->isSelected()) {
            getDocument()->removeCurrentComponent(&it);
            delete pLbl;
        } else {
            pLbl->moveTo(pLbl->getXPos(), yPxl);
            yPxl += pLbl->getHeight();
        }
    }
    updateSelectionGeometry();
}

// -----------------------------------------------------------------
// Name : guiListDocument::onButtonEvent
// -----------------------------------------------------------------
bool guiList::guiListDocument::onButtonEvent(ButtonAction * pEvent, guiComponent * pCpnt)
{
    return m_pOwner->onButtonEvent(pEvent, pCpnt);
}

// -----------------------------------------------------------------
// Name : guiListLabel::onButtonEvent
// -----------------------------------------------------------------
guiObject * guiList::guiListLabel::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible) {
        return NULL;
    }

    if (pEvent->eButton == Button1 || pEvent->eButton == Button2)
    {
        if (m_pOwner->onButtonEvent(pEvent, this)) {
            return this;
        }
    }
    return NULL;
}

//// -----------------------------------------------------------------
//// Name : createDefaultList
////  Static default constructor
//// -----------------------------------------------------------------
//guiList * guiList::createDefaultList(int width, int height, string sId)
//{
//    Texture * frmtex[8];
//    frmtex[0] = _tex->findTexture("gui/interface:LstTL");
//    frmtex[1] = _tex->findTexture("gui/interface:LstTC");
//    frmtex[2] = _tex->findTexture("gui/interface:LstTR");
//    frmtex[3] = _tex->findTexture("gui/interface:LstCL");
//    frmtex[4] = _tex->findTexture("gui/interface:LstCR");
//    frmtex[5] = _tex->findTexture("gui/interface:LstBL");
//    frmtex[6] = _tex->findTexture("gui/interface:LstBC");
//    frmtex[7] = _tex->findTexture("gui/interface:LstBR");
//    guiList * pBox = new guiList();
//    pBox->init(
//        TEXT_FONT, TEXT_COLOR, frmtex,
//        sId, 0, 0, width, height);
//
//    // Attach document
//    guiDocument * pDoc = new guiListDocument(pBox);
//    pDoc->init(
//        "",
//        _tex->findTexture("gui/interface:ComboListBg"),
//        0, 0, 1, 1);
//    pBox->setDocument(pDoc);
//
//    return pBox;
//}
