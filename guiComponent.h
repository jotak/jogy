#ifndef _GUI_COMPONENT_H
#define _GUI_COMPONENT_H

#include "guiObject.h"

#define HIGHLIGHT_TYPE_NONE         0
#define HIGHLIGHT_TYPE_NORMAL_RED   1

class ComponentOwnerInterface;

class guiComponent : public guiObject
{
public:
    // Constructor / destructor
    guiComponent();
    virtual ~guiComponent();

    // Events
    virtual void onFocusLost() {};

    // Status
    bool isVisible() { return m_bVisible; };
    virtual void setVisible(bool bVisible);
    bool isEnabled() { return m_bEnabled; };
    virtual void setEnabled(bool bEnabled) { m_bEnabled = bEnabled; };

    // Member access
    void setId(string id) { m_sCpntId = id; };
    string getId() { return m_sCpntId; };
    void setOwner(ComponentOwnerInterface * pDoc) { m_pOwner = pDoc; };
    ComponentOwnerInterface * getOwner() { return m_pOwner; };

    // Other
    virtual bool isAt(int xPxl, int yPxl) { return m_bVisible && guiObject::isAt(xPxl, yPxl); };
    void highlight(u8 type);

    // Clone / init
    virtual void init(string sCpntId, int xPxl, int yPxl, int wPxl, int hPxl, IGeometry * pGeometry);

protected:
    bool m_bVisible;
    bool m_bEnabled;
    u8 m_uHighlight;
    string m_sCpntId;
    ComponentOwnerInterface * m_pOwner;
};

#endif
