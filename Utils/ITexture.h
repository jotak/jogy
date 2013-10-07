#ifndef _ITEXTURE_H
#define _ITEXTURE_H

class ITexture
{
public:
	virtual ~ITexture() {};

    virtual u16 getWidth() = 0;
    virtual u16 getHeight() = 0;
};

#endif
