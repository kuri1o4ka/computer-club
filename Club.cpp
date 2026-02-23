#include "Club.h"

#include <algorithm>
#include <iostream>
#include <ostream>

void Club::handleEvent(Events& event) {
    switch(event.id) {
        case 1:
            handleClientComes(event);
            break;
        case 2:
            handleClientSits(event);
            break;
        case 3:
            handleClientWaits(event);
            break;
        case 4:
            handleClientLeaves(event);
            break;
    }
}

int Club::countingEarning(int timeMinutes, int pricePerHour) {
    int hours = timeMinutes / 60;
    if (timeMinutes % 60 > 0) {
        hours++;
    }
    return hours * pricePerHour;
}
void Club::occupyTable(int numberOfTable, int currentTime) {
    int index = numberOfTable - 1;
    if (index < 0 or index >= tables.size()) {
        std::cout << formatTime(currentTime) << " 13 InvalidTableNumber" << std::endl;
        return;
    }
    tables[index].isBusy = true;
    tables[index].startTime = currentTime;
}
void Club::freeTable(int numberOfTable, int currentTime) {
    int index = numberOfTable - 1;
    if (index < 0 or index >= tables.size()) {
        std::cout << formatTime(currentTime) << " 13 InvalidTableNumber" << std::endl;
        return;
    }
    tables[index].isBusy = false;
    int totalTimeOfTable = currentTime - tables[index].startTime;
    tables[index].totalTime += totalTimeOfTable;
    tables[index].totalEarning += countingEarning(totalTimeOfTable, config.pricePerHour);
}
void Club::handleClientComes(Events &events) {
    std::string client = events.name[0];
    std::cout << events.line << std::endl;

    if (events.time < config.openTime or events.time >= config.closeTime) {
        std::cout << formatTime(events.time) << " 13 NotOpenYet" << std::endl;
        return;
    }
    if (clients.find(client) != clients.end()) {
        std::cout << formatTime(events.time) << " 13 YouShallNotPass" << std::endl;
        return;
    }
    clients[client] = -1;
}

void Club::handleClientSits(Events &event) {
    std::string client = event.name[0];
    std::cout << event.line << std::endl;
    int newTable;
    try {
        newTable = std::stoi(event.name[1]);
    } catch (const std::exception& ex) {
        std::cout << formatTime(event.time) << " 13 InvalidTableNumber" << std::endl;
        return;
    }
    int index = newTable - 1;

    if (clients.find(client) == clients.end()) {
        std::cout << formatTime(event.time) << " 13 ClientUnknown" << std::endl;
        return;
    }

    int currentTable = clients[client];

    if (currentTable == newTable) {
        std::cout << formatTime(event.time) << " 13 PlaceIsBusy" << std::endl;
        return;
    }
    if (tables[index].isBusy) {
        std::cout << formatTime(event.time) << " 13 PlaceIsBusy" << std::endl;
        return;
    }
    if (currentTable != -1) {
        freeTable(currentTable, event.time);

        if (!waitQueue.empty()) {
            std::string nextClient = waitQueue.front();
            waitQueue.pop();

            occupyTable(currentTable, event.time);
            clients[nextClient] = currentTable;

            std::cout << formatTime(event.time) << " 12 " << nextClient << " " << currentTable << std::endl;
        }
    }
    occupyTable(newTable, event.time);
    clients[client] = newTable;
}
void Club::handleClientWaits(Events &event) {
    std::string client = event.name[0];
    std::cout << event.line << std::endl;
    if (clients.find(client) == clients.end()) {
        std::cout << formatTime(event.time) << " 13 ClientUnknown" << std::endl;
        return;
    }

    bool freeTable = false;
    for (const auto& table : tables) {
        if (!table.isBusy) {
            freeTable = true;
            break;
        }
    }

    if (freeTable) {
        std::cout << formatTime(event.time) << " 13 ICanWaitNoLonger!" << std::endl;
        return;
    }

    if (waitQueue.size() >= tables.size()) {
        std::cout << formatTime(event.time) << " 11 " << client << std::endl;
        return;
    }

    waitQueue.push(client);
}

void Club::handleClientLeaves(Events &event) {
    std::string client = event.name[0];
    std::cout << event.line << std::endl;
    if (clients.find(client) == clients.end()){
        std::cout << formatTime(event.time) << " 13 ClientUnknown" << std::endl;
        return;
    }
    int tableNumber = clients[client];

    if (tableNumber != -1) {
        freeTable(tableNumber, event.time);
        if (!waitQueue.empty()) {
            std::string nextClient = waitQueue.front();
            waitQueue.pop();

            occupyTable(tableNumber, event.time);
            clients[nextClient] = tableNumber;

            std::cout << formatTime(event.time) << " 12 " << nextClient << " " << tableNumber << std::endl;
        }
    }
    clients.erase(client);
}

void Club::closeDay(int closeTime) {
    std::vector<std::string> lastClients;
    for (const auto& client : clients) {
        lastClients.push_back(client.first);
    }

    std::sort(lastClients.begin(), lastClients.end());

    for (const auto& client : lastClients) {
        int tableNumber = clients[client];
        if (tableNumber != -1) {
            freeTable(tableNumber, closeTime);
        }
        std::cout << formatTime(closeTime) << " 11 " << client << std::endl;
    }

    std::cout << formatTime(closeTime) << std::endl;

    for (int i = 0; i < getTableCount(); i++) {
        Table table = getTable(i);
        std::cout << (i + 1) << " " << table.totalEarning << " " << formatTime(table.totalTime) << std::endl;
    }
}

int Club::getClientTable(const std::string& clientName) const{
    auto it = clients.find(clientName);
    if (it != clients.end()) {
        return it->second;
    }
    return -1;
}

std::string Club::getClientByTable(int tableNumber) const {
    for (const auto& [client, table] : clients) {
        if (table == tableNumber) {
            return client;
        }
    }
    return "";
}