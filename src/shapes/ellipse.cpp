#include "wx/svg/cairo/shapes/ellipse.h"
#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/log.h"

IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGEllipse, wxCairoSVGNode)

wxCairoSVGEllipse::wxCairoSVGEllipse(wxCairoSVGContext* context): wxCairoSVGNode(context) {
}

wxCairoSVGEllipse::~wxCairoSVGEllipse() {
}


void wxCairoSVGEllipse::LoadXML(wxXmlNode* node, wxCairoSVGNode* parent) {
	wxCairoSVGNode::LoadXML(node, parent);
	node->GetPropVal(_T("cx"), _T("0.0")).ToDouble(&m_cx);
	node->GetPropVal(_T("cy"), _T("0.0")).ToDouble(&m_cy);
	node->GetPropVal(_T("rx"), _T("0.0")).ToDouble(&m_rx);
	node->GetPropVal(_T("ry"), _T("0.0")).ToDouble(&m_ry);
}

void wxCairoSVGEllipse::Render(cairo_t* cairo_image, const wxPoint& pos) {
	if ((m_rx <= 0) || (m_ry <= 0)) return;

	wxCairoSVGPaintServer* fillServer = wxCairoSVGPaintServer::FromString(m_context, m_fill);
	wxCairoSVGPaintServer* strokeServer = wxCairoSVGPaintServer::FromString(m_context, m_stroke);
	if (fillServer == NULL && strokeServer == NULL) return;

	cairo_save (cairo_image);
	cairo_translate (cairo_image, m_cx, m_cy);
	cairo_scale (cairo_image, m_rx, m_ry);
	cairo_arc (cairo_image, 0., 0., 1., 0., 2 * M_PI);
	cairo_restore (cairo_image);

	if (fillServer) fillServer->Fill(cairo_image, (strokeServer != NULL));
	if (strokeServer) strokeServer->Stroke(cairo_image, m_strokeWidth, false);

	wxLogTrace(_T("svg"), _T("  wxCairoSVGEllipse::Render x %f y %f rx %f ry %f"), m_cx, m_cy, m_rx, m_ry);
	wxCairoSVGNode::Render(cairo_image, pos);
}
