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
#include <wx/menu.h>
//*)
#include "clock.h"
#include <wx/xml/xml.h>
#include <wx/filename.h>
#include <wx/cursor.h>


class clocksFrame: public wxFrame
{
    public:

        clocksFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~clocksFrame();

        void LoadClocks(const wxFileName& fn);
        void LoadClock(wxXmlNode* pClockNode);
        void AddClockToSizer(StudioClock* pClock, wxXmlNode* pPositionNode);
        void SetClockColours(StudioClock* pClock, wxXmlNode* pColoursNode);
        void SetClockOptions(StudioClock* pClock, wxXmlNode* pOptionsNode);
        void SetClockTimeZone(StudioClock* pClock, wxXmlNode* pTimeZoneNode);
        void SetupGrid(wxXmlNode* pGridNode);
        wxColour ExtractColour(const wxString& sColour);
    private:

        //(*Handlers(clocksFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnRightUp(wxMouseEvent& event);
        //*)

        //(*Identifiers(clocksFrame)
        static const long ID_MENUITEM_QUIT;
        //*)

        //(*Declarations(clocksFrame)
        wxGridBagSizer* m_pSizer;
        wxMenu m_menuPopup;
        wxMenuItem* m_pmiQuit;
        //*)

        wxCursor* m_pCursor;
        DECLARE_EVENT_TABLE()
};

#endif // CLOCKSMAIN_H
