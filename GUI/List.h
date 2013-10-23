#ifndef _GUI_LIST_H
#define _GUI_LIST_H

#include "Container.h"
#include "Label.h"


namespace jogy {

class List : public Container
{
public:
    // class ListLabel
    class ListLabel : public Label
    {
    public:
        ListLabel() { m_bSelected = false; };
        virtual Object * onButtonEvent(ButtonAction * pEvent);
        virtual bool isAt(int xPxl, int yPxl)
        {
            int w = m_iWidth;
            m_iWidth = m_iBoxWidth;
            bool b = Label::isAt(xPxl, yPxl);
            m_iWidth = w;
            return b;
        };
        bool isSelected() { return m_bSelected; };
        void setSelected(bool bSel) { m_bSelected = bSel; };

    protected:
        bool m_bSelected;
    };
    // class ListDocument
    class ListDocument : public Document
    {
    public:
        ListDocument(List * pList) { m_pOwner = pList; };
        virtual bool onButtonEvent(ButtonAction * pEvent, Component * pCpnt);
    protected:
        List * m_pOwner;
    };

    // Constructor / destructor
    List();
    virtual ~List();

    // Inherited functions
    virtual u32 getType() { return Component::getType(); };
    virtual bool onKeyDown(unsigned char c);
    virtual bool onSpecialKeyDown(int key) { return false; };
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functions
    virtual Object * onButtonEvent(ButtonAction * pEvent);
    virtual bool onCatchButtonEvent(ButtonAction * pEvent);

    // Events
    virtual void onFocusLost();
    virtual bool onButtonEvent(ButtonAction * pEvent, Component * pCpnt);

    // Member access
    fontid getFontId() { return m_pLabelTemplate->getFontId(); };
    void setFontId(fontid id) { m_pLabelTemplate->setFontId(id); };
    Color getTextColor() { return m_pLabelTemplate->getDiffuseColor(); };
    void setTextColor(Color textColor) { m_pLabelTemplate->setDiffuseColor(textColor); };
    InputButton pickActionOnSelection();
    ListLabel * getLastSelectedLabel() { return m_pLastSelectedLabel; };
    bool hasFocus() { return m_bHasFocus; };

    // Other
    void setFocus();
    ListLabel * addItem(string sText, string sId);
    void clear();
    void removeSelection();
    int count() { return getDocument()->getComponents().size(); };

    // Sort
    void sort();
    bool sortCompare(BaseObject * A, BaseObject * B);

    // Builder
    virtual List * build();
    List * withLabelInfo(Color textColor, fontid fontId, IGeometryText * pLabelGeo) {
    	m_pLabelTemplate->withText("", fontId, textColor)->withGeometry(pLabelGeo);
    	return this;
    }
    List * withSelectionGeometry(ITexture * pTex, IGeometryQuads * pGeo);

protected:
    void updateSelectionGeometry();
    void shiftSelect(ListLabel * pClickedLabel);

    IGeometryQuads * m_pSelectionGeometry;
    bool m_bHasSelection;
    bool m_bHasFocus;
    Label * m_pLabelTemplate;
    ListLabel * m_pLastSelectedLabel;
    bool m_bCatchMouseUp;
    InputButton m_ActionOnSelection;
};
}

#endif
