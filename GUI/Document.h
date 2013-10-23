#ifndef _GUI_DOCUMENT_H
#define _GUI_DOCUMENT_H

#include "../ComponentOwnerInterface.h"
#include "Component.h"
#include "../Geometries/IGeometryQuads.h"

namespace jogy {

class Container;
class IGeometryQuads;

class Document : public Object, public ComponentOwnerInterface
{
public:
    // Constructor / destructor
    Document();
    virtual ~Document();

    // GraphicObject virtual function
    virtual u32 getType() { return GOTYPE_DOCUMENT; };
    virtual void update(double delta);
    virtual void displayAt(int iXOffset, int iYOffset, Color cpntColor, Color docColor);

    // Active input functions
    virtual Object * onButtonEvent(ButtonAction * pEvent);
    virtual Object * onCursorMoveEvent(int xPxl, int yPxl);

    // Size and position
    virtual void onResize(int iOldWidth, int iOldHeight);

    // Creation process functions
    void setTitle(string sTitle) { m_sTitle = sTitle; };

    // Components management
    void addComponent(Component * cpnt);
    void deleteAllComponents();
    list<Component*> getComponents() { return m_pComponentsList; };
    bool deleteComponent(Component * pCpnt);
    Component * removeCurrentComponent(list<Component*>::iterator * it);
    Component * getComponentAt(int xPxl, int yPxl);
    Component * getComponent(string cpntId);

    // Other member access functions
    string getTitle() { return m_sTitle; };
    void setFocusedComponent(Component * pCpnt);
    Component * getFocusedComponent() { return m_pFocusedComponent; };
    bool isEnabled() { return m_bEnabled; };
    void setEnabled(bool bEnabled) { m_bEnabled = bEnabled; };
    void doClick(string sCpntId);

    // Handlers
    virtual void onLoad() {};
    virtual void onShow() {};
    virtual void onHide() {};
    virtual void bringAbove(Component * pCpnt);

    // Other functions
    void close() { m_bNeedDestroy = true; };
    bool doesNeedDestroy() { return m_bNeedDestroy; };
    virtual void onDestroy(void * pDestroyInfo) {};
    bool didContentChange() { return m_bContentChanged; };
    void setContentChanged() { m_bContentChanged = true; };
    void setOwner(Container * pOwner) { m_pOwner = pOwner; };

    // Builder
    virtual Document * build();
    Document * withGeometry(ITexture * pTex, IGeometryQuads * pGeo);
    Document * withTitle(string title) {
    	m_sTitle = title; return this;
    }

protected:
    string m_sTitle;
    Component * m_pFocusedComponent;
    bool m_bNeedDestroy;
    bool m_bContentChanged;
    bool m_bEnabled;
    Container * m_pOwner;
    IGeometryQuads * m_pGeometry;

private:
    list<Component*> m_pComponentsList;

    void rebuildGeometry();
};
}

#endif
