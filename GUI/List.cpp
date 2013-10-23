#include "List.h"
#include "../jogy.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : List
// -----------------------------------------------------------------
List::List()
{
    m_pSelectionGeometry = NULL;
    m_bHasSelection = false;
    m_bHasFocus = false;
    m_pLastSelectedLabel = NULL;
    m_bCatchMouseUp = false;
    m_ActionOnSelection = (InputButton)0;
    m_pLabelTemplate = new Label();
}

// -----------------------------------------------------------------
// Name : ~List
//  Destructor
// -----------------------------------------------------------------
List::~List()
{
    if (m_bHasFocus) {
        onFocusLost();
    }
    FREE(m_pSelectionGeometry);
    FREE(m_pLabelTemplate);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
List * List::build() {
	withFrameFitBehavior(FB_FitDocumentToFrameWhenSmaller, FB_FitDocumentToFrameWhenSmaller);
    Container::build();
    return this;
}

// -----------------------------------------------------------------
// Name : withSelectionGeometry
// -----------------------------------------------------------------
List * List::withSelectionGeometry(ITexture * pTex, IGeometryQuads * pGeo) {
    m_pSelectionGeometry = pGeo;
    QuadData quad(0, 1, 0, 1, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void List::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }

    Container::displayAt(iXOffset, iYOffset, cpntColor, docColor);

    if (m_bHasSelection)
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
bool List::onCatchButtonEvent(ButtonAction * pEvent)
{
	list<Component*> lstCpnt = getDocument()->getComponents();
    if ((pEvent->eEvent != EventDown && pEvent->eEvent != EventDoubleClick) || lstCpnt.empty()) {
        return false;
    }
    switch (pEvent->eButton)
    {
    case ButtonUp:
    case ButtonDown:
    {
        ListLabel * pClicked = NULL;
        if (m_pLastSelectedLabel == NULL) {
            pClicked = (ListLabel*) *(lstCpnt.begin());
        } else {
        	// Find previous selection
        	if (pEvent->eButton == ButtonUp) {
        		for (list<Component*>::reverse_iterator it = lstCpnt.rbegin(); it != lstCpnt.rend(); ++it) {
        			if (*it == m_pLastSelectedLabel) {
        				pClicked = (ListLabel*) *(++it);
        				break;
        			}
        		}
        	} else {
        		// ButtonDown
        		for (list<Component*>::iterator it = lstCpnt.begin(); it != lstCpnt.end(); ++it) {
        			if (*it == m_pLastSelectedLabel) {
        				pClicked = (ListLabel*) *(++it);
        				break;
        			}
        		}
        	}
        }
        if (pClicked != NULL)
        {
            if (Jogy::interface->isShiftPressed()) {
                shiftSelect(pClicked);
            } else {
        		for (list<Component*>::iterator it = lstCpnt.begin(); it != lstCpnt.end(); ++it) {
                    ((ListLabel*)*it)->setSelected(*it == pClicked);
                }
                updateSelectionGeometry();
            }
            m_pLastSelectedLabel = pClicked;
        }
        break;
    }
    default:
    {
        if (pEvent->eEvent == EventDown) {
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
Object * List::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible || m_pDoc == NULL) {
        return NULL;
    }

    if (pEvent->eButton == Button1 && pEvent->eEvent == EventDown)
    {
        // Get focus
        setFocus();
        m_bCatchMouseUp = false;
        Object * pObj = Container::onButtonEvent(pEvent);
        if (pObj == NULL)
        {
            // Deselect all
    		for (Component* &pCpnt : getDocument()->getComponents()) {
                ((ListLabel*)pCpnt)->setSelected(false);
            }
            m_pLastSelectedLabel = NULL;
            updateSelectionGeometry();
        }
        return pObj;
    }
    return Container::onButtonEvent(pEvent);
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
bool List::onButtonEvent(ButtonAction * pEvent, Component * pCpnt)
{
    if (pEvent->eEvent == EventDown)
    {
        if (pEvent->eButton == Button2) {
            return false;
        }

        ListLabel * pClickedLabel = (ListLabel*) pCpnt;
        if (pEvent->eButton == Button1 && Jogy::interface->isShiftPressed()) {
            shiftSelect(pClickedLabel);
        } else if ((pEvent->eButton == Button1 && Jogy::interface->isCtrlPressed()) || pEvent->eButton == Button2) {
            // Add/remove current label to/from selection
            pClickedLabel->setSelected(!pClickedLabel->isSelected());
            updateSelectionGeometry();
        }
        else
        {
            if (!pClickedLabel->isSelected())
            {
        		for (Component* &pCpnt : getDocument()->getComponents()) {
                    ((ListLabel*)pCpnt)->setSelected(pCpnt == pClickedLabel);
                }
                updateSelectionGeometry();
            } else { // We'll do deselection on mouse up to allow dragging
                m_bCatchMouseUp = true;
            }
        }
        m_pLastSelectedLabel = pClickedLabel;
        return true;
    }
    else if (pEvent->eEvent == EventDrag)
    {
        m_bCatchMouseUp = false;
        return m_pOwner->onButtonEvent(pEvent, this);
    }
    else if (pEvent->eEvent == EventUp)
    {
        bool bReturn = true;
        if (m_bCatchMouseUp && m_pLastSelectedLabel != NULL)
        {
            // Clear current selection and add label to it
    		for (Component* &pCpnt : getDocument()->getComponents()) {
                ((ListLabel*)pCpnt)->setSelected(pCpnt == m_pLastSelectedLabel);
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
void List::shiftSelect(ListLabel * pClickedLabel)
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
    		for (Component* &pCpnt : getDocument()->getComponents()) {
                if (!bInBlock && (pCpnt == pClickedLabel || pCpnt == m_pLastSelectedLabel)) {
                    bInBlock = true;
                } else if (bInBlock) {
                    if (pCpnt == pClickedLabel || pCpnt == m_pLastSelectedLabel) {
                        break;
                    }
                    ((ListLabel*)pCpnt)->setSelected(bSelect);
                }
            }
        }
    }
    updateSelectionGeometry();
}

// -----------------------------------------------------------------
// Name : onFocusLost
// -----------------------------------------------------------------
void List::onFocusLost()
{
    m_bHasFocus = false;
    Jogy::interface->unregisterKeyboardListener(this);
    Jogy::interface->unregisterUncursoredEventListener(this);
}

// -----------------------------------------------------------------
// Name : onKeyDown
//  Catch some events like "Ctrl+a" to select all
//  See also List::onSpecialKeyDown
// -----------------------------------------------------------------
bool List::onKeyDown(unsigned char c)
{
    if (m_bHasFocus)
    {
        // ctrl+A = select all
        if (c == 1)  // It looks like ascii 1 is "ctrl+a". TODO : Must be tested on different systems!
        {
    		for (Component* &pCpnt : getDocument()->getComponents()) {
                ((ListLabel*)pCpnt)->setSelected(true);
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
void List::updateSelectionGeometry()
{
    // First, count number of selected items
    int nSelLabels = 0;
	for (Component* &pCpnt : getDocument()->getComponents()) {
        if (((ListLabel*)pCpnt)->isSelected()) {
        	nSelLabels++;
        }
    }

    // Get or load texture
    ITexture * texture = m_pSelectionGeometry->getTexture();
    if (nSelLabels == 0)
    {
        // Exit
    	m_bHasSelection = false;
        m_pLastSelectedLabel = NULL;
        return;
    }

    // Loop through labels again to build geometry
    QuadData ** pQuads = new QuadData*[nSelLabels];
    int iQuad = 0;
	for (Component* &pCpnt : getDocument()->getComponents()) {
		ListLabel* pLbl = (ListLabel*) pCpnt;
        if (pLbl->isSelected()) {
            pQuads[iQuad++] = new QuadData(0, m_iInnerWidth, pLbl->getYPos(), pLbl->getYPos() + pLbl->getHeight(), texture);
        }
    }

	m_bHasSelection = true;
	m_pSelectionGeometry->build(nSelLabels, pQuads);
    QuadData::releaseQuads(nSelLabels, pQuads);
}

// -----------------------------------------------------------------
// Name : setFocus
// -----------------------------------------------------------------
void List::setFocus()
{
    m_bHasFocus = true;
    Jogy::interface->registerKeyboardListener(this);
    Jogy::interface->registerUncursoredEventListener(this);
}

// -----------------------------------------------------------------
// Name : clear
// -----------------------------------------------------------------
void List::clear()
{
    getDocument()->deleteAllComponents();
    updateSelectionGeometry();
}

// -----------------------------------------------------------------
// Name : sort
// -----------------------------------------------------------------
void List::sort()
{
//    SortInterface::sort(getDocument()->getComponentsList());
//    int yPxl = 0;
//    Component * pCpnt = getDocument()->getFirstComponent();
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
bool List::sortCompare(BaseObject * A, BaseObject * B)
{
    ListLabel * pLblA = (ListLabel*) A;
    ListLabel * pLblB = (ListLabel*) B;
    return pLblA->getText().compare(pLblB->getText()) > 0;
}

// -----------------------------------------------------------------
// Name : addItem
// -----------------------------------------------------------------
List::ListLabel * List::addItem(string sText, string sId)
{
    Component * pCpnt = *(getDocument()->getComponents().rbegin());
    int yPxl = (pCpnt == NULL) ? 0 : pCpnt->getYPos() + pCpnt->getHeight();
    ListLabel * pLbl = new ListLabel();
    pLbl->withText(sText, m_pLabelTemplate->getFontId(), m_pLabelTemplate->getDiffuseColor())
    		->withGeometry((IGeometryText*)m_pLabelTemplate->getGeometry()->clone())
    		->withCpntId(sId)
    		->withDimensions(m_iInnerWidth - 10, 0)
    		->withPosition(5, yPxl)
    		->build();
    getDocument()->addComponent(pLbl);
    getDocument()->setHeight(yPxl + pLbl->getHeight());
    return pLbl;
}

// -----------------------------------------------------------------
// Name : removeSelection
// -----------------------------------------------------------------
void List::removeSelection()
{
    int yPxl = 0;
	for (list<Component*>::iterator it = getDocument()->getComponents().begin(); it != getDocument()->getComponents().end(); ++it) {
		ListLabel * pLbl = (ListLabel*) *it;
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
// Name : pickActionOnSelection
// -----------------------------------------------------------------
InputButton List::pickActionOnSelection()
{
	InputButton action = m_ActionOnSelection;
    m_ActionOnSelection = (InputButton)0;
    return action;
}

// -----------------------------------------------------------------
// Name : ListDocument::onButtonEvent
// -----------------------------------------------------------------
bool List::ListDocument::onButtonEvent(ButtonAction * pEvent, Component * pCpnt)
{
    return m_pOwner->onButtonEvent(pEvent, pCpnt);
}

// -----------------------------------------------------------------
// Name : ListLabel::onButtonEvent
// -----------------------------------------------------------------
Object * List::ListLabel::onButtonEvent(ButtonAction * pEvent)
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
//List * List::createDefaultList(int width, int height, string sId)
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
//    List * pBox = new List();
//    pBox->init(
//        TEXT_FONT, TEXT_COLOR, frmtex,
//        sId, 0, 0, width, height);
//
//    // Attach document
//    Document * pDoc = new ListDocument(pBox);
//    pDoc->init(
//        "",
//        _tex->findTexture("gui/interface:ComboListBg"),
//        0, 0, 1, 1);
//    pBox->setDocument(pDoc);
//
//    return pBox;
//}
}
