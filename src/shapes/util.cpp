#include "wx/svg/cairo/shapes/util.h"

#include "wx/svg/cairo/paintservers/paintserver.h"

#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxRealPointList);

bool wxCairoSVGShapeUtil::ParsePolyPoints(const wxString& value, wxRealPointList* points) {

	int pos = 0;
	wxString x, y;
	bool foundComma = false;

	while (value.Length() > pos) {
		wxChar c = value[pos++];
		if (c == ',') { foundComma = true; continue; };
		if ((c >= '0' && c <= '9') || (c == '.')) {
			if (!foundComma) { x += c; } else {	y += c; };
		} else {
			if (!x.IsEmpty() && !y.IsEmpty()) {
				double dx, dy;
				x.ToDouble(&dx);
				y.ToDouble(&dy);
				points->Insert(new wxRealPoint(dx, dy));
				x = _T("");
				y = _T("");
				foundComma = 0;
			}
		}
	}

	if (!x.IsEmpty() && !y.IsEmpty()) {
		double dx, dy;
		x.ToDouble(&dx);
		y.ToDouble(&dy);
		points->Insert(new wxRealPoint(dx, dy));
	}

	return true;
}
