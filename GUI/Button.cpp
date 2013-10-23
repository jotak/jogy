#include "Button.h"
#include "../ComponentOwnerInterface.h"
#include "../jogy.h"

#define MULTICLICKS_TIMER_FIRST     1.0f
#define MULTICLICKS_TIMER_NEXT      0.1f

namespace jogy {

// -----------------------------------------------------------------
// Name : Button
//  Constructor
// -----------------------------------------------------------------
Button::Button() : Component()
{
    m_ClickedOption = None;
    m_HoverOption = None;
    m_bMouseDown = m_bMouseOver = false;
    m_bClickState = false;
    m_pLabel = new Label();
    m_pGeometryClicked = NULL;
    m_pGeometryHover = NULL;
    m_pGeometryBase = NULL;
    m_pGeometryAttachedImage = NULL;
    m_bCatchButton2Events = false;
    m_bCatchDoubleClicks = false;
    m_bMultiClicks = false;
    m_fMultiClicksTimer = 0.0f;
    m_ClickCallback = NULL;
}

// -----------------------------------------------------------------
// Name : ~Button
//  Destructor
// -----------------------------------------------------------------
Button::~Button()
{
    FREE(m_pLabel);
    FREE(m_pGeometryClicked);
    FREE(m_pGeometryHover);
    FREE(m_pGeometryAttachedImage);
    FREE(m_pGeometryBase);
}

// -----------------------------------------------------------------
// Name : withLabel
// -----------------------------------------------------------------
Button * Button::withLabel(string sText, fontid fontId, Color textColor, IGeometryText * pLabelGeo)
{
	m_pLabel->withPosition(0, 0)
			->withDimensions(0, 0);
	m_pLabel->withText(sText, fontId, textColor)
			->withGeometry(pLabelGeo);
	return this;
}

// -----------------------------------------------------------------
// Name : withBaseGeometry
// -----------------------------------------------------------------
Button * Button::withBaseGeometry(ITexture * pTex, IGeometryQuads * pGeo)
{
    m_pGeometryBase = pGeo;
    QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : withClickedGeometry
// -----------------------------------------------------------------
Button * Button::withClickedGeometry(ITexture * pTex, IGeometryQuads * pGeo)
{
    m_pGeometryClicked = pGeo;
    QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : withHoverGeometry
// -----------------------------------------------------------------
Button * Button::withHoverGeometry(ITexture * pTex, IGeometryQuads * pGeo)
{
    m_pGeometryHover = pGeo;
    QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
Button * Button::build()
{
    Component::build();
    m_pLabel->centerOn(this);
    return this;
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void Button::update(double delta)
{
    Component::update(delta);
    if (m_bEnabled && m_bVisible && m_bMultiClicks && m_bMouseDown && m_bMouseOver && m_fMultiClicksTimer > 0)
    {
        m_fMultiClicksTimer -= delta;
        if (m_fMultiClicksTimer <= 0)
        {
            m_pOwner->onButtonEvent(&m_MultiClicksEvent, this);
            m_fMultiClicksTimer = MULTICLICKS_TIMER_NEXT;
        }
    }
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void Button::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (m_bVisible) {
    	IGeometry * pGeoToDisplay = m_pGeometryBase;
        bool bNeedPop = false;
        bool bAddMode = false;
        if (m_bClickState)
        {
            switch (m_ClickedOption)
            {
            case None:
                break;
            case ReplaceTex:
            	pGeoToDisplay = m_pGeometryClicked;
                break;
            case AddTex:
            	pGeoToDisplay->display(iXOffset, iYOffset, cpntColor * docColor);
            	pGeoToDisplay = m_pGeometryClicked;
                break;
            case Decal:
                iXOffset += 3;
                iYOffset += 3;
                break;
            case Scale:
            {
                float coef = 1.2f;
                Vertex fCenter = Jogy::interface->screenTransform(
                        iXOffset + getXPos() + getWidth() / 2,
                        iYOffset + getYPos() + getHeight() / 2);
                Jogy::interface->pushMatrix();
                Jogy::interface->translate(fCenter.x * (1 - coef), fCenter.y * (1 - coef), 0.0f);
                Jogy::interface->scale(coef, coef, 1.0f);
                bNeedPop = true;
                break;
            }
            case Enlight:
            {
                cpntColor = (cpntColor + 1) / 2;
                bAddMode = true;
                break;
            }
            }
        }

        if (!m_bEnabled) {
        	Color c(1,1,1,0.3f);
            cpntColor.multiply(&c);
        } else if (m_bMouseOver && !m_bClickState) {
            switch (m_HoverOption)
            {
            case None:
                break;
            case ReplaceTex:
            	pGeoToDisplay = m_pGeometryHover;
                break;
            case AddTex:
            	pGeoToDisplay->display(iXOffset, iYOffset, cpntColor * docColor);
            	pGeoToDisplay = m_pGeometryHover;
                break;
            case Decal:
                iXOffset += 3;
                iYOffset += 3;
                break;
            case Scale:
            {
                float coef = 1.2f;
                Vertex fCenter = Jogy::interface->screenTransform(
                        iXOffset + getXPos() + getWidth() / 2,
                        iYOffset + getYPos() + getHeight() / 2);
                Jogy::interface->pushMatrix();
                Jogy::interface->translate(fCenter.x * (1 - coef), fCenter.y * (1 - coef), 0.0f);
                Jogy::interface->scale(coef, coef, 1.0f);
                bNeedPop = true;
                break;
            }
            case Enlight:
            {
                cpntColor = (cpntColor + 1) / 2;
                bAddMode = true;
                break;
            }
            }
        }
        bool bPrevMode = false;
        if (bAddMode) {
            bPrevMode = Jogy::interface->setAdditiveMode(true);
        }
        pGeoToDisplay->display(iXOffset, iYOffset, cpntColor * docColor);
        m_pLabel->displayAt(iXOffset, iYOffset, cpntColor, docColor);
        if (m_pGeometryAttachedImage != NULL) {
            m_pGeometryAttachedImage->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor * m_DiffuseColor);
        }
        if (bAddMode) {
        	Jogy::interface->setAdditiveMode(bPrevMode);
        }
        if (bNeedPop) {
        	Jogy::interface->popMatrix();
        }
    }
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
Object * Button::onButtonEvent(ButtonAction * pEvent)
{
    if ((pEvent->eButton != Button1 && (pEvent->eButton != Button2 || !m_bCatchButton2Events)) || !m_bEnabled || !m_bVisible) {
        return NULL;
    }
    switch (pEvent->eEvent)
    {
    case EventDown:
    {
        m_bMouseDown = true;
        m_bMouseOver = true;
        m_bClickState = true;
// TODO audio        AudioManager::getInstance()->playSound(SOUND_CLICK);
        if (m_bMultiClicks)
        {
            m_fMultiClicksTimer = MULTICLICKS_TIMER_FIRST;
            memcpy(&m_MultiClicksEvent, pEvent, sizeof(ButtonAction));
            bool bClose = doClick(pEvent);
            return bClose ? NULL : this;
        }
        return this;
    }
    case EventUp:
    {
        m_bMouseDown = false;
        m_bClickState = false;
        bool bClose = false;
        if (!m_bMultiClicks && m_bMouseOver) {
            bClose = doClick(pEvent);
        }
        return bClose ? NULL : this;
    }
    case EventDrag:
    {
        m_bClickState = m_bMouseOver = isAt(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset);
        return this;
    }
    case EventDoubleClick:
    {
        if (m_bCatchDoubleClicks)
        {
            bool bClose = false;
            if (m_bMouseOver && m_pOwner != NULL) {
                bClose = !m_pOwner->onButtonEvent(pEvent, this);
            }
            return bClose ? NULL : this;
        }
        break;
    }
    default:
        break;
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : doClick
// -----------------------------------------------------------------
bool Button::doClick(ButtonAction * pEvent)
{
    bool bClose = false;
	if (m_ClickCallback != NULL) {
		bClose = !m_ClickCallback(m_pOwner);
	} else if (m_pOwner != NULL) {
        bClose = !m_pOwner->onButtonEvent(pEvent, this);
	}
	return bClose;
}

// -----------------------------------------------------------------
// Name : onCursorMoveEvent
// -----------------------------------------------------------------
Object * Button::onCursorMoveEvent(int xPxl, int yPxl)
{
    m_bMouseOver = true;
    return this;
}

// -----------------------------------------------------------------
// Name : onCursorMoveOutEvent
// -----------------------------------------------------------------
void Button::onCursorMoveOutEvent()
{
    m_bMouseOver = false;
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void Button::onResize(int iOldWidth, int iOldHeight)
{
    if (iOldWidth == m_iWidth && iOldHeight == m_iHeight) {
        return;
    }

    if (m_pGeometryBase != NULL) {
        QuadData quad(0, m_iWidth, 0, m_iHeight, m_pGeometryBase->getTexture());
        m_pGeometryBase->build(&quad);
    }
    if (m_pGeometryClicked != NULL) {
        QuadData quad(0, m_iWidth, 0, m_iHeight, m_pGeometryClicked->getTexture());
        m_pGeometryClicked->build(&quad);
    }
    if (m_pGeometryAttachedImage != NULL) {
        QuadData quad(0, m_iWidth, 0, m_iHeight, m_pGeometryAttachedImage->getTexture());
        m_pGeometryAttachedImage->build(&quad);
    }
    if (m_pGeometryHover != NULL) {
        QuadData quad(0, m_iWidth, 0, m_iHeight, m_pGeometryHover->getTexture());
        m_pGeometryHover->build(&quad);
    }

    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : setBaseTexture
// -----------------------------------------------------------------
void Button::setBaseTexture(ITexture * pTex)
{
    m_pGeometryBase->setTexture(pTex);
}

// -----------------------------------------------------------------
// Name : setText
// -----------------------------------------------------------------
void Button::setText(string sText)
{
    m_pLabel->setText(sText);
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : moveTo
// -----------------------------------------------------------------
void Button::moveTo(int xPxl, int yPxl)
{
    Component::moveTo(xPxl, yPxl);
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : moveBy
// -----------------------------------------------------------------
void Button::moveBy(int xPxl, int yPxl)
{
    Component::moveBy(xPxl, yPxl);
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : autoPad
// -----------------------------------------------------------------
void Button::autoPad(int margin)
{
    setWidth(m_pLabel->getWidth() + 2 * margin);
    setHeight(m_pLabel->getHeight() + 2 * margin);
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : autoPadWidth
// -----------------------------------------------------------------
void Button::autoPadWidth(int margin, int minWidth)
{
    int newWidth = m_pLabel->getWidth() + 2 * margin;
    if (newWidth < minWidth) {
        setWidth(minWidth);
    } else {
        setWidth(newWidth);
    }
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : attachImage
// -----------------------------------------------------------------
void Button::attachImage(ITexture * pTex, IGeometryQuads * pImageGeo)
{
	FREE(m_pGeometryAttachedImage);
	QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
	m_pGeometryAttachedImage = pImageGeo;
	m_pGeometryAttachedImage->build(&quad);
}

// -----------------------------------------------------------------
// Name : setEnabled
// -----------------------------------------------------------------
void Button::setEnabled(bool bEnabled)
{
    Component::setEnabled(bEnabled);
    if (!bEnabled)
    {
        m_bMouseDown = m_bMouseOver = false;
        m_bClickState = false;
    }
}
}
