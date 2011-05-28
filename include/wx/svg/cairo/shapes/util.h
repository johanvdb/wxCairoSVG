/*
 * util.h
 *
 *  Created on: Apr 17, 2011
 *      Author: johanvdb
 */

#ifndef WXCAIROSVGSHAPEUTIL_H_
#define WXCAIROSVGSHAPEUTIL_H_

#include "wx/gdicmn.h"
#include "wx/list.h"
#include "cairo.h"

class wxCairoSVGPaintServer;

WX_DECLARE_LIST(wxRealPoint, wxRealPointList);

class wxCairoSVGShapeUtil {
public:
	static bool ParsePolyPoints(const wxString& value, wxRealPointList* points);

private:
	wxCairoSVGShapeUtil() {};
	~wxCairoSVGShapeUtil() {};
};

#endif /* WXCAIROSVGSHAPEUTIL_H_ */
