// -----------------------------------------------------------------
// IGeometryQuads
// -----------------------------------------------------------------
#include "IGeometryQuads.h"
#include "../jogy.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : IGeometryQuads
//  Constructor
// -----------------------------------------------------------------
IGeometryQuads::IGeometryQuads() : IGeometry()
{
    m_iNbQuads = 0;
    m_pAllQuads = NULL;
}

// -----------------------------------------------------------------
// Name : ~IGeometryQuads
//  Destructor
// -----------------------------------------------------------------
IGeometryQuads::~IGeometryQuads()
{
    if (m_iNbQuads > 0)
    {
        for (int i = 0; i < m_iNbQuads; i++) {
            delete m_pAllQuads[i];
        }
        delete[] m_pAllQuads;
    }
}

// -----------------------------------------------------------------
// Name : build
// -----------------------------------------------------------------
void IGeometryQuads::build(int nQuads, QuadData ** pAllQuads)
{
    if (m_iNbQuads > 0)
    {
        for (int i = 0; i < m_iNbQuads; i++) {
            delete m_pAllQuads[i];
        }
        delete[] m_pAllQuads;
    }
    m_iNbQuads = nQuads;
    if (m_iNbQuads == 0) {
        m_pAllQuads = NULL;
        return;
    }

    m_pAllQuads = new QuadData*[m_iNbQuads];
    for (int i = 0; i < m_iNbQuads; i++)
    {
        // QuadData
        m_pAllQuads[i] = pAllQuads[i]->clone();
    }
    reloadGmem();
}

// -----------------------------------------------------------------
// Name : reload
// -----------------------------------------------------------------
void IGeometryQuads::reload()
{
	build(m_iNbQuads, m_pAllQuads);
}

// -----------------------------------------------------------------
// Name : getTexture
// -----------------------------------------------------------------
ITexture * IGeometryQuads::getTexture(int iQuad)
{
    return m_pAllQuads[iQuad]->pTexture;
}

// -----------------------------------------------------------------
// Name : setTexture
// -----------------------------------------------------------------
void IGeometryQuads::setTexture(ITexture * pTex, int iQuad)
{
    m_pAllQuads[iQuad]->pTexture = pTex;
}

// -----------------------------------------------------------------
// Name : QuadData
//  Constructor
// -----------------------------------------------------------------
QuadData::QuadData(int xstart, int xend, int ystart, int yend, ITexture * pTexture) {
	vStart = Jogy::interface->screenTransform(xstart, ystart);
	vEnd = Jogy::interface->screenTransform(xend, yend);
	this->pTexture = pTexture;
}

// -----------------------------------------------------------------
// Name : QuadData
//  Constructor
// -----------------------------------------------------------------
QuadData::QuadData(Vertex vStart, Vertex vEnd, ITexture * pTexture) {
	this->vStart = vStart;
	this->vEnd = vEnd;
	this->pTexture = pTexture;
}
}
