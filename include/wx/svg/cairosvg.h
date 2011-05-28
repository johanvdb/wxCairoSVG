/*
 * cairosvg.h
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#ifndef WXCAIROSVG_H_
#define WXCAIROSVG_H_

#include "wx/string.h"
#include "wx/stream.h"
#include "wx/gdicmn.h"
#include "wx/xml/xml.h"
#include "wx/image.h"
#include "wx/hashmap.h"
#include "wx/arrstr.h"
#include "wx/thread.h"
#include "wx/dc.h"

#include "wx/svg/cairo/context.h"
#include "wx/svg/cairo/node.h"

#include <cairo.h>

WX_DECLARE_HASH_MAP( int, wxImage*, wxIntegerHash, wxIntegerEqual, wxCairoSVGImageHash );

class wxCairoSVG: public wxObject {
public:
	wxCairoSVG();
	wxCairoSVG(const wxString& filename);
	wxCairoSVG(wxInputStream& instream);
	virtual ~wxCairoSVG();

	bool LoadFile(const wxString& filename);
	bool LoadFile(wxInputStream& instream);
	bool LoadXML(const wxXmlDocument& xml);

	bool IsOk();

	void Render(cairo_t* cairo_image, const wxPoint& pos = wxDefaultPosition);
	wxImage* GetImage(double scale);

private:
	wxString m_name;
	wxCairoSVGContext m_context;
	long m_width;
	long m_height;

	wxCairoSVGImageHash m_image_cache_hash;

	void Init();
	wxCairoSVGNode* CreateNode(int level, wxXmlNode* node, wxCairoSVGNode* parent);
	DECLARE_DYNAMIC_CLASS(wxCairoSVG);
};

WX_DECLARE_STRING_HASH_MAP(wxCairoSVG*, wxCairoSVGHash);

class wxCairoSVGCache: public wxThread {
public:
	static void Init();
	static void Destroy();

	wxCairoSVG* Get(const wxString& key);
	wxCairoSVG* GetOrCreate(const wxString& filename);
	bool Put(const wxString& key, wxCairoSVG* value);
	void PutRenderQueue(const wxString& filename);
	void RenderQueue();
	static wxCairoSVGCache* m_instance;

protected:
	wxThread::ExitCode Entry();

private:
	bool stopThread;
	bool stopped;
	wxCairoSVGHash m_hash;
	wxArrayString m_renderList;
	wxMutex m_mutex;

	wxCairoSVGCache();
	~wxCairoSVGCache();
};

#endif /* WXCAIROSVG_H_ */
