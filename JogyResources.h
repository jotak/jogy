#ifndef _JOGY_RESOURCES_H
#define _JOGY_RESOURCES_H

namespace jogy {

class IGeometryQuads;

class Resources {
public:
    static IGeometryQuads * pStickGeo;
    static IGeometryQuads * pStickedGeo;
    static IGeometryQuads * pScrollButtons[4];   // top, bottom, left, right
    static int iScrollButtonWidth;
    static int iScrollButtonHeight;
};
}

#endif
