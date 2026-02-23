#ifndef COMPCLUB_CLUB_H
#define COMPCLUB_CLUB_H
#include <queue>
#include <unordered_map>
#include <vector>
#include "Parser.h"

struct Table {
    bool isBusy = false;
    int startTime = 0;
    int totalTime = 0;
    int totalEarning = 0;
};

class Club{
    private:
    Config config;
    std::vector<Table> tables;
    std::unordered_map<std::string, int> clients;
    std::queue<std::string> waitQueue;
public:
    Club(const Config& config) : config(config) {
        tables.resize(config.tableCount);
    }

    static int countingEarning(int timeMinutes, int pricePerHour);
    void occupyTable(int numberOfTable, int currentTime);
    void freeTable(int numberOfTable, int currentTime);
    void handleEvent(Events &event);

    void handleClientComes(Events &events);
    void handleClientSits(Events &event);
    void handleClientWaits(Events &event);
    void handleClientLeaves(Events &events);
    void closeDay(int closeTime);

    bool clientInClub(const std::string& name) const { return clients.find(name) != clients.end(); }
    const Table& getTable(int index) const { return tables[index]; }
    int getTableCount() const { return tables.size(); }
    int getClientTable(const std::string& clientName) const;
    std::string getClientByTable(int tableNumber) const;
};

#endif //COMPCLUB_CLUB_H