/*
 * gradientstop.h
 *
 *  Created on: Apr 17, 2011
 *      Author: johanvdb
 */

#ifndef GRADIENTSTOP_H_
#define GRADIENTSTOP_H_

#include "wx/svg/cairo/paintservers/color.h"
#include "wx/list.h"

class wxCairoSVGGradientStop {
public:
	wxCairoSVGGradientStop();
	virtual ~wxCairoSVGGradientStop();
	virtual void LoadXML(wxXmlNode* node);

	double m_offset;
	int m_r;
	int m_g;
	int m_b;
	double m_opacity;
};

WX_DECLARE_LIST(wxCairoSVGGradientStop, wxCairoSVGGradientStopList);

#endif /* GRADIENTSTOP_H_ */
