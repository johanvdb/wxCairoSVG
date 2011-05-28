/*
 * color.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#include "wx/svg/cairo/paintservers/color.h"

#include "wx/log.h"
#include "wx/tokenzr.h"

IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGColor, wxCairoSVGPaintServer)

wxCairoSVGColor::wxCairoSVGColor(wxCairoSVGContext* context): wxCairoSVGPaintServer(context) {
}

wxCairoSVGColor::~wxCairoSVGColor() {
}

void wxCairoSVGColor::Init() {
	m_ok = false;
	m_r = 0;
	m_g = 0;
	m_b = 0;
}

unsigned char wxCairoSVGColor::ParseHex(wxChar c) {
	if ((c >= '0') && (c <= '9')) {
		return c - '0';
	}
	if ((c >= 'A') && (c <= 'F')) {
		return c - 'A' + 10;
	}
	if ((c >= 'a') && (c <= 'f')) {
		return c - 'a' + 10;
	}
	return 0;
}

void wxCairoSVGColor::SetSource(cairo_t* cairo_image) {
	double r = (double) m_r / 255;
	double g = (double) m_g / 255;
	double b = (double) m_b / 255;
	wxLogTrace(_T("svg"), _T("wxCairoSVGColor::SetSource %s %f %f %f"), (m_ok)? _T("true"):_T("false"), r, g, b);
	if (m_ok)
	cairo_set_source_rgb(cairo_image, r, g, b);
}

void wxCairoSVGColor::SetColor(const wxString& value) {
	int r, b, g = 0;

	wxLogTrace(_T("svg"), _T("Set color ") + value + _T(" (%f %f %f)"), m_r, m_g, m_b);
}

void wxCairoSVGColor::SetColor(int r, int g, int b) {
	m_r = r;
	m_g = g;
	m_b = b;
	m_ok = true;
}

double wxCairoSVGColor::GetRed() {
	return m_r;
}

double wxCairoSVGColor::GetGreen() {
	return m_g;
}

double wxCairoSVGColor::GetBlue() {
	return m_b;
}

wxCairoSVGColor* wxCairoSVGColor::FromString(wxCairoSVGContext* context, const wxString& value) {
	int r = 0; int g = 0; int b = 0;
	if (FromString(value, &r, &g, &b)) {
		wxCairoSVGColor* color = new wxCairoSVGColor(context);
		color->SetColor(r, g, b);
		return color;
	} else {
		return NULL;
	}
}

bool wxCairoSVGColor::FromString(const wxString& value, int *r_out, int *g_out, int *b_out) {
	bool ok = false;
	int r = 0; int b = 0; int g = 0;

	if (value.StartsWith(_T("#")) && value.length() == 7) {
		r = (ParseHex(value[1]) << 4) + ParseHex(value[2]);
		g = (ParseHex(value[3]) << 4) + ParseHex(value[4]);
		b = (ParseHex(value[5]) << 4) + ParseHex(value[6]);
		ok = true;
	}
	else if (value.StartsWith(_T("#")) && value.length() == 4) {
		r = (ParseHex(value[1]) << 4) + ParseHex(value[1]);
		g = (ParseHex(value[2]) << 4) + ParseHex(value[2]);
		b = (ParseHex(value[3]) << 4) + ParseHex(value[3]);
		ok = true;
	}
	else if (value == _T("black")) { r = 0; g = 0; b = 0; ok = true; }
	else if (value == _T("silver")) { r = 192; g = 192; b = 192; ok = true; }
	else if (value == _T("gray")) { r = 128; g = 128; b = 128; ok = true; }
	else if (value == _T("white")) { r = 255; g = 255; b = 255; ok = true; }
	else if (value == _T("maroon")) { r = 128; g = 0; b = 0; ok = true; }
	else if (value == _T("red")) { r = 255; g = 0; b = 0; ok = true; }
	else if (value == _T("purple")) { r = 128; g = 0; b = 128; ok = true; }
	else if (value == _T("fuchsia")) { r = 255; g = 0; b = 255; ok = true; }
	else if (value == _T("green")) { r = 0; g = 128; b = 0; ok = true; }
	else if (value == _T("lime")) { r = 0; g = 255; b = 0; ok = true; }
	else if (value == _T("olive")) { r = 128; g = 128; b = 0; ok = true; }
	else if (value == _T("yellow")) { r = 255; g = 255; b = 0; ok = true; }
	else if (value == _T("navy")) { r = 0; g = 0; b = 128; ok = true; }
	else if (value == _T("blue")) { r = 0; g = 0; b = 255; ok = true; }
	else if (value == _T("teal")) { r = 0; g = 128; b = 128; ok = true; }
	else if (value == _T("aqua")) { r = 0; g = 255; b = 255; ok = true; }
	else if (value.Matches(_T("rgb(*,*,*)"))) {
		wxArrayString values = wxStringTokenize(value.Mid(4, value.Length() - 5), _T(","));
		double r_double, g_double, b_double;
		values[0].ToDouble(&r_double);
		values[1].ToDouble(&g_double);
		values[2].ToDouble(&b_double);
		if (values[0].Contains(_T("%"))) r = 256.0 * r_double / 100.0; else r = (int) r_double;
		if (values[0].Contains(_T("%"))) g = 256.0 * r_double / 100.0; else g = (int) g_double;
		if (values[0].Contains(_T("%"))) b = 256.0 * r_double / 100.0; else b = (int) b_double;
		ok = true;
	}
	if (ok) {
		*r_out = r;
		*g_out = g;
		*b_out = b;
	}

	wxLogTrace(_T("svg"), _T("wxCairoSVGColor::FromString %s %d %d %d"), value.c_str(), r, g, b);

	return ok;
}

bool wxCairoSVGColor::IsOk() {
	return m_ok;
}
