#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Club.h"
#include "Parser.h"

TEST_CASE("Parser of time") {
    std::string t1 = "09:00";
    std::string t2 = "23:59";
    std::string t3 = "25:00";

    CHECK(parseTime(t1) == 540);
    CHECK(parseTime(t2) == 1439);
    CHECK(parseTime(t3) == -1);
    CHECK(formatTime(540) == "09:00");
}

TEST_CASE("Client arrived") {
    Club club(Config{3, 540, 1140, 10});
    Events e;
    e.time = 600;
    e.id = 1;
    e.name = {"client1"};

    club.handleEvent(e);
    CHECK(club.clientInClub("client1") == true);
}

TEST_CASE("Client sits") {
    Club club(Config{3, 540, 1140, 10});
    Events e1; e1.time=600; e1.id=1; e1.name={"client1"}; club.handleEvent(e1);
    Events e2; e2.time=610; e2.id=2; e2.name={"client1", "1"}; club.handleEvent(e2);

    const Table& table = club.getTable(0);
    CHECK(table.isBusy == true);
    CHECK(club.getClientTable("client1") == 1);
}

TEST_CASE("Error: unknown client") {
    Club club(Config{3, 540, 1140, 10});

    Events e;
    e.time = 610;
    e.id = 2;
    e.name = {"unknown", "1"};

    club.handleEvent(e);

    const Table& table = club.getTable(0);
    CHECK(table.isBusy == false);
}

TEST_CASE("Queue") {
    Club club(Config{2, 540, 1140, 10});

    Events e;
    e.time=600; e.id=1; e.name={"c1"}; club.handleEvent(e);
    e.time=605; e.id=2; e.name={"c1", "1"}; club.handleEvent(e);
    e.time=610; e.id=1; e.name={"c2"}; club.handleEvent(e);
    e.time=615; e.id=2; e.name={"c2", "2"}; club.handleEvent(e);

    e.time=700; e.id=1; e.name={"c3"}; club.handleEvent(e);
    e.time=705; e.id=3; e.name={"c3"}; club.handleEvent(e);

    e.time=800; e.id=4; e.name={"c1"}; club.handleEvent(e);

    const Table& table = club.getTable(0);
    CHECK(table.isBusy == true);
    CHECK(club.getClientByTable(1) == "c3");
}