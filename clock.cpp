#include "clock.h"
#include <wx/dcbuffer.h>
#include <list>
#include <wx/dcmemory.h>
#include <wx/log.h>

using namespace std;

// StudioClock
#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(StudioClock, wxWindow)
#else
wxIMPLEMENT_DYNAMIC_CLASS(StudioClock, wxWindow);
#endif // WXSPAM





StudioClock::StudioClock()
    : wxWindow()
{

    m_timer.SetOwner(this);

    Connect(wxEVT_PAINT, (wxObjectEventFunction)&StudioClock::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&StudioClock::OnSize);
    Connect(wxID_ANY, wxEVT_TIMER,(wxObjectEventFunction)&StudioClock::OnTimer);


}

StudioClock::StudioClock(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
    wxWindow(),
    m_nMode(ANALOGUE),
    m_bShowTimezone(false),
    m_bShowSeconds(false),
    m_bShowHours(false),
    m_bShowHours24(false),
    m_clrAnalogueBack(*wxBLACK),
    m_clrAnalogueFront(*wxWHITE),
    m_clrAnalogueHands(*wxWHITE),
    m_clrAnalogueSecondHand(*wxRED),
    m_clrText(wxColour(220,220,220)),
    m_clrAnalogueHour(wxColour(240,240,240)),
    m_clrAnalogueHour24(wxColour(200,0,0)),
    m_nRefreshType(SWEEP)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("StudioClock"));

    wxSetCursor(wxCURSOR_BLANK);

    m_timer.SetOwner(this);

    SetMinSize(size);

    m_sTimeZoneLabel = "LOCAL";
    m_bUTCOffset = false;
    m_tsOffset = wxTimeSpan(0);


    SetBackgroundStyle(wxBG_STYLE_CUSTOM);


    SetStudioColourGAR();


    Connect(wxEVT_PAINT, (wxObjectEventFunction)&StudioClock::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&StudioClock::OnSize);
    Connect(wxID_ANY, wxEVT_TIMER,(wxObjectEventFunction)&StudioClock::OnTimer);


    SetRefreshType(m_nRefreshType);
}

StudioClock::~StudioClock()
{
}


void StudioClock::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);

    switch(m_nMode)
    {
        case ANALOGUE:
            PaintNormalClock(dc);
            break;
        case STUDIO:
            PaintStudioClock(dc);
            break;
    }
}

wxDateTime StudioClock::GetClockTime()
{
    wxDateTime dtNow = wxDateTime::UNow();
    if(m_bUTCOffset)
    {
        dtNow = dtNow.ToUTC(false);
        //wxLogDebug("%s %s", dtNow.ToUTC(false).Format("%H").c_str(), dtNow.ToUTC(true).Format("%H").c_str());
    }
    dtNow += m_tsOffset;
    return dtNow;
}

void StudioClock::PaintNormalClock(wxBufferedPaintDC& dc)
{

    wxSize sz = GetClientSize();

    wxPoint center = wxPoint(sz.x / 2, sz.y / 2);
    wxCoord radius = wxMin(sz.x / 2 - 20, sz.y / 2 - 20);
    radius = wxMax(radius, 30);

    float factor = wxMax(radius/60.0, 1.0);

    // draw lightblue background
    dc.SetPen(wxPen(*wxBLACK));
    dc.SetBrush(wxBrush(*wxBLACK));
    dc.DrawRectangle(0, 0, sz.x, sz.y);

    // draw clock border
    dc.SetPen(wxPen(m_clrAnalogueFront));
    dc.SetBrush(wxBrush(m_clrAnalogueFront));
    dc.DrawCircle(center, radius);
    dc.SetPen(wxPen(m_clrAnalogueBack));
    dc.SetBrush(wxBrush(m_clrAnalogueBack));


    dc.DrawCircle(center, static_cast<int>(radius - 1.0 * factor));

    // paint lines for minutes
    float pi = 3.141592654;
    float r_outer = radius - 5 * factor;
    float r_inner_min = radius - 7 * factor;
    float r_inner_hour = radius - 10 * factor;
    float r_inner_text = radius - 15 * factor;
    float r_inner_text24 = radius - 23 * factor;
    int linewidth = 1;
    for (int r = 1; r <= 60; r ++)
    {

        float s = sin(r/60.0*2.0*pi);
        float c = -cos(r/60.0*2.0*pi);

        linewidth = wxMax(static_cast<int>(factor / 2), 1);
        dc.SetPen(wxPen(m_clrAnalogueFront, linewidth));
        wxPoint outer = wxPoint(static_cast<int>(r_outer * s), static_cast<int>(r_outer * c));
        wxPoint inner = wxPoint(static_cast<int>(r_inner_min * s), static_cast<int>(r_inner_min * c));
        dc.DrawLine(center + inner, center + outer);

        if (r % 5 == 0)
        {
            linewidth = wxMax(static_cast<int>(factor), 1);
            dc.SetPen(wxPen(m_clrAnalogueFront, linewidth));

            // paint larger lines for the hours
            outer = wxPoint(static_cast<int>(r_outer * s), static_cast<int>(r_outer * c));
            inner = wxPoint(static_cast<int>(r_inner_hour * s), static_cast<int>(r_inner_hour * c));
            dc.DrawLine(center + inner, center + outer);

            // paint value of the hour
            if(m_bShowHours)
            {
                dc.SetTextForeground(m_clrAnalogueHour);

                wxString hour;
                hour.Printf("%i", r / 5);

                wxString hour24;
                hour24.Printf("%i", ((r / 5)+12)%24);

                dc.SetFont(wxFont(static_cast<int>(6 * factor), wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "tahoma", wxFONTENCODING_DEFAULT));

                // calculate the size of the text and paint it in the window
                wxCoord w = 0, h = 0;
                dc.GetTextExtent(hour, &w, &h);
                wxPoint text_pos = wxPoint(static_cast<int>(r_inner_text * s - w / 2), static_cast<int>(r_inner_text * c - h / 2));
                dc.DrawText(hour, center + text_pos);

                if(m_bShowHours24)
                {
                    dc.SetTextForeground(m_clrAnalogueHour24);

                    dc.SetFont(wxFont(static_cast<int>(4 * factor), wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "tahoma", wxFONTENCODING_DEFAULT));
                    dc.GetTextExtent(hour24, &w, &h);
                    text_pos = wxPoint(static_cast<int>(r_inner_text24 * s - w / 2), static_cast<int>(r_inner_text24 * c - h / 2));
                    dc.DrawText(hour24, center + text_pos);
                }
            }
            //dc.DrawRotatedText(hour, center.x + text_pos.x, center.y + text_pos.y, (60-r)/60.0*360);  // cool :)
        }
    }

    // draw hour, minute and second hand of the clock
    wxDateTime time = GetClockTime();


    PaintDigits(dc, factor, center, time, false);


    int h = time.GetHour();
    int m = time.GetMinute();
    int s = time.GetSecond();
    int ms = time.GetMillisecond();

    int r = 1;
/*    int h_deg = static_cast<int>((h % 12) / 12.0 * 360);
    int m_deg = static_cast<int>(m / 60.0 * 360.0);
    int s_deg = static_cast<int>(s / 60.0 * 360.0);*/
    float h_rad = (h % 12 + m / 60.0 + s / 3600.0) / 12.0 * 2.0 * pi;
    float m_rad = (m / 60.0 + s / 3600.0 + ms /3600000.0) * 2.0 * pi;
    float s_rad;
    if(m_timer.GetInterval() < 500)
    {
         s_rad = (s / 60.0 + ms/60000.0) * 2.0 * pi;
    }
    else
    {
         s_rad = (s / 60.0) * 2.0 * pi;
    }

    r = static_cast<int>(factor * 35);
    linewidth = wxMax(static_cast<int>(factor * 3), 1);
    dc.SetPen(wxPen(m_clrAnalogueHands, linewidth));
    dc.DrawLine(center, center + wxPoint(static_cast<int>(r * sin(h_rad)), static_cast<int>(r * -cos(h_rad))));

    r = static_cast<int>(factor * 45);
    linewidth = wxMax(static_cast<int>(factor * 2), 1);
    dc.SetPen(wxPen(m_clrAnalogueHands, linewidth));
    dc.DrawLine(center, center + wxPoint(static_cast<int>(r * sin(m_rad)), static_cast<int>(r * -cos(m_rad))));

    r = static_cast<int>(factor * 50);
    linewidth = wxMax(static_cast<int>(factor), 1);
    dc.SetPen(wxPen(m_clrAnalogueSecondHand, linewidth));
    dc.DrawLine(center, center + wxPoint(static_cast<int>(r * sin(s_rad)), static_cast<int>(r * -cos(s_rad))));
    dc.DrawLine(center, center - wxPoint(static_cast<int>(r/6 * sin(s_rad)), static_cast<int>(r/6 * -cos(s_rad))));

    dc.SetPen(m_clrAnalogueFront);
    dc.SetBrush(m_clrAnalogueFront);
    dc.DrawCircle(center, factor * 2);

}



void StudioClock::PaintStudioClock(wxBufferedPaintDC& dc)
{
    // Get window dimensions
    wxSize sz = GetClientSize();

    wxPoint center = wxPoint(sz.x / 2, sz.y / 2);
    wxCoord radius = wxMin(sz.x / 2 - 20, sz.y / 2 - 20);
    radius = wxMax(radius, 50);

    float factor = wxMax(radius/60.0, 1.0);


    dc.SetPen(*wxBLACK);
    dc.SetBrush(*wxBLACK);
    dc.DrawRectangle(0, 0, sz.x, sz.y);

    // paint lines for minutes
    float pi = 3.141592654;
    float r_outer = radius - 5 * factor;
    float r_inner_min = radius - 7 * factor;
    float r_inner_hour = radius - 10 * factor;
    float r_inner_text = radius - 15 * factor;
    int linewidth = 1;

    // draw hour, minute and second hand of the clock
    wxDateTime time = GetClockTime();

    int nHour = time.GetHour();
    int nMin = time.GetMinute();
    int nSec = time.GetSecond();

    for (int r = 0; r < 60; r ++)
    {

        float s = sin(r/60.0*2.0*pi);
        float c = -cos(r/60.0*2.0*pi);

        if(nSec >= r || r == 60)
        {
            dc.SetPen(m_arrActive[(r)/10]);
            dc.SetBrush(m_arrActive[(r)/10]);
        }
        else
        {
            dc.SetPen(m_arrInActive[(r)/10]);
            dc.SetBrush(m_arrInActive[(r)/10]);
        }
        wxPoint outer = wxPoint(static_cast<int>(r_outer * s), static_cast<int>(r_outer * c));
        dc.DrawCircle(center+outer, factor*2);

        if (r % 5 == 0)
        {
            dc.SetPen(m_arrActive[(r)/10]);
            dc.SetBrush(m_arrActive[(r)/10]);
            // paint larger lines for the hours
            wxPoint inner = wxPoint(static_cast<int>(r_inner_hour * s), static_cast<int>(r_inner_hour * c));
            dc.DrawCircle(center+inner, factor*2);
        }
    }

    PaintDigits(dc, factor, center, time, true);
}

void StudioClock::PaintDigits(wxBufferedPaintDC& dc, float factor, const wxPoint& center, const wxDateTime& time, bool bShowHM)
{
    dc.SetTextForeground(m_clrText);

    dc.SetFont(wxFont(static_cast<int>(17 * factor), wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "tahoma", wxFONTENCODING_DEFAULT));

    wxString sTime = time.Format("%H:%M");
    wxString sSec = time.Format("%S");
    wxCoord w = 0, h = 0;
    dc.GetTextExtent(sTime, &w, &h);

    int nHourBottom = center.y+h/2;
    int nHourTop = center.y-h/2;
    if(bShowHM)
    {
        dc.DrawText(sTime, center.x-w/2, center.y-h/2);
    }

    if(m_bShowSeconds)
    {
        dc.SetFont(wxFont(static_cast<int>(12 * factor), wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "tahoma", wxFONTENCODING_DEFAULT));
        dc.GetTextExtent(sSec, &w, &h);
        dc.DrawText(sSec, center.x-w/2, nHourBottom);
    }

    if(m_bShowTimezone)
    {
        dc.SetFont(wxFont(static_cast<int>(8 * factor), wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "tahoma", wxFONTENCODING_DEFAULT));
        dc.GetTextExtent(m_sTimeZoneLabel, &w, &h);
        dc.DrawText(m_sTimeZoneLabel, center.x-w/2, nHourTop-h);
    }
}

void StudioClock::SetClockMode(int nMode)
{
    m_nMode = nMode;
    Refresh();
}

void StudioClock::SetStudioColour(unsigned int nSection, const wxColour& clrActive, const wxColour& clrInActive)
{
    if(nSection < 6)
    {
        m_arrActive[nSection] = clrActive;
        m_arrInActive[nSection] = clrInActive;
    }
    Refresh();
}

void StudioClock::SetStudioColour(unsigned int nSection, const wxColour& clrActive)
{
    SetStudioColour(nSection, clrActive, wxColour(std::max(0,clrActive.Red()/3), std::max(0,clrActive.Green()/3), std::max(0,clrActive.Blue()/3)));
}


void StudioClock::SetStudioColourGAR()
{
     m_arrActive = {wxColour(48,255,48), wxColour(255,166,77), wxColour(255,61,1),  wxColour(48,255,48), wxColour(255,166,77), wxColour(255,61,1)};
    m_arrInActive = {wxColour(24,64,24),wxColour(64,41,38), wxColour(64,31,31), wxColour(24,64,24),wxColour(64,41,38), wxColour(64,31,31)};
    Refresh();
}

void StudioClock::SetStudioColourDefault()
{
    m_arrActive = {wxColour(255,166,77), wxColour(255,255,51), wxColour(48,255,48), wxColour(255,61,1),  wxColour(255,61,61), wxColour(255,61,61)};
    m_arrInActive = {wxColour(64,41,38), wxColour(64,64,26), wxColour(24,64,24), wxColour(64,31,31),  wxColour(64,31,31), wxColour(64,31,31)};
    Refresh();
}

void StudioClock::SetStudioColourWhite()
{
    m_arrActive = {*wxWHITE, *wxWHITE,*wxWHITE,*wxWHITE,*wxWHITE,*wxWHITE};
    m_arrInActive = {wxColour(60,60,60),wxColour(60,60,60),wxColour(60,60,60),wxColour(60,60,60),wxColour(60,60,60),wxColour(60,60,60)};
    Refresh();
}

void StudioClock::ShowTimezone(bool bShow)
{
    m_bShowTimezone = bShow;
    Refresh();
}

void StudioClock::ShowSeconds(bool bShow)
{
    m_bShowSeconds = bShow;
    Refresh();
}

void StudioClock::ShowHours(bool bShow, bool bShow24)
{
    m_bShowHours = bShow;
    m_bShowHours24 = bShow24;
}

void StudioClock::SetTimezone(const wxString& sTimeZone, const wxTimeSpan& tsOffset, bool bOffsetFromUTC)
{
    m_sTimeZoneLabel = sTimeZone;
    m_tsOffset = tsOffset;
    m_bUTCOffset = bOffsetFromUTC;
    Refresh();
}

void StudioClock::SetAnalogueColourBack(const wxColour& clr)
{
    m_clrAnalogueBack = clr;
    Refresh();
}

void StudioClock::SetAnalogueColourFront(const wxColour& clr)
{
    m_clrAnalogueFront = clr;
    Refresh();
}

void StudioClock::SetTextColour(const wxColour& clr)
{
    m_clrText= clr;
    Refresh();
}

void StudioClock::SetAnalogueHourColour(const wxColour& clr)
{
    m_clrAnalogueHour = clr;
    Refresh();
}

void StudioClock::SetAnalogueHour24Colour(const wxColour& clr)
{
    m_clrAnalogueHour24 = clr;
    Refresh();
}

void StudioClock::SetAnalogueColourHands(const wxColour& clr)
{
    m_clrAnalogueHands = clr;
    Refresh();
}

void StudioClock::SetAnalogueColourSecondHand(const wxColour& clr)
{
    m_clrAnalogueSecondHand = clr;
    Refresh();
}

void StudioClock::OnTimer(wxTimerEvent& event)
{
    Refresh();
    if(m_nMode == STUDIO || m_nRefreshType == TICK)
    {
        Update();
    }

    Tick();
}

void StudioClock::SetRefreshType(int nType)
{
    m_nRefreshType = nType;


    if(m_timer.IsRunning())
    {
        m_timer.Stop();
    }
    Tick();
}

void StudioClock::Tick()
{
    unsigned long nMilliseconds = wxDateTime::UNow().GetMillisecond();
    if(m_nRefreshType == TICK || m_nMode == STUDIO)
    {
        m_timer.Start(1000-nMilliseconds);
    }
    else
    {
        nMilliseconds %= 50;
        m_timer.Start(50-nMilliseconds);
    }
}
