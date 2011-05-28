/*
 * rect.h
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#ifndef WXCAIROSVGPOLYGON_H_
#define WXCAIROSVGPOLYGON_H_

#include "wx/svg/cairo/node.h"

class wxCairoSVGPolygon: public wxCairoSVGNode {
public:
	wxCairoSVGPolygon(wxCairoSVGContext* context);
	virtual ~wxCairoSVGPolygon();

	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);
	virtual void Render(cairo_t* cairo_image, const wxPoint& pos = wxDefaultPosition);

	DECLARE_ABSTRACT_CLASS(wxCairoSVGPolygon)

private:
	wxString m_data;
	wxRealPointList m_points;
};

#endif /* WXCAIROSVGPOLYGON_H_ */
