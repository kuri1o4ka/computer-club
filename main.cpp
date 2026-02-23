#include <fstream>
#include <iostream>
#include "Parser.h"
#include "Club.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./main <file>" << std::endl;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return EXIT_FAILURE;
    }


    Config config;
    std::string error = parseConfig(inputFile, config);
    if (!error.empty()) {
        std::cout << error << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<Events> events;
    error = parseEvent(inputFile, events);
    if (!error.empty()) {
        std::cout << error << std::endl;
        return EXIT_FAILURE;
    }
    inputFile.close();

    std::string outFileName = "out_" + std::string(argv[1]);
    std::ofstream outFile(outFileName);
    if (!outFile.is_open()) {
        std::cout << "Error create output file" << std::endl;
        return EXIT_FAILURE;
    }

    auto buf = std::cout.rdbuf();
    std::cout.rdbuf(outFile.rdbuf());

    Club club(config);
    std::string openTime = formatTime(config.openTime);
    std::cout << openTime << std::endl;
    for (auto& event : events) {
        club.handleEvent(event);
    }

    club.closeDay(config.closeTime);

    std::cout.rdbuf(buf);
    outFile.close();

    return EXIT_SUCCESS;
}