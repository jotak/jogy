#ifndef _IGEOMETRY_H
#define _IGEOMETRY_H

#include "../Utils/BaseObject.h"

#pragma pack(push, 1)
class Vertex
{
public:
    void set(float x, float y, float z, float u, float v) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->u = u;
        this->v = v;
    };
    float u, v, x, y, z;
};
#pragma pack(pop)


typedef Vertex (*ScreenTransform)(int, int);

class IGeometry
{
public:
    IGeometry() {};
    virtual ~IGeometry() {};

    virtual void update(double delta) = 0;
    virtual void display(int x, int y, Color color) = 0;
    virtual void reload() = 0;
    virtual void reloadGmem() = 0;
    virtual IGeometry * clone() = 0;

    static ScreenTransform screenTransform;
};

#endif
