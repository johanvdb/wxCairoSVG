#include "wx/svg/cairo/shapes/rect.h"
#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/log.h"

IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGRect, wxCairoSVGNode)

wxCairoSVGRect::wxCairoSVGRect(wxCairoSVGContext* context): wxCairoSVGNode(context) {
}

wxCairoSVGRect::~wxCairoSVGRect() {
}


void wxCairoSVGRect::LoadXML(wxXmlNode* node, wxCairoSVGNode* parent) {
	wxCairoSVGNode::LoadXML(node, parent);

	node->GetPropVal(_T("x"), _T("0.0")).ToDouble(&m_x);
	node->GetPropVal(_T("y"), _T("0.0")).ToDouble(&m_y);
	node->GetPropVal(_T("width"), _T("0.0")).ToDouble(&m_width);
	node->GetPropVal(_T("height"), _T("0.0")).ToDouble(&m_height);
}

void wxCairoSVGRect::Render(cairo_t* cairo_image, const wxPoint& pos) {

	wxCairoSVGPaintServer* fillServer = wxCairoSVGPaintServer::FromString(m_context, m_fill);
	wxCairoSVGPaintServer* strokeServer = wxCairoSVGPaintServer::FromString(m_context, m_stroke);

	if (fillServer == NULL && strokeServer == NULL) return;

	cairo_rectangle(cairo_image, m_x, m_y, m_width, m_height);

	wxLogTrace(_T("svg"), _T("  wxCairoSVGRect::Render x %f y %f w %f h %f"), m_x, m_y, m_width, m_height);

	if (fillServer) fillServer->Fill(cairo_image, (strokeServer != NULL));
	if (strokeServer) strokeServer->Stroke(cairo_image, m_strokeWidth, false);

	wxCairoSVGNode::Render(cairo_image, pos);
}
