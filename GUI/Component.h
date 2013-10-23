#ifndef _GUI_COMPONENT_H
#define _GUI_COMPONENT_H

#include "Object.h"

#define HIGHLIGHT_TYPE_NONE         0
#define HIGHLIGHT_TYPE_NORMAL_RED   1

namespace jogy {

class ComponentOwnerInterface;

class Component : public Object
{
public:
    // Constructor / destructor
    Component();
    virtual ~Component();

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
    virtual bool isAt(int xPxl, int yPxl) { return m_bVisible && Object::isAt(xPxl, yPxl); };
    void highlight(u8 type);

    // Builder
    virtual Component * build();
    Component * withCpntId(string cpntId) {
    	m_sCpntId = cpntId; return this;
    }

protected:
    bool m_bVisible;
    bool m_bEnabled;
    u8 m_uHighlight;
    string m_sCpntId;
    ComponentOwnerInterface * m_pOwner;
};
}

#endif
