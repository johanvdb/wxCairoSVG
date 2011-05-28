/*
 * rect.h
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#ifndef WXCAIROSVGPOLYLINE_H_
#define WXCAIROSVGPOLYLINE_H_

#include "wx/svg/cairo/node.h"

class wxCairoSVGPolyLine: public wxCairoSVGNode {
public:
	wxCairoSVGPolyLine(wxCairoSVGContext* context);
	virtual ~wxCairoSVGPolyLine();

	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);
	virtual void Render(cairo_t* cairo_image, const wxPoint& pos = wxDefaultPosition);

	DECLARE_ABSTRACT_CLASS(wxCairoSVGPolyLine)

private:
	wxString m_data;
	wxRealPointList m_points;
};

#endif /* WXCAIROSVGPOLYLINE_H_ */
