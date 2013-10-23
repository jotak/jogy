#ifndef _IGEOMETRY_TEXT_H
#define _IGEOMETRY_TEXT_H

#include "IGeometry.h"

namespace jogy {

class IGeometryText : public IGeometry
{
public:
    IGeometryText();
    virtual ~IGeometryText();

    virtual void setText(string sText, u8 uFontId) = 0;
    virtual void reload();

protected:
    int m_iNbQuads;
    string m_sText;
    int m_iTextLength;
    u8 m_uFontId;
    float m_fScale;
};
}

#endif
