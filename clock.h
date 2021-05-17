#pragma once
#include <wx/window.h>
#include <vector>
#include <wx/bitmap.h>
#include <wx/timer.h>
#include <array>
#include <map>

class wxBufferedPaintDC;
class StudioClock : public wxWindow
{
    #ifdef WXSPAM
    DECLARE_DYNAMIC_CLASS(StudioClock)
    #else
    wxDECLARE_DYNAMIC_CLASS(StudioClock);
    #endif // WXSPAM
public:
    StudioClock();
    virtual ~StudioClock();

    StudioClock(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

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


    void SetTimezone(const wxString& sLabel, const wxTimeSpan& tsOffset, bool bOffsetFromUTC);



    int GetClockType() const {return m_nMode;}


    wxSize DoGetBestSize()
    {
        return wxSize(50,100);
    }
    void SetRefreshType(int nType);

    enum {SWEEP, TICK};



    enum {ANALOGUE, STUDIO};

    protected:

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event)
    {
        Refresh();
    }
    void OnTimer(wxTimerEvent& event);


    void Tick();


    void PaintNormalClock(wxBufferedPaintDC& dc);
    void PaintStudioClock(wxBufferedPaintDC& dc);
    void PaintDigits(wxBufferedPaintDC& dc, float factor, const wxPoint& center, const wxDateTime& time, bool bShowHM);

    wxDateTime GetClockTime();

    int m_nMode;
    wxTimer m_timer;
    bool m_bShowTimezone;
    bool m_bShowSeconds;
    bool m_bShowHours;
    bool m_bShowHours24;

    wxTimeSpan m_tsOffset;
    wxString m_sTimeZoneLabel;
    bool m_bUTCOffset;

    std::array<wxColour, 6> m_arrActive;
    std::array<wxColour, 6> m_arrInActive;


    wxColour m_clrAnalogueBack;
    wxColour m_clrAnalogueFront;
    wxColour m_clrAnalogueHands;
    wxColour m_clrAnalogueSecondHand;
    wxColour m_clrText;
    wxColour m_clrAnalogueHour;
    wxColour m_clrAnalogueHour24;

    int m_nRefreshType;

    wxDateTime m_dtLastUpdate;


};



