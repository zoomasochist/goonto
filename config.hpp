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
    bool enabled;
    int chance;
    int max;
};

struct popup_t
{
    bool enabled;
    std::pair<int, int> opacity;
    int censor_chance;
    long rate;

    mitosis_t t;
};

struct config_t
{
    popup_t popups;
    web_t web;
};

config_t loadConfig(std::string);

#endif