#ifndef _GUI_COMBOBOX_H
#define _GUI_COMBOBOX_H

#include "guiContainer.h"
#include "guiButton.h"

class guiComboBox : public guiComponent, public ComponentOwnerInterface
{
public:
    // Constructor / destructor
    guiComboBox();
    virtual ~guiComboBox();

    // Inherited functions
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Input functions
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);

    // Events
    virtual void onFocusLost();
    virtual bool onButtonEvent(ButtonAction * pEvent, guiComponent * pCpnt);

    // Size and position
    virtual bool isAt(int xPxl, int yPxl);
    virtual void onResize(int iOldWidth, int iOldHeight);
    void moveTo(int xPxl, int yPxl);
    void moveBy(int xPxl, int yPxl);
    virtual void setWidth(int iWidth);
    virtual void setDimensions(int iWidth, int iHeight);

    // List elements
    void clearList();
    guiButton * addString(string sText, string sId);
    string getText() { return m_pLabel->getText(); };
    void setItem(int id);
    guiButton * getItem(string sId);
    guiButton * getItem(u16 uId);
    guiButton * getSelectedItem();
    int getSelectedItemId();
    u16 getItemsCount();

    // Clone / init
    virtual void init(Texture ** pMainTex, Texture * pDocTex, Color textColor, FontId fontId, FrameFitBehavior wfit, FrameFitBehavior hfit, int iMaxWidth, int iMaxHeight, Texture * btnTex1, Texture * btnTex2, BtnClickOptions btnClickOpt, int btnHeight, Texture ** frameTexs, string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl);

protected:
    void centerLabel();

    guiLabel * m_pLabel;
    guiContainer * m_pList;
    guiButton * m_pListButtonTemplate;
    double m_dListPos;

private:
    int computeQuadsList(QuadData *** pQuads, Texture ** pTextures);
};

#endif
