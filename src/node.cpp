#include "wx/svg/cairo/node.h"
#include "wx/log.h"

IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGNode, wxObject);

#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxCairoSVGNodeList);

wxCairoSVGNode::wxCairoSVGNode(wxCairoSVGContext* context) {
	m_next = NULL;
	m_parent = NULL;
	m_firstChild = NULL;
	m_context = context;
	m_context->AddNode(this);
}

wxCairoSVGNode::~wxCairoSVGNode() {
}

wxCairoSVGNode* wxCairoSVGNode::GetNext() {
	return m_next;
}

wxCairoSVGNode* wxCairoSVGNode::GetParent() {
	return m_parent;
}

wxCairoSVGNode* wxCairoSVGNode::GetFirstChild() {
	return m_firstChild;
}

wxString wxCairoSVGNode::GetId() {
	return m_id;
}

void wxCairoSVGNode::SetId(const wxString& id) {
	m_id = id;
}

void wxCairoSVGNode::AddChild(wxCairoSVGNode* child) {
	if (m_firstChild) {
		m_firstChild->AddSibling(child);
	} else {
		m_firstChild = child;
	}
}

void wxCairoSVGNode::AddSibling(wxCairoSVGNode* sibling) {
	if (m_next) m_next->AddSibling(sibling); else
		m_next = sibling;
}

double wxCairoSVGNode::GetOpacity() {
	return m_opacity;
}

void wxCairoSVGNode::LoadXML(wxXmlNode* node, wxCairoSVGNode* parent) {
	m_parent = parent;
	m_id = node->GetPropVal(_T("id"), _T(""));
	if (m_id.IsEmpty()) m_id = node->GetPropVal(_T("xml:id"), _T(""));
	m_fill = node->GetPropVal(_T("fill"), _T("black"));
	m_stroke = node->GetPropVal(_T("stroke"), _T("none"));
	m_display = node->GetPropVal(_T("display"), _T(""));

	node->GetPropVal(_T("stroke-width"), _T("1")).ToDouble(&m_strokeWidth);
	node->GetPropVal(_T("opacity"), _T("1")).ToDouble(&m_opacity);
}

void wxCairoSVGNode::Render(cairo_t* cairo_image, const wxPoint& pos) {
	if (m_display == _T("none")) return;
	wxCairoSVGNode *child = m_firstChild;
	while (child) {
		if (child->GetOpacity() != 1) {
			cairo_push_group(cairo_image);
		}
		child->Render(cairo_image, pos);
		if (child->GetOpacity() != 1) {
			cairo_pop_group_to_source(cairo_image);
			cairo_paint_with_alpha(cairo_image, child->GetOpacity());
		}
		child = child->GetNext();
	}
}
