#pragma once
#include <wx/window.h>
#include <vector>
#include <wx/bitmap.h>
#include <wx/timer.h>
#include <array>
#include <map>

class wxBufferedPaintDC;
class DigitalClock : public wxWindow
{
    wxDECLARE_DYNAMIC_CLASS(DigitalClock);
public:
    DigitalClock();
    virtual ~DigitalClock();

    DigitalClock(wxWindow *parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void SetClockMode(int nMode);

    void ShowTimezone(bool bShow);
    void ShowSeconds(bool bShow);
    void Show24(bool bShow);

    void SetTimezone(const wxString& sLabel, const wxTimeSpan& tsOffset, bool bOffsetFromUTC);

    wxSize DoGetBestSize()
    {
        return wxSize(200,50);
    }
    void SetRefreshType(int nType);

    protected:

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event)
    {
        Refresh();
    }
    void OnTimer(wxTimerEvent& event);


    void Tick();


    void PaintNormalClock(wxBufferedPaintDC& dc);
    wxDateTime GetClockTime();

    wxTimer m_timer;
    bool m_bShowTimezone;
    bool m_bShowSeconds;
    bool m_bShowHours24;

    wxTimeSpan m_tsOffset;
    wxString m_sTimeZoneLabel;
    bool m_bUTCOffset;

    int m_nRefreshType;

    wxDateTime m_dtLastUpdate;


};




