/***************************************************************
 * Name:      clocksApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2021-05-14
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "clocksApp.h"

//(*AppHeaders
#include "clocksMain.h"
#include <wx/image.h>
//*)
#include <wx/cmdline.h>

IMPLEMENT_APP(clocksApp);

void clocksApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetLogo(wxT("Clocks"));
	parser.AddParam(wxT("Path"));

}

bool clocksApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	m_sPath = parser.GetParam(0);

	return true;
}


bool clocksApp::OnInit()
{
    if(!wxApp::OnInit())
		return false;

    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	clocksFrame* Frame = new clocksFrame(0, m_sPath);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    return wxsOK;

}
