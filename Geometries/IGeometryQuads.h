#ifndef _IGEOMETRY_QUADS_H
#define _IGEOMETRY_QUADS_H

#include "IGeometry.h"
#include "../Utils/ITexture.h"

namespace jogy {

class QuadData
{
public:
    QuadData(int xstart, int xend, int ystart, int yend, ITexture * pTexture);
    QuadData(Vertex vStart, Vertex vEnd, ITexture * pTexture);

    static void releaseQuads(int nQuads, QuadData ** pQuads) {
        for (int i = 0; i < nQuads; i++) {
            delete pQuads[i];
        }
        delete[] pQuads;
    };

    QuadData * clone() {
    	return new QuadData(vStart, vEnd, pTexture);
    };

    Vertex vStart, vEnd;
    ITexture * pTexture;
};

class IGeometryQuads : public IGeometry
{
public:
	IGeometryQuads();
    virtual ~IGeometryQuads();

    virtual void build(int nQuads, QuadData ** pAllQuads);
    void build(QuadData * pQuad) { build(1, &pQuad); };
    virtual void reload();

    ITexture * getTexture(int iQuad = 0);
    void setTexture(ITexture * pTex, int iQuad = 0);

protected:
    int m_iNbQuads;
    QuadData ** m_pAllQuads;
};
}

#endif
