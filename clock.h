#pragma once
#include <wx/window.h>
#include <vector>
#include <wx/bitmap.h>
#include <wx/timer.h>
#include <array>
#include <map>

class wxBufferedPaintDC;
class AnalogueClock : public wxWindow
{
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(AnalogueClock)
    #else
    wxDECLARE_DYNAMIC_CLASS(AnalogueClock);
    #endif // WXSPAM
public:
    AnalogueClock();
    virtual ~AnalogueClock();

    AnalogueClock(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void SetClockMode(int nMode);

    void SetStudioColour(unsigned int nSection, const wxColour& clrActive, const wxColour& clrInActive);
    void SetStudioColour(unsigned int nSection, const wxColour& clrActive);
    void SetStudioColourDefault();
    void SetStudioColourWhite();
    void SetStudioColourGAR();

    void SetAnalogueColourBack(const wxColour& clr);
    void SetAnalogueColourFront(const wxColour& clr);
    void SetAnalogueColourHands(const wxColour& clr);
    void SetAnalogueColourSecondHand(const wxColour& clr);
    void SetTextColour(const wxColour& clr);
    void SetAnalogueHourColour(const wxColour& clr);
    void SetAnalogueHour24Colour(const wxColour& clr);

    void ShowTimezone(bool bShow);
    void ShowSeconds(bool bShow);
    void ShowHours(bool bShow, bool bShow24);


    void SetTimezone(wxDateTime::TZ zone, const wxString& sLabel="");
    void SetTimezone(const wxString& sTimeZone);



    int GetClockType() const {return m_nMode;}

    const std::map<wxString, wxDateTime::TZ>& GetTimeZones() { return m_mTimeZone;}

    wxSize DoGetBestSize()
    {
        return wxSize(50,100);
    }
    void SetRefreshRate(unsigned int nMilliseconds);




    enum {ANALOGUE, STUDIO};

    protected:

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event)
    {
        Refresh();
    }
    void OnTimer(wxTimerEvent& event)
    {
        Refresh();
        //Update();
    }




    void PaintNormalClock(wxBufferedPaintDC& dc);
    void PaintStudioClock(wxBufferedPaintDC& dc);
    void PaintDigits(wxBufferedPaintDC& dc, float factor, const wxPoint& center, const wxDateTime& time, bool bShowHM);

    int m_nMode;
    wxTimer m_timer;
    bool m_bShowTimezone;
    bool m_bShowSeconds;
    bool m_bShowHours;
    bool m_bShowHours24;
    wxDateTime::TZ m_zone;
    wxString m_sTimeZoneLabel;

    std::array<wxColour, 6> m_arrActive;
    std::array<wxColour, 6> m_arrInActive;

    std::map<wxString, wxDateTime::TZ> m_mTimeZone;

    wxColour m_clrAnalogueBack;
    wxColour m_clrAnalogueFront;
    wxColour m_clrAnalogueHands;
    wxColour m_clrAnalogueSecondHand;
    wxColour m_clrText;
    wxColour m_clrAnalogueHour;
    wxColour m_clrAnalogueHour24;
};



