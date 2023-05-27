#include <fstream>
#include <random>
#include <nlohmann/json.hpp>

#include "pack.hpp"

using json = nlohmann::json;

// TODO: Not dupe RNG with main
static std::random_device os_seed;
static std::mt19937 generator(os_seed());
static std::uniform_int_distribution<uint_least32_t> distribute(1, 100000);

Pack::Pack(std::string path)
{
    // Textual
    std::ifstream promptf(path + "/prompt.json");
    const json prompt_json = json::parse(promptf);
    prompts = prompt_json.get<std::vector<std::string>>();

    std::ifstream webf(path + "/web.json");
    const json web_json = json::parse(webf);

    for (auto& [_, site] : web_json.items())
    {
        const auto url = site["url"].get<std::string>();
        const auto queries = site.value<std::vector<std::string>>("queries", { });
        web.push_back(std::make_tuple(url, queries));
    }

    std::ifstream captionf(path + "/captions.json");
    const json captions_json = json::parse(captionf);
    captions = captions_json.get<std::vector<std::string>>();

    // Media
    for (auto const& entry : std::filesystem::directory_iterator{path + "/img"}) 
    {
        if (entry.is_regular_file())
            images.push_back(entry.path());
    }

    for (auto const& entry : std::filesystem::directory_iterator{path + "/aud"}) 
    {
        if (entry.is_regular_file())
            audio.push_back(entry.path());
    }
}

std::string Pack::RandomImage()
{
    auto iter = images.begin();
    std::advance(iter, distribute(generator) % images.size());
    return *iter;
}

std::string Pack::RandomUrl()
{
    auto t = web.begin();
    std::advance(t, distribute(generator) % web.size());
    const auto [root, exts] = *t;

    if (exts.size() != 0)
    {
        auto ext = exts.begin();
        std::advance(ext, distribute(generator) % exts.size());
        return root + *ext;
    }

    return root;
}

std::string Pack::RandomPrompt()
{
    auto iter = prompts.begin();
    std::advance(iter, distribute(generator) % prompts.size());
    return *iter;
}

std::string Pack::RandomAudio()
{
    auto iter = audio.begin();
    std::advance(iter, distribute(generator) % audio.size());
    return *iter;
}