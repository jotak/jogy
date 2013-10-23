#ifndef _GUI_COMPONENT_OWNER_INTERFACE_H
#define _GUI_COMPONENT_OWNER_INTERFACE_H

namespace jogy {

class Component;
class ButtonAction;

class ComponentOwnerInterface
{
public:
	virtual ~ComponentOwnerInterface() {};

	// this function must return false if it's going to delete the calling component (like when closing frame)
    virtual bool onButtonEvent(ButtonAction * pEvent, Component * pCpnt) { return true; };
    virtual void bringAbove(Component * pCpnt) {};
};
}

#endif
