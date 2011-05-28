/*
 * rect.h
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#ifndef WXCAIROSVGELLIPSE_H_
#define WXCAIROSVGELLIPSE_H_

#include "wx/svg/cairo/node.h"

class wxCairoSVGEllipse: public wxCairoSVGNode {
public:
	wxCairoSVGEllipse(wxCairoSVGContext* context);
	virtual ~wxCairoSVGEllipse();

	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);
	virtual void Render(cairo_t* cairo_image, const wxPoint& pos = wxDefaultPosition);

	DECLARE_ABSTRACT_CLASS(wxCairoSVGEllipse)

private:
	double m_cx;
	double m_cy;
	double m_rx;
	double m_ry;
};

#endif /* WXCAIROSVGELLIPSE_H_ */
