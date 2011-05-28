#include "wx/svg/cairo/shapes/polyline.h"
#include "wx/svg/cairo/shapes/util.h"
#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/log.h"
#include "wx/tokenzr.h"

IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGPolyLine, wxCairoSVGNode)

wxCairoSVGPolyLine::wxCairoSVGPolyLine(wxCairoSVGContext* context): wxCairoSVGNode(context) {
}

wxCairoSVGPolyLine::~wxCairoSVGPolyLine() {
}

void wxCairoSVGPolyLine::LoadXML(wxXmlNode* node, wxCairoSVGNode* parent) {
	wxCairoSVGNode::LoadXML(node, parent);
	wxString points = node->GetPropVal(_T("points"), _T(""));
	wxCairoSVGShapeUtil::ParsePolyPoints(points, &m_points);
}

void wxCairoSVGPolyLine::Render(cairo_t* cairo_image, const wxPoint& pos) {

	wxCairoSVGPaintServer* fillServer = wxCairoSVGPaintServer::FromString(m_context, m_fill);
	wxCairoSVGPaintServer* strokeServer = wxCairoSVGPaintServer::FromString(m_context, m_stroke);

	if (fillServer == NULL && strokeServer == NULL) return;

	wxLogTrace(_T("svg"), _T("wxCairoSVGPolyLine::Render points %d"), m_points.size());

	if (m_points.size() <= 0) return;

	cairo_move_to(cairo_image, m_points[0]->x, m_points[0]->y);
	wxLogTrace(_T("svg"), _T("wxCairoSVGPolyLine::Render cairo_move_to %f %f"), m_points[0]->x, m_points[0]->y);

	for (int i = 1; i < m_points.size(); i++) {
		cairo_line_to(cairo_image, m_points[i]->x, m_points[i]->y);
		wxLogTrace(_T("svg"), _T("wxCairoSVGPolyLine::Render cairo_line_to %f %f"), m_points[i]->x, m_points[i]->y);
	}

	if (fillServer) fillServer->Fill(cairo_image, (strokeServer != NULL));
	if (strokeServer) strokeServer->Stroke(cairo_image, m_strokeWidth, false);

	wxCairoSVGNode::Render(cairo_image, pos);
}
