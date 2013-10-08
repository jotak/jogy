#include "jogy.h"

// -----------------------------------------------------------------
// Name : init
//  static
// -----------------------------------------------------------------
void Jogy::init(IGameInterface * interface,
		IGeometryQuads * pStickGeo,
		ITexture * pStickTex,
		IGeometryQuads * pStickedGeo,
		ITexture * pStickedTex) {

	Jogy::interface = interface;
	Jogy::Resources::pStickGeo = pStickGeo;
	Jogy::Resources::pStickedGeo = pStickedGeo;
    QuadData quad(0, 15, 0, 15, pStickTex/*"stick"*/);
    pStickGeo->build(&quad);
    QuadData quad2(0, 15, 0, 15, pStickedTex/*"sticked"*/);
    pStickedGeo->build(&quad2);
}

// -----------------------------------------------------------------
// Name : destroy
//  static
// -----------------------------------------------------------------
void Jogy::destroy() {

	delete Jogy::interface;
	FREE(Jogy::Resources::pStickGeo);
	FREE(Jogy::Resources::pStickedGeo);
}
