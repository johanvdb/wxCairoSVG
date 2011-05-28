/*
 * node.h
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#ifndef WXCAIROSVGNODE_H_
#define WXCAIROSVGNODE_H_

#include "wx/object.h"
#include "wx/xml/xml.h"
#include "wx/gdicmn.h"
#include "wx/svg/cairo/context.h"
#include "wx/svg/cairo/shapes/util.h"

#include <cairo.h>

class wxCairoSVGNode: public wxObject {
public:
	wxCairoSVGNode(wxCairoSVGContext* context);
	virtual ~wxCairoSVGNode();
	virtual void LoadXML(wxXmlNode* node, wxCairoSVGNode* parent);
	virtual void Render(cairo_t* cairo_image, const wxPoint& pos = wxDefaultPosition);
	wxCairoSVGNode* GetNext();
	wxCairoSVGNode* GetParent();
	wxCairoSVGNode* GetFirstChild();
	wxString GetId();
	void SetId(const wxString& id);

	void AddChild(wxCairoSVGNode* child);
	void AddSibling(wxCairoSVGNode* sibling);
	double GetOpacity();

	DECLARE_ABSTRACT_CLASS(wxCairoSVGNode);
protected:
	wxString m_display;
	wxString m_id;
	wxString m_fill;
	wxString m_stroke;
	double m_strokeWidth;
	double m_opacity;
	wxCairoSVGContext* m_context;
private:
	wxCairoSVGNode* m_parent;
	wxCairoSVGNode* m_next;
	wxCairoSVGNode* m_firstChild;
};

WX_DECLARE_LIST(wxCairoSVGNode, wxCairoSVGNodeList);

#endif /* WXCAIROSVGNODE_H_ */
