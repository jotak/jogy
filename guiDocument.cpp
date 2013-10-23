#include "guiDocument.h"
#include "guiToggleButton.h"
#include "guiContainer.h"
#include "Utils/ButtonAction.h"

// -----------------------------------------------------------------
// Name : guiDocument
//  Constructor
// -----------------------------------------------------------------
guiDocument::guiDocument()
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
// Name : ~guiDocument
//  Destructor
// -----------------------------------------------------------------
guiDocument::~guiDocument()
{
	FREE(m_pGeometry);
    FREEVEC(m_pComponentsList);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
guiDocument * guiDocument::build()
{
    guiObject::build();
    rebuildGeometry();
    onLoad();
    return this;
}

// -----------------------------------------------------------------
// Name : withGeometry
// -----------------------------------------------------------------
guiDocument * guiDocument::withGeometry(ITexture * pTex, IGeometryQuads * pGeo)
{
    m_pGeometry = pGeo;
    QuadData quad(0, 1, 0, 1, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void guiDocument::update(double delta)
{
    if (!m_bEnabled) {
        return;
    }
	for (guiComponent* &pCpnt : m_pComponentsList) {
		pCpnt->update(delta);
    }
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiDocument::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    // Display document background
    m_pGeometry->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, docColor);

    // Display components
	for (guiComponent* &pCpnt : m_pComponentsList) {
		pCpnt->displayAt(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor, docColor);
    }
    m_bContentChanged = false;
}

// -----------------------------------------------------------------
// Name : rebuildGeometry
// -----------------------------------------------------------------
void guiDocument::rebuildGeometry()
{
    QuadData quad(0, m_iWidth, 0, m_iHeight, m_pGeometry->getTexture());
    m_pGeometry->build(&quad);
}

// -----------------------------------------------------------------
// Name : getComponentAt
// -----------------------------------------------------------------
guiComponent * guiDocument::getComponentAt(int xPxl, int yPxl)
{
    for (list<guiComponent*>::reverse_iterator it = m_pComponentsList.rbegin(); it != m_pComponentsList.rend(); ++it) {
    	guiComponent * pCpnt = *it;
        if (pCpnt->isAt(xPxl, yPxl)) {
            return *it;
        }
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiDocument::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled) {
        return NULL;
    }

    if (pEvent->eEvent == Event_Down) {
        guiComponent * cpnt = getComponentAt(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset);
        if (cpnt != m_pFocusedComponent && pEvent->eButton == Button1) {
            setFocusedComponent(cpnt);
        }
        if (cpnt != NULL) {
            guiObject * pObj = cpnt->onButtonEvent(pEvent);
            if (pObj != NULL) {
                return pObj;
            }
        }
        if (pEvent->eButton == Button2) {
            return this;  // Drag (=scroll) document
        }
    } else if (pEvent->eEvent == Event_Drag) {
        moveBy(pEvent->xPos - pEvent->xPosInit, pEvent->yPos - pEvent->yPosInit);
        m_pOwner->checkDocumentPosition();
        return this;
    }

    return NULL;
}

// -----------------------------------------------------------------
// Name : onCursorMoveEvent
// -----------------------------------------------------------------
guiObject * guiDocument::onCursorMoveEvent(int xPxl, int yPxl)
{
    guiComponent * cpnt = getComponentAt(xPxl, yPxl);
    if (cpnt != NULL) {
        return cpnt->onCursorMoveEvent(xPxl, yPxl);
    } else {
        return this;
    }
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void guiDocument::onResize(int iOldWidth, int iOldHeight)
{
    guiObject::onResize(iOldWidth, iOldHeight);
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
void guiDocument::setFocusedComponent(guiComponent * pCpnt)
{
    if (m_pFocusedComponent != NULL) {
        m_pFocusedComponent->onFocusLost();
    }
    m_pFocusedComponent = pCpnt;
}

// -----------------------------------------------------------------
// Name : getComponent
// -----------------------------------------------------------------
guiComponent * guiDocument::getComponent(string cpntId)
{
	for (guiComponent* &pCpnt : m_pComponentsList) {
    	if (cpntId == pCpnt->getId()) {
    		return pCpnt;
    	}
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : deleteAllComponents
// -----------------------------------------------------------------
void guiDocument::deleteAllComponents()
{
    FREEVEC(m_pComponentsList);
    m_pFocusedComponent = NULL;
    m_bContentChanged = true;
}

// -----------------------------------------------------------------
// Name : deleteComponent
// -----------------------------------------------------------------
bool guiDocument::deleteComponent(guiComponent * pCpnt)
{
    for (list<guiComponent*>::iterator it = m_pComponentsList.begin(); it != m_pComponentsList.end(); ++it) {
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
guiComponent * guiDocument::removeCurrentComponent(list<guiComponent*>::iterator * it)
{
	guiComponent * toDelete = **it;
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
void guiDocument::addComponent(guiComponent * cpnt)
{
    m_pComponentsList.push_back(cpnt);
    if (cpnt->getOwner() == NULL) {
        cpnt->setOwner(this);
    }
}

// -----------------------------------------------------------------
// Name : bringAbove
// -----------------------------------------------------------------
void guiDocument::bringAbove(guiComponent * cpnt)
{
	m_pComponentsList.remove(cpnt);
	m_pComponentsList.push_front(cpnt);
}

// -----------------------------------------------------------------
// Name : doClick
// -----------------------------------------------------------------
void guiDocument::doClick(string sCpntId)
{
    guiComponent * pCpnt = getComponent(sCpntId);
    if (pCpnt == NULL || !pCpnt->isVisible() || !pCpnt->isEnabled()) {
        return;
    }
    ButtonAction action;
    action.eButton = Button1;
    action.eEvent = Event_Down;
    action.xOffset = action.yOffset = 0;
    action.xPos = action.xPosInit = pCpnt->getXPos() + 1;
    action.yPos = action.yPosInit = pCpnt->getYPos() + 1;
    pCpnt->onButtonEvent(&action);
    action.eEvent = Event_Up;
    pCpnt->onButtonEvent(&action);
}
