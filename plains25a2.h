// 
// 234218 Data Structures 1.
// Semester: 2025A (Winter).
// Wet Exercise #1.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef PLAINS25WINTER_WET1_H_
#define PLAINS25WINTER_WET1_H_

#include "wet2util.h"

#include "unordered_map"
#include "union_find_teams.h"


//struct Jockey {
//    int record;
//    int teamId;
//
//    Jockey(int teamId) : teamId(teamId), record(0) {}
//};

class Plains {
private:
    //
    // Here you may add anything you want
    //
    unordered_map<int, int> jockeysRecords;
    unordered_map<int, int> jockeysTeams;
    UnionFindTeams teams;
    unordered_map<int, unordered_map<int, int>> records;

    void addJockeyRecord(int, int);

    void addTeamRecord(Team&, int);

    void eraseFromRecords(Team& team);

    void unionTeams(int team1, int team2);

public:
    // <DO-NOT-MODIFY> {
    Plains();

    ~Plains();

    StatusType add_team(int teamId);

    StatusType add_jockey(int jockeyId, int teamId);

    StatusType update_match(int victoriousJockeyId, int losingJockeyId);

    StatusType merge_teams(int teamId1, int teamId2);

    StatusType unite_by_record(int record);

    output_t<int> get_jockey_record(int jockeyId);

    output_t<int> get_team_record(int teamId);
    // } </DO-NOT-MODIFY>
};

#endif // PLAINS25WINTER_WET1_H_
