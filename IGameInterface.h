#ifndef _IGAME_INTERFACE_H
#define _IGAME_INTERFACE_H

#include "Geometries/IGeometry.h"
#include "guiObject.h"

class IGameInterface
{
public:
    virtual ~IGameInterface() {};

    virtual void pushMatrix() = 0;
    virtual void popMatrix() = 0;
    virtual void translate(float x, float y, float z) = 0;
    virtual void scale(float x, float y, float z) = 0;
    virtual Vertex screenTransform(int x, int y) = 0;
    virtual bool setAdditive(bool) = 0;
    virtual string wrapText(string text, unsigned boxWidth, fontid font) = 0;
    virtual unsigned computeTextWidth(string text, fontid font) = 0;
    virtual unsigned computeTextHeight(string text, fontid font) = 0;
    virtual unsigned getScreenWidth() = 0;
    virtual unsigned getScreenHeight() = 0;
};

#endif
