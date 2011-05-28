/*
 * rect.h
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#ifndef WXCAIROSVGRECT_H_
#define WXCAIROSVGRECT_H_

#include "wx/svg/cairo/node.h"
#include "wx/svg/cairo/context.h"

class wxCairoSVGRect: public wxCairoSVGNode {
public:
	wxCairoSVGRect(wxCairoSVGContext* context);
	virtual ~wxCairoSVGRect();

	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);
	virtual void Render(cairo_t* cairo_image, const wxPoint& pos = wxDefaultPosition);

	DECLARE_ABSTRACT_CLASS(wxCairoSVGRect)

private:
	double m_x;
	double m_y;
	double m_width;
	double m_height;
	double m_rx;
	double m_ry;
	double m_strokeMiterLimit;
};

#endif /* WXCAIROSVGRECT_H_ */
