/*
 * rect.h
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#ifndef WXCAIROSVGPATH_H_
#define WXCAIROSVGPATH_H_

#include "wx/svg/cairo/node.h"

#include "wx/list.h"
#include "wx/dynarray.h"

class wxCairoSVGPathCommand: public wxObject {
public:
	wxArrayDouble m_params;
	wxString m_cmd;
	bool m_relative;

	DECLARE_ABSTRACT_CLASS(wxCairoSVGPathCommand)
};

WX_DECLARE_LIST(wxCairoSVGPathCommand, wxCairoSVGPathCommandList);

class wxCairoSVGPath: public wxCairoSVGNode {
public:
	wxCairoSVGPath(wxCairoSVGContext* context);
	virtual ~wxCairoSVGPath();

	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);
	virtual void Render(cairo_t* cairo_image, const wxPoint& pos = wxDefaultPosition);

	DECLARE_ABSTRACT_CLASS(wxCairoSVGPath)

private:
	wxCairoSVGPathCommandList m_commands;

	bool ReadParam(const wxString& data, int& i, wxArrayDouble& m_params);
	void ReadParams(const wxString& data, int& i, wxArrayDouble& m_params);
};

#endif /* WXCAIROSVGRECT_H_ */
