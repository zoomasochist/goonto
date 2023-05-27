#include <fstream>
#include <nlohmann/json.hpp>

#include "config.hpp"

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(web_t, enabled, rate)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(mitosis_t, chance, max)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(popup_t, closable, enabled, opacity, censor_chance,
    rate, follow_cursor_chance, mitosis)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(typing_t, enabled, rate)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(notif_t, close_automatically, enabled, rate)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(audio_t, enabled)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(config_t, notifs, audio, popups, typing, web)

config_t loadConfig(std::string path)
{
    std::ifstream configf(path);
    const json json_stream = json::parse(configf, nullptr, false, true);
    return json_stream.get<config_t>();
}