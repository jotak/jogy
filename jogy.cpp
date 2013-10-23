#include "jogy.h"

using namespace jogy;

IGeometryQuads * Resources::pScrollButtons[] = {NULL, NULL, NULL, NULL};
int Resources::iScrollButtonWidth = 0;
int Resources::iScrollButtonHeight = 0;

// -----------------------------------------------------------------
// Name : init
//  static
// -----------------------------------------------------------------
void Jogy::init(IGameInterface * interface,
		IGeometryQuads * pStickGeo,
		ITexture * pStickTex,
		IGeometryQuads * pStickedGeo,
		ITexture * pStickedTex,
		IGeometryQuads ** scrollGeometries,
		ITexture ** scrollTextures) {

	Jogy::interface = interface;
	Resources::pStickGeo = pStickGeo;
	Resources::pStickedGeo = pStickedGeo;
    QuadData quad(0, 15, 0, 15, pStickTex/*"stick"*/);
    pStickGeo->build(&quad);
    QuadData quad2(0, 15, 0, 15, pStickedTex/*"sticked"*/);
    pStickedGeo->build(&quad2);

    for (int i = 0; i < 4; i++) {
    	Resources::pScrollButtons[i] = scrollGeometries[i];
    }

    // Initialize static data
    ITexture * pTex = *scrollTextures;
    QuadData quadTop(0, pTex->getWidth(), 0, pTex->getHeight(), pTex);
    Resources::pScrollButtons[0]->build(&quadTop);
    Resources::iScrollButtonWidth = pTex->getWidth();
    Resources::iScrollButtonHeight = pTex->getHeight();
    pTex++;
    QuadData quadBottom(0, pTex->getWidth(), 0, pTex->getHeight(), pTex);
    Resources::pScrollButtons[1]->build(&quadBottom);
    pTex++;
    QuadData quadLeft(0, pTex->getWidth(), 0, pTex->getHeight(), pTex);
    Resources::pScrollButtons[2]->build(&quadLeft);
    pTex++;
    QuadData quadRight(0, pTex->getWidth(), 0, pTex->getHeight(), pTex);
    Resources::pScrollButtons[3]->build(&quadRight);
}

// -----------------------------------------------------------------
// Name : destroy
//  static
// -----------------------------------------------------------------
void Jogy::destroy() {

	FREE(Jogy::interface);
	FREE(Resources::pStickGeo);
	FREE(Resources::pStickedGeo);
    for (int i = 0; i < 4; i++) {
        FREE(Resources::pScrollButtons[i]);
    }
}
