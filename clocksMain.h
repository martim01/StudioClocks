/***************************************************************
 * Name:      clocksMain.h
 * Purpose:   Defines Application Frame
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2021-05-14
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef CLOCKSMAIN_H
#define CLOCKSMAIN_H

//(*Headers(clocksFrame)
#include <wx/frame.h>
#include <wx/gbsizer.h>
//*)
#include "clock.h"
#include <wx/xml/xml.h>
#include <wx/filename.h>
class clocksFrame: public wxFrame
{
    public:

        clocksFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~clocksFrame();

        void LoadClocks(const wxFileName& fn);
        void LoadClock(wxXmlNode* pClockNode);
        void AddClockToSizer(AnalogueClock* pClock, wxXmlNode* pPositionNode);
        void SetClockColours(AnalogueClock* pClock, wxXmlNode* pColoursNode);
        void SetClockOptions(AnalogueClock* pClock, wxXmlNode* pOptionsNode);
        wxColour ExtractColour(const wxString& sColour);
    private:

        //(*Handlers(clocksFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(clocksFrame)
        //*)

        //(*Declarations(clocksFrame)
        wxGridBagSizer* m_pSizer;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // CLOCKSMAIN_H
