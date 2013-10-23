#ifndef _JOGY_BASE_OBJECT_H
#define _JOGY_BASE_OBJECT_H

#include "../Utils/JogyUtils.h"

#define NAME_MAX_CHARS          64
#define DESCRIPTION_MAX_CHARS   1024

namespace jogy {

class BaseObject
{
public:
	BaseObject() { m_pAttachment = NULL; };
    virtual ~BaseObject() {};
    void setAttachment(BaseObject * pObj) { m_pAttachment = pObj; };
    BaseObject * getAttachment() { return m_pAttachment; };

protected:
    BaseObject * m_pAttachment;
};

}

#endif
