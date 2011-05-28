/*
 * paintserver.cpp
 *
 *  Created on: Apr 16, 2011
 *      Author: johanvdb
 */

#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/svg/cairo/paintservers/color.h"

#include "wx/log.h"

wxCairoSVGPaintServer::wxCairoSVGPaintServer(wxCairoSVGContext* context): wxCairoSVGNode(context) {

}

wxCairoSVGPaintServer::~wxCairoSVGPaintServer() {

}

void wxCairoSVGPaintServer::LoadXML(wxXmlNode* node, wxCairoSVGNode* parent) {
	wxCairoSVGNode::LoadXML(node, parent);
}

wxCairoSVGPaintServer* wxCairoSVGPaintServer::FromString(wxCairoSVGContext* context, const wxString paintServer) {

	if (paintServer.Matches(_T("url(#?*)"))) {
		wxString id = paintServer;
		id = paintServer.Mid(5, paintServer.Length() - 6);
		wxLogTrace(_T("svg"), _T("wxCairoSVGPaintServer::FromString IRI reference to %s"), id.c_str());
		return wxDynamicCast(context->FindNode(id), wxCairoSVGPaintServer);
	} else {
		wxCairoSVGPaintServer* server = wxDynamicCast(context->FindNode(paintServer), wxCairoSVGPaintServer);
		if (server == NULL) {
			wxCairoSVGColor* color = wxCairoSVGColor::FromString(context, paintServer);
			if (color != NULL) {
				color->SetId(paintServer);
				context->AddNodeToIndex(color);
				server = color;
			}
		}
		return server;
	}

	return NULL;
}

void wxCairoSVGPaintServer::Fill(cairo_t* cairo_image, bool preserve) {
	Op(cairo_image, preserve, WXCAIROSVGPAINTSERVERFILL);
}

void wxCairoSVGPaintServer::Stroke(cairo_t* cairo_image, double strokeWidth, bool preserve) {
	cairo_set_line_width(cairo_image, strokeWidth);
	Op(cairo_image, preserve, WXCAIROSVGPAINTSERVERSTROKE);
}

void wxCairoSVGPaintServer::Op(cairo_t* cairo_image, bool preserve, wxCairoSVGPaintServerOp op) {
	SetSource(cairo_image);

	wxLogTrace(_T("svg"), _T("wxCairoSVGPaintServer::Op %s"), GetClassInfo()->GetClassName());

	switch (op) {
	case WXCAIROSVGPAINTSERVERFILL:
			if (preserve) {
				cairo_fill_preserve(cairo_image);
			} else {
				cairo_fill(cairo_image);
			}
			break;
	case WXCAIROSVGPAINTSERVERSTROKE:
			if (preserve) {
				cairo_stroke_preserve(cairo_image);
			} else {
				cairo_stroke(cairo_image);
			}
			break;
	}
}
