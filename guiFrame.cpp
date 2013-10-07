#include "guiFrame.h"
#include "FrameEffects/guiFrameIntro.h"
#include "FrameEffects/guiFrameOutro.h"
#include "FrameEffects/guiFrameMouseFocus.h"
#include "FrameEffects/guiFrameFlash.h"

#define INTRO_EFFECT_ID     100
#define OUTRO_EFFECT_ID     101
#define FOCUS_EFFECT_ID     102
#define FLASH_EFFECT_ID     103
#define RETRACT_DELAY       0.3f
#define RETRACT_TEMP_DELAY  2.0f
#define RETRACT_MARGIN      7

// -----------------------------------------------------------------
// Name : guiFrame
// -----------------------------------------------------------------
guiFrame::guiFrame() : guiContainer()
{
    m_PositionType = FP_Floating;
    m_bFocused = true;
    m_iDragXPos = 0;
    m_iDragYPos = 0;
    m_bIsPointed = false;
    m_uRetractBorder = 0;
    m_iRetractState = 2;
    m_pStickGeo = NULL;
    m_pStickedGeo = NULL;
    m_bMovable = true;
    m_fRetractTimer = 0;
    m_iStickX = m_iStickY = 0;
    m_bSticked = false;
}

// -----------------------------------------------------------------
// Name : ~guiFrame
//  Destructor
// -----------------------------------------------------------------
guiFrame::~guiFrame()
{
    FREEVEC(m_pEffects);
    FREE(m_pStickGeo);
    FREE(m_pStickedGeo);
}

// -----------------------------------------------------------------
// Name : init
// -----------------------------------------------------------------
void guiFrame::init(FramePosition positionType, FrameFitBehavior widthFit, FrameFitBehavior heightFit, int iXOffset, int iYOffset, int iMaxWidth, int iMaxHeight, ITexture ** pMainTexs, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl, IGeometryQuads * pGeometry, IStencilGeometry * pStencil)
{
    guiContainer::init(widthFit, heightFit, iXOffset, iYOffset, iMaxWidth, iMaxHeight, pMainTexs, sCpntId, xPxl, yPxl, wPxl, hPxl, pGeometry, pStencil);
    m_PositionType = positionType;
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void guiFrame::update(double delta)
{
    if (!m_bVisible) {
        return;
    }

    // Update effects
    for (list<guiFrameEffect*>::iterator it = m_pEffects.begin(); it != m_pEffects.end(); ++it) {
        if ((*it)->isActive())
        {
        	(*it)->update(delta);
            if ((*it)->isFinished() && (*it)->getOnFinished() == EFFECT_REMOVE_ON_FINISHED)
            {
            	delete *it;
            	it = m_pEffects.erase(it);
            }
        }
    }

    // Retract?
    if (!m_bSticked && m_uRetractBorder != 0 && m_fRetractTimer > 0)
    {
        m_fRetractTimer -= delta;
        if (m_fRetractTimer < RETRACT_DELAY)
        {
            if (m_fRetractTimer < 0) {
                m_fRetractTimer = 0;
            }
            switch (m_uRetractBorder)
            {
            case 1: // retract to top
            {
                if (m_iRetractState == 1) {
                    moveTo(getXPos(), (m_fRetractTimer / RETRACT_DELAY) * (RETRACT_MARGIN - getHeight()));
                } else {
                    moveTo(getXPos(), (1 - (m_fRetractTimer / RETRACT_DELAY)) * (RETRACT_MARGIN - getHeight()));
                }
                break;
            }
            case 2: // retract to right
            {
                if (m_iRetractState == 1) {
                    moveTo(_params->getScreenXSize() - ((m_fRetractTimer / RETRACT_DELAY) * (RETRACT_MARGIN - getWidth()) + getWidth()), getYPos());
                } else {
                    moveTo(_params->getScreenXSize() - ((m_fRetractTimer / RETRACT_DELAY) * (getWidth() - RETRACT_MARGIN) + RETRACT_MARGIN), getYPos());
                }
                break;
            }
            case 3: // retract to bottom
            {
                if (m_iRetractState == 1) {
                    moveTo(getXPos(), _params->getScreenYSize() - ((m_fRetractTimer / RETRACT_DELAY) * (RETRACT_MARGIN - getHeight()) + getHeight()));
                } else {
                    moveTo(getXPos(), _params->getScreenYSize() - ((m_fRetractTimer / RETRACT_DELAY) * (getHeight() - RETRACT_MARGIN) + RETRACT_MARGIN));
                }
                break;
            }
            case 4: // retract to left
            {
                if (m_iRetractState == 1) {
                    moveTo((m_fRetractTimer / RETRACT_DELAY) * (RETRACT_MARGIN - getWidth()), getYPos());
                } else {
                    moveTo((1 - (m_fRetractTimer / RETRACT_DELAY)) * (RETRACT_MARGIN - getWidth()), getYPos());
                }
                break;
            }
            }
            if (m_fRetractTimer == 0) {
                m_iRetractState *= 2;
            }
        }
    }

    if (!m_bSticked && !m_bIsPointed && m_uRetractBorder != 0 && m_iRetractState == 2)
    {
        m_fRetractTimer = RETRACT_DELAY + RETRACT_TEMP_DELAY;
        m_iRetractState = -1;
    }

    m_bIsPointed = false;
    guiContainer::update(delta);
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiFrame::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }

	for (guiFrameEffect* &pEffect : m_pEffects) {
        if (pEffect->isActive()) {
        	pEffect->onBeginDisplay(iXOffset, iYOffset, &cpntColor, &docColor);
        }
    }

    guiContainer::displayAt(iXOffset, iYOffset, cpntColor, docColor);
    if (m_pStickGeo)
    {
        m_iStickX = iXOffset + getXPos() + getWidth() - 16;
        m_iStickY = iYOffset + getYPos();
        if (m_bSticked) {
            m_pStickedGeo->display(m_iStickX, m_iStickY, cpntColor);
        } else {
            m_pStickGeo->display(m_iStickX, m_iStickY, cpntColor);
        }
    }

	for (guiFrameEffect* &pEffect : m_pEffects) {
        if (pEffect->isActive()) {
        	pEffect->onEndDisplay();
        }
    }
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiFrame::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible || m_pDoc == NULL) {
        return NULL;
    }

    guiObject * pObj = guiContainer::onButtonEvent(pEvent);
    if (pObj != NULL) {
        return pObj;
    }

    if (pEvent->eButton == Button1) {
        if (pEvent->eEvent == Event_Down) {
            if (m_pStickGeo && pEvent->xPos >= m_iStickX && pEvent->xPos <= m_iStickX+16 && pEvent->yPos >= m_iStickY && pEvent->yPos <= m_iStickY+16) {
                m_bSticked = !m_bSticked;
                return this;
            }
            if (m_bMovable) {  // prepare for dragging
                m_iDragXPos = m_iXPxl;
                m_iDragYPos = m_iYPxl;
                return this;
            }
        } else if (pEvent->eEvent == Event_Up) {
            return this;
        } else if (pEvent->eEvent == Event_DoubleClick) {
            m_bFocused = !m_bFocused;
            return this;
        } else if (pEvent->eEvent == Event_Drag && m_bMovable) {
            m_iDragXPos += pEvent->xPos - pEvent->xPosInit;
            m_iDragYPos += pEvent->yPos - pEvent->yPosInit;
            return this;
        }
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : onCursorMoveEvent
// -----------------------------------------------------------------
guiObject * guiFrame::onCursorMoveEvent(int xPxl, int yPxl)
{
    if (!m_bVisible) {
        return NULL;
    }

    m_bIsPointed = true;
    extract();

    return guiContainer::onCursorMoveEvent(xPxl, yPxl);
}

// -----------------------------------------------------------------
// Name : show
// -----------------------------------------------------------------
void guiFrame::show()
{
    setVisible(true);
    activateEffect(INTRO_EFFECT_ID);
}

// -----------------------------------------------------------------
// Name : hide
// -----------------------------------------------------------------
void guiFrame::hide()
{
    activateEffect(OUTRO_EFFECT_ID);
}

// -----------------------------------------------------------------
// Name : removeEffect
// -----------------------------------------------------------------
void guiFrame::removeEffect(u16 uEffectId)
{
    for (list<guiFrameEffect*>::iterator it = m_pEffects.begin(); it != m_pEffects.end(); ++it) {
        if ((*it)->getId() == uEffectId) {
        	delete *it;
        	m_pEffects.erase(it);
            return;
        }
    }
}

// -----------------------------------------------------------------
// Name : activateEffect
// -----------------------------------------------------------------
void guiFrame::activateEffect(u16 uEffectId)
{
	for (guiFrameEffect* &pEffect : m_pEffects) {
        if (pEffect->getId() == uEffectId) {
        	pEffect->reset();
            break;
        }
    }
}

// -----------------------------------------------------------------
// Name : checkPositionIfDragged
// -----------------------------------------------------------------
void guiFrame::checkPositionIfDragged()
{
    if ((m_iDragXPos == m_iXPxl && m_iDragYPos == m_iYPxl) || m_PositionType == FP_Fixed) {
        return;
    }

    // Stuck to screen borders
    int dx = -m_iDragXPos;
    if (abs(dx) < 10) {
        moveTo(0, m_iYPxl);
    } else {
        dx = _params->getScreenXSize() - (m_iDragXPos + m_iWidth);
        if (abs(dx) < 10) {
            moveTo(_params->getScreenXSize() + 1 - m_iWidth, m_iYPxl);
        } else {
            moveTo(m_iDragXPos, m_iYPxl);
        }
    }

    int dy = -m_iDragYPos;
    if (abs(dy) < 10) {
        moveTo(m_iXPxl, 0);
    } else {
        dy = _params->getScreenYSize() - (m_iDragYPos + m_iHeight);
        if (abs(dy) < 10) {
            moveTo(m_iXPxl, _params->getScreenYSize() + 1 - m_iHeight);
        } else {
            moveTo(m_iXPxl, m_iDragYPos);
        }
    }

    // Now check that it's not completly out of screen
    if (m_iXPxl + m_iWidth < 5) {
        moveTo(5 - m_iWidth, m_iYPxl);
    } else if (m_iXPxl > _params->getScreenXSize() - 5) {
        moveTo(_params->getScreenXSize() - 5, m_iYPxl);
    }
    if (m_iYPxl + m_iHeight < 5) {
        moveTo(m_iXPxl, 5 - m_iHeight);
    } else if (m_iYPxl > _params->getScreenYSize() - 5) {
        moveTo(m_iXPxl, _params->getScreenYSize() - 5);
    }
}

// -----------------------------------------------------------------
// Name : flash
// -----------------------------------------------------------------
void guiFrame::flash(float fFlashTime)
{
    guiFrameFlash * pEffect = new guiFrameFlash(FLASH_EFFECT_ID, fFlashTime);
    addEffect(pEffect);
    pEffect->setActive(true);
}

// -----------------------------------------------------------------
// Name : addEffect
// -----------------------------------------------------------------
void guiFrame::addEffect(guiFrameEffect * pEffect)
{
    m_pEffects.push_back(pEffect);
    pEffect->setFrame(this);
}

// -----------------------------------------------------------------
// Name : setRetractible
// -----------------------------------------------------------------
void guiFrame::setRetractible(u8 uBorder)
{
    m_uRetractBorder = uBorder;
    m_fRetractTimer = 0;
    m_iRetractState = 2;  // shown
    m_bSticked = false;
    FREE(m_pStickGeo);
    FREE(m_pStickedGeo);
    if (uBorder != 0)
    {
        QuadData quad(0, 15, 0, 15, "stick");
        m_pStickGeo = new GeometryQuads(&quad, VB_Static);
        QuadData quad2(0, 15, 0, 15, "sticked");
        m_pStickedGeo = new GeometryQuads(&quad2, VB_Static);
    }
}

// -----------------------------------------------------------------
// Name : extract
// -----------------------------------------------------------------
void guiFrame::extract()
{
    if (!m_bSticked && m_uRetractBorder != 0 && m_iRetractState < 0)
    {
        if (m_fRetractTimer >= RETRACT_DELAY)
        {
            m_fRetractTimer = 0;
            m_iRetractState = 2;
        }
        else
        {
            m_fRetractTimer = RETRACT_DELAY - m_fRetractTimer;
            m_iRetractState = 1;
        }
    }
}

// -----------------------------------------------------------------
// Name : setEnabled
// -----------------------------------------------------------------
void guiFrame::setEnabled(bool bEnabled)
{
    guiContainer::setEnabled(bEnabled);
	for (guiFrameEffect* &pEffect : m_pEffects) {
        if (!bEnabled && pEffect->isActive() && pEffect->getOnFinished() == EFFECT_ACTIVATE_ON_FINISHED) {
        	pEffect->setOnFinished(EFFECT_NOTHING_ON_FINISHED);
        }
    }
}

// -----------------------------------------------------------------
// Name : centerOnScreen
// -----------------------------------------------------------------
void guiFrame::centerOnScreen()
{
    moveTo((_params->getScreenXSize() - getWidth()) / 2, (_params->getScreenYSize() - getHeight()) / 2);
}

//// -----------------------------------------------------------------
//// Name : createDefaultFrame
////  Static default constructor
//// -----------------------------------------------------------------
//guiFrame * guiFrame::createDefaultFrame(FrameFitBehavior widthFit, FrameFitBehavior heightFit, int width, int height, bool bAlpha, string sId)
//{
//    guiFrame * pFrame = new guiFrame();
//    Texture * iTexs[8];
//    iTexs[0] = _tex->findTexture("gui/interface:FrmTL");
//    iTexs[1] = _tex->findTexture("gui/interface:FrmTC");
//    iTexs[2] = _tex->findTexture("gui/interface:FrmTR");
//    iTexs[3] = _tex->findTexture("gui/interface:FrmCL");
//    iTexs[4] = _tex->findTexture("gui/interface:FrmCR");
//    iTexs[5] = _tex->findTexture("gui/interface:FrmBL");
//    iTexs[6] = _tex->findTexture("gui/interface:FrmBC");
//    iTexs[7] = _tex->findTexture("gui/interface:FrmBR");
//    int maxw = (widthFit == FB_FitFrameToDocumentWhenSmaller) ? width : 0;
//    int maxh = (heightFit == FB_FitFrameToDocumentWhenSmaller) ? height : 0;
//    pFrame->init(FP_Floating, widthFit, heightFit, 0, 0, maxw, maxh, iTexs, sId, 0, 0, width, height);
//
//    guiFrameEffect * pEffect = new guiFrameIntro(INTRO_EFFECT_ID, 0.5f);
//    pEffect->setActive(false);
//    pFrame->addEffect(pEffect);
//    pEffect = new guiFrameOutro(OUTRO_EFFECT_ID, 0.5f);
//    pEffect->setActive(false);
//    pFrame->addEffect(pEffect);
//
//    if (bAlpha)
//    {
//        pEffect = new guiFrameMouseFocus(FOCUS_EFFECT_ID, 1.0f);
//        pFrame->addEffect(pEffect);
//    }
//    return pFrame;
//}
