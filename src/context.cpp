#include "wx/svg/cairo/context.h"
#include "wx/svg/cairo/node.h"

wxCairoSVGContext::wxCairoSVGContext() {
	m_nodeList = new wxCairoSVGNodeList();
	m_nodeList->DeleteContents(true);
	m_nodeHash = NULL;
}

wxCairoSVGContext::~wxCairoSVGContext() {
	if (m_nodeHash != NULL) delete m_nodeHash;
	delete m_nodeList;
}

void wxCairoSVGContext::AddNode(wxCairoSVGNode* node) {
	m_nodeList->Append(node);
}

void wxCairoSVGContext::AddNodeToIndex(wxCairoSVGNode* node) {
	if (!node->GetId().IsEmpty()) {
		(*m_nodeHash)[node->GetId()] = node;
	}
}

wxCairoSVGNode* wxCairoSVGContext::FindNode(const wxString& id) {
	if (m_nodeHash == NULL) return NULL;
	wxCairoSVGNodeHash::iterator i = m_nodeHash->find(id);
	if (i == m_nodeHash->end()) return NULL;
	return i->second;
}

void wxCairoSVGContext::Index() {
	if (m_nodeHash != NULL) delete m_nodeHash;
	m_nodeHash = new wxCairoSVGNodeHash();
	wxCairoSVGNodeList::iterator i;
	for (i = m_nodeList->begin(); i != m_nodeList->end(); ++i) {
		wxString id = (*i)->GetId();
		if (!id.IsEmpty()) {
			(*m_nodeHash)[id] = *i;
		}
	}
}
