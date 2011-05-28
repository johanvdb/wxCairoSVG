#ifndef WXCAIROSVGSOLIDCOLOR_H_
#define WXCAIROSVGSOLIDCOLOR_H_

#include "paintserver.h"

class wxCairoSVGSolidColor: public wxCairoSVGPaintServer {
public:
	wxCairoSVGSolidColor(wxString value);
	virtual ~wxCairoSVGSolidColor();
	void SetSource(cairo_t* cairo_image);
private:
	wxCairoSVGColor m_solidColor;
	double m_solidOpacity;

	DECLARE_ABSTRACT_CLASS(wxCairoSVGSolidColor)
};

#endif /* WXCAIROSVGSOLIDCOLOR_H_ */
