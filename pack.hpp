#ifndef __PACK_HPP
#define __PACK_HPP

class Pack
{
private:
    // Textual
    std::vector<std::string> prompts;
    std::vector<std::tuple<std::string, std::vector<std::string>>> web;
    std::vector<std::string> captions;
    // Media
    std::vector<std::string> images;
    std::vector<std::string> audio;
    std::vector<std::string> videos;
public:
    Pack(std::string);

    std::string RandomImage();
    std::string RandomUrl();
    std::string RandomPrompt();
    std::string RandomAudio();
    std::string RandomVideo();
};

#endif