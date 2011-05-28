#ifndef WXCAIROSVGCOLOR_H_
#define WXCAIROSVGCOLOR_H_

#include "paintserver.h"

class wxCairoSVGColor: public wxCairoSVGPaintServer {
public:
	wxCairoSVGColor(wxCairoSVGContext* context);
	virtual ~wxCairoSVGColor();
	virtual void Init();
	virtual void SetColor(const wxString& value);
	virtual void SetColor(int r, int g, int b);
	static unsigned char ParseHex(wxChar c);
	double GetRed();
	double GetGreen();
	double GetBlue();
	static wxCairoSVGColor* FromString(wxCairoSVGContext* context, const wxString& value);
	static bool FromString(const wxString& value, int *r_out, int *g_out, int *b_out);
	bool IsOk();

protected:
	virtual void SetSource(cairo_t* cairo_image);

private:
	int m_r;
	int m_g;
	int m_b;

	bool m_ok;

	DECLARE_ABSTRACT_CLASS(wxCairoSVGColor)
};

#endif /* WXCAIROSVGCOLOR_H_ */
