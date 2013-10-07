#ifndef _GUI_DOCUMENT_H
#define _GUI_DOCUMENT_H

#include "ComponentOwnerInterface.h"
#include "guiComponent.h"

class guiContainer;
class IGeometryQuads;

class guiDocument : public guiObject, public ComponentOwnerInterface
{
public:
    // Constructor / destructor
    guiDocument();
    virtual ~guiDocument();

    // GraphicObject virtual function
    virtual u32 getType() { return GOTYPE_DOCUMENT; };
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Active input functions
    virtual guiObject * onButtonEvent(ButtonAction * pEvent);
    virtual guiObject * onCursorMoveEvent(int xPxl, int yPxl);

    // Size and position
    virtual void onResize(int iOldWidth, int iOldHeight);

    // Creation process functions
    void setTitle(string sTitle) { m_sTitle = sTitle; };

    // Components management
    void addComponent(guiComponent * cpnt);
    void deleteAllComponents();
    list<guiComponent*> getComponents() { return m_pComponentsList; };
    bool deleteComponent(guiComponent * pCpnt);
    guiComponent * removeCurrentComponent(list<guiComponent*>::iterator * it);
    guiComponent * getComponentAt(int xPxl, int yPxl);
    guiComponent * getComponent(string cpntId);

    // Other member access functions
    string getTitle() { return m_sTitle; };
    void setFocusedComponent(guiComponent * pCpnt);
    guiComponent * getFocusedComponent() { return m_pFocusedComponent; };
    bool isEnabled() { return m_bEnabled; };
    void setEnabled(bool bEnabled) { m_bEnabled = bEnabled; };
    void doClick(string sCpntId);

    // Handlers
    virtual void onLoad() {};
    virtual void onShow() {};
    virtual void onHide() {};
    virtual void bringAbove(guiComponent * pCpnt);

    // Other functions
    void close() { m_bNeedDestroy = true; };
    bool doesNeedDestroy() { return m_bNeedDestroy; };
    virtual void onDestroy(void * pDestroyInfo) {};
    bool didContentChange() { return m_bContentChanged; };
    void setContentChanged() { m_bContentChanged = true; };
    void setOwner(guiContainer * pOwner) { m_pOwner = pOwner; };

    // Clone / init
    virtual void init(string sTitle, int xPxl, int yPxl, int wPxl, int hPxl, IGeometryQuads * pGeometry);

protected:
    string m_sTitle;
    guiComponent * m_pFocusedComponent;
    bool m_bNeedDestroy;
    bool m_bContentChanged;
    bool m_bEnabled;
    guiContainer * m_pOwner;

private:
    list<guiComponent*> m_pComponentsList;
};

#endif
