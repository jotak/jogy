#include "SmartSlider.h"
#include "../ComponentOwnerInterface.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : SmartSlider
//  Constructor
// -----------------------------------------------------------------
SmartSlider::SmartSlider() : Component()
{
    m_iSliderPos = 0;
    m_iItemSize = 0;
    m_pMainGeometry = NULL;
    m_pDisabledGeometry = NULL;
    m_pSelectorGeometry = NULL;
    m_pSelectedItem = NULL;
    m_pLabel = new Label();
    m_pDisableReasonLabel = new Label();
    m_iSpacing = m_iTheoricSize = m_iSelectorPos = 0;
}

// -----------------------------------------------------------------
// Name : ~SmartSlider
//  Destructor
// -----------------------------------------------------------------
SmartSlider::~SmartSlider()
{
    FREEVEC(m_pItems);
    FREE(m_pMainGeometry);
    FREE(m_pDisabledGeometry);
    FREE(m_pSelectorGeometry);
    delete m_pLabel;
    delete m_pDisableReasonLabel;
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
SmartSlider * SmartSlider::build()
{
    Component::build();

    // Rebuild geometries with now known itemSize
    loadGeometry();
    QuadData quad(0, m_iItemSize, 0, m_iItemSize, m_pSelectorGeometry->getTexture());
    m_pSelectorGeometry->build(&quad);
    QuadData quad2(0, m_iItemSize, 0, m_iItemSize, m_pDisabledGeometry->getTexture());
    m_pDisabledGeometry->build(&quad2);
    m_pLabel->build();
    m_pDisableReasonLabel->build();

    setHeight(m_iItemSize + m_pLabel->getHeight() + 3);

    return this;
}

// -----------------------------------------------------------------
// Name : withMainGeometry
// -----------------------------------------------------------------
SmartSlider * SmartSlider::withMainGeometry(IGeometryQuads * pGeo)
{
    m_pMainGeometry = pGeo;
	return this;
}

// -----------------------------------------------------------------
// Name : withLabelInfo
// -----------------------------------------------------------------
SmartSlider * SmartSlider::withLabelInfo(Color textColor, fontid fontId, IGeometryText * pLabelGeo)
{
	m_pLabel->withText("", fontId, textColor)->withGeometry(pLabelGeo);
	m_pDisableReasonLabel->withText("", fontId, textColor)->withGeometry((IGeometryText*)pLabelGeo->clone());
	return this;
}

// -----------------------------------------------------------------
// Name : withSelectorGeometry
// -----------------------------------------------------------------
SmartSlider * SmartSlider::withSelectorGeometry(ITexture * pTex, IGeometryQuads * pGeo) {
    m_pSelectorGeometry = pGeo;
    QuadData quad(0, 1, 0, 1, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : withDisabledGeometry
// -----------------------------------------------------------------
SmartSlider * SmartSlider::withDisabledGeometry(ITexture * pTex, IGeometryQuads * pGeo) {
    m_pDisabledGeometry = pGeo;
    QuadData quad(0, 1, 0, 1, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void SmartSlider::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (m_bVisible)
    {
        if (!m_bEnabled) {
            cpntColor = cpntColor * 0.3f;
        }
        int x = m_iXPxl + iXOffset + m_iSliderPos;
        int y = m_iYPxl + iYOffset;
        m_pMainGeometry->display(x, y, cpntColor);

        // Find all disabled items
        int darkX = x;
    	for (guiSliderItem* &pItem : m_pItems) {
            if (!pItem->m_bEnabled) {
                m_pDisabledGeometry->display(darkX, y, cpntColor);
            }
            darkX += m_iItemSize + m_iSpacing;
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
//            Jogy::interface->setBlendingMode(true);
            cpntColor = cpntColor * Color(1, 1, 1, 0.3f);
            x += m_iSelectorPos;
            m_pSelectorGeometry->display(x, y, cpntColor);
//      if (!bWasBlending)
//        getDisplay()->disableBlending();
        }
    }
}

// -----------------------------------------------------------------
// Name : onCursorMoveEvent
// -----------------------------------------------------------------
Object * SmartSlider::onCursorMoveEvent(int xPxl, int yPxl)
{
    if (!m_bEnabled || !m_bVisible) {
        return NULL;
    }
    Component::onCursorMoveEvent(xPxl, yPxl);

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
Object * SmartSlider::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible) {
        return NULL;
    }
    if (m_pSelectedItem != NULL && m_pSelectedItem->m_bEnabled && pEvent->eButton == Button1 && pEvent->eEvent == EventDown) {
        m_pOwner->onButtonEvent(pEvent, this);
        return NULL;
    } else {
        return NULL;
    }
}

// -----------------------------------------------------------------
// Name : addItem
// -----------------------------------------------------------------
void SmartSlider::addItem(guiSliderItem * pItem, bool bFirst)
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
void SmartSlider::deleteItems()
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
void SmartSlider::loadGeometry()
{
	if (m_pMainGeometry == NULL) {
		return;
    }

    m_iTheoricSize = 0;
    if (m_pItems.empty()) {
    	m_pMainGeometry->build(0, NULL);
    } else {
    	int size = m_pItems.size();
        QuadData ** pQuads = new QuadData*[size];
        int i = 0;
    	for (guiSliderItem* &pItem : m_pItems) {
            pQuads[i] = new QuadData(m_iTheoricSize, m_iTheoricSize + m_iItemSize, 0, m_iItemSize, pItem->m_pTex);
            m_iTheoricSize += m_iItemSize + m_iSpacing;
            i++;
        }
    	m_pMainGeometry->build(size, pQuads);
        QuadData::releaseQuads(size, pQuads);
    }
    if (m_iTheoricSize < m_iWidth) {
        m_iTheoricSize = m_iWidth;
    }
}
}
