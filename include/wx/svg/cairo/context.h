#ifndef WXCAIROSVGCONTEXT_H_
#define WXCAIROSVGCONTEXT_H_

#include "wx/hashmap.h"
#include "wx/object.h"

class wxCairoSVGNode;
class wxCairoSVGNodeList;

WX_DECLARE_STRING_HASH_MAP(wxCairoSVGNode*, wxCairoSVGNodeHash);

class wxCairoSVGContext {
public:
	wxCairoSVGNode* m_rootNode;

	wxCairoSVGContext();
	virtual ~wxCairoSVGContext();
	void AddNode(wxCairoSVGNode* node);
	void AddNodeToIndex(wxCairoSVGNode* node);
	wxCairoSVGNode* FindNode(const wxString& id);
	void Index();

private:
	wxCairoSVGNodeHash* m_nodeHash;
	wxCairoSVGNodeList* m_nodeList;
};

#endif /* WXCAIROSVGCONTEXT_H_ */
