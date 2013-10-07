// -----------------------------------------------------------------
// IStencilGeometry
// -----------------------------------------------------------------
#include "IStencilGeometry.h"

// -----------------------------------------------------------------
// Name : IStencilGeometry
//  Constructor
// -----------------------------------------------------------------
IStencilGeometry::IStencilGeometry() : IGeometry()
{
    m_iWidth = 0;
    m_iHeight = 0;
}

// -----------------------------------------------------------------
// Name : ~IStencilGeometry
//  Destructor
// -----------------------------------------------------------------
IStencilGeometry::~IStencilGeometry()
{
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
void IStencilGeometry::build(int iWidth, int iHeight)
{
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    Vertex vEnd = IGeometry::screenTransform(iWidth, iHeight);
    m_Vertices[0].set(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    m_Vertices[1].set(vEnd.x, 0.0f, 0.0f, 1.0f, 0.0f);
    m_Vertices[2].set(vEnd.x, vEnd.y, 0.0f, 1.0f, 1.0f);
    m_Vertices[3].set(0.0f, vEnd.y, 0.0f, 0.0f, 1.0f);
    reloadGmem();
}

// -----------------------------------------------------------------
// Name : reload
// -----------------------------------------------------------------
void IStencilGeometry::reload()
{
	build(m_iWidth, m_iHeight);
}
