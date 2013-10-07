#ifndef _GUI_LIST_H
#define _GUI_LIST_H

#include "guiContainer.h"
#include "guiLabel.h"

class guiList : public guiContainer, public KeyboardListener, public EventListener
{
public:
    // class guiListLabel
    class guiListLabel : public guiLabel
    {
    public:
        guiListLabel() { m_bSelected = false; };
        virtual guiObject * onButtonEvent(ButtonAction * pEvent);
        virtual bool isAt(int xPxl, int yPxl)
        {
            int w = m_iWidth;
            m_iWidth = m_iBoxWidth;
            bool b = guiLabel::isAt(xPxl, yPxl);
            m_iWidth = w;
            return b;
        };
        bool isSelected() { return m_bSelected; };
        void setSelected(bool bSel) { m_bSelected = bSel; };

    protected:
        bool m_bSelected;
    };
    // class guiListDocument
    class guiListDocument : public guiDocument
    {
    public:
        guiListDocument(guiList * pList) { m_pOwner = pList; };
        virtual bool onButtonEvent(ButtonAction * pEvent, guiComponent * pCpnt);
    protected:
        guiList * m_pOwner;
    };

    // Constructor / destructor
    guiList();
    virtual ~guiList();

    // Inherited functions
    virtual u32 getType() { return guiComponent::getType(); };
    virtual bool onKeyDown(unsigned char c);
    virtual bool onSpecialKeyDown(int key) { return false; };
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functions
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);
    virtual bool onCatchButtonEvent(ButtonAction * pEvent);

    // Events
    virtual void onFocusLost();
    virtual bool onButtonEvent(ButtonAction * pEvent, guiComponent * pCpnt);

    // Member access
    fontid getFontId() { return m_FontId; };
    void setFontId(fontid id) { m_FontId = id; };
    void setTextColor(Color textColor) { m_TextColor = textColor; };
    Color getTextColor() { return m_TextColor; };
    InputButton getActionOnSelection()
    {
        InputButton action = m_ActionOnSelection;
        m_ActionOnSelection = (InputButton)0;
        return action;
    };
    guiListLabel * getLastSelectedLabel() { return m_pLastSelectedLabel; };
    bool hasFocus() { return m_bHasFocus; };

    // Clone / init
    virtual void init(fontid fontId, Color textColor, ITexture ** pMainTexs, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl);

    // Other
    void setFocus();
    guiListLabel * addItem(string sText, string sId);
    void clear();
    void removeSelection();
    int count() { return getDocument()->getComponents().size(); };

    // Sort
    void sort();
    bool sortCompare(BaseObject * A, BaseObject * B);

    // Static default constructors
    static guiList * createDefaultList(int width, int height, string sId);

protected:
    void updateSelectionGeometry();
    void shiftSelect(guiListLabel * pClickedLabel);

    IGeometryQuads * m_pSelectionGeometry;
    fontid m_FontId;
    Color m_TextColor;
    bool m_bHasFocus;
    guiListLabel * m_pLastSelectedLabel;
    bool m_bCatchMouseUp;
    InputButton m_ActionOnSelection;
};

#endif
