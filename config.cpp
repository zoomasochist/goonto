#include <fstream>
#include <nlohmann/json.hpp>

#include "config.hpp"

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(web_t, enabled, rate)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(mitosis_t, enabled, chance, max)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(popup_t, enabled, opacity, censor_chance, rate)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(config_t, popup, web)

config_t loadConfig(std::string path)
{
    std::ifstream configf(path);
    const json json_stream = json::parse(configf);
    return json_stream.get<config_t>();
}