// 
// 234218 Data Structures 1.
// Semester: 2025A (Winter).
// Wet Exercise #1.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT remove or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef PLAINS25WINTER_WET1_H_
#define PLAINS25WINTER_WET1_H_

#include "wet2util.h"

#include "hash_table.h"
#include "union_find_teams.h"


class Plains {
private:

    struct Jockey {
        int teamId;
        int record;

        Jockey() = default;

        Jockey(int teamId) : teamId(teamId), record(0) {}
    };

    HashTable<Jockey> m_jockeys;

    UnionFindTeams m_teams;

    HashTable<HashTable<int>> m_records;

    /**
    * @brief Adds a record to a jockey
    * @param jockeyId The id of the jockey
    * @param recordToAdd The record to add
    */
    void addJockeyRecord(int jockeyId, int recordToAdd);

    /**
     * @brief Adds a record to a team and updates the records hash table accordingly
     * @param team The team to add the record to
     * @param recordToAdd The record to add
     */
    void addTeamRecord(Team& team, int recordToAdd);

    /**
    * @brief Erases a team from the records hash table
    * @param team The team to erase
    */
    void eraseFromRecords(Team& team);

    /**
    * @brief Unions two teams and updates the records hash table accordingly
    * @param team1 The id of the first team
    * @param team2 The id of the second team
    */
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
