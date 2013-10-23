#ifndef _JOGY_ITEXTURE_H
#define _JOGY_ITEXTURE_H

namespace jogy {

class ITexture
{
public:
	virtual ~ITexture() {};

    virtual u16 getWidth() = 0;
    virtual u16 getHeight() = 0;
};

}

#endif
