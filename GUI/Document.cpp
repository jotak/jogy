#include "Document.h"
#include "ToggleButton.h"
#include "Container.h"
#include "../Utils/ButtonAction.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : Document
//  Constructor
// -----------------------------------------------------------------
Document::Document()
{
	m_pGeometry = NULL;
    m_pFocusedComponent = NULL;
    m_bNeedDestroy = false;
    m_bContentChanged = false;
    m_bEnabled = true;
    m_sTitle = "";
    m_pOwner = NULL;
}

// -----------------------------------------------------------------
// Name : ~Document
//  Destructor
// -----------------------------------------------------------------
Document::~Document()
{
	FREE(m_pGeometry);
    FREEVEC(m_pComponentsList);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
Document * Document::build()
{
    Object::build();
    rebuildGeometry();
    onLoad();
    return this;
}

// -----------------------------------------------------------------
// Name : withGeometry
// -----------------------------------------------------------------
Document * Document::withGeometry(ITexture * pTex, IGeometryQuads * pGeo)
{
    m_pGeometry = pGeo;
    QuadData quad(0, 1, 0, 1, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void Document::update(double delta)
{
    if (!m_bEnabled) {
        return;
    }
	for (Component* &pCpnt : m_pComponentsList) {
		pCpnt->update(delta);
    }
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void Document::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    // Display document background
    m_pGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, docColor);

    // Display components
	for (Component* &pCpnt : m_pComponentsList) {
		pCpnt->displayAt(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor, docColor);
    }
    m_bContentChanged = false;
}

// -----------------------------------------------------------------
// Name : rebuildGeometry
// -----------------------------------------------------------------
void Document::rebuildGeometry()
{
    QuadData quad(0, m_iWidth, 0, m_iHeight, m_pGeometry->getTexture());
    m_pGeometry->build(&quad);
}

// -----------------------------------------------------------------
// Name : getComponentAt
// -----------------------------------------------------------------
Component * Document::getComponentAt(int xPxl, int yPxl)
{
    for (list<Component*>::reverse_iterator it = m_pComponentsList.rbegin(); it != m_pComponentsList.rend(); ++it) {
    	Component * pCpnt = *it;
        if (pCpnt->isAt(xPxl, yPxl)) {
            return *it;
        }
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
Object * Document::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled) {
        return NULL;
    }

    if (pEvent->eEvent == EventDown) {
        Component * cpnt = getComponentAt(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset);
        if (cpnt != m_pFocusedComponent && pEvent->eButton == Button1) {
            setFocusedComponent(cpnt);
        }
        if (cpnt != NULL) {
            Object * pObj = cpnt->onButtonEvent(pEvent);
            if (pObj != NULL) {
                return pObj;
            }
        }
        if (pEvent->eButton == Button2) {
            return this;  // Drag (=scroll) document
        }
    } else if (pEvent->eEvent == EventDrag) {
        moveBy(pEvent->xPos - pEvent->xPosInit, pEvent->yPos - pEvent->yPosInit);
        m_pOwner->checkDocumentPosition();
        return this;
    }

    return NULL;
}

// -----------------------------------------------------------------
// Name : onCursorMoveEvent
// -----------------------------------------------------------------
Object * Document::onCursorMoveEvent(int xPxl, int yPxl)
{
    Component * cpnt = getComponentAt(xPxl, yPxl);
    if (cpnt != NULL) {
        return cpnt->onCursorMoveEvent(xPxl, yPxl);
    } else {
        return this;
    }
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void Document::onResize(int iOldWidth, int iOldHeight)
{
    Object::onResize(iOldWidth, iOldHeight);
    if (m_iWidth == iOldWidth && m_iHeight == iOldHeight) {
        return;
    }
    if (m_pGeometry != NULL) {
    	rebuildGeometry();
    }
}

// -----------------------------------------------------------------
// Name : setFocusedComponent
// -----------------------------------------------------------------
void Document::setFocusedComponent(Component * pCpnt)
{
    if (m_pFocusedComponent != NULL) {
        m_pFocusedComponent->onFocusLost();
    }
    m_pFocusedComponent = pCpnt;
}

// -----------------------------------------------------------------
// Name : getComponent
// -----------------------------------------------------------------
Component * Document::getComponent(string cpntId)
{
	for (Component* &pCpnt : m_pComponentsList) {
    	if (cpntId == pCpnt->getId()) {
    		return pCpnt;
    	}
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : deleteAllComponents
// -----------------------------------------------------------------
void Document::deleteAllComponents()
{
    FREEVEC(m_pComponentsList);
    m_pFocusedComponent = NULL;
    m_bContentChanged = true;
}

// -----------------------------------------------------------------
// Name : deleteComponent
// -----------------------------------------------------------------
bool Document::deleteComponent(Component * pCpnt)
{
    for (list<Component*>::iterator it = m_pComponentsList.begin(); it != m_pComponentsList.end(); ++it) {
    	if (pCpnt == *it) {
    		m_pComponentsList.erase(it);
            if (m_pFocusedComponent == pCpnt) {
            	m_pFocusedComponent = NULL;
            }
            m_bContentChanged = true;
    		delete pCpnt;
    		return true;
    	}
    }
    return false;
}

// -----------------------------------------------------------------
// Name : removeCurrentComponent
// -----------------------------------------------------------------
Component * Document::removeCurrentComponent(list<Component*>::iterator * it)
{
	Component * toDelete = **it;
    if (m_pFocusedComponent == toDelete) {
        m_pFocusedComponent = NULL;
    }
    m_bContentChanged = true;
    *it = m_pComponentsList.erase(*it);
    return toDelete;
}

// -----------------------------------------------------------------
// Name : addComponent
// -----------------------------------------------------------------
void Document::addComponent(Component * cpnt)
{
    m_pComponentsList.push_back(cpnt);
    if (cpnt->getOwner() == NULL) {
        cpnt->setOwner(this);
    }
}

// -----------------------------------------------------------------
// Name : bringAbove
// -----------------------------------------------------------------
void Document::bringAbove(Component * cpnt)
{
	m_pComponentsList.remove(cpnt);
	m_pComponentsList.push_front(cpnt);
}

// -----------------------------------------------------------------
// Name : doClick
// -----------------------------------------------------------------
void Document::doClick(string sCpntId)
{
    Component * pCpnt = getComponent(sCpntId);
    if (pCpnt == NULL || !pCpnt->isVisible() || !pCpnt->isEnabled()) {
        return;
    }
    ButtonAction action;
    action.eButton = Button1;
    action.eEvent = EventDown;
    action.xOffset = action.yOffset = 0;
    action.xPos = action.xPosInit = pCpnt->getXPos() + 1;
    action.yPos = action.yPosInit = pCpnt->getYPos() + 1;
    pCpnt->onButtonEvent(&action);
    action.eEvent = EventUp;
    pCpnt->onButtonEvent(&action);
}
}
