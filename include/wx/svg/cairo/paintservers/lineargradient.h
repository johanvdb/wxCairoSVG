#ifndef WXCAIROSVGLINEARGRADIENT_H_
#define WXCAIROSVGLINEARGRADIENT_H_

#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/svg/cairo/paintservers/gradientstop.h"

#include "cairo.h"

class wxCairoSVGLinearGradient: public wxCairoSVGPaintServer {
public:
	wxCairoSVGLinearGradient(wxCairoSVGContext* context);
	virtual ~wxCairoSVGLinearGradient();
	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);

protected:
	virtual void Op(cairo_t* cairo_image, bool preserve, wxCairoSVGPaintServerOp op);

private:
	wxCairoSVGGradientStopList m_stopList;
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2;
	wxString m_gradientUnits;
	wxString m_gradientTransform;
	DECLARE_ABSTRACT_CLASS(wxCairoSVGLinearGradient)
};

#endif /* WXCAIROSVGLINEARGRADIENT_H_ */
