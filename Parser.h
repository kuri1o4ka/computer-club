#ifndef COMPCLUB_PARSER_H
#define COMPCLUB_PARSER_H
#include <string>
#include <vector>

struct Config {
    int tableCount = 0;
    int openTime = 0 , closeTime = 0;
    int pricePerHour = 0;
};

struct Events {
    int time = 0;
    int id = 0;
    std::vector<std::string> name;
    std::string line;
};

int parseTime(std::string &string);
std::string formatTime(int minutes);
std::string parseConfig(std::istream &inputFile, Config &config);
std::string parseEvent(std::istream &inputFile, std::vector<Events> &events);

#endif //COMPCLUB_PARSER_H