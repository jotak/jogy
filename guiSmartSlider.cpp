#include "guiSmartSlider.h"
#include "ComponentOwnerInterface.h"
#include "../Display/DisplayEngine.h"
#include "../Input/InputEngine.h"

// -----------------------------------------------------------------
// Name : guiSmartSlider
//  Constructor
// -----------------------------------------------------------------
guiSmartSlider::guiSmartSlider() : guiComponent()
{
    m_iSliderPos = 0;
    m_iItemSize = 0;
    m_pDisabledGeometry = NULL;
    m_pSelectorGeometry = NULL;
    m_pSelectedItem = NULL;
    m_pLabel = new guiLabel();
    m_pDisableReasonLabel = new guiLabel();
    m_iSpacing = m_iTheoricSize = m_iSelectorPos = 0;
}

// -----------------------------------------------------------------
// Name : ~guiSmartSlider
//  Destructor
// -----------------------------------------------------------------
guiSmartSlider::~guiSmartSlider()
{
    FREEVEC(m_pItems);
    FREE(m_pDisabledGeometry);
    FREE(m_pSelectorGeometry);
    delete m_pLabel;
    delete m_pDisableReasonLabel;
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void guiSmartSlider::init(int iItemSize, int iSpacing, FontId fontId, Color textColor, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl)
{
    guiComponent::init(sCpntId, xPxl, yPxl, wPxl, hPxl);
    m_pLabel->init("dummy", fontId, textColor, "", 0, 0, 0, 0);
    m_pDisableReasonLabel->init("dummy", fontId, Color(1,0,0), "", 0, 0, 0, 0);
    m_iSpacing = iSpacing;
    m_iItemSize = iItemSize;

    QuadData quad1(0, m_iItemSize, 0, m_iItemSize, "EmptyWhiteSquare");
    m_pSelectorGeometry = new GeometryQuads(&quad1, VB_Static);
    QuadData quad2(0, m_iItemSize, 0, m_iItemSize, "Disabled");
    m_pDisabledGeometry = new GeometryQuads(&quad2, VB_Static);

    loadGeometry();
    setHeight(m_iItemSize + m_pLabel->getHeight() + 3);
}

// -----------------------------------------------------------------
// Name : clone
// -----------------------------------------------------------------
guiObject * guiSmartSlider::clone()
{
    guiSmartSlider * pObj = new guiSmartSlider();
    pObj->init(m_iItemSize, m_iSpacing, m_pLabel->getFontId(), m_pLabel->getDiffuseColor(), m_sCpntId, m_iXPxl, m_iYPxl, m_iWidth, m_iHeight);
    return pObj;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiSmartSlider::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (m_bVisible)
    {
        if (!m_bEnabled) {
            cpntColor = cpntColor * 0.3f;
        }
        CoordsScreen coords = CoordsScreen(m_iXPxl + iXOffset + m_iSliderPos, m_iYPxl + iYOffset, GUIPLANE);
        ((GeometryQuads*)m_pGeometry)->display(coords, cpntColor);

        // Find all disabled items
        CoordsScreen darkCoords = coords;
    	for (guiSliderItem* &pItem : m_pItems) {
            if (!pItem->m_bEnabled) {
                m_pDisabledGeometry->display(darkCoords, cpntColor);
            }
            darkCoords.x += m_iItemSize + m_iSpacing;
        }

        // Draw selection-related (selector, label...)
        if (m_pSelectedItem != NULL)
        {
            // just ignore stencil for text
            m_pLabel->displayAt(iXOffset, iYOffset, cpntColor, docColor);
            if (!m_pSelectedItem->m_bEnabled) {
                m_pDisableReasonLabel->displayAt(iXOffset, iYOffset, cpntColor, docColor);
            }
//      bool bWasBlending = (blendColor.a >= 0 && blendColor.a < 1);
//      if (!bWasBlending)
            _display->enableBlending();
            cpntColor = cpntColor * Color(1, 1, 1, 0.3f);
            coords.x += m_iSelectorPos;
            m_pSelectorGeometry->display(coords, cpntColor);
//      if (!bWasBlending)
//        getDisplay()->disableBlending();
        }
    }
}

// -----------------------------------------------------------------
// Name : onCursorMoveEvent
// -----------------------------------------------------------------
guiObject * guiSmartSlider::onCursorMoveEvent(int xPxl, int yPxl)
{
    if (!m_bEnabled || !m_bVisible) {
        return NULL;
    }
    guiComponent::onCursorMoveEvent(xPxl, yPxl);

    xPxl -= m_iXPxl;
    yPxl -= m_iYPxl;
    if (yPxl >= m_iItemSize)
    {
        m_pSelectedItem = NULL;
        return this;
    }

    m_iSliderPos = xPxl - (m_iTheoricSize * xPxl / m_iWidth);
    m_iSelectorPos = 0;
	for (guiSliderItem* &pItem : m_pItems) {
        if (xPxl < m_iSelectorPos + m_iItemSize + m_iSliderPos)
        {
            if (m_pSelectedItem != pItem)
            {
                m_pSelectedItem = pItem;
                m_pLabel->setText(pItem->getInfo());
                int textX = (getWidth() - m_pLabel->getWidth()) / 2;
                int textY = getYPos() + getHeight() - m_pLabel->getHeight();
                if (!m_pSelectedItem->m_bEnabled)
                {
                    m_pDisableReasonLabel->setText(pItem->m_sDisabledReason);
                    textX -= m_pDisableReasonLabel->getWidth() / 2 - 5;
                    m_pDisableReasonLabel->moveTo(textX + m_pLabel->getWidth() + 10, textY);
                }
                m_pLabel->moveTo(textX, textY);
            }
            return this;
        }
        m_iSelectorPos += m_iItemSize + m_iSpacing;
    }
    m_pSelectedItem = NULL;
    return this;
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiSmartSlider::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible) {
        return NULL;
    }
    if (m_pSelectedItem != NULL && m_pSelectedItem->m_bEnabled && pEvent->eButton == Button1 && pEvent->eEvent == Event_Down) {
        m_pOwner->onButtonEvent(pEvent, this);
        return NULL;
    } else {
        return NULL;
    }
}

// -----------------------------------------------------------------
// Name : addItem
// -----------------------------------------------------------------
void guiSmartSlider::addItem(guiSliderItem * pItem, bool bFirst)
{
    if (bFirst) {
        m_pItems.push_front(pItem);
    } else {
        m_pItems.push_back(pItem);
    }
}

// -----------------------------------------------------------------
// Name : deleteItems
// -----------------------------------------------------------------
void guiSmartSlider::deleteItems()
{
	FREEVEC(m_pItems);
    m_iSliderPos = 0;
    m_pSelectedItem = NULL;
    m_pLabel->setText("");
    m_pDisableReasonLabel->setText("");
}

// -----------------------------------------------------------------
// Name : loadGeometry
// -----------------------------------------------------------------
void guiSmartSlider::loadGeometry()
{
	if (m_pGeometry == NULL) {
		return;
    }

    m_iTheoricSize = 0;
    if (m_pItems.empty()) {
        if (m_pGeometry == NULL) {
            m_pGeometry = new GeometryQuads(VB_Static);
        } else {
            ((GeometryQuads*)m_pGeometry)->modify(0, NULL);
        }
    } else {
    	int size = m_pItems.size();
        QuadData ** pQuads = new QuadData*[size];
        int i = 0;
    	for (guiSliderItem* &pItem : m_pItems) {
            pQuads[i] = new QuadData(m_iTheoricSize, m_iTheoricSize + m_iItemSize, 0, m_iItemSize, pItem->m_pTex);
            m_iTheoricSize += m_iItemSize + m_iSpacing;
            i++;
        }
        if (m_pGeometry == NULL) {
            m_pGeometry = new GeometryQuads(size, pQuads, VB_Static);
        } else {
            ((GeometryQuads*)m_pGeometry)->modify(size, pQuads);
        }
        QuadData::releaseQuads(size, pQuads);
    }
    if (m_iTheoricSize < m_iWidth) {
        m_iTheoricSize = m_iWidth;
    }
}
