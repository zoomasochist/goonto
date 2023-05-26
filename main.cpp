#include <numeric>
#include <utility>
#include <chrono>
#include <thread>
#include <wx/wx.h>

// For ShellExecute in OpenSite
#ifdef _WIN32
#warning "Windows code is majorly untested. Tread carefully."
#include <windows.h>
#include <shlobj.h>
#endif

#include <stdlib.h>

#include "pack.hpp"
#include "config.hpp"

#define TRY_OR_FAIL(exp) do { \
    try { exp; } \
    catch (const std::exception &e) \
    { wxMessageDialog(NULL, e.what()).ShowModal(); exit(1); } \
} while (0)

// Accepts the width and height of an image and returns a "reasonable" width and height
// for the image to be scaled to, as to fit on the user's display.
std::pair<int, int> reasonableSize(int w, int h)
{
    int monitor_w, monitor_h;
    wxDisplaySize(&monitor_w, &monitor_h);

    double ratio = std::min((double)monitor_w / w, (double)monitor_h / h) / 2.5;

    return std::make_pair(w * ratio, h * ratio);
}

std::pair<int, int> reasonablePosition(void)
{
    int mw, mh;
    wxDisplaySize(&mw, &mh);

    return std::make_pair(rand() % mw, rand() % mh);
}

enum
{
    OpenImage_Timer = wxID_HIGHEST,
    OpenSite_Timer,
};

class Goonto: public wxApp
{
wxTimer *popup_timer;
wxTimer *web_timer;
Pack *pack;
config_t config;
public:
bool OnInit()
{
    srand(time(NULL));
    wxInitAllImageHandlers();

    TRY_OR_FAIL(pack = new Pack("test/futa2"));
    TRY_OR_FAIL(config = loadConfig("./example.json"));
    // config = new Config("~/.config/goonto.json");
    
    if (config.popups.enabled)
    {
        popup_timer = new wxTimer(this, OpenImage_Timer);
        popup_timer->Start(config.popups.rate);
    }

    if (config.web.enabled)
    {
        web_timer = new wxTimer(this, OpenSite_Timer);
        web_timer->Start(config.web.rate);
    }

    return true;
}

void OpenImage(wxTimerEvent& WXUNUSED(event))
{
    auto path = pack->RandomImage();
    auto [x, y] = reasonablePosition();

    wxFrame *frame = new wxFrame(NULL, -1, "Goonto", wxPoint(x, y), wxSize(450, 340),
        wxBORDER_NONE | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    wxImage image(path);

    auto [width, height] = reasonableSize(image.GetWidth(), image.GetHeight());
    image.Rescale(width, height);

    wxBitmap bitmap(image);
    wxStaticBitmap *static_bitmap = new wxStaticBitmap(frame, -1, wxNullBitmap);
    
    static_bitmap->SetBitmap(bitmap);
    sizer->Add(static_bitmap);
    frame->SetSizer(sizer);

    frame->SetSize(width, height);

    frame->Show(true);
}

void OpenSite(wxTimerEvent& WXUNUSED(event))
{
    auto url = pack->RandomUrl();
#ifdef _WIN32
    // This is a hack and will likely break on some inputs,
    // but all of those inputs are also invalid URLs.
    std::wstring wide(url.begin(), url.end());
    ShellExecute(0, 0, wide, 0, 0, SW_SHOW);
#elif __linux__
    std::string cmd = "xdg-open " + url;
    (void)system(cmd.c_str());
#elif __APPLE__
    std::string cmd = "open " + url;
    (void)system(cmd.c_str());
#else
#error "unimplemented"
#endif
}

// void ChangeWallpaper(wxTimerEvent& WXUNUSED(event))
// {
//     auto path = pack->RandomImage();
// #ifdef _WIN32
//     SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, path.c_str(), SPIF_UPDATEINIFILE);
// #elif __linux__

// #else
// #error "unimplemented"
// #endif
// }

private:
wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(Goonto, wxApp)
    EVT_TIMER(OpenImage_Timer, Goonto::OpenImage)
    EVT_TIMER(OpenSite_Timer, Goonto::OpenSite)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(Goonto);