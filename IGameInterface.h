#ifndef _IGAME_INTERFACE_H
#define _IGAME_INTERFACE_H

#include "GUI/Object.h"
#include "Geometries/IGeometry.h"

class IGameInterface
{
public:
    virtual ~IGameInterface() {};

    virtual void pushMatrix() = 0;
    virtual void popMatrix() = 0;
    virtual void translate(float x, float y, float z) = 0;
    virtual void scale(float x, float y, float z) = 0;
    virtual jogy::Vertex screenTransform(int x, int y) = 0;
    virtual bool setAdditiveMode(bool) = 0;
    virtual bool setBlendingMode(bool) = 0;
    virtual string wrapText(string text, unsigned boxWidth, fontid font) = 0;
    virtual unsigned computeTextWidth(string text, fontid font) = 0;
    virtual unsigned computeTextHeight(string text, fontid font) = 0;
    virtual unsigned getScreenWidth() = 0;
    virtual unsigned getScreenHeight() = 0;
    virtual void displayFront(jogy::Object*, int, int, jogy::Color, jogy::Color) = 0;
    virtual void cancelDisplayFront(jogy::Object*) = 0;
    virtual i2d computeCharPosInText(int idx, string text, fontid font) = 0;
    virtual int computeCharIdxInText(int x, int y, string text, fontid font) = 0;
    virtual void registerKeyboardListener(jogy::Object*) = 0;
    virtual void unregisterKeyboardListener(jogy::Object*) = 0;
    virtual void registerUncursoredEventListener(jogy::Object*) = 0;
    virtual void unregisterUncursoredEventListener(jogy::Object*) = 0;
    virtual bool isShiftPressed() = 0;
    virtual bool isCtrlPressed() = 0;
};

#endif
