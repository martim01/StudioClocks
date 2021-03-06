/***************************************************************
 * Name:      clocksApp.h
 * Purpose:   Defines Application Class
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2021-05-14
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef CLOCKSAPP_H
#define CLOCKSAPP_H

#include <wx/app.h>

class clocksApp : public wxApp
{
    public:
        virtual bool OnInit();
        void OnInitCmdLine(wxCmdLineParser& parser);
        bool OnCmdLineParsed(wxCmdLineParser& parser);

    private:
        wxString m_sPath;
};

#endif // CLOCKSAPP_H
