#ifndef _JOGY_H
#define _JOGY_H

#include "IGameInterface.h"
#include "GUI/TabbedFrame.h"
#include "GUI/ToggleButton.h"
#include "GUI/SmartSlider.h"
#include "GUI/EditBox.h"
#include "GUI/List.h"
#include "GUI/Popup.h"
#include "GUI/Image.h"
#include "FrameEffects/FrameIntro.h"
#include "FrameEffects/FrameOutro.h"
#include "FrameEffects/FrameMouseFocus.h"
#include "FrameEffects/FrameFlash.h"
#include "JogyResources.h"

class Jogy
{
public:
	static void init(IGameInterface * interface,
			jogy::IGeometryQuads * pStickGeo,
			jogy::ITexture * pStickTex,
			jogy::IGeometryQuads * pStickedGeo,
			jogy::ITexture * pStickedTex,
			jogy::IGeometryQuads ** scrollGeometries,
			jogy::ITexture ** scrollTextures);

	static void destroy();

	static IGameInterface * interface;
};

#endif
