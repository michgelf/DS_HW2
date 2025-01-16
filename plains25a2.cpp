// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"
#include <exception>


Plains::Plains() {

}

Plains::~Plains() {

}

StatusType Plains::add_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (teams.find(teamId) != teams.end()) { /// in our mimush we do null
        return StatusType::FAILURE;
    }
    try {
        teams[teamId] = unique_ptr<Team>(new Team(teamId));
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::add_jockey(int jockeyId, int teamId) {
    if (jockeyId <= 0 || teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (jockeys.find(jockeyId) != jockeys.end() || teams.find(teamId) == teams.end()
        || !teams[teamId]->is_active()) {
        return StatusType::FAILURE;
    }

    try {
        Team* team = teams[teamId].get();
        jockeys[jockeyId] = unique_ptr<Jockey>(new Jockey(jockeyId, team));
        team->num_jockeys++;
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::update_match(int victoriousJockeyId, int losingJockeyId) {
    if (victoriousJockeyId <= 0 || losingJockeyId <= 0 || victoriousJockeyId == losingJockeyId) {
        return StatusType::INVALID_INPUT;
    }
    if (jockeys.find(victoriousJockeyId) == jockeys.end() ||
        jockeys.find(losingJockeyId) == jockeys.end() ||
        findRealTeam(jockeys[victoriousJockeyId]->team) ==
        findRealTeam(jockeys[losingJockeyId]->team)
            ) {
        return StatusType::FAILURE;
    }

    addJockeyRecord(jockeys[victoriousJockeyId].get(), 1);
    addJockeyRecord(jockeys[losingJockeyId].get(), -1);

    return StatusType::SUCCESS;
}

StatusType Plains::merge_teams(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    if (teams.find(teamId1) == teams.end() ||
        teams.find(teamId2) == teams.end() ||
        !teams[teamId1]->is_active() ||
        !teams[teamId2]->is_active()) {
        return StatusType::FAILURE;
    }

    unionTeams(teams[teamId1].get(), teams[teamId2].get());
    return StatusType::SUCCESS;

}

StatusType Plains::unite_by_record(int record) {
    if (record <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (records.find(record) == records.end() || records.find(-record) == records.end() ||
        records[record].size() != 1 || records[-record].size() != 1) {
        return StatusType::FAILURE;
    }

    unionTeams(records[record].begin()->second, records[-record].begin()->second);
    return StatusType::SUCCESS;
}

output_t<int> Plains::get_jockey_record(int jockeyId) {
    if (jockeyId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (jockeys.find(jockeyId) == jockeys.end()) {
        return StatusType::FAILURE;
    }

    return jockeys[jockeyId]->record;
}

output_t<int> Plains::get_team_record(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (teams.find(teamId) == teams.end() || !teams[teamId]->is_active()) {
        return StatusType::FAILURE;
    }

    return teams[teamId]->record;
}

// private methods


Team* Plains::findRealTeam(Team* team) {
    // find the root of the tree
    Team* realTeam = team;
    while (realTeam->merged_to != nullptr) {
        realTeam = realTeam->merged_to;
    }

    // compress the paths of the tree
    Team* currTeam = team;
    while (currTeam != realTeam) {
        Team* tmp = currTeam->merged_to;
        currTeam->merged_to = realTeam;
        currTeam = tmp;
    }
    return realTeam;
}

void Plains::addTeamRecord(Team* team, int record) {
    if (record == 0) {
        return;
    }

    // remove the team from the old record
    if (team->record != 0) {
        records[team->record].erase(team->id);
        if (records[team->record].empty()) {
            records.erase(team->record);
        }
    }

    team->record += record;

    // add the team to the new record
    if (team->record != 0) {
        records[team->record][team->id] = team;
    }
}


void Plains::unionTeams(Team* team1, Team* team2) {
    // find which one the bigger team
    Team* bigTeam = team1, * smallTeam = team2;
    if (team2->num_jockeys > team1->num_jockeys) {
        bigTeam = team2;
        smallTeam = team1;
    }

    // find the new id
    int newId = team1->id;
    if (team2->record > team1->record) {
        newId = team2->id;
    }

    // swap the teams if needed
    if (newId != bigTeam->id) {
        std::swap(teams[bigTeam->id], teams[smallTeam->id]);
        std::swap(smallTeam->id, bigTeam->id);
        std::swap(smallTeam->record, bigTeam->record);
    }

    addTeamRecord(bigTeam, smallTeam->record);
    addTeamRecord(smallTeam, -smallTeam->record); // deletes the record of the small team
    smallTeam->merged_to = bigTeam;
    bigTeam->num_jockeys += smallTeam->num_jockeys;
}

void Plains::addJockeyRecord(Jockey* jockey, int record) {
    jockey->record += record;
    Team* team = findRealTeam(jockey->team);
    addTeamRecord(team, record);
}