#ifndef WXCAIROSVGRADIALGRADIENT_H_
#define WXCAIROSVGRADIALGRADIENT_H_

#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/svg/cairo/paintservers/gradientstop.h"

#include "cairo.h"

class wxCairoSVGRadialGradient: public wxCairoSVGPaintServer {
public:
	wxCairoSVGRadialGradient(wxCairoSVGContext* context);
	virtual ~wxCairoSVGRadialGradient();
	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);

protected:
	virtual void Op(cairo_t* cairo_image, bool preserve, wxCairoSVGPaintServerOp op);

private:
	wxCairoSVGGradientStopList m_stopList;
	double m_cx;
	double m_cy;
	double m_r;
	wxString m_gradientUnits;
	wxString m_gradientTransform;
	DECLARE_ABSTRACT_CLASS(wxCairoSVGRadialGradient)
};

#endif /* WXCAIROSVGRADIALGRADIENT_H_ */
