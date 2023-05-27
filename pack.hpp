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
public:
    Pack(std::string);
    // Returns the path of a randomly chosen image in the pack.
    std::string RandomImage();
    std::string RandomUrl();
    std::string RandomPrompt();
    std::string RandomAudio();
};

#endif