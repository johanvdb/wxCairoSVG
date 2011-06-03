#include "wx/wx.h"
#include "wx/svg/cairosvg.h"

class MyApp: public wxApp {
	public:
	static wxString resourcePath;

	virtual bool OnInit();
	virtual int OnExit();
	void OnMenuExit(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

wxString MyApp::resourcePath;

class MyFrame: public wxFrame {

public:
	MyFrame(wxString file, wxWindow *parent,
			wxWindowID id,
			const wxString& title,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxDEFAULT_FRAME_STYLE,
			const wxString& name = wxFrameNameStr): wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE | wxWANTS_CHARS, name) {
		m_svg.LoadFile(file);
		m_scale = 1;
		Show(true);
	}

	void OnChar(wxKeyEvent& event) {
		if (event.GetKeyCode() == WXK_UP) {
			m_scale += 0.5;
			Refresh(true);
		}
		if (event.GetKeyCode() == WXK_DOWN) {
			m_scale -= 0.5;
			Refresh(true);
		}
		if (event.GetKeyCode() == WXK_SPACE) {
			m_scale = 1;
		}
	}

	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnSize(wxSizeEvent& event);

private:
	wxCairoSVG m_svg;
	double m_scale;
	DECLARE_EVENT_TABLE()
};

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(MyApp, wxApp)
	EVT_MENU(wxID_EXIT, MyApp::OnMenuExit)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_PAINT(MyFrame::OnPaint)
	EVT_SIZE(MyFrame::OnSize)
	EVT_CHAR(MyFrame::OnChar)
END_EVENT_TABLE()

bool MyApp::OnInit() {
	if (argc < 2) {
		resourcePath = wxPathOnly(argv[0]);
		if (!resourcePath.IsEmpty()) resourcePath.Append(wxFILE_SEP_PATH);
		#ifdef __WXMAC__
			resourcePath.Append(wxString(wxT("..")) + wxFILE_SEP_PATH + wxString(wxT("Resources")));
			resourcePath.Append(wxFILE_SEP_PATH);
		#endif
		wxString file = MyApp::resourcePath +_T("images") + wxFILE_SEP_PATH + _T("chips") + wxFILE_SEP_PATH + _T("chip025.svg");
		new MyFrame(file, NULL, wxID_ANY, _T("Cairo SVG Test"));
	} else {
		for (int i = 1; i < argc; i++) {
			new MyFrame(argv[i], NULL, wxID_ANY, argv[i]);
		}
	}
	return true;
}

int MyApp::OnExit() {
}

void MyApp::OnMenuExit(wxCommandEvent& event) {
	ExitMainLoop();
}

void MyFrame::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);

	wxRect rect = GetClientRect();

	if(rect.width == 0 || rect.height == 0)
	{
		return;
	}

	double scale = 0.5;
	int x = 5;

	dc.SetBrush(*wxWHITE_BRUSH);
	dc.SetPen(*wxWHITE_PEN);
	dc.DrawRectangle(rect);

	wxImage* img = m_svg.GetImage(m_scale);
	if (img != NULL) {
		if (img->IsOk()) {
			wxBitmap bmp = wxBitmap(*img, 32);
			dc.SetPen(*wxGREEN_PEN);
			dc.DrawRectangle(wxRect(x-1, 4, img->GetWidth() + 2, img->GetHeight() + 2));
			dc.DrawBitmap(bmp, x, 5);
			x += img->GetWidth() + 5;
		}
		delete img;
	}


//	for (int i = 0; i < 3; i++) {
//		wxImage* img = m_svg.GetImage(scale);
//		if (img != NULL) {
//			if (img->IsOk()) {
//				wxBitmap bmp = wxBitmap(*img, 32);
//				dc.DrawBitmap(bmp, x, 5);
//				x += img->GetWidth() + 5;
//			}
//			delete img;
//		}
//		scale = scale * 2.0;
//	}

//	{
//		wxImage* img = m_svg.GetImage((double) rect.width / 800.0 * m_scale);
//		if (img != NULL) {
//			if (img->IsOk()) {
//				int y = 5;
//				while (y < rect.height) {
//					x = 5;
//					while (x < rect.width) {
//						wxBitmap bmp = wxBitmap(*img, 32);
//						dc.DrawBitmap(bmp, x, y);
//						x += img->GetWidth() + 5;
//					}
//					y = y + img->GetHeight() + 5;
//				}
//			}
//			delete img;
//		}
//	}
}

void MyFrame::OnSize(wxSizeEvent& event) {
	Refresh(true);
}

