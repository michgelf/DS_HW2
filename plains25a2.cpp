// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"


Plains::Plains() {

}

Plains::~Plains() {

}

StatusType Plains::add_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (teams.hasEverExisted(teamId)) {
        return StatusType::FAILURE;
    }

    teams.make_set(teamId);
    return StatusType::SUCCESS;
}

StatusType Plains::add_jockey(int jockeyId, int teamId) {
    if (jockeyId <= 0 || teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (jockeysTeams.contains(jockeyId) || !teams.contains(teamId)) {
        return StatusType::FAILURE;
    }

    jockeysTeams[jockeyId] = teamId;
    jockeysRecords[jockeyId] = 0;
    return StatusType::SUCCESS;

}

StatusType Plains::update_match(int victoriousJockeyId, int losingJockeyId) {
    if (victoriousJockeyId <= 0 || losingJockeyId <= 0 || victoriousJockeyId == losingJockeyId) {
        return StatusType::INVALID_INPUT;
    }

    if (!jockeysTeams.contains(victoriousJockeyId) || !jockeysTeams.contains(losingJockeyId) ||
        teams.findRootId(jockeysTeams[victoriousJockeyId]) ==
        teams.findRootId(jockeysTeams[losingJockeyId])) {
        return StatusType::FAILURE;
    }

    addJockeyRecord(victoriousJockeyId, 1);
    addJockeyRecord(losingJockeyId, -1);

    return StatusType::SUCCESS;
}

StatusType Plains::merge_teams(int teamId1, int teamId2) {
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    if (!teams.contains(teamId1) || !teams.contains(teamId2)) {
        return StatusType::FAILURE;
    }

    unionTeams(teamId1, teamId2);
    return StatusType::SUCCESS;
}

StatusType Plains::unite_by_record(int record) {
    if (record <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (!records.contains(record) || !records.contains(-record) || records[record].size() != 1 ||
        records[-record].size() != 1) {
        return StatusType::FAILURE;
    }

    unionTeams(records[record].front(), records[-record].front());
    return StatusType::SUCCESS;
}

output_t<int> Plains::get_jockey_record(int jockeyId) {
    if (jockeyId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (!jockeysRecords.contains(jockeyId)) {
        return StatusType::FAILURE;
    }

    return jockeysRecords[jockeyId];
}


output_t<int> Plains::get_team_record(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (!teams.contains(teamId)) {
        return StatusType::FAILURE;
    }

    return teams.find_set(teamId).record;
}

// private methods

void Plains::eraseFromRecords(Team& team) {
    if (team.record != 0) {
        records[team.record].erase(team.id);
        if (records[team.record].empty()) {
            records.erase(team.record);
        }
    }
}

void Plains::addTeamRecord(Team& team, int recordToAdd) {
    if (recordToAdd == 0) {
        return;
    }

    int oldRecord = team.record;
    eraseFromRecords(team);
    int newRecord = oldRecord + recordToAdd;
    team.record = newRecord;
    if (newRecord != 0) {
        records[newRecord][team.id] = team.id;
    }
}

void Plains::addJockeyRecord(int jockeyId, int recordToAdd) {
    jockeysRecords[jockeyId] += recordToAdd;
    Team& team = teams.find_set(jockeysTeams[jockeyId]);
    addTeamRecord(team, recordToAdd);
}

void Plains::unionTeams(int teamId1, int teamId2) {
    int root1 = teams.findRootId(teamId1);
    int root2 = teams.findRootId(teamId2);
    Team& t1 = teams.getRootSet(root1);
    Team& t2 = teams.getRootSet(root2);
    eraseFromRecords(t1);
    eraseFromRecords(t2);
    Team& mergedTeam = teams.union_sets(root1, root2);
    if (mergedTeam.record != 0) {
        records[mergedTeam.record][mergedTeam.id] = mergedTeam.id;
    }
}

