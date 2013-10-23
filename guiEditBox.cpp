#include "guiEditBox.h"

#define CARET_BLINK_DELAY     0.4f

// -----------------------------------------------------------------
// Name : guiEditBox
// -----------------------------------------------------------------
guiEditBox::guiEditBox() : m_TextColor(1.0f, 1.0f, 1.0f, 1.0f)
{
    m_pStencilGeometry = NULL;
    m_pTextGeometry = NULL;
    m_pCaretGeometry = NULL;
    m_pSelectionGeometry = NULL;
    m_iNbLines = 0;
    m_sText = "";
    m_FontId = 0;
    m_iCaretPos = 0;
    m_bHasFocus = false;
    m_fBlinkTimer = 0;
    m_iXScrollPos = m_iYScrollPos = 0;
    m_iSelectionStart = -1;
    m_bMultiLines = false;
    m_iSelectionEnd = 0;
    m_pMainGeometry = NULL;
    m_iTLBorderWidth = 0;
    m_iTLBorderHeight = 0;
    m_iBRBorderWidth = 0;
    m_iBRBorderHeight = 0;
}

// -----------------------------------------------------------------
// Name : ~guiEditBox
//  Destructor
// -----------------------------------------------------------------
guiEditBox::~guiEditBox() {
	Jogy::interface->unregisterKeyboardListener(this);
    FREE(m_pStencilGeometry);
    FREE(m_pTextGeometry);
    FREE(m_pCaretGeometry);
    FREE(m_pSelectionGeometry);
    FREE(m_pMainGeometry);
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
guiEditBox * guiEditBox::build() {
    guiComponent::build();

    ITexture * pTLTex = m_pMainGeometry->getTexture(0);
    ITexture * pBRTex = m_pMainGeometry->getTexture(8);

    // Set height according to number of lines
    setHeight(pTLTex->getHeight() + pBRTex->getHeight()
    		+ m_iNbLines * Jogy::interface->computeTextHeight(NULL, m_FontId));

    rebuildMainGeometry();

    // Set borders dimension variables
    ITexture * pTex = m_pMainGeometry->getTexture(0);
    m_iTLBorderWidth = pTex->getWidth();
    m_iTLBorderHeight = pTex->getHeight();

    pTex = m_pMainGeometry->getTexture(8);
    m_iBRBorderWidth = pTex->getWidth();
    m_iBRBorderHeight = pTex->getHeight();

    return this;
}

// -----------------------------------------------------------------
// Name : withMainGeometry
// -----------------------------------------------------------------
guiEditBox * guiEditBox::withMainGeometry(ITexture ** pTex, IGeometryQuads * pGeo)
{
    // Main geometry (borders & background)
    m_pMainGeometry = pGeo;

    // 9 Quads
    QuadData ** pQuads = new QuadData*[9];
    pQuads[0] = new QuadData(0, 1, 0, 1, pTex[0]);
    pQuads[1] = new QuadData(1, 2, 0, 1, pTex[1]);
    pQuads[2] = new QuadData(2, 3, 0, 1, pTex[2]);
    pQuads[3] = new QuadData(0, 1, 1, 2, pTex[3]);
    pQuads[4] = new QuadData(1, 2, 1, 2, pTex[4]);
    pQuads[5] = new QuadData(2, 3, 1, 2, pTex[5]);
    pQuads[6] = new QuadData(0, 1, 2, 3, pTex[6]);
    pQuads[7] = new QuadData(1, 2, 2, 3, pTex[7]);
    pQuads[8] = new QuadData(2, 3, 2, 3, pTex[8]);
    m_pMainGeometry->build(9, pQuads);
    QuadData::releaseQuads(9, pQuads);

    return this;
}

// -----------------------------------------------------------------
// Name : withCaretGeometry
// -----------------------------------------------------------------
guiEditBox * guiEditBox::withCaretGeometry(ITexture * pTex, IGeometryQuads * pGeo) {
    m_pCaretGeometry = pGeo;
    QuadData quad(0, pTex->getWidth(), 0, pTex->getHeight(), pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : withStencilGeometry
// -----------------------------------------------------------------
guiEditBox * guiEditBox::withStencilGeometry(ITexture * pTex, IStencilGeometry * pGeo) {
    m_pStencilGeometry = pGeo;
	return this;
}

// -----------------------------------------------------------------
// Name : withSelectionGeometry
// -----------------------------------------------------------------
guiEditBox * guiEditBox::withSelectionGeometry(ITexture * pTex, IGeometryQuads * pGeo) {
    m_pSelectionGeometry = pGeo;
    QuadData quad(0, 1, 0, 1, pTex);
    pGeo->build(&quad);
	return this;
}

// -----------------------------------------------------------------
// Name : withLabel
// -----------------------------------------------------------------
guiEditBox * guiEditBox::withLabel(string sText, fontid fontId, Color textColor, IGeometryText * pGeo) {
    m_FontId = fontId;
    m_sText = sText;
    m_TextColor = textColor;
    m_pTextGeometry = pGeo;
	return this;
}

// -----------------------------------------------------------------
// Name : rebuildMainGeometry
// -----------------------------------------------------------------
void guiEditBox::rebuildMainGeometry()
{
    // 9 Quads
    QuadData ** pQuads = new QuadData*[9];
    int xPxlMiddleStart = m_pMainGeometry->getTexture(0)->getWidth();
    int xPxlMiddleEnd = m_iWidth - m_pMainGeometry->getTexture(2)->getWidth();
    int yPxlMiddleStart = m_pMainGeometry->getTexture(0)->getHeight();
    int yPxlMiddleEnd = m_iHeight - m_pMainGeometry->getTexture(5)->getHeight();
    pQuads[0] = new QuadData(0,               xPxlMiddleStart,  0,                yPxlMiddleStart,  m_pMainGeometry->getTexture(0));
    pQuads[1] = new QuadData(xPxlMiddleStart, xPxlMiddleEnd,    0,                yPxlMiddleStart,  m_pMainGeometry->getTexture(1));
    pQuads[2] = new QuadData(xPxlMiddleEnd,   m_iWidth,         0,                yPxlMiddleStart,  m_pMainGeometry->getTexture(2));
    pQuads[3] = new QuadData(0,               xPxlMiddleStart,  yPxlMiddleStart,  yPxlMiddleEnd,    m_pMainGeometry->getTexture(3));
    pQuads[4] = new QuadData(xPxlMiddleStart, xPxlMiddleEnd,    yPxlMiddleStart,  yPxlMiddleEnd,    m_pMainGeometry->getTexture(4));
    pQuads[5] = new QuadData(xPxlMiddleEnd,   m_iWidth,         yPxlMiddleStart,  yPxlMiddleEnd,    m_pMainGeometry->getTexture(5));
    pQuads[6] = new QuadData(0,               xPxlMiddleStart,  yPxlMiddleEnd,    m_iHeight,        m_pMainGeometry->getTexture(6));
    pQuads[7] = new QuadData(xPxlMiddleStart, xPxlMiddleEnd,    yPxlMiddleEnd,    m_iHeight,        m_pMainGeometry->getTexture(7));
    pQuads[8] = new QuadData(xPxlMiddleEnd,   m_iWidth,         yPxlMiddleEnd,    m_iHeight,        m_pMainGeometry->getTexture(8));

    m_pMainGeometry->build(9, pQuads);
    QuadData::releaseQuads(9, pQuads);
}

// -----------------------------------------------------------------
// Name : update
// -----------------------------------------------------------------
void guiEditBox::update(double delta)
{
    guiComponent::update(delta);
    if (m_bHasFocus)
    {
        m_fBlinkTimer += delta;
        if (m_fBlinkTimer > CARET_BLINK_DELAY) {
            m_fBlinkTimer = -CARET_BLINK_DELAY;
        }
    }
}

// -----------------------------------------------------------------
// Name : displayAt
// -----------------------------------------------------------------
void guiEditBox::displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor)
{
    if (!m_bVisible) {
        return;
    }

    // display borders and background
    int x = m_iXPxl + iXOffset;
    int y = m_iYPxl + iYOffset;
    m_pMainGeometry->display(x, y, cpntColor);

    x += m_iTLBorderWidth;
    y += m_iTLBorderHeight;
    int xStencil = x;
    int yStencil = y;

    m_pStencilGeometry->fillStencil(xStencil, yStencil, true);
//TODO in fillStencil:     int iPreviousState = _display->setStencilState(2);
    // Stencil display block

    x += m_iXScrollPos;
    y += m_iYScrollPos;

    if (hasSelectedText()) {
    	// display selection
    	m_pSelectionGeometry->display(x, y, Color(0.8,0.8,0.8));
    }

    // draw text
    m_pTextGeometry->display(x, y, cpntColor * m_TextColor);

    if (m_bHasFocus && m_fBlinkTimer >= 0) {
    	// Display caret
    	i2d pos = Jogy::interface->computeCharPosInText(m_iCaretPos, m_sText, m_FontId);
    	m_pCaretGeometry->display(x + pos.x, y + pos.y, cpntColor);
    }
    m_pStencilGeometry->fillStencil(xStencil, yStencil, false);
//TODO:     _display->setStencilState(iPreviousState);
}

// -----------------------------------------------------------------
// Name : onButtonEvent
// -----------------------------------------------------------------
guiObject * guiEditBox::onButtonEvent(ButtonAction * pEvent)
{
    if (!m_bEnabled || !m_bVisible) {
        return NULL;
    }
    switch (pEvent->eButton)
    {
    case Button1:
    {
        switch (pEvent->eEvent)
        {
        case Event_Down:
            onButton1Down(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset);
            break;
        case Event_Drag:
            onButton1Drag(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset);
            break;
        case Event_DoubleClick:
            onButton1DoubleClick(pEvent->xPos - pEvent->xOffset, pEvent->yPos - pEvent->yOffset);
            break;
        default:
            break;
        }
        return this;
    }
    case ButtonZ:
    {
        // TODO
        return this;
    }
    case ButtonX:
    {
        // TODO
        return this;
    }
    default:
        break;
    }
    return NULL;
}

// -----------------------------------------------------------------
// Name : onButton1Down
// -----------------------------------------------------------------
void guiEditBox::onButton1Down(int xPxl, int yPxl)
{
    m_bHasFocus = true;
	Jogy::interface->registerKeyboardListener(this);
    deselect();

    m_iCaretPos = Jogy::interface->computeCharIdxInText(xPxl - m_iXPxl - m_iTLBorderWidth,
    		yPxl - m_iYPxl - m_iTLBorderHeight,
    		m_sText, m_FontId);

    i2d pos = Jogy::interface->computeCharPosInText(m_iCaretPos, m_sText, m_FontId);
    if (pos.x > getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) {
        m_iXScrollPos = (getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) - pos.x - 4;
    } else if (pos.x + m_iXScrollPos < 0) {
        m_iXScrollPos = 0;
    }
    int fontHeight = Jogy::interface->computeTextHeight(NULL, m_FontId);
    if (pos.y + fontHeight > getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) {
        m_iYScrollPos = (getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) - pos.y - fontHeight;
    } else if (pos.y + m_iYScrollPos < 0) {
        m_iYScrollPos = 0;
    }
    m_fBlinkTimer = 0;
}

// -----------------------------------------------------------------
// Name : onButton1Drag
// -----------------------------------------------------------------
void guiEditBox::onButton1Drag(int xPxl, int yPxl)
{
    if (!hasSelectedText()) {
        m_iSelectionStart = m_iSelectionEnd = m_iCaretPos;
    }
    int oldCaretPos = m_iCaretPos;
    m_iCaretPos = Jogy::interface->computeCharIdxInText(xPxl - m_iXPxl - m_iTLBorderWidth,
    		yPxl - m_iYPxl - m_iTLBorderHeight,
    		m_sText, m_FontId);

    if (oldCaretPos != m_iCaretPos) {
        if (m_iCaretPos < m_iSelectionStart)  {
            m_iSelectionStart = m_iCaretPos;
            updateSelectionGeometry();
        } else if (m_iCaretPos > m_iSelectionEnd) {
            m_iSelectionEnd = m_iCaretPos;
            updateSelectionGeometry();
        } else if (oldCaretPos == m_iSelectionStart) {
            m_iSelectionStart = m_iCaretPos;
            if (m_iSelectionEnd < m_iSelectionStart) {
                m_iSelectionStart = m_iSelectionEnd;
                m_iSelectionEnd = m_iCaretPos;
                updateSelectionGeometry();
            } else if (m_iSelectionStart != m_iSelectionEnd) {
                updateSelectionGeometry();
            }
        } else if (oldCaretPos == m_iSelectionEnd) {
            m_iSelectionEnd = m_iCaretPos;
            if (m_iSelectionEnd < m_iSelectionStart) {
                m_iSelectionEnd = m_iSelectionStart;
                m_iSelectionStart = m_iCaretPos;
                updateSelectionGeometry();
            } else if (m_iSelectionStart != m_iSelectionEnd) {
                updateSelectionGeometry();
            }
        }
        i2d pos = Jogy::interface->computeCharPosInText(m_iCaretPos, m_sText, m_FontId);
        if (pos.x > getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) {
            m_iXScrollPos = (getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) - pos.x - 4;
        } else if (pos.x + m_iXScrollPos < 0) {
            m_iXScrollPos = 0;
        }
        int fontHeight = Jogy::interface->computeTextHeight(NULL, m_FontId);
        if (pos.y + fontHeight > getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) {
            m_iYScrollPos = (getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) - pos.y - fontHeight;
        } else if (pos.y + m_iYScrollPos < 0) {
            m_iYScrollPos = 0;
        }
        m_fBlinkTimer = 0;
    }
}

// -----------------------------------------------------------------
// Name : onButton1DoubleClick
// -----------------------------------------------------------------
void guiEditBox::onButton1DoubleClick(int xPxl, int yPxl)
{
    m_iSelectionStart = getStartOfWord(m_iCaretPos);
    m_iCaretPos = m_iSelectionEnd = getEndOfWord(m_iCaretPos);
    if (m_iSelectionEnd != m_iSelectionStart) {
        updateSelectionGeometry();
    }
    i2d pos = Jogy::interface->computeCharPosInText(m_iCaretPos, m_sText, m_FontId);
    if (pos.x > getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) {
        m_iXScrollPos = (getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) - pos.x - 4;
    } else if (pos.x + m_iXScrollPos < 0) {
        m_iXScrollPos = 0;
    }
    int fontHeight = Jogy::interface->computeTextHeight(NULL, m_FontId);
    if (pos.y + fontHeight > getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) {
        m_iYScrollPos = (getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) - pos.y - fontHeight;
    } else if (pos.y + m_iYScrollPos < 0) {
        m_iYScrollPos = 0;
    }
    m_fBlinkTimer = 0;
}

// -----------------------------------------------------------------
// Name : onFocusLost
// -----------------------------------------------------------------
void guiEditBox::onFocusLost()
{
    m_bHasFocus = false;
	Jogy::interface->unregisterKeyboardListener(this);
}

// -----------------------------------------------------------------
// Name : onKeyDown
// -----------------------------------------------------------------
bool guiEditBox::onKeyDown(unsigned char c)
{
    if (m_bHasFocus)
    {
        if (Jogy::interface->isCtrlPressed())
        {
            if (c == 22 || c == 24 || c == 3) // Ctrl+c/v/x : copy/paste/cut ; TODO : does it work in other systems?
            {
                if (hasSelectedText()) // find current selection
                {
                    if (c == 3 || c == 24) // get selected text
                    {
                        int nbChars = m_iSelectionEnd - m_iSelectionStart;
                        char * sClipboard = new char[nbChars+1];
                        for (int i = m_iSelectionStart; i <= m_iSelectionEnd; i++) {
                            sClipboard[i-m_iSelectionStart] = m_sText[i];
                        }
                        sClipboard[nbChars] = '\0';
                        copyStringToClipboard(sClipboard);
                        delete[] sClipboard;
                    }
                    if (c == 24 || c == 22) // delete current selection
                    {
                        int nDel = m_iSelectionEnd - m_iSelectionStart;
                        m_iCaretPos = m_iSelectionStart;
                        int iLength = m_sText.length();
                        for (int i = m_iCaretPos; i < 1 + iLength - nDel; i++) {
                            m_sText[i] = m_sText[i+nDel];
                        }
                        if (m_pTextGeometry != NULL) {
                            m_pTextGeometry->setText(m_sText, m_FontId);
                        }
                    	deselect();
                    }
                }
                if (c == 22) // paste
                {
                    char sBuffer[1024] = "";
                    getStringFromClipboard(sBuffer, 1024);
                    int iLength = m_sText.length();
                    int iBufLen = (int) strlen(sBuffer);
                    m_sText[iLength+iBufLen] = '\0';
                    for (int i = iLength-1; i >= m_iCaretPos; i--) {
                    	m_sText[i+iBufLen] = m_sText[i];
                    }
                    for (int i = 0; i < iBufLen; i++) {
                    	m_sText[i+m_iCaretPos] = sBuffer[i];
                    }
                    m_iCaretPos += iBufLen;
                    if (m_pTextGeometry != NULL) {
                    	m_pTextGeometry->setText(m_sText, m_FontId);
                    }
                }
            }
            return true;
        }
        if (c == 13) {
            c = '\n';
        }
        if (c == '\n' && !m_bMultiLines) {
            return false;
        }
        if (c == 8) // backspace
        {
            if (m_iCaretPos == 0 && !hasSelectedText()) {
                return true;
            }
            // move backward and delete
            if (!hasSelectedText()) {
                m_iCaretPos--;
            }
            c = 127;
        }
        if (c == 127 || hasSelectedText()) // delete
        {
            int nDel = 1;
            if (hasSelectedText()) // delete selection
            {
                nDel = m_iSelectionEnd - m_iSelectionStart;
                m_iCaretPos = m_iSelectionStart;
                deselect();
            }
            int iLength = m_sText.length();
            for (int i = m_iCaretPos; i < 1 + iLength - nDel; i++) {
                m_sText[i] = m_sText[i+nDel];
            }
            if (m_pTextGeometry != NULL) {
                m_pTextGeometry->setText(m_sText, m_FontId);
            }
        }
        if (c != 127)
        {
            int iLength = m_sText.length();
            for (int i = iLength; i > m_iCaretPos; i--) {
                m_sText[i] = m_sText[i-1];
            }
            m_sText[iLength+1] = '\0';
            char str[2] = "a";
            str[0] = (char) c;
            m_sText[m_iCaretPos] = str[0];
            if (m_pTextGeometry != NULL) {
                m_pTextGeometry->setText(m_sText, m_FontId);
            }
            m_iCaretPos++;
        }
        i2d pos = Jogy::interface->computeCharPosInText(m_iCaretPos, m_sText, m_FontId);
        if (pos.x > getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) {
            m_iXScrollPos = (getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) - pos.x - 4;
        } else if (pos.x + m_iXScrollPos < 0) {
            m_iXScrollPos = 0;
        }
        int fontHeight = Jogy::interface->computeTextHeight(NULL, m_FontId);
        if (pos.y + fontHeight > getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) {
            m_iYScrollPos = (getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) - pos.y - fontHeight;
        } else if (pos.y + m_iYScrollPos < 0) {
            m_iYScrollPos = 0;
        }
        m_fBlinkTimer = 0;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------
// Name : onSpecialKeyDown
// -----------------------------------------------------------------
bool guiEditBox::onSpecialKeyDown(InputButton key)
{
    if (m_bHasFocus) {
        int oldCaretPos = -1;
        if (Jogy::interface->isShiftPressed()) {
            if (!hasSelectedText()) {
                m_iSelectionStart = m_iSelectionEnd = m_iCaretPos;
            }
            oldCaretPos = m_iCaretPos;
        } else {
        	deselect();
        }
        switch (key)
        {
        case KeyLeft:
        {
            if (Jogy::interface->isCtrlPressed()) {
                m_iCaretPos = getStartOfWord(m_iCaretPos);
            } else {
                m_iCaretPos = max(m_iCaretPos-1, 0);
            }
            break;
        }
        case KeyRight:
        {
            if (Jogy::interface->isCtrlPressed()) {
                m_iCaretPos = getEndOfWord(m_iCaretPos);
            } else {
                m_iCaretPos = min((unsigned)(m_iCaretPos+1), m_sText.length());
            }
            break;
        }
        case KeyUp:
        {
            i2d pos = Jogy::interface->computeCharPosInText(m_iCaretPos, m_sText, m_FontId);
            pos.y -= (Jogy::interface->computeTextHeight(NULL, m_FontId) - 1);
            m_iCaretPos = Jogy::interface->computeCharIdxInText(pos.x, pos.y, m_sText, m_FontId);
            break;
        }
        case KeyDown:
        {
            i2d pos = Jogy::interface->computeCharPosInText(m_iCaretPos, m_sText, m_FontId);
            pos.y += (Jogy::interface->computeTextHeight(NULL, m_FontId) + 1);
            m_iCaretPos = Jogy::interface->computeCharIdxInText(pos.x, pos.y, m_sText, m_FontId);
            break;
        }
        case KeyHome:
        {
            if (Jogy::interface->isCtrlPressed()) {
                m_iCaretPos = 0;
            } else {
                m_iCaretPos = getStartOfLine(m_iCaretPos);
            }
            break;
        }
        case KeyEnd:
        {
            if (Jogy::interface->isCtrlPressed()) {
                m_iCaretPos = m_sText.length();
            } else {
                m_iCaretPos = getEndOfLine(m_iCaretPos);
            }
            break;
        }
        }
        i2d pos = Jogy::interface->computeCharPosInText(m_iCaretPos, m_sText, m_FontId);
        if (pos.x > getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) {
            m_iXScrollPos = (getWidth() - m_iTLBorderWidth - m_iBRBorderWidth) - pos.x - 4;
        } else if (pos.x + m_iXScrollPos < 0) {
            m_iXScrollPos = 0;
        }
        if (pos.y + Jogy::interface->computeTextHeight(NULL, m_FontId) > getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) {
            m_iYScrollPos = (getHeight() - m_iTLBorderHeight - m_iBRBorderHeight) - pos.y - Jogy::interface->computeTextHeight(NULL, m_FontId);
        } else if (pos.y + m_iYScrollPos < 0) {
            m_iYScrollPos = 0;
        }
        m_fBlinkTimer = 0;
        if (oldCaretPos >= 0 && oldCaretPos != m_iCaretPos) {
            if (m_iCaretPos < m_iSelectionStart) {
                m_iSelectionStart = m_iCaretPos;
                updateSelectionGeometry();
            } else if (m_iCaretPos > m_iSelectionEnd) {
                m_iSelectionEnd = m_iCaretPos;
                updateSelectionGeometry();
            } else if (oldCaretPos == m_iSelectionStart) {
                m_iSelectionStart = m_iCaretPos;
                if (m_iSelectionEnd < m_iSelectionStart) {
                    m_iSelectionStart = m_iSelectionEnd;
                    m_iSelectionEnd = m_iCaretPos;
                    updateSelectionGeometry();
                } else if (m_iSelectionStart != m_iSelectionEnd) {
                    updateSelectionGeometry();
                }
            } else if (oldCaretPos == m_iSelectionEnd) {
                m_iSelectionEnd = m_iCaretPos;
                if (m_iSelectionEnd < m_iSelectionStart) {
                    m_iSelectionEnd = m_iSelectionStart;
                    m_iSelectionStart = m_iCaretPos;
                    updateSelectionGeometry();
                } else if (m_iSelectionStart != m_iSelectionEnd) {
                    updateSelectionGeometry();
                }
            }
        }
        return true;
    }
    return false;
}

// -----------------------------------------------------------------
// Name : setText
// -----------------------------------------------------------------
void guiEditBox::setText(string sText)
{
    if (sText == m_sText) {
        return;
    }
    m_sText = sText;
    if (m_pTextGeometry != NULL) {
        m_pTextGeometry->setText(m_sText, m_FontId);
    }
}

// -----------------------------------------------------------------
// Name : onResize
// -----------------------------------------------------------------
void guiEditBox::onResize(int iOldWidth, int iOldHeight)
{
    guiComponent::onResize(iOldWidth, iOldHeight);
    if (m_iWidth == iOldWidth && m_iHeight == iOldHeight) {
        return;
    }
    // Main geometry (borders & background)
    if (m_pMainGeometry != NULL) {
    	rebuildMainGeometry();
    }
    // Stencil
    if (m_pStencilGeometry != NULL) {
        m_pStencilGeometry->build(m_iWidth - m_iTLBorderWidth - m_iBRBorderWidth, m_iHeight - m_iTLBorderHeight - m_iBRBorderHeight);
    }
}

// -----------------------------------------------------------------
// Name : updateSelectionGeometry
// -----------------------------------------------------------------
void guiEditBox::updateSelectionGeometry()
{
    int nbSelLines = 1;
    for (int i = m_iSelectionStart; i < m_iSelectionEnd; i++)
    {
        if (m_sText[i] == '\n') {
            nbSelLines++;
        }
    }

    ITexture * texture = m_pSelectionGeometry->getTexture();
    QuadData ** pQuads = new QuadData*[nbSelLines];

    // Upper-left quad 1
    i2d p1 = Jogy::interface->computeCharPosInText(m_iSelectionStart, m_sText, m_FontId);
    i2d p2;
    int iQuad = 0;
    for (int i = m_iSelectionStart; i < m_iSelectionEnd; i++) {
        if (m_sText[i] == '\n') {
            p2 = Jogy::interface->computeCharPosInText(i, m_sText, m_FontId);
            p2.y += Jogy::interface->computeTextHeight(NULL, m_FontId);
            pQuads[iQuad] = new QuadData(p1.x, p2.x, p1.y, p2.y, texture);
            iQuad++;
            p1.x = 0;
            p1.y = p2.y;
        }
    }
    p2 = Jogy::interface->computeCharPosInText(m_iSelectionEnd, m_sText, m_FontId);
    p2.y += Jogy::interface->computeTextHeight(NULL, m_FontId);
    pQuads[iQuad] = new QuadData(p1.x, p2.x, p1.y, p2.y, texture);

    m_pSelectionGeometry->build(nbSelLines, pQuads);
    QuadData::releaseQuads(nbSelLines, pQuads);
}

// -----------------------------------------------------------------
// Name : getStartOfWord
// -----------------------------------------------------------------
int guiEditBox::getStartOfWord(int caretpos)
{
    int start = caretpos;
    if (caretpos-1 < 0) {
        return start;
    }
    start--;
    bool isFirstWord = !(m_sText[start] == ' ' ||
                         m_sText[start] == '\n' ||
                         m_sText[start] == '\t' ||
                         m_sText[start] == '.' ||
                         m_sText[start] == '-' ||
                         m_sText[start] == ',' ||
                         m_sText[start] == '!' ||
                         m_sText[start] == '?' ||
                         m_sText[start] == ';' ||
                         m_sText[start] == '<' ||
                         m_sText[start] == '>' ||
                         m_sText[start] == '(' ||
                         m_sText[start] == ')' ||
                         m_sText[start] == '\"' ||
                         m_sText[start] == '\'' ||
                         m_sText[start] == '[' ||
                         m_sText[start] == ']' ||
                         m_sText[start] == '{' ||
                         m_sText[start] == '}' ||
                         m_sText[start] == '%' ||
                         m_sText[start] == '*' ||
                         m_sText[start] == ':' ||
                         m_sText[start] == '\\');
    for (int i = caretpos-2; i >= 0; i--)
    {
        bool isWord = !(m_sText[start-1] == ' ' ||
                        m_sText[start-1] == '\n' ||
                        m_sText[start-1] == '\t' ||
                        m_sText[start-1] == '.' ||
                        m_sText[start-1] == '-' ||
                        m_sText[start-1] == ',' ||
                        m_sText[start-1] == '!' ||
                        m_sText[start-1] == '?' ||
                        m_sText[start-1] == ';' ||
                        m_sText[start-1] == '<' ||
                        m_sText[start-1] == '>' ||
                        m_sText[start-1] == '(' ||
                        m_sText[start-1] == ')' ||
                        m_sText[start-1] == '\"' ||
                        m_sText[start-1] == '\'' ||
                        m_sText[start-1] == '[' ||
                        m_sText[start-1] == ']' ||
                        m_sText[start-1] == '{' ||
                        m_sText[start-1] == '}' ||
                        m_sText[start-1] == '%' ||
                        m_sText[start-1] == '*' ||
                        m_sText[start-1] == ':' ||
                        m_sText[start-1] == '\\');
        if (isWord == isFirstWord)
            start--;
        else
            break;
    }
    return start;
}

// -----------------------------------------------------------------
// Name : getEndOfWord
// -----------------------------------------------------------------
int guiEditBox::getEndOfWord(int caretpos)
{
    int end = caretpos;
    int iLength = m_sText.length();
    if (end >= iLength) {
        return end;
    }
    bool isFirstWord = !(m_sText[end] == ' ' ||
                         m_sText[end] == '\n' ||
                         m_sText[end] == '\t' ||
                         m_sText[end] == '.' ||
                         m_sText[end] == '-' ||
                         m_sText[end] == ',' ||
                         m_sText[end] == '!' ||
                         m_sText[end] == '?' ||
                         m_sText[end] == ';' ||
                         m_sText[end] == '<' ||
                         m_sText[end] == '>' ||
                         m_sText[end] == '(' ||
                         m_sText[end] == ')' ||
                         m_sText[end] == '\"' ||
                         m_sText[end] == '\'' ||
                         m_sText[end] == '[' ||
                         m_sText[end] == ']' ||
                         m_sText[end] == '{' ||
                         m_sText[end] == '}' ||
                         m_sText[end] == '%' ||
                         m_sText[end] == '*' ||
                         m_sText[end] == ':' ||
                         m_sText[end] == '\\');
    for (int i = caretpos+1; i <= iLength; i++)
    {
        bool isWord = !(m_sText[end] == ' ' ||
                        m_sText[end] == '\n' ||
                        m_sText[end] == '\t' ||
                        m_sText[end] == '.' ||
                        m_sText[end] == '-' ||
                        m_sText[end] == ',' ||
                        m_sText[end] == '!' ||
                        m_sText[end] == '?' ||
                        m_sText[end] == ';' ||
                        m_sText[end] == '<' ||
                        m_sText[end] == '>' ||
                        m_sText[end] == '(' ||
                        m_sText[end] == ')' ||
                        m_sText[end] == '\"' ||
                        m_sText[end] == '\'' ||
                        m_sText[end] == '[' ||
                        m_sText[end] == ']' ||
                        m_sText[end] == '{' ||
                        m_sText[end] == '}' ||
                        m_sText[end] == '%' ||
                        m_sText[end] == '*' ||
                        m_sText[end] == ':' ||
                        m_sText[end] == '\\');
        if (isWord == isFirstWord)
            end++;
        else
            break;
    }
    return end;
}

// -----------------------------------------------------------------
// Name : getStartOfLine
// -----------------------------------------------------------------
int guiEditBox::getStartOfLine(int caretpos)
{
    while (caretpos - 1 >= 0 && m_sText[caretpos-1] != '\n' && m_sText[caretpos-1] != '\r') {
        caretpos--;
    }
    return caretpos;
}

// -----------------------------------------------------------------
// Name : getEndOfLine
// -----------------------------------------------------------------
int guiEditBox::getEndOfLine(int caretpos)
{
    int len = m_sText.length();
    while (caretpos + 1 <= len && m_sText[caretpos] != '\n' && m_sText[caretpos] != '\r') {
        caretpos++;
    }
    return caretpos;
}

// -----------------------------------------------------------------
// Name : setFocus
// -----------------------------------------------------------------
void guiEditBox::setFocus()
{
    m_bHasFocus = true;
	Jogy::interface->registerKeyboardListener(this);
}

//// -----------------------------------------------------------------
//// Name : createDefaultEditBox
////  Static default constructor
//// -----------------------------------------------------------------
//guiEditBox * guiEditBox::createDefaultEditBox(int iNbLines, bool bMultiLines, int wPxl, string sId)
//{
//    Texture * iTexs[9];
//    iTexs[0] = _tex->findTexture("gui/interface:LstTL");
//    iTexs[1] = _tex->findTexture("gui/interface:LstTC");
//    iTexs[2] = _tex->findTexture("gui/interface:LstTR");
//    iTexs[3] = _tex->findTexture("gui/interface:LstCL");
//    iTexs[4] = _tex->findTexture("gui/interface:EditBg");
//    iTexs[5] = _tex->findTexture("gui/interface:LstCR");
//    iTexs[6] = _tex->findTexture("gui/interface:LstBL");
//    iTexs[7] = _tex->findTexture("gui/interface:LstBC");
//    iTexs[8] = _tex->findTexture("gui/interface:LstBR");
//    guiEditBox * pBox = new guiEditBox();
//    pBox->init(
//        _tex->findTexture("gui/interface:Caret"),
//        "", TEXT_FONT, TEXT_COLOR_DARK, iNbLines, bMultiLines, iTexs,
//        sId, 0, 0, wPxl, 1);
//    return pBox;
//}
