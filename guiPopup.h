#ifndef _GUI_POPUP_H
#define _GUI_POPUP_H

#include "guiFrame.h"
#include "guiDocument.h"
#include "guiButton.h"
#include "guiEditBox.h"

#define POPUP_EFFECT_OUTRO  200
#define POPUP_EFFECT_FOCUS  201
#define POPUP_EFFECT_INTRO  202

class InputEngine;

class guiPopupDocument : public guiDocument
{
public:
    guiPopupDocument() { m_pClickedComponent = NULL; };
    virtual bool onButtonEvent(ButtonAction * pEvent, guiComponent * pCpnt)
    {
        m_pClickedComponent = pCpnt;
        return true;
    };
    guiComponent * getClickedComponent()
    {
        guiComponent * pCpnt = m_pClickedComponent;
        m_pClickedComponent = NULL;
        return pCpnt;
    };

protected:
    guiComponent * m_pClickedComponent;
};

class guiPopup : public guiFrame
{
public:
    virtual ~guiPopup();

    void update(double delta);

    // Member access
    guiEditBox * getEditBox() { return (guiEditBox*) m_pDoc->getComponent("DefaultEditBox"); };
    guiComponent * getClickedComponent() { return ((guiPopupDocument*)getDocument())->getClickedComponent(); };
    guiButton * getButton(int iButton);

    // Static default constructors
    static guiPopup * createEmptyPopup();
    static guiPopup * createYesNoPopup(string sText);
    static guiPopup * createOkAutoclosePopup(string sText);
    static guiPopup * createOkCancelPopup(string sText);
    static guiPopup * createTextAndMultiButtonsPopup(string sText, int iNbButtons, int iWidth);
    static guiPopup * createTextInputPopup(string sText, int iNbLines, bool bMultiLines, int iBoxWidth);
    static guiPopup * createTimedPopup(string sText, double fTimer, int iWidth);

protected:
    guiPopup(); // constructor is private ; use static constructor functions instead, or write subclass
    double m_fTimer;
    bool m_bAutoClose;
};

#endif
