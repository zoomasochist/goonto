#ifndef _CONFIG_HPP
#define _CONFIG_HPP
#include <utility>

struct web_t
{
    bool enabled;
    long rate;
};

struct mitosis_t
{
    int chance;
    int max;
};

struct popup_t
{
    bool enabled;
    std::pair<int, int> opacity;
    int censor_chance;
    long rate;
    int follow_cursor_chance;
    bool closable;

    mitosis_t mitosis;
};

struct typing_t
{
    bool enabled;
    long rate;
};

struct audio_t
{
    bool enabled;
};

struct notif_t
{
    bool enabled;
    long rate;
    bool close_automatically;
};

struct video_t
{
    bool enabled;
    long rate;
    bool multiple;
    int censor_chance;
};

struct config_t
{
    popup_t popups;
    notif_t notifs;
    web_t   web;
    audio_t audio;
    typing_t typing;
    video_t videos;
};

config_t loadConfig(std::string);

#endif