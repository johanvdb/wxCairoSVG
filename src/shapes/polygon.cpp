#include "wx/svg/cairo/shapes/polygon.h"
#include "wx/svg/cairo/shapes/util.h"
#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/log.h"
#include "wx/tokenzr.h"

IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGPolygon, wxCairoSVGNode)

wxCairoSVGPolygon::wxCairoSVGPolygon(wxCairoSVGContext* context): wxCairoSVGNode(context) {
}

wxCairoSVGPolygon::~wxCairoSVGPolygon() {
}

void wxCairoSVGPolygon::LoadXML(wxXmlNode* node, wxCairoSVGNode* parent) {
	wxCairoSVGNode::LoadXML(node, parent);
	wxString points = node->GetPropVal(_T("points"), _T(""));
	wxCairoSVGShapeUtil::ParsePolyPoints(points, &m_points);
}

void wxCairoSVGPolygon::Render(cairo_t* cairo_image, const wxPoint& pos) {

	wxCairoSVGPaintServer* fillServer = wxCairoSVGPaintServer::FromString(m_context, m_fill);
	wxCairoSVGPaintServer* strokeServer = wxCairoSVGPaintServer::FromString(m_context, m_stroke);

	if (fillServer == NULL && strokeServer == NULL) return;

	wxLogTrace(_T("svg"), _T("wxCairoSVGPolygon::Render points %d"), m_points.size());

	if (m_points.size() <= 0) return;

	cairo_move_to(cairo_image, m_points[0]->x, m_points[0]->y);
	for (int i = 1; i < m_points.size(); i++) {
		cairo_line_to(cairo_image, m_points[i]->x, m_points[i]->y);
	}
	cairo_close_path(cairo_image);

	if (fillServer) fillServer->Fill(cairo_image, (strokeServer != NULL));
	if (strokeServer) strokeServer->Stroke(cairo_image, m_strokeWidth, false);
	wxCairoSVGNode::Render(cairo_image, pos);
}
