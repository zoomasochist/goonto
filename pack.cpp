#include <fstream>
#include <nlohmann/json.hpp>

#include "pack.hpp"

using json = nlohmann::json;

Pack::Pack(std::string path)
{
    // Textual
    std::ifstream promptf(path + "/prompt.json");
    const json prompt_json = json::parse(promptf);
    prompt_json.get<std::vector<std::string>>();

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
    captions_json.get<std::vector<std::string>>();

    // Media
    for (auto const& entry : std::filesystem::directory_iterator{path + "/img"}) 
    {
        if (entry.is_regular_file())
            images.push_back(entry.path());
    }
}

std::string Pack::RandomImage()
{
    auto iter = images.begin();
    std::advance(iter, std::rand() % images.size());
    return *iter;
}

std::string Pack::RandomUrl()
{
    auto t = web.begin();
    std::advance(t, std::rand() % web.size());
    const auto [root, exts] = *t;

    if (exts.size() != 0)
    {
        auto ext = exts.begin();
        std::advance(ext, std::rand() % exts.size());
        return root + *ext;
    }

    return root;
}