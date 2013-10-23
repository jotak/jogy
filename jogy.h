#ifndef _JOGY_H
#define _JOGY_H

#include "IGameInterface.h"

class IGeometryQuads;
class ITexture;

class Jogy
{
public:
	static void init(IGameInterface * interface,
			IGeometryQuads * pStickGeo,
			ITexture * pStickTex,
			IGeometryQuads * pStickedGeo,
			ITexture * pStickedTex);

	static void destroy();

	static IGameInterface * interface;

	class Resources {
	public:
	    static IGeometryQuads * pStickGeo;
	    static IGeometryQuads * pStickedGeo;
	};
};

#endif
