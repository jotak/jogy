#ifndef _GUI_EDITBOX_H
#define _GUI_EDITBOX_H

#include "guiComponent.h"
#include "Geometries/IGeometryQuads.h"
#include "Geometries/IStencilGeometry.h"
#include "Geometries/IGeometryText.h"

class guiEditBox : public guiComponent
{
public:
    // Constructor / destructor
    guiEditBox();
    virtual ~guiEditBox();

    // Inherited functions
    virtual u32 getType() { return GOTYPE_EDITBOX; };
    virtual bool onKeyDown(unsigned char c);
    virtual bool onSpecialKeyDown(int key);

    // Update / display
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functions
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);

    // Events
    virtual void onFocusLost();
    virtual void onResize(int iOldWidth, int iOldHeight);

    // Member access
    fontid getFontId() { return m_FontId; };
    void setFontId(fontid id) { m_FontId = id; };
    void setTextColor(Color textColor) { m_TextColor = textColor; };
    Color getTextColor() { return m_TextColor; };
    void setText(string sText);
    string getText() { return m_sText; };
    void setNbLines(int iNbLines) { m_iNbLines = iNbLines; };
    int getNbLines() { return m_iNbLines; };

    // Other
    void setFocus();
    bool hasSelectedText() { return m_iSelectionStart >= 0 && m_iSelectionStart != m_iSelectionEnd; };
    void deselect() { m_iSelectionStart = -1; };

    // Builder
    virtual guiEditBox * build();
    guiEditBox * withMainGeometry(ITexture ** pTex, IGeometryQuads * pGeo);
    guiEditBox * withCaretGeometry(ITexture * pTex, IGeometryQuads * pGeo);
    guiEditBox * withStencilGeometry(ITexture * pTex, IStencilGeometry * pGeo);
    guiEditBox * withSelectionGeometry(ITexture * pTex, IGeometryQuads * pGeo);
    guiEditBox * withLabel(string sText, fontid fontId, Color textColor, IGeometryText * pGeo);
    guiEditBox * withLinesInfo(int iNbLines, bool bMultiLines) {
    	m_iNbLines = iNbLines; m_bMultiLines = bMultiLines; return this;
    }

protected:
    void updateSelectionGeometry();
    int getStartOfWord(int caretpos);
    int getEndOfWord(int caretpos);
    int getStartOfLine(int caretpos);
    int getEndOfLine(int caretpos);
    void onButton1Down(int xPxl, int yPxl);
    void onButton1Drag(int xPxl, int yPxl);
    void onButton1DoubleClick(int xPxl, int yPxl);

    IGeometryQuads * m_pMainGeometry;
    IStencilGeometry * m_pStencilGeometry;
    IGeometryText * m_pTextGeometry;
    IGeometryQuads * m_pCaretGeometry;
    IGeometryQuads * m_pSelectionGeometry;
    int m_iNbLines;
    bool m_bMultiLines;
    string m_sText;
    fontid m_FontId;
    Color m_TextColor;
    int m_iCaretPos;
    bool m_bHasFocus;
    double m_fBlinkTimer;
    int m_iXScrollPos;
    int m_iYScrollPos;
    int m_iSelectionStart;
    int m_iSelectionEnd;
    int m_iXInnerOffset;
    int m_iYInnerOffset;

private:
    void rebuildMainGeometry();
};

#endif
