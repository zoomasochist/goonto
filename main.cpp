#include <numeric>
#include <utility>
#include <chrono>
#include <atomic>
#include <thread>
#include <random>
#include <wx/wx.h>
#include <wx/notifmsg.h>
#include <wx/mediactrl.h>
#include <wx/display.h>
#include <vlc/vlc.h>

#ifdef _WIN32

#include <windows.h>
#include <shlobj.h>

#elif  __linux__

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#endif

#include "pack.hpp"
#include "config.hpp"

#define TRY_OR_FAIL(exp) do { \
    try { exp; } \
    catch (const std::exception &e) \
    { wxMessageDialog(NULL, e.what()).ShowModal(); exit(1); } \
} while (0)

// Supposedly <random> achieves better randomness than rand()..
static std::random_device os_seed;
static std::mt19937 generator(os_seed());
std::uniform_int_distribution<uint_least32_t> distribute(35915);

inline int rand(int to)
{
    return distribute(generator) % to;
}

inline int rand(int from, int to)
{
    return distribute(generator) % (from - to + 1) + to;
}

// Accepts the width and height of an image and returns a "reasonable" width and height
// for the image to be scaled to, as to fit on the user's display.
std::pair<int, int> reasonableSize(int w, int h)
{
    int monitor_w, monitor_h;
    wxDisplaySize(&monitor_w, &monitor_h);

    double ratio = std::min((double)monitor_w / w, (double)monitor_h / h) / 2.5;

    return std::make_pair(w * ratio, h * ratio);
}

inline std::pair<int, int> reasonablePosition(void)
{
    int width, height = 1080;
    int mon_count = wxDisplay::GetCount();

    for (int i = 0; i < mon_count; i++)
    {
        wxRect geo = wxDisplay(i).GetGeometry();
        width += geo.GetWidth();
        height = geo.GetHeight();
    }

    return std::make_pair(rand(width), rand(height));
}

enum
{
    OpenImage_Timer = wxID_HIGHEST,
    OpenSite_Timer,
    NewNotif_Timer,
    Typing_Timer,
    Audio_Timer,
    Video_Timer,
};

class Goonto: public wxApp
{
wxTimer *popup_timer;
wxTimer *web_timer;
wxTimer *notif_timer;
wxTimer *typing_timer;
wxTimer *audio_timer;
wxTimer *video_timer;

std::atomic<bool> m_videoPlaying;

libvlc_instance_t *vid_inst = libvlc_new(0, NULL);
libvlc_media_player_t *vid_mp = nullptr;
libvlc_instance_t *inst = libvlc_new(0, NULL);
libvlc_media_player_t *mp = nullptr;

Pack *pack;
config_t config;
public:
bool OnInit()
{
    wxInitAllImageHandlers();
    // this->SetExitOnFrameDelete(false);

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

    if (config.notifs.enabled)
    {
        notif_timer = new wxTimer(this, NewNotif_Timer);
        notif_timer->Start(config.notifs.rate);
    }

    if (config.typing.enabled)
    {
        typing_timer = new wxTimer(this, Typing_Timer);
        typing_timer->Start(config.typing.rate);
    }

    if (config.audio.enabled)
    {
        audio_timer = new wxTimer(this, Audio_Timer);
        audio_timer->Start(3000);
    }

    if (config.videos.enabled)
    {
        video_timer = new wxTimer(this, Video_Timer);
        video_timer->Start(config.videos.rate);
    }

    return true;
}

int OnExit()
{
    delete popup_timer;
    delete web_timer;
    delete notif_timer;
    delete typing_timer;
    delete audio_timer;
    delete video_timer;
    delete pack;

    if (mp != nullptr)
        libvlc_media_player_release(mp);
    libvlc_release(inst);
 
    return 0;
}

inline void OpenImage(wxTimerEvent &WXUNUSED(event))
{
    OpenImage();
}

// Open image
//    (1) somewhere random
//    (2) under the cursor
void OpenImage()
{
    if (rand(100) < config.popups.follow_cursor_chance)
        OpenImageUnderCursor();

    auto [x, y] = reasonablePosition();

    OpenImageAt(x, y);
}

void OpenImageUnderCursor()
{
    const wxPoint pt = wxGetMousePosition();

    OpenImageAt(pt.x, pt.y);
}

// Open image 
void OpenImageAt(int x, int y)
{
    auto path = pack->RandomImage();
    wxImage image(path);

    if (!image.IsOk()) OpenImage();

    auto [width, height] = reasonableSize(image.GetWidth(), image.GetHeight());
    image.Rescale(width, height);

    x -= width / 2;
    y -= height / 2;

    wxFrame *frame = new wxFrame(NULL, -1, "Goonto", wxPoint(x, y), wxSize(450, 340),
        wxBORDER_NONE | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP);
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    if (rand(100) < config.popups.censor_chance)
        image = image.Blur(10);

    wxBitmap bitmap(image);
    wxStaticBitmap *static_bitmap = new wxStaticBitmap(frame, -1, wxNullBitmap);

    static_bitmap->SetBitmap(bitmap);
    sizer->Add(static_bitmap);
    frame->SetSizer(sizer);
    frame->SetSize(width, height);

    auto [min, max] = config.popups.opacity;
    frame->SetTransparent(rand(min, max) * 2.55);

    if (config.popups.closable)
        static_bitmap->Bind(wxEVT_LEFT_DOWN, &Goonto::OnClickClose, this);

    // Don't steal focus
    frame->ShowWithoutActivating();
}

void OnClickClose(wxMouseEvent &event)
{
    wxObject *obj = event.GetEventObject();
    // OnClickClose is only called from StaticBitmap objects, but it
    // doesn't hurt to check.
    if (!obj->IsKindOf(wxCLASSINFO(wxStaticBitmap)))
        return;

    wxDynamicCast(obj, wxStaticBitmap)->GetParent()->Close();

    if (rand(100) < config.popups.mitosis.chance)
    {
        int total = 1 + rand(config.popups.mitosis.max);
        for (int i = 0; i < total; i++)
            OpenImage();
    }
}

void OpenSite(wxTimerEvent &WXUNUSED(event))
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

void NewNotif(wxTimerEvent &WXUNUSED(event))
{
    auto text = pack->RandomPrompt();
    wxNotificationMessage notif("Goonto", text);
    notif.Show(config.notifs.close_automatically ? -1 : 0);
}

void Type(wxTimerEvent &WXUNUSED(event))
{
    auto text = pack->RandomPrompt();
#ifdef __linux__
    Display *display = XOpenDisplay(NULL);
    for (char& c : text) {
        unsigned int keycode = XKeysymToKeycode(display, c);
        XTestFakeKeyEvent(display, keycode, True, 0);
        XTestFakeKeyEvent(display, keycode, False, 0);
    }

    XFlush(display);
#else
#error "unimplemented"
#endif
}

void PlayAudio(wxTimerEvent &WXUNUSED(event))
{
    if (mp != nullptr && libvlc_media_player_is_playing(mp))
        return;

    auto track = pack->RandomAudio();
    libvlc_media_t *m = libvlc_media_new_path(inst, track.c_str());
    mp = libvlc_media_player_new_from_media(m);
    libvlc_media_player_play (mp);

    libvlc_media_release(m);
}

void OpenVideo(wxTimerEvent &WXUNUSED(event))
{
    if (m_videoPlaying) return;
    m_videoPlaying = true;

    auto video = pack->RandomVideo();
    auto [x, y] = reasonablePosition();

    // wxFrame *frame = new wxFrame(NULL, -1, "Goonto", wxPoint(x, y), wxSize(450, 340),
    //     wxBORDER_NONE | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP);
    // wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    // wxMediaCtrl *mediaCtrl = new wxMediaCtrl(frame, -1);

    // sizer->Add(mediaCtrl);
    // frame->SetSizer(sizer);

    // bool worked = mediaCtrl->Load(video);
    // if (!worked) {
    //     std::cout << "no worked" << std::endl;
    //     exit(1);
    // }

    libvlc_media_t *m = libvlc_media_new_location(vid_inst, video.c_str());
    vid_mp = libvlc_media_player_new_from_media(m);
    libvlc_media_player_play(vid_mp);

    libvlc_media_release(m);
    // mediaCtrl->SetVolume(0);
    // mediaCtrl->Play();
    // frame->ShowWithoutActivating();
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
    EVT_TIMER(Video_Timer, Goonto::OpenVideo)
    EVT_TIMER(NewNotif_Timer, Goonto::NewNotif)
    EVT_TIMER(Typing_Timer, Goonto::Type)
    EVT_TIMER(Audio_Timer, Goonto::PlayAudio)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(Goonto);