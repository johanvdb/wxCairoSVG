#ifndef WXCAIROSVGPAINTSERVER_H_
#define WXCAIROSVGPAINTSERVER_H_

#include "wx/svg/cairo/node.h"

enum wxCairoSVGPaintServerOp {
	WXCAIROSVGPAINTSERVERFILL,
	WXCAIROSVGPAINTSERVERSTROKE
};

class wxCairoSVGPaintServer: public wxCairoSVGNode {
public:
	wxCairoSVGPaintServer(wxCairoSVGContext* context);
	virtual ~wxCairoSVGPaintServer();
	virtual void Fill(cairo_t* cairo_image, bool preserve = false);
	virtual void Stroke(cairo_t* cairo_image, double strokeWidth, bool preserve = false);
	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);

	static wxCairoSVGPaintServer* FromString(wxCairoSVGContext* context, const wxString paintServer);

protected:
	virtual void SetSource(cairo_t* cairo_image) {};
	virtual void Op(cairo_t* cairo_image, bool preserve, wxCairoSVGPaintServerOp op);
};

#endif /* WXCAIROSVGPAINTSERVER_H_ */
