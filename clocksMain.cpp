/***************************************************************
 * Name:      clocksMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2021-05-14
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#include "clocksMain.h"
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
//(*InternalHeaders(clocksFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(clocksFrame)
const long clocksFrame::ID_MENUITEM_QUIT = wxNewId();
//*)

BEGIN_EVENT_TABLE(clocksFrame,wxFrame)
    //(*EventTable(clocksFrame)
    //*)
END_EVENT_TABLE()

clocksFrame::clocksFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(clocksFrame)
    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER|wxNO_BORDER, _T("id"));
    SetBackgroundColour(wxColour(0,0,0));
    m_pSizer = new wxGridBagSizer(0, 0);
    SetSizer(m_pSizer);
    m_pmiQuit = new wxMenuItem((&m_menuPopup), ID_MENUITEM_QUIT, _("Quit"), wxEmptyString, wxITEM_NORMAL);
    m_menuPopup.Append(m_pmiQuit);
    m_pSizer->Fit(this);
    m_pSizer->SetSizeHints(this);

    Connect(ID_MENUITEM_QUIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&clocksFrame::OnQuit);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&clocksFrame::OnClose);
    Connect(wxEVT_RIGHT_UP,(wxObjectEventFunction)&clocksFrame::OnRightUp);
    //*)



    #if defined(__WXGTK__)
    wxSetCursor(wxCURSOR_BLANK);
    #elif defined(__WXMSW__)
    ShowCursor(0);
    #endif // defined


    wxFileName fn(".", "clocks.xml");
    LoadClocks(fn);

    SetSize(1920,1080);
}

clocksFrame::~clocksFrame()
{
    wxLogDebug("~");
    //(*Destroy(clocksFrame)
    //*)
}

void clocksFrame::LoadClocks(const wxFileName& fn)
{
    wxXmlDocument xml;
    if(xml.Load(fn.GetFullPath()))
    {
        for(wxXmlNode* pGridNode = xml.GetRoot()->GetChildren(); pGridNode; pGridNode = pGridNode->GetNext())
        {
            if(pGridNode->GetName().CmpNoCase("layout") == 0)
            {
                SetupGrid(pGridNode);
                break;
            }
        }

        for(wxXmlNode* pClockNode = xml.GetRoot()->GetChildren(); pClockNode; pClockNode = pClockNode->GetNext())
        {
            if(pClockNode->GetName().CmpNoCase("clock") == 0)
            {
                LoadClock(pClockNode);
            }
        }
    }
    else
    {
        wxMessageBox("Could not load clocks");
    }
}

void clocksFrame::LoadClock(wxXmlNode* pClockNode)
{
    StudioClock* pClock = new StudioClock(this,wxID_ANY,wxDefaultPosition,wxDefaultSize);

    for(wxXmlNode* pNode = pClockNode->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        if(pNode->GetName().CmpNoCase("position") == 0)
        {
            AddClockToSizer(pClock, pNode);
        }
        else if(pNode->GetName().CmpNoCase("type") == 0)
        {
            if(pNode->GetNodeContent().CmpNoCase("studio") == 0)
            {
                pClock->SetClockMode(StudioClock::STUDIO);
            }
            else
            {
                pClock->SetClockMode(StudioClock::ANALOGUE);
            }
        }
        else if(pNode->GetName().CmpNoCase("colours") == 0)
        {
            SetClockColours(pClock, pNode);
        }
        else if(pNode->GetName().CmpNoCase("timezone") == 0)
        {
            SetClockTimeZone(pClock, pNode);
        }
        else if(pNode->GetName().CmpNoCase("options") == 0)
        {
            SetClockOptions(pClock, pNode);
        }
    }
}

void clocksFrame::SetClockTimeZone(StudioClock* pClock, wxXmlNode* pTimeZoneNode)
{
    wxString sLabel;
    unsigned long nHours(0);
    unsigned long nMinutes(0);
    bool bUtc(false);

    for(wxXmlNode* pNode = pTimeZoneNode->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        if(pNode->GetName().CmpNoCase("label") == 0)
        {
            sLabel = pNode->GetNodeContent();
        }
        else if(pNode->GetName().CmpNoCase("offset") == 0)
        {
            wxLogDebug(pNode->GetNodeContent());

            pNode->GetNodeContent().BeforeFirst(':').ToULong(&nHours);
            pNode->GetNodeContent().AfterFirst(':').ToULong(&nMinutes);
        }
        else if(pNode->GetName().CmpNoCase("utc") == 0)
        {
            bUtc = (pNode->GetNodeContent().CmpNoCase("true") == 0);
        }
    }

    pClock->SetTimezone(sLabel, wxTimeSpan(nHours, nMinutes), bUtc);

}
void clocksFrame::SetupGrid(wxXmlNode* pGridNode)
{
    for(wxXmlNode* pNode = pGridNode->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        unsigned long nTemp;
        if(pNode->GetName().CmpNoCase("rows") == 0 && pNode->GetNodeContent().ToULong(&nTemp))
        {
            m_pSizer->SetRows(nTemp);
        }
        if(pNode->GetName().CmpNoCase("cols") == 0 && pNode->GetNodeContent().ToULong(&nTemp))
        {
            m_pSizer->SetCols(nTemp);
        }
    }
    wxLogDebug("Grid: rows=%d cols=%d", m_pSizer->GetRows(), m_pSizer->GetCols());

    for(size_t i = 0; i < m_pSizer->GetCols(); ++i)
    {
        m_pSizer->AddGrowableCol(i);
    }
    for(size_t i = 0; i < m_pSizer->GetRows(); ++i)
    {
        m_pSizer->AddGrowableRow(i);
    }
}

void clocksFrame::AddClockToSizer(StudioClock* pClock, wxXmlNode* pPositionNode)
{
    wxGBPosition pos(0,0);
    wxGBSpan span(1,1);
    for(wxXmlNode* pNode = pPositionNode->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        if(pNode->GetName().CmpNoCase("row") == 0)
        {
            unsigned long nTemp;
            if(pNode->GetNodeContent().ToULong(&nTemp))
            {
                pos.SetRow(nTemp);
            }
        }
        else if(pNode->GetName().CmpNoCase("col") == 0)
        {
            unsigned long nTemp;
            if(pNode->GetNodeContent().ToULong(&nTemp))
            {
                pos.SetCol(nTemp);
            }
        }
        else if(pNode->GetName().CmpNoCase("rows") == 0)
        {
            unsigned long nTemp;
            if(pNode->GetNodeContent().ToULong(&nTemp))
            {
                span.SetRowspan(nTemp);
            }
        }
        else if(pNode->GetName().CmpNoCase("cols") == 0)
        {
            unsigned long nTemp;
            if(pNode->GetNodeContent().ToULong(&nTemp))
            {
                span.SetColspan(nTemp);
            }
        }
    }
    wxLogDebug("Add clock %d,%d,%d,%d", pos.GetCol(), pos.GetRow(), span.GetColspan(), span.GetRowspan());
    m_pSizer->Add(pClock, pos, span, wxALL|wxEXPAND, 2);

    pClock->Connect(wxEVT_RIGHT_UP,(wxObjectEventFunction)&clocksFrame::OnRightUp,0,this);
}

void clocksFrame::SetClockColours(StudioClock* pClock, wxXmlNode* pColoursNode)
{
    for(wxXmlNode* pNode = pColoursNode->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        wxColour clr = ExtractColour(pNode->GetNodeContent());
        if(clr.IsOk())
        {
            if(pNode->GetName().CmpNoCase("text") == 0)
            {
                pClock->SetTextColour(clr);
            }
            else if(pNode->GetName().CmpNoCase("sextant") == 0)
            {
                unsigned long nSextant;
                if(pNode->GetAttribute("pos").ToULong(&nSextant) && nSextant < 6)
                {
                    pClock->SetStudioColour(nSextant, clr);
                }
            }
            else if(pNode->GetName().CmpNoCase("background") == 0)
            {
                pClock->SetAnalogueColourBack(clr);
            }
            else if(pNode->GetName().CmpNoCase("foreground") == 0)
            {
                pClock->SetAnalogueColourFront(clr);
            }
            else if(pNode->GetName().CmpNoCase("hands") == 0)
            {
                pClock->SetAnalogueColourHands(clr);
            }
            else if(pNode->GetName().CmpNoCase("secondhand") == 0)
            {
                pClock->SetAnalogueColourSecondHand(clr);
            }
            else if(pNode->GetName().CmpNoCase("hour") == 0)
            {
                pClock->SetAnalogueHourColour(clr);
            }
            else if(pNode->GetName().CmpNoCase("hour24") == 0)
            {
                pClock->SetAnalogueHour24Colour(clr);
            }
        }
    }
}

wxColour clocksFrame::ExtractColour(const wxString& sColour)
{
    wxArrayString as(wxStringTokenize(sColour, ","));

    if(as.GetCount() == 3)
    {
        unsigned long nRed, nGreen, nBlue, nMax;
        nMax = 255;

        if(as[0].ToULong(&nRed) && as[1].ToULong(&nGreen) && as[2].ToULong(&nBlue))
        {
            return wxColour(std::min(nRed, nMax),std::min(nGreen, nMax), std::min(nBlue, nMax));
        }
        else
        {
            return wxColour();
        }
    }

    return wxColour(sColour);
}


void clocksFrame::SetClockOptions(StudioClock* pClock, wxXmlNode* pOptionsNode)
{
    for(wxXmlNode* pNode = pOptionsNode->GetChildren(); pNode; pNode = pNode->GetNext())
    {
        if(pNode->GetName().CmpNoCase("refresh") == 0)
        {
            if(pNode->GetNodeContent().CmpNoCase("sweep") == 0)
            {
                pClock->SetRefreshType(StudioClock::SWEEP);
            }
            else if(pNode->GetNodeContent().CmpNoCase("tick") == 0)
            {
                pClock->SetRefreshType(StudioClock::TICK);
            }
        }
        else if(pNode->GetName().CmpNoCase("hour") == 0)
        {
            if(pNode->GetNodeContent() == "12")
            {
                pClock->ShowHours(true, false);
            }
            else if(pNode->GetNodeContent() == "24")
            {
                pClock->ShowHours(true, true);
            }
            else
            {
                pClock->ShowHours(false, false);
            }
        }
        else if(pNode->GetName().CmpNoCase("timezone") == 0)
        {
            pClock->ShowTimezone(pNode->GetNodeContent().CmpNoCase("true") == 0);
        }
        else if(pNode->GetName().CmpNoCase("seconds") == 0)
        {
            pClock->ShowSeconds(pNode->GetNodeContent().CmpNoCase("true") == 0);
        }
    }
}


void clocksFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void clocksFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void clocksFrame::OnClose(wxCloseEvent& event)
{
    wxLogDebug("Close");
    event.Skip();
}

void clocksFrame::OnRightUp(wxMouseEvent& event)
{
    PopupMenu(&m_menuPopup);
}
