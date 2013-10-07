#include "guiButton.h"
#include "ComponentOwnerInterface.h"

#define MULTICLICKS_TIMER_FIRST     1.0f
#define MULTICLICKS_TIMER_NEXT      0.1f

// -----------------------------------------------------------------
// Name : guiButton
//  Constructor
// -----------------------------------------------------------------
guiButton::guiButton() : guiImage()
{
    m_ClickOption = BCO_None;
    m_OverOption = BCO_None;
    m_bMouseDown = m_bMouseOver = false;
    m_bClickState = false;
    m_pLabel = new guiLabel();
    m_pGeometryClicked = NULL;
    m_pGeometryOver = NULL;
    m_pGeometryNormal = NULL;
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
    FREE(m_pGeometryOver);
    FREE(m_pGeometryAttachedImage);
    m_pGeometry = m_pGeometryNormal;
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void guiButton::init(string sText, fontid fontId, Color textColor, ITexture * pClickedTex, BtnClickOptions clickOption, ITexture * pOverTex, BtnClickOptions overOption, ITexture * pTex, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl, IGeometryQuads * pLabelGeo, IGeometryQuads * pImgGeo, IGeometryQuads * pClickedGeo, IGeometryQuads * pOverGeo)
{
    guiImage::init(pTex, sCpntId, xPxl, yPxl, wPxl, hPxl, pImgGeo);
    m_pLabel->init(sText, fontId, textColor, "ButtonLabel", 0, 0, 0, 0, pLabelGeo);
    m_pLabel->centerOn(this);
    m_pGeometryNormal = (GeometryQuads*) pImgGeo;
    m_ClickOption = clickOption;
    if (m_ClickOption == BCO_ReplaceTex || m_ClickOption == BCO_AddTex)
    {
        m_pGeometryClicked = pClickedGeo;
        QuadData quad(0, m_iWidth, 0, m_iHeight, pClickedTex);
        pClickedGeo->build(&quad);
    }
    m_OverOption = overOption;
    if (m_OverOption == BCO_ReplaceTex || m_OverOption == BCO_AddTex)
    {
    	m_pGeometryOver = pOverGeo;
        QuadData quad(0, m_iWidth, 0, m_iHeight, pOverTex);
        m_pGeometryOver->build(&quad);
    }
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void guiButton::update(double delta)
{
    guiImage::update(delta);
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
    if (m_bVisible)
    {
        m_pGeometry = m_pGeometryNormal;
        bool bNeedPop = false;
        bool bAddMode = false;
        if (m_bClickState)
        {
            switch (m_ClickOption)
            {
            case BCO_None:
                break;
            case BCO_ReplaceTex:
                m_pGeometry = m_pGeometryClicked;
                break;
            case BCO_AddTex:
                guiImage::displayAt(iXOffset, iYOffset, cpntColor, docColor);
                m_pGeometry = m_pGeometryClicked;
                break;
            case BCO_Decal:
                iXOffset += 3;
                iYOffset += 3;
                break;
            case BCO_Scale:
            {
                float coef = 1.2f;
                Vertex fCenter = IGeometry::screenTransform(
                        iXOffset + getXPos() + getWidth() / 2,
                        iYOffset + getYPos() + getHeight() / 2);
                glPushMatrix();
                glTranslatef(fCenter.x * (1 - coef), fCenter.y * (1 - coef), 0.0f);
                glScalef(coef, coef, 1.0f);
                bNeedPop = true;
                break;
            }
            case BCO_Enlight:
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
            switch (m_OverOption)
            {
            case BCO_None:
                break;
            case BCO_ReplaceTex:
                m_pGeometry = m_pGeometryOver;
                break;
            case BCO_AddTex:
                guiImage::displayAt(iXOffset, iYOffset, cpntColor, docColor);
                m_pGeometry = m_pGeometryOver;
                break;
            case BCO_Decal:
                iXOffset += 3;
                iYOffset += 3;
                break;
            case BCO_Scale:
            {
                float coef = 1.2f;
                Vertex fCenter = IGeometry::screenTransform(
                        iXOffset + getXPos() + getWidth() / 2,
                        iYOffset + getYPos() + getHeight() / 2);
                glPushMatrix();
                glTranslatef(fCenter.x * (1 - coef), fCenter.y * (1 - coef), 0.0f);
                glScalef(coef, coef, 1.0f);
                bNeedPop = true;
                break;
            }
            case BCO_Enlight:
            {
                cpntColor = (cpntColor + 1) / 2;
                bAddMode = true;
                break;
            }
            }
        }
        bool bPrevMode = false;
        if (bAddMode) {
            bPrevMode = _display->setAdditiveMode(true);
        }
        guiImage::displayAt(iXOffset, iYOffset, cpntColor, docColor);
        m_pLabel->displayAt(iXOffset, iYOffset, cpntColor, docColor);
        if (m_pGeometryAttachedImage != NULL) {
            m_pGeometryAttachedImage->display(m_iXPxl + iXOffset, m_iYPxl + iYOffset, cpntColor * m_DiffuseColor);
        }
        if (bAddMode) {
            _display->setAdditiveMode(bPrevMode);
        }
        if (bNeedPop) {
            glPopMatrix();
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
    IGeometry * ptmp = m_pGeometry;
    m_pGeometry = m_pGeometryNormal;
    guiImage::onResize(iOldWidth, iOldHeight);
    m_pGeometry = m_pGeometryClicked;
    guiImage::onResize(iOldWidth, iOldHeight);
    m_pGeometry = m_pGeometryAttachedImage;
    guiImage::onResize(iOldWidth, iOldHeight);
    m_pGeometry = ptmp;
    m_pLabel->centerOn(this);
}

// -----------------------------------------------------------------
// Name : setNormalTexture
// -----------------------------------------------------------------
void guiButton::setNormalTexture(ITexture * pTex)
{
    m_pGeometryNormal->setTexture(pTex);
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
    guiImage::setEnabled(bEnabled);
    if (!bEnabled)
    {
        m_bMouseDown = m_bMouseOver = false;
        m_bClickState = false;
    }
}

//// -----------------------------------------------------------------
//// Name : createDefaultNormalButton
////  Static default constructor
////  Use it to avoid passing always the same 3591218 arguments to "init"
//// -----------------------------------------------------------------
//guiButton * guiButton::createDefaultNormalButton(string sText, string sId)
//{
//    guiButton * pBtn = new guiButton();
//    pBtn->init(
//        sText, H2_FONT, H2_COLOR,
//        _tex->findTexture("gui/interface:Transparent"),
//        BCO_None,
//        _tex->findTexture("gui/interface:Transparent"),
//        BCO_Decal,
//        _tex->findTexture("gui/interface:Transparent"),
//        sId, 0, 0, 50, 32);
//    return pBtn;
//}
//
//// -----------------------------------------------------------------
//// Name : createDefaultSmallButton
////  Static default constructor
////  Use it to avoid passing always the same 3591218 arguments to "init"
//// -----------------------------------------------------------------
//guiButton * guiButton::createDefaultSmallButton(string sText, int width, string sId)
//{
//    guiButton * pBtn = new guiButton();
//    pBtn->init(
//        sText, TEXT_FONT, TEXT_COLOR,
//        _tex->findTexture("gui/interface:SmallButtonClicked"),
//        BCO_ReplaceTex,
//        NULL, BCO_None,
//        _tex->findTexture("gui/interface:SmallButtonNormal"),
//        sId, 0, 0, width, 20);
//    return pBtn;
//}
//
//// -----------------------------------------------------------------
//// Name : createDefaultWhiteButton
////  Static default constructor
////  Use it to avoid passing always the same 3591218 arguments to "init"
//// -----------------------------------------------------------------
//guiButton * guiButton::createDefaultWhiteButton(string sText, int width, int height, string sId)
//{
//    guiButton * pBtn = new guiButton();
//    pBtn->init(
//        sText, TEXT_FONT, TEXT_COLOR,
//        _tex->findTexture("gui/interface:WhiteButtonClicked"),
//        BCO_ReplaceTex,
//        NULL, BCO_None,
//        _tex->findTexture("gui/interface:WhiteButtonNormal"),
//        sId, 0, 0, width, height);
//    return pBtn;
//}
//
//// -----------------------------------------------------------------
//// Name : createDefaultImageButton
////  Static default constructor
////  Use it to avoid passing always the same 3591218 arguments to "init"
//// -----------------------------------------------------------------
//guiButton * guiButton::createDefaultImageButton(Texture * pTex, string sId)
//{
//    guiButton * pBtn = new guiButton();
//    pBtn->init(
//        "", TEXT_FONT, TEXT_COLOR,
//        NULL, BCO_Decal, NULL, BCO_None,
//        pTex, sId, 0, 0, -1, -1);
//    return pBtn;
//}
