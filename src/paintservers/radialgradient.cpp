/*
 * color.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#include "wx/svg/cairo/paintservers/radialgradient.h"

#include "wx/log.h"
#include "wx/tokenzr.h"

IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGRadialGradient, wxCairoSVGPaintServer)

wxCairoSVGRadialGradient::wxCairoSVGRadialGradient(wxCairoSVGContext* context): wxCairoSVGPaintServer(context) {
	m_stopList.DeleteContents(true);
}

wxCairoSVGRadialGradient::~wxCairoSVGRadialGradient() {
}

void wxCairoSVGRadialGradient::LoadXML(wxXmlNode* node, wxCairoSVGNode* parent) {
	wxCairoSVGPaintServer::LoadXML(node, parent);

	m_gradientUnits = node->GetPropVal(_T("gradientUnits"), _T("objectBoundingBox"));
	m_gradientTransform = node->GetPropVal(_T("gradientTransform"), _T(""));
	node->GetPropVal(_T("cx"), _T("0.5")).ToDouble(&m_cx);
	node->GetPropVal(_T("cy"), _T("0.5")).ToDouble(&m_cy);
	node->GetPropVal(_T("r"), _T("0.5")).ToDouble(&m_r);

	wxXmlNode* child = node->GetChildren();
	while (child) {
		if (child->GetName() == _T("stop")) {
			wxCairoSVGGradientStop* stop = new wxCairoSVGGradientStop();
			stop->LoadXML(child);
			m_stopList.Append(stop);
		}
		child = child->GetNext();
	}
	wxLogTrace(_T("svg"), _T("wxCairoSVGRadialGradient::LoadXML %s"), m_id.c_str());
}


void wxCairoSVGRadialGradient::Op(cairo_t* cairo_image, bool preserve, wxCairoSVGPaintServerOp op) {

	cairo_pattern_t *pat;
	bool saved = false;

	if (m_gradientUnits == _T("objectBoundingBox")) {
		cairo_save(cairo_image);
		saved = true;
		double minx;
		double miny;
		double maxx;
		double maxy;

		cairo_path_extents(cairo_image, &minx, &miny, &maxx, &maxy);
		cairo_matrix_t mat;
		cairo_matrix_init(&mat, maxx-minx, 0, 0, maxy-miny, minx, miny);
		cairo_transform(cairo_image, &mat);
		wxLogTrace(_T("svg"), _T("wxCairoSVGRadialGradient::Op transformed to objectBoundingBox minx %f miny %f maxx %f maxy %f"),
				minx, miny, maxx, maxy);
	} else if (m_gradientTransform.Contains(_T("matrix"))) {
		double m[6];
		wxArrayString params = wxStringTokenize(m_gradientTransform.Mid(m_gradientTransform.Find(_T("matrix(")) + 7), _T(" "));
		if (params.Count() == 6) {
			for (int i = 0; i < 6; i++) {
				params[i].Strip().ToDouble(&m[i]);
			}
			cairo_save(cairo_image);
			saved = true;
			cairo_matrix_t mat;
			cairo_matrix_init(&mat, m[0], m[1], m[2], m[3], m[4], m[5]);
			cairo_transform(cairo_image, &mat);
			wxLogTrace(_T("svg"), _T("wxCairoSVGRadialGradient::Op matrix transform %f %f %f %f %f %f"),
					m[0], m[1], m[2], m[3], m[4], m[5]);
		}
	}

	pat = cairo_pattern_create_radial(m_cx, m_cy, 0, m_cx, m_cy, m_r);
	wxLogTrace(_T("svg"), _T("wxCairoSVGRadialGradient::Op cairo_pattern_create_radial cx %f cy %f r %f"),
			m_cx, m_cy, m_r);

	for (wxCairoSVGGradientStopList::iterator i = m_stopList.begin(); i != m_stopList.end(); ++i) {
		wxCairoSVGGradientStop* stop = *i;
		if (stop->m_opacity == 1) {
			cairo_pattern_add_color_stop_rgb(pat, stop->m_offset, (double) stop->m_r / 255.0, (double) stop->m_g / 255.0, (double) stop->m_b / 255.0);
		} else {
			cairo_pattern_add_color_stop_rgba(pat, stop->m_offset, (double) stop->m_r / 255.0, (double) stop->m_g / 255.0, (double) stop->m_b / 255.0, stop->m_opacity);
		}
	}

	cairo_set_source(cairo_image, pat);
	wxCairoSVGPaintServer::Op(cairo_image, preserve, op);

	if (saved) cairo_restore(cairo_image);

	cairo_pattern_destroy(pat);

	wxLogTrace(_T("svg"), _T("wxCairoSVGRadialGradient::Op done"));
}
