/*
 * rect.h
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#ifndef WXCAIROSVGCIRCLE_H_
#define WXCAIROSVGCIRCLE_H_

#include "wx/svg/cairo/node.h"

class wxCairoSVGCircle: public wxCairoSVGNode {
public:
	wxCairoSVGCircle(wxCairoSVGContext* context);
	virtual ~wxCairoSVGCircle();

	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);
	virtual void Render(cairo_t* cairo_image, const wxPoint& pos = wxDefaultPosition);

	DECLARE_ABSTRACT_CLASS(wxCairoSVGCircle)

private:
	double m_cx;
	double m_cy;
	double m_r;
};

#endif /* WXCAIROSVGCIRCLE_H_ */
