#include "guiButton.h"
#include "ComponentOwnerInterface.h"

#define MULTICLICKS_TIMER_FIRST     1.0f
#define MULTICLICKS_TIMER_NEXT      0.1f

// -----------------------------------------------------------------
// Name : guiButton
//  Constructor
// -----------------------------------------------------------------
guiButton::guiButton() : guiComponent()
{
    m_ClickedOption = None;
    m_HoverOption = None;
    m_bMouseDown = m_bMouseOver = false;
    m_bClickState = false;
    m_pLabel = new guiLabel();
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
// Name : ~guiButton
//  Destructor
// -----------------------------------------------------------------
guiButton::~guiButton()
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
guiButton * guiButton::withLabel(string sText, fontid fontId, Color textColor, IGeometryText * pLabelGeo)
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
guiButton * guiButton::withBaseGeometry(ITexture * pTex, IGeometryQuads * pGeo)
{
    m_pGeometryBase = pGeo;
    QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : withClickedGeometry
// -----------------------------------------------------------------
guiButton * guiButton::withClickedGeometry(ITexture * pTex, IGeometryQuads * pGeo)
{
    m_pGeometryClicked = pGeo;
    QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : withHoverGeometry
// -----------------------------------------------------------------
guiButton * guiButton::withHoverGeometry(ITexture * pTex, IGeometryQuads * pGeo)
{
    m_pGeometryHover = pGeo;
    QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
guiButton * guiButton::build()
{
    guiComponent::build();
    m_pLabel->centerOn(this);
    return this;
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void guiButton::update(double delta)
{
    guiComponent::update(delta);
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
void guiButton::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
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
guiObject * guiButton::onButtonEvent(ButtonAction * pEvent)
{
    if ((pEvent->eButton != Button1 && (pEvent->eButton != Button2 || !m_bCatchButton2Events)) || !m_bEnabled || !m_bVisible) {
        return NULL;
    }
    switch (pEvent->eEvent)
    {
    case Event_Down:
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
    case Event_Up:
    {
        m_bMouseDown = false;
        m_bClickState = false;
        bool bClose = false;
        if (!m_bMultiClicks && m_bMouseOver) {
            bClose = doClick(pEvent);
        }
        return bClose ? NULL : this;
    }
    case Event_Drag:
    {
        m_bClickState = m_bMouseOver = isAt(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset);
        return this;
    }
    case Event_DoubleClick:
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
bool guiButton::doClick(ButtonAction * pEvent)
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
guiObject * guiButton::onCursorMoveEvent(int xPxl, int yPxl)
{
    m_bMouseOver = true;
    return this;
}

// -----------------------------------------------------------------
// Name : onCursorMoveOutEvent
// -----------------------------------------------------------------
void guiButton::onCursorMoveOutEvent()
{
    m_bMouseOver = false;
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void guiButton::onResize(int iOldWidth, int iOldHeight)
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
void guiButton::setBaseTexture(ITexture * pTex)
{
    m_pGeometryBase->setTexture(pTex);
}

// -----------------------------------------------------------------
// Name : setText
// -----------------------------------------------------------------
void guiButton::setText(string sText)
{
    m_pLabel->setText(sText);
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : moveTo
// -----------------------------------------------------------------
void guiButton::moveTo(int xPxl, int yPxl)
{
    guiComponent::moveTo(xPxl, yPxl);
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : moveBy
// -----------------------------------------------------------------
void guiButton::moveBy(int xPxl, int yPxl)
{
    guiComponent::moveBy(xPxl, yPxl);
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : autoPad
// -----------------------------------------------------------------
void guiButton::autoPad(int margin)
{
    setWidth(m_pLabel->getWidth() + 2 * margin);
    setHeight(m_pLabel->getHeight() + 2 * margin);
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : autoPadWidth
// -----------------------------------------------------------------
void guiButton::autoPadWidth(int margin, int minWidth)
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
void guiButton::attachImage(ITexture * pTex, IGeometryQuads * pImageGeo)
{
	FREE(m_pGeometryAttachedImage);
	QuadData quad(0, m_iWidth, 0, m_iHeight, pTex);
	m_pGeometryAttachedImage = pImageGeo;
	m_pGeometryAttachedImage->build(&quad);
}

// -----------------------------------------------------------------
// Name : setEnabled
// -----------------------------------------------------------------
void guiButton::setEnabled(bool bEnabled)
{
    guiComponent::setEnabled(bEnabled);
    if (!bEnabled)
    {
        m_bMouseDown = m_bMouseOver = false;
        m_bClickState = false;
    }
}
