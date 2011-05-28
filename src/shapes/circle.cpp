#include "wx/svg/cairo/shapes/circle.h"
#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/log.h"

IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGCircle, wxCairoSVGNode)

wxCairoSVGCircle::wxCairoSVGCircle(wxCairoSVGContext* context): wxCairoSVGNode(context) {
}

wxCairoSVGCircle::~wxCairoSVGCircle() {
}


void wxCairoSVGCircle::LoadXML(wxXmlNode* node, wxCairoSVGNode* parent) {
	wxCairoSVGNode::LoadXML(node, parent);
	node->GetPropVal(_T("cx"), _T("0.0")).ToDouble(&m_cx);
	node->GetPropVal(_T("cy"), _T("0.0")).ToDouble(&m_cy);
	node->GetPropVal(_T("r"), _T("0.0")).ToDouble(&m_r);
}

void wxCairoSVGCircle::Render(cairo_t* cairo_image, const wxPoint& pos) {
	wxCairoSVGPaintServer* fillServer = wxCairoSVGPaintServer::FromString(m_context, m_fill);
	wxCairoSVGPaintServer* strokeServer = wxCairoSVGPaintServer::FromString(m_context, m_stroke);
	if (fillServer == NULL && strokeServer == NULL) return;
	cairo_arc(cairo_image, m_cx, m_cy, m_r, 0, 2*M_PI);
	if (fillServer) fillServer->Fill(cairo_image, (strokeServer != NULL));
	if (strokeServer) strokeServer->Stroke(cairo_image, m_strokeWidth, false);
	wxCairoSVGNode::Render(cairo_image, pos);
}
