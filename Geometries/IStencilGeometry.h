#ifndef _ISTENCIL_GEOMETRY_H
#define _ISTENCIL_GEOMETRY_H

#include "IGeometry.h"

namespace jogy {

class IStencilGeometry : public IGeometry
{
public:
    IStencilGeometry();
    virtual ~IStencilGeometry();

    virtual void fillStencil(int x, int y, bool bAdd) = 0;
    void build(int iWidth, int iHeight);
    virtual void reload();

private:
    int m_iWidth, m_iHeight;
    Vertex m_Vertices[4];
};
}

#endif
