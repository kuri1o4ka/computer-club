#include "Parser.h"

#include <iostream>
#include <iomanip>
#include <sstream>

int parseTime(std::string &string) {
    if (string.length() == 5 and string[2] == ':') {
        int hour = stoi(string.substr(0, 2));
        int minute = stoi(string.substr(3, 2));
        if ((0 <= hour and hour < 24) and (0 <= minute and minute < 60)) {
            return hour * 60 + minute;
        }
    }
    return -1;
}
std::string formatTime(int minutes) {
    if (minutes < 0 or minutes >= 24 * 60) {
        return "Error formatting time.";
    }

    int hours = minutes / 60;
    int mins = minutes % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << mins;

    return oss.str();
}

std::string parseConfig(std::istream &inputFile, Config &config) {
    if (!inputFile) {
        return "Input file error";
    }
    std::string line;
    if (!std::getline(inputFile, line)) {
        return "Missing table count";
    }

    try {
        config.tableCount = stoi(line);
        if (config.tableCount <= 0) {
            return line;
        }
    }
    catch (...) {
        return line;
    }

    if (!std::getline(inputFile, line)) {
        return line;
    }

    std::istringstream strStream(line);
    std::string open, close;


    if (!(strStream >> open >> close)) {
        return line;
    }

    config.openTime = parseTime(open);
    config.closeTime = parseTime(close);

    if ((config.openTime == -1 or config.closeTime == -1) or (config.openTime >= config.closeTime)) {
        return line;
    }

    if (!std::getline(inputFile, line)) {
        return line;
    }
    try {
        config.pricePerHour = stoi(line);
        if (config.pricePerHour <= 0) {
            return line;
        }
    }
    catch (...) {
        return line;
    }
    return "";
}

std::string parseEvent(std::istream &inputFile, std::vector <Events> &event) {
    if (!inputFile) {
        return "Error reading input file.";
    }
    std::string line;
    int errTime = -1;

    while (std::getline(inputFile, line)) {
        if (line.empty() and inputFile.eof()) {
            break;
        }

        Events events;
        events.line = line;

        std::string timeStr, idStr;
        std::istringstream strStream(line);

        if (!(strStream >> timeStr >> idStr)) {
            return line;
        }

        int time = parseTime(timeStr);
        if (time == -1) {
            return line;
        }

        if (time < errTime) {
            return line;
        }
        errTime = time;

        int id;
        try {
            id = stoi(idStr);
        } catch (...) {
            return line;
        }

        if (id < 1 or id > 4) {
            return line;
        }

        events.time = time;
        events.id = id;

        std::string args;
        while (strStream >> args) {
            events.name.push_back(args);
        }

        int tempId = 0;
        switch (events.id) {
            case 1: tempId = 1; break;
                case 2: tempId = 2; break;
                case 3: tempId = 1; break;
                case 4: tempId = 1; break;
        }
        if (events.name.size() != tempId) {
            return line;
        }
        event.push_back(events);
    }
    return "";
}
