#include "clock.h"
#include <wx/dcbuffer.h>
#include <list>
#include <wx/dcmemory.h>
#include <wx/log.h>

using namespace std;

// AnalogueClock
#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(AnalogueClock, wxWindow)
#else
wxIMPLEMENT_DYNAMIC_CLASS(AnalogueClock, wxWindow);
#endif // WXSPAM





AnalogueClock::AnalogueClock()
    : wxWindow()
{

    m_timer.SetOwner(this);
    m_timer.Start(20);

    Connect(wxEVT_PAINT, (wxObjectEventFunction)&AnalogueClock::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&AnalogueClock::OnSize);
    Connect(wxID_ANY, wxEVT_TIMER,(wxObjectEventFunction)&AnalogueClock::OnTimer);


}

AnalogueClock::AnalogueClock(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size) :
    wxWindow(),
    m_nMode(ANALOGUE),
    m_bShowTimezone(false),
    m_bShowSeconds(false),
    m_bShowHours(false),
    m_bShowHours24(false),
    m_zone(wxDateTime::Local),
    m_clrAnalogueBack(*wxBLACK),
    m_clrAnalogueFront(*wxWHITE),
    m_clrAnalogueHands(*wxWHITE),
    m_clrAnalogueSecondHand(*wxRED),
    m_clrText(wxColour(220,220,220)),
    m_clrAnalogueHour(wxColour(240,240,240)),
    m_clrAnalogueHour24(wxColour(200,0,0))
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);


    wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("AnalogueClock"));

    m_timer.SetOwner(this);
    m_timer.Start(20);

    SetMinSize(size);
    m_sTimeZoneLabel = "LOCAL";

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);


    SetStudioColourGAR();


    Connect(wxEVT_PAINT, (wxObjectEventFunction)&AnalogueClock::OnPaint);
    Connect(wxEVT_SIZE, (wxObjectEventFunction)&AnalogueClock::OnSize);
    Connect(wxID_ANY, wxEVT_TIMER,(wxObjectEventFunction)&AnalogueClock::OnTimer);


    m_mTimeZone["LOCAL"] = wxDateTime::Local;
    m_mTimeZone["WET"] = wxDateTime::WET;
    m_mTimeZone["WEST"] = wxDateTime::WEST;
    m_mTimeZone["CET"] = wxDateTime::CET;
    m_mTimeZone["CEST"] = wxDateTime::CEST;
    m_mTimeZone["EET"] = wxDateTime::EET;
    m_mTimeZone["EEST"] = wxDateTime::EEST;
    m_mTimeZone["MSK"] = wxDateTime::MSK;
    m_mTimeZone["MSD"] = wxDateTime::MSD;
    m_mTimeZone["AST"] = wxDateTime::AST;
    m_mTimeZone["ADT"] = wxDateTime::ADT;
    m_mTimeZone["EDT"] = wxDateTime::EDT;
    m_mTimeZone["CST"] = wxDateTime::CST;
    m_mTimeZone["MST"] = wxDateTime::MST;
    m_mTimeZone["MDT"] = wxDateTime::MDT;
    m_mTimeZone["PST"] = wxDateTime::PST;
    m_mTimeZone["PDT"] = wxDateTime::PDT;
    m_mTimeZone["HST"] = wxDateTime::HST;
    m_mTimeZone["AKST"] = wxDateTime::AKST;
    m_mTimeZone["AKDT"] = wxDateTime::AKDT;
    m_mTimeZone["A_WST"] = wxDateTime::A_WST;
    m_mTimeZone["A_CST"] = wxDateTime::A_CST;
    m_mTimeZone["A_EST"] = wxDateTime::A_EST;
    m_mTimeZone["A_EEST"] = wxDateTime::A_ESST;
    m_mTimeZone["NZST"] = wxDateTime::NZST;
    m_mTimeZone["NZDT"] = wxDateTime::NZDT;
    m_mTimeZone["UTC"] = wxDateTime::UTC;
    m_mTimeZone["GMT_12"] = wxDateTime::GMT_12;
    m_mTimeZone["GMT_11"] = wxDateTime::GMT_11;
    m_mTimeZone["GMT_10"] = wxDateTime::GMT_10;
    m_mTimeZone["GMT_9"] = wxDateTime::GMT_9;
    m_mTimeZone["GMT_8"] = wxDateTime::GMT_8;
    m_mTimeZone["GMT_7"] = wxDateTime::GMT_7;
    m_mTimeZone["GMT_6"] = wxDateTime::GMT_6;
    m_mTimeZone["GMT_5"] = wxDateTime::GMT_5;
    m_mTimeZone["GMT_4"] = wxDateTime::GMT_4;
    m_mTimeZone["GMT_3"] = wxDateTime::GMT_3;
    m_mTimeZone["GMT_2"] = wxDateTime::GMT_2;
    m_mTimeZone["GMT_1"] = wxDateTime::GMT_1;
    m_mTimeZone["GMT0"] = wxDateTime::GMT0;
    m_mTimeZone["GMT1"] = wxDateTime::GMT1;
    m_mTimeZone["GMT2"] = wxDateTime::GMT2;
    m_mTimeZone["GMT3"] = wxDateTime::GMT3;
    m_mTimeZone["GMT4"] = wxDateTime::GMT4;
    m_mTimeZone["GMT5"] = wxDateTime::GMT5;
    m_mTimeZone["GMT6"] = wxDateTime::GMT6;
    m_mTimeZone["GMT7"] = wxDateTime::GMT7;
    m_mTimeZone["GMT8"] = wxDateTime::GMT8;
    m_mTimeZone["GMT9"] = wxDateTime::GMT9;
    m_mTimeZone["GMT10"] = wxDateTime::GMT10;
    m_mTimeZone["GMT11"] = wxDateTime::GMT11;
    m_mTimeZone["GMT12"] = wxDateTime::GMT12;
    m_mTimeZone["GMT13"] = wxDateTime::GMT13;

}

AnalogueClock::~AnalogueClock()
{
}


void AnalogueClock::OnPaint(wxPaintEvent& event)
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

void AnalogueClock::PaintNormalClock(wxBufferedPaintDC& dc)
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
    float r_inner_text24 = radius - 22 * factor;
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

                dc.SetFont(wxFont(static_cast<int>(5 * factor), wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "tahoma", wxFONTENCODING_DEFAULT));

                // calculate the size of the text and paint it in the window
                wxCoord w = 0, h = 0;
                dc.GetTextExtent(hour, &w, &h);
                wxPoint text_pos = wxPoint(static_cast<int>(r_inner_text * s - w / 2), static_cast<int>(r_inner_text * c - h / 2));
                dc.DrawText(hour, center + text_pos);

                if(m_bShowHours24)
                {
                    dc.SetTextForeground(m_clrAnalogueHour24);

                    dc.SetFont(wxFont(static_cast<int>(3 * factor), wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "tahoma", wxFONTENCODING_DEFAULT));
                    dc.GetTextExtent(hour24, &w, &h);
                    text_pos = wxPoint(static_cast<int>(r_inner_text24 * s - w / 2), static_cast<int>(r_inner_text24 * c - h / 2));
                    dc.DrawText(hour24, center + text_pos);
                }
            }
            //dc.DrawRotatedText(hour, center.x + text_pos.x, center.y + text_pos.y, (60-r)/60.0*360);  // cool :)
        }
    }

    // draw hour, minute and second hand of the clock
    wxDateTime time = wxDateTime::UNow();
    if(m_zone != wxDateTime::Local)
    {
        time.MakeTimezone(m_zone, false);
    }

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



void AnalogueClock::PaintStudioClock(wxBufferedPaintDC& dc)
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
    wxDateTime time = wxDateTime::UNow();
    if(m_zone != wxDateTime::Local)
    {
        time.MakeTimezone(m_zone, false);
    }
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

void AnalogueClock::PaintDigits(wxBufferedPaintDC& dc, float factor, const wxPoint& center, const wxDateTime& time, bool bShowHM)
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

void AnalogueClock::SetClockMode(int nMode)
{
    m_nMode = nMode;
    Refresh();
}

void AnalogueClock::SetStudioColour(unsigned int nSection, const wxColour& clrActive, const wxColour& clrInActive)
{
    if(nSection < 6)
    {
        m_arrActive[nSection] = clrActive;
        m_arrInActive[nSection] = clrInActive;
    }
    Refresh();
}

void AnalogueClock::SetStudioColour(unsigned int nSection, const wxColour& clrActive)
{
    SetStudioColour(nSection, clrActive, wxColour(std::max(0,clrActive.Red()/3), std::max(0,clrActive.Green()/3), std::max(0,clrActive.Blue()/3)));
}

void AnalogueClock::SetRefreshRate(unsigned int nMilliseconds)
{
    m_timer.Stop();
    m_timer.Start(nMilliseconds);
}

void AnalogueClock::SetStudioColourGAR()
{
     m_arrActive = {wxColour(48,255,48), wxColour(255,166,77), wxColour(255,61,1),  wxColour(48,255,48), wxColour(255,166,77), wxColour(255,61,1)};
    m_arrInActive = {wxColour(24,64,24),wxColour(64,41,38), wxColour(64,31,31), wxColour(24,64,24),wxColour(64,41,38), wxColour(64,31,31)};
    Refresh();
}

void AnalogueClock::SetStudioColourDefault()
{
    m_arrActive = {wxColour(255,166,77), wxColour(255,255,51), wxColour(48,255,48), wxColour(255,61,1),  wxColour(255,61,61), wxColour(255,61,61)};
    m_arrInActive = {wxColour(64,41,38), wxColour(64,64,26), wxColour(24,64,24), wxColour(64,31,31),  wxColour(64,31,31), wxColour(64,31,31)};
    Refresh();
}

void AnalogueClock::SetStudioColourWhite()
{
    m_arrActive = {*wxWHITE, *wxWHITE,*wxWHITE,*wxWHITE,*wxWHITE,*wxWHITE};
    m_arrInActive = {wxColour(60,60,60),wxColour(60,60,60),wxColour(60,60,60),wxColour(60,60,60),wxColour(60,60,60),wxColour(60,60,60)};
    Refresh();
}

void AnalogueClock::ShowTimezone(bool bShow)
{
    m_bShowTimezone = bShow;
    Refresh();
}

void AnalogueClock::ShowSeconds(bool bShow)
{
    m_bShowSeconds = bShow;
    Refresh();
}

void AnalogueClock::ShowHours(bool bShow, bool bShow24)
{
    m_bShowHours = bShow;
    m_bShowHours24 = bShow24;
}

void AnalogueClock::SetTimezone(wxDateTime::TZ zone, const wxString& sLabel)
{
    m_zone = zone;
    if(sLabel.empty())
    {
    }
    else
    {
        m_sTimeZoneLabel = sLabel;
    }
    Refresh();
}

void AnalogueClock::SetTimezone(const wxString& sTimeZone)
{
    auto itTz = m_mTimeZone.find(sTimeZone);
    if(itTz != m_mTimeZone.end())
    {
        m_sTimeZoneLabel = sTimeZone;
        m_zone = itTz->second;
        Refresh();
    }
}

void AnalogueClock::SetAnalogueColourBack(const wxColour& clr)
{
    m_clrAnalogueBack = clr;
    Refresh();
}

void AnalogueClock::SetAnalogueColourFront(const wxColour& clr)
{
    m_clrAnalogueFront = clr;
    Refresh();
}

void AnalogueClock::SetTextColour(const wxColour& clr)
{
    m_clrText= clr;
    Refresh();
}

void AnalogueClock::SetAnalogueHourColour(const wxColour& clr)
{
    m_clrAnalogueHour = clr;
    Refresh();
}

void AnalogueClock::SetAnalogueHour24Colour(const wxColour& clr)
{
    m_clrAnalogueHour24 = clr;
    Refresh();
}

void AnalogueClock::SetAnalogueColourHands(const wxColour& clr)
{
    m_clrAnalogueHands = clr;
    Refresh();
}

void AnalogueClock::SetAnalogueColourSecondHand(const wxColour& clr)
{
    m_clrAnalogueSecondHand = clr;
    Refresh();
}
