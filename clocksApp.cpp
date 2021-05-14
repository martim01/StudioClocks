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

IMPLEMENT_APP(clocksApp);

bool clocksApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	clocksFrame* Frame = new clocksFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
