/*
 * cairosvg.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: johanvdb
 */

#include "wx/svg/cairo/shapes/rect.h"
#include "wx/svg/cairo/shapes/polyline.h"
#include "wx/svg/cairo/shapes/circle.h"
#include "wx/svg/cairo/shapes/ellipse.h"
#include "wx/svg/cairo/shapes/path.h"
#include "wx/svg/cairo/shapes/polygon.h"
#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/svg/cairo/paintservers/lineargradient.h"
#include "wx/svg/cairo/paintservers/radialgradient.h"

#include "wx/defs.h"
#include "wx/log.h"
#include "wx/utils.h"
#include "wx/stopwatch.h"

#include "wx/svg/cairosvg.h"

IMPLEMENT_DYNAMIC_CLASS(wxCairoSVG, wxObject);

wxCairoSVG::wxCairoSVG() {
	Init();
}

wxCairoSVG::wxCairoSVG(const wxString& filename) {
	Init();
	LoadFile(filename);
}

wxCairoSVG::wxCairoSVG(wxInputStream& instream) {
	Init();
	LoadFile(instream);
}

wxCairoSVG::~wxCairoSVG() {
	for (wxCairoSVGImageHash::iterator i = m_image_cache_hash.begin(); i != m_image_cache_hash.end(); i++) {
		delete i->second;
	}
}

bool wxCairoSVG::LoadFile(const wxString& filename) {
	wxXmlDocument xml(filename);
	m_name = filename;
	wxLogVerbose(_T("wxCairoSVG::LoadFile ") + filename);
	return LoadXML(xml);
}

bool wxCairoSVG::LoadFile(wxInputStream& instream) {
	wxXmlDocument xml(instream);
	return LoadXML(xml);
}

bool wxCairoSVG::LoadXML(const wxXmlDocument& xml) {
	if (!xml.IsOk()) return false;
	double w, h;
	xml.GetRoot()->GetPropVal(_T("width"), _T("10")).ToDouble(&w);
	xml.GetRoot()->GetPropVal(_T("height"), _T("10")).ToDouble(&h);
	m_width = round(w+0.5);
	m_height = round(h+0.5);
	m_context.m_rootNode = CreateNode(1, xml.GetRoot(), NULL);
}

void wxCairoSVG::Render(cairo_t* cairo_image, const wxPoint& pos) {
	if (IsOk()) {
		if (m_context.m_rootNode->GetOpacity() != 1) {
			cairo_push_group(cairo_image);
		}
		m_context.m_rootNode->Render(cairo_image, pos);
		if (m_context.m_rootNode->GetOpacity() != 1) {
			cairo_pop_group_to_source(cairo_image);
			cairo_paint_with_alpha(cairo_image, m_context.m_rootNode->GetOpacity());
		}
	}
}

wxImage* wxCairoSVG::GetImage(double scale) {

	if (!IsOk()) {
		wxLogError(_T("wxCairoSVG::GetImage Image nog OK but GetImage called %s"), m_name.c_str());
		return NULL;
	}
	if (scale == 0) {
		wxLogError(_T("wxCairoSVG::GetImage called with 0 scale %s"), m_name.c_str());
		return NULL;
	}

	wxCairoSVGImageHash::iterator i = m_image_cache_hash.find((int) (scale * 100.0));
	if (i != m_image_cache_hash.end()) {
		return new wxImage(*(i->second));
	}

	wxStopWatch stopWatch;

	wxRect rect(0, 0, (int) ((double) m_width * scale), (int) ((double) m_height * scale));

	// Allocate a buffer large enough to store the image data.
	unsigned int image_buffer_len = rect.width * rect.height * 4;
	unsigned char* image_buffer = (unsigned char*)malloc(image_buffer_len);
	memset(image_buffer, 0, image_buffer_len);

	cairo_surface_t* cairo_surface = cairo_image_surface_create_for_data(
								image_buffer,
								CAIRO_FORMAT_ARGB32,
								rect.width,
								rect.height,
								rect.width * 4);
	cairo_t* cairo_image = cairo_create(cairo_surface);

	// Draw SVG
	{
		cairo_set_source_rgba(cairo_image, 0, 0, 0, 0);
		cairo_paint(cairo_image);
		cairo_scale(cairo_image, scale, scale);
		Render(cairo_image);
	}

	// Now translate the raw image data from the format stored
	// by cairo into a format understood by wxImage.
	unsigned char* output = (unsigned char*)malloc(image_buffer_len);
	unsigned char* alpha = (unsigned char*)malloc(rect.width * rect.height);
	int offset = 0;
	int aoffset = 0;

	for(size_t count = 0; count < image_buffer_len; count+=4) {
		int i = (wxBYTE_ORDER == wxBIG_ENDIAN)? 0: 3;
		int d = (wxBYTE_ORDER == wxBIG_ENDIAN)? 1:-1;

		int a = *(image_buffer+count+i+(d*0));
		int r = *(image_buffer+count+i+(d*1));
		int g = *(image_buffer+count+i+(d*2));
		int b = *(image_buffer+count+i+(d*3));

		*(alpha + aoffset) = a;
		aoffset++;
		*(output + offset) = r;
		offset++;
		*(output + offset) = g;
		offset++;
		*(output + offset) = b;
		offset++;

	}

	// Create a wxImage from the buffer and then a wxBitmap from that
	wxImage* img = new wxImage(rect.width, rect.height, output, alpha, false);
	if ((img == NULL) || (!img->IsOk())) wxLogError(_T("wxCairoSVG::GetImage new image is not ok. %d %d %ld %ld"),
			rect.width, rect.height, output, alpha);

	cairo_destroy(cairo_image);
	cairo_surface_destroy(cairo_surface);
	free(image_buffer);

	wxLogVerbose(_T("wxCairoSVG::GetImage cache not valid. Generated new image %s scale %f in %ldms"), m_name.c_str(), scale, stopWatch.Time());

	m_image_cache_hash[(int) (scale * 100.0)] = img;

	return new wxImage(*img);
}

bool wxCairoSVG::IsOk() {
	return m_context.m_rootNode != NULL;
}

/* Private methods begin here */

void wxCairoSVG::Init() {
	m_context.m_rootNode = NULL;
	m_name = _T("unnamed");
}

wxCairoSVGNode* wxCairoSVG::CreateNode(int level, wxXmlNode* node, wxCairoSVGNode* parent) {

	wxCairoSVGNode* newNode = NULL;

	wxString tree = _T("");
	for (int i = 0; i < level; i++) tree += _T("-");

//	if (node->GetType() == wxXML_TEXT_NODE) newNode = parent;
//	else
	if (node->GetName() == _T("rect")) newNode = new wxCairoSVGRect(&m_context);
	else if (node->GetName() == _T("polyline")) newNode = new wxCairoSVGPolyLine(&m_context);
	else if (node->GetName() == _T("polygon")) newNode = new wxCairoSVGPolygon(&m_context);
	else if (node->GetName() == _T("circle")) newNode = new wxCairoSVGCircle(&m_context);
	else if (node->GetName() == _T("ellipse")) newNode = new wxCairoSVGEllipse(&m_context);
	else if (node->GetName() == _T("path")) newNode = new wxCairoSVGPath(&m_context);
	else if (node->GetName() == _T("linearGradient")) newNode = new wxCairoSVGLinearGradient(&m_context);
	else if (node->GetName() == _T("radialGradient")) newNode = new wxCairoSVGRadialGradient(&m_context);
	else newNode = new wxCairoSVGNode(&m_context);

	newNode->LoadXML(node, parent);

	m_context.Index();

	wxLogTrace(_T("svg"), tree + _T(" ") + node->GetName() + _T(" ") + newNode->GetClassInfo()->GetClassName()
			+ _T(" ") + newNode->GetId());

	wxXmlNode *child = node->GetChildren();
	while (child) {
		newNode->AddChild(CreateNode(level + 1, child, newNode));
		child = child->GetNext();
	}

	return newNode;
}

wxCairoSVGCache::wxCairoSVGCache(): wxThread(wxTHREAD_JOINABLE) {
	stopped = false;
	stopThread = false;
}

wxCairoSVGCache::~wxCairoSVGCache() {
	for (wxCairoSVGHash::iterator i = m_hash.begin(); i != m_hash.end(); ++i) {
		delete i->second;
	}
}

void wxCairoSVGCache::Init() {
	wxASSERT(m_instance == NULL);
	m_instance = new wxCairoSVGCache();
}

void wxCairoSVGCache::Destroy() {
	if (m_instance != NULL) {
		wxLogVerbose(_T("wxCairoSVGCache::Delete Waiting for thread to stop"));
		m_instance->stopThread = true;

		for (int i = 0; i < 20; i++) {
			if (!m_instance->stopped) {
				wxSleep(1);
			}
		}

		if (!m_instance->stopped) {
			wxLogVerbose(_T("wxCairoSVGCache::Delete Stop timed out. Killing"));
			m_instance->Kill();
		} else {
			wxLogVerbose(_T("wxCairoSVGCache::Delete Thread stopped, freeing nicely"));
			m_instance->Wait();
		}

		delete m_instance;
		m_instance = NULL;
	}
}

wxCairoSVG* wxCairoSVGCache::Get(const wxString& key) {
	wxMutexLocker lock(m_mutex);
	wxCairoSVGHash::iterator i = m_hash.find(key);
	if (i == m_hash.end()) return NULL;
	return i->second;
}

wxCairoSVG* wxCairoSVGCache::GetOrCreate(const wxString& filename) {
	wxCairoSVG* o = Get(filename);
	if (o == NULL) {
		o = new wxCairoSVG(filename);
		if (!Put(filename, o)) {
			delete o;
			o = Get(filename);
		}
	}
	return o;
}

bool wxCairoSVGCache::Put(const wxString& key, wxCairoSVG* value) {
	wxMutexLocker lock(m_mutex);
	wxCairoSVGHash::iterator i = m_hash.find(key);
	if (i == m_hash.end()) {
		m_hash[key] = value;
		return true;
	}
	return false;
}

void wxCairoSVGCache::PutRenderQueue(const wxString& filename) {
	m_renderList.Add(filename);
}

void wxCairoSVGCache::RenderQueue() {
	Create();
	Run();
}

wxThread::ExitCode wxCairoSVGCache::Entry() {
	wxLogVerbose(_T("wxCairoSVGCache render thread running"));
	for (int i = 0; i < m_renderList.size(); i++) {
		if (!stopThread) wxCairoSVGCache::m_instance->GetOrCreate(m_renderList[i]);
	}
	m_renderList.clear();
	stopped = true;
	wxLogVerbose(_T("wxCairoSVGCache render thread stopped"));
	return 0;
}

wxCairoSVGCache* wxCairoSVGCache::m_instance;
