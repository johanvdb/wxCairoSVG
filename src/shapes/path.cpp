#include "wx/svg/cairo/shapes/path.h"
#include "wx/svg/cairo/paintservers/paintserver.h"
#include "wx/svg/cairo/shapes/util.h"
#include "wx/log.h"
#include "wx/tokenzr.h"

IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGPathCommand, wxObject)
IMPLEMENT_ABSTRACT_CLASS(wxCairoSVGPath, wxCairoSVGNode)

#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxCairoSVGPathCommandList);

wxCairoSVGPath::wxCairoSVGPath(wxCairoSVGContext* context): wxCairoSVGNode(context) {
	m_commands.DeleteContents(true);
}

wxCairoSVGPath::~wxCairoSVGPath() {
}

void wxCairoSVGPath::LoadXML(wxXmlNode* node, wxCairoSVGNode* parent) {

	wxCairoSVGNode::LoadXML(node, parent);

	wxString data = node->GetPropVal(_T("d"), _T(""));

	int i = 0;
	while (i < data.length()) {
		wxCairoSVGPathCommand* cmd = new wxCairoSVGPathCommand();
		cmd->m_cmd = data[i++];
		ReadParams(data, i, cmd->m_params);
		m_commands.Append(cmd);
	}
}

void wxCairoSVGPath::Render(cairo_t* cairo_image, const wxPoint& pos) {
	if (m_commands.size() == 0) return;

	wxCairoSVGPaintServer* fillServer = wxCairoSVGPaintServer::FromString(m_context, m_fill);
	wxCairoSVGPaintServer* strokeServer = wxCairoSVGPaintServer::FromString(m_context, m_stroke);

	if (fillServer == NULL && strokeServer == NULL) return;

	wxRealPoint rel(0, 0);
	wxRealPoint control2(-1, -1);
	wxString prevCmd = _T("");

	wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render path step count %d"), m_commands.size());

	for (int i = 0; i < m_commands.size(); i++) {
		wxCairoSVGPathCommand* cmd = m_commands[i];
		if (cmd->m_cmd == _T("M")) {
			for (int j = 0; j < cmd->m_params.size() - 1; j += 2) {
				cairo_move_to(cairo_image, cmd->m_params[j], cmd->m_params[j+1]);
				rel = wxRealPoint(cmd->m_params[j], cmd->m_params[j+1]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render move absolute to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("m")) {
			for (int j = 0; j < cmd->m_params.size() - 1; j += 2) {
				cairo_rel_move_to(cairo_image, cmd->m_params[j], cmd->m_params[j+1]);
				rel = wxRealPoint(rel.x + cmd->m_params[j], rel.y + cmd->m_params[j+1]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render move relative to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("L")) {
			for (int j = 0; j < cmd->m_params.size() - 1; j += 2) {
				cairo_line_to(cairo_image, cmd->m_params[j], cmd->m_params[j+1]);
				rel = wxRealPoint(cmd->m_params[j], cmd->m_params[j+1]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render line absolute to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("l")) {
			for (int j = 0; j < cmd->m_params.size() - 1; j += 2) {
				cairo_rel_line_to(cairo_image, cmd->m_params[j], cmd->m_params[j+1]);
				rel = wxRealPoint(rel.x + cmd->m_params[j], rel.y + cmd->m_params[j+1]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render line relative to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("H")) {
			for (int j = 0; j < cmd->m_params.size(); j++) {
				cairo_line_to(cairo_image, cmd->m_params[j], rel.y);
				rel = wxRealPoint(cmd->m_params[j], rel.y);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render hline absolute to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("h")) {
			for (int j = 0; j < cmd->m_params.size(); j++) {
				cairo_line_to(cairo_image, rel.x + cmd->m_params[j], rel.y);
				rel = wxRealPoint(rel.x + cmd->m_params[j], rel.y);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render hline relative to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("V")) {
			for (int j = 0; j < cmd->m_params.size(); j++) {
				cairo_line_to(cairo_image, rel.x, cmd->m_params[j]);
				rel = wxRealPoint(rel.x, cmd->m_params[j]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render vline absolute to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("v")) {
			for (int j = 0; j < cmd->m_params.size(); j++) {
				cairo_line_to(cairo_image, rel.x, rel.y + cmd->m_params[j]);
				rel = wxRealPoint(rel.x, rel.y + cmd->m_params[j]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render vline relative to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("C")) {
			for (int j = 0; j < cmd->m_params.size() - 1; j += 6) {
				cairo_curve_to(cairo_image, cmd->m_params[j], cmd->m_params[j+1],
						cmd->m_params[j+2], cmd->m_params[j+3],
						cmd->m_params[j+4], cmd->m_params[j+5]);
				rel = wxRealPoint(cmd->m_params[j+4], cmd->m_params[j+5]);
				control2 = wxRealPoint(cmd->m_params[j+2], cmd->m_params[j+3]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render curve absolute to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("c")) {
			for (int j = 0; j < cmd->m_params.size() - 1; j += 6) {
				cairo_rel_curve_to(cairo_image, cmd->m_params[j], cmd->m_params[j+1],
						cmd->m_params[j+2], cmd->m_params[j+3],
						cmd->m_params[j+4], cmd->m_params[j+5]);
				control2 = wxRealPoint(cmd->m_params[j+2] + rel.x, cmd->m_params[j+3] + rel.y);
				rel = wxRealPoint(rel.x + cmd->m_params[j+4], rel.y + cmd->m_params[j+5]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render curve relative to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("S")) {
			for (int j = 0; j < cmd->m_params.size() - 1; j += 6) {
				cairo_curve_to(cairo_image, rel.x + rel.x - control2.x, rel.y + rel.y - control2.y,
						cmd->m_params[j], cmd->m_params[j+1],
						cmd->m_params[j+2], cmd->m_params[j+3]);
				rel = wxRealPoint(cmd->m_params[j+2], cmd->m_params[j+3]);
				control2 = wxRealPoint(cmd->m_params[j], cmd->m_params[j+1]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render shorthand absolute to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd == _T("s")) {
			for (int j = 0; j < cmd->m_params.size() - 1; j += 6) {
				cairo_rel_curve_to(cairo_image, rel.x - control2.x, rel.y - control2.y,
						cmd->m_params[j], cmd->m_params[j+1],
						cmd->m_params[j+2], cmd->m_params[j+3]);
				control2 = wxRealPoint(cmd->m_params[j] + rel.x, cmd->m_params[j+1] + rel.y);
				rel = wxRealPoint(rel.x + cmd->m_params[j+2], rel.y + cmd->m_params[j+3]);
				wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render shorthand relative to %f x %f"), rel.x, rel.y);
			}
		}
		else if (cmd->m_cmd.Lower() == _T("z")) {
			cairo_close_path(cairo_image);
			wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render close at %f x %f"), rel.x, rel.y);
		}
		else {
			wxLogTrace(_T("svg"), _T("wxCairoSVGPath::Render unknown command ") + cmd->m_cmd);
		}
	}
	if (fillServer) fillServer->Fill(cairo_image, (strokeServer != NULL));
	if (strokeServer) strokeServer->Stroke(cairo_image, m_strokeWidth, false);
	wxCairoSVGNode::Render(cairo_image, pos);
}

/**
 * M23.141,206.847h10.683c16.087-52.68,33.246-108.906,49.06-160.687
 * h15.159c16.155,52.893,32.626,106.701,48.853,159.826l11.094,0.104v17.951c-17.814,0-35.729-0.312-53.917-0.312v-17.743h24.287
 * l-11.612-37.966H64.211l-11.854,38.827h21.364v17.746H23.141V206.847L23.141,206.847z M90.496,82.026L69.618,150.28h41.722
 * L90.496,82.026z
 */

bool wxCairoSVGPath::ReadParam(const wxString& data, int& i, wxArrayDouble& m_params) {

	if (i >= data.Length()) return false;

	wxString tmp = _T("");
	bool seenDot = false;
	bool result = true;
	bool done = false;

	while (!done) {
		if ((data[i] >= '0') && (data[i] <= '9')) {
			tmp += data[i];
			i++;
		} else if ((data[i] == '+') && (tmp.Length() == 0)) {
			tmp += data[i];
			i++;
		} else if ((data[i] == '-') && (tmp.Length() == 0)) {
			tmp += data[i];
			i++;
		} else if ((data[i] == '.') && (tmp.Length() > 0)) {
			tmp += data[i];
			i++;
		} else {
			while (data[i] == ',') i++;
			while (data[i] == ' ') i++;
			done = true;
		}
	}

	double param = 0;
	if (tmp.Length() == 0) return false;
	tmp.ToDouble(&param);
	m_params.Add(param);
	return true;
}

void wxCairoSVGPath::ReadParams(const wxString& data, int& i, wxArrayDouble& m_params) {
	while (ReadParam(data, i, m_params));
}
