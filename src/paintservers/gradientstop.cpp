/*
 * gradientstop.cpp
 *
 *  Created on: Apr 17, 2011
 *      Author: johanvdb
 */

#include "wx/svg/cairo/paintservers/gradientstop.h"
#include "wx/log.h"
#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxCairoSVGGradientStopList);

wxCairoSVGGradientStop::wxCairoSVGGradientStop() {
	m_r = 0;
	m_g = 0;
	m_b = 0;
	m_opacity = 1;
	m_offset = 0;
}

wxCairoSVGGradientStop::~wxCairoSVGGradientStop() {
}

void wxCairoSVGGradientStop::LoadXML(wxXmlNode* node) {
	node->GetPropVal(_T("offset"), _T("0")).ToDouble(&m_offset);
	wxString stopColor = node->GetPropVal(_T("stop-color"), _T(""));
	if (stopColor.IsEmpty()) {
		wxLogTrace(_T("svg"), _T("wxCairoSVGGradientStop::LoadXML no stop color in attribs. Looking in style"));
		wxString style = node->GetPropVal(_T("style"), _T(""));
		stopColor = style.Mid(style.find(_T("stop-color:")) + 11);
	}

	wxCairoSVGColor::FromString(stopColor, &m_r, &m_g, &m_b);
	node->GetPropVal(_T("opacity"), _T("1")).ToDouble(&m_opacity);
	wxLogTrace(_T("svg"), _T("wxCairoSVGGradientStop::LoadXML offset %f opacity %f color %s r %d g %d b %d"),
			m_offset, m_opacity, stopColor.c_str(), m_r, m_g, m_b);
}
