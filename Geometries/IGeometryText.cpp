// -----------------------------------------------------------------
// IGeometryText
// -----------------------------------------------------------------
#include "IGeometryText.h"

namespace jogy {

// -----------------------------------------------------------------
// Name : IGeometryText
//  Constructor
// -----------------------------------------------------------------
IGeometryText::IGeometryText() : IGeometry()
{
    m_sText = string("");
    m_fScale = -1;
    m_iNbQuads = 0;
    m_iTextLength = 0;
    m_uFontId = 0;
}

// -----------------------------------------------------------------
// Name : ~IGeometryText
//  Destructor
// -----------------------------------------------------------------
IGeometryText::~IGeometryText()
{
}

// -----------------------------------------------------------------
// Name : reload
// -----------------------------------------------------------------
void IGeometryText::reload()
{
	setText(m_sText, m_uFontId);
}
}
