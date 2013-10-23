#ifndef _GUI_POPUP_H
#define _GUI_POPUP_H

#include "Frame.h"
#include "Document.h"
#include "Button.h"
#include "EditBox.h"

#define POPUP_EFFECT_OUTRO  200
#define POPUP_EFFECT_FOCUS  201
#define POPUP_EFFECT_INTRO  202


namespace jogy {

class InputEngine;

class PopupDocument : public Document
{
public:
    PopupDocument() { m_pClickedComponent = NULL; };
    virtual bool onButtonEvent(ButtonAction * pEvent, Component * pCpnt)
    {
        m_pClickedComponent = pCpnt;
        return true;
    };
    Component * getClickedComponent()
    {
        Component * pCpnt = m_pClickedComponent;
        m_pClickedComponent = NULL;
        return pCpnt;
    };

protected:
    Component * m_pClickedComponent;
};

class Popup : public Frame
{
public:
    virtual ~Popup();

    void update(double delta);

    // Member access
    EditBox * getEditBox() { return (EditBox*) m_pDoc->getComponent("DefaultEditBox"); };
    Component * getClickedComponent() { return ((PopupDocument*)getDocument())->getClickedComponent(); };
    Button * getButton(int iButton);

protected:
    Popup(); // constructor is private ; use static constructor functions instead, or write subclass
    double m_fTimer;
    bool m_bAutoClose;
};
}

#endif
