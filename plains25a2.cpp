// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a2.h"


Plains::Plains() {}

Plains::~Plains() {

}

StatusType Plains::add_team(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (m_teams.hasEverExisted(teamId)) {
        return StatusType::FAILURE;
    }
    m_teams.makeSet(teamId);
    return StatusType::SUCCESS;
}

StatusType Plains::add_jockey(int jockeyId, int teamId) {
    if (jockeyId <= 0 || teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (m_jockeys.contains(jockeyId) || !m_teams.contains(teamId)) {
        return StatusType::FAILURE;
    }

    m_jockeys[jockeyId] = Jockey(teamId);
    return StatusType::SUCCESS;

}

StatusType Plains::update_match(int victoriousJockeyId, int losingJockeyId) {
    if (victoriousJockeyId <= 0 || losingJockeyId <= 0 || victoriousJockeyId == losingJockeyId) {
        return StatusType::INVALID_INPUT;
    }
    // Ensure both jockeys exist and are in different teams.
    if (!m_jockeys.contains(victoriousJockeyId) || !m_jockeys.contains(losingJockeyId) ||
        m_teams.findRoot(m_jockeys[victoriousJockeyId].teamId) ==
        m_teams.findRoot(m_jockeys[losingJockeyId].teamId)) {
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

    if (!m_teams.contains(teamId1) || !m_teams.contains(teamId2)) {
        return StatusType::FAILURE;
    }

    unionTeams(teamId1, teamId2);
    return StatusType::SUCCESS;
}

StatusType Plains::unite_by_record(int record) {
    if (record <= 0) {
        return StatusType::INVALID_INPUT;
    }
    // ensure the record exists and has exactly one team for each sign.
    if (!m_records.contains(record) || !m_records.contains(-record) ||
        m_records[record].size() != 1 ||
        m_records[-record].size() != 1) {
        return StatusType::FAILURE;
    }

    unionTeams(m_records[record].front(), m_records[-record].front());
    return StatusType::SUCCESS;
}

output_t<int> Plains::get_jockey_record(int jockeyId) {
    if (jockeyId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (!m_jockeys.contains(jockeyId)) {
        return StatusType::FAILURE;
    }

    return m_jockeys[jockeyId].record;
}


output_t<int> Plains::get_team_record(int teamId) {
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (!m_teams.contains(teamId)) {
        return StatusType::FAILURE;
    }

    return m_teams.findSet(teamId).record;
}

// private methods

void Plains::eraseFromRecords(Team &team) {
    if (team.record != 0) {
        m_records[team.record].remove(team.id);
        if (m_records[team.record].empty()) {
            m_records.remove(team.record);
        }
    }
}

void Plains::addTeamRecord(Team &team, int recordToAdd) {
    if (recordToAdd == 0) {
        return;
    }

    int oldRecord = team.record;
    eraseFromRecords(team);
    int newRecord = oldRecord + recordToAdd;
    team.record = newRecord;
    if (newRecord != 0) {
        m_records[newRecord][team.id] = team.id;
    }
}

void Plains::addJockeyRecord(int jockeyId, int recordToAdd) {
    m_jockeys[jockeyId].record += recordToAdd;
    Team &team = m_teams.findSet(m_jockeys[jockeyId].teamId);
    addTeamRecord(team, recordToAdd);
}

void Plains::unionTeams(int teamId1, int teamId2) {
    int root1 = m_teams.findRoot(teamId1);
    int root2 = m_teams.findRoot(teamId2);
    Team &t1 = m_teams.getRootSet(root1);
    Team &t2 = m_teams.getRootSet(root2);
    eraseFromRecords(t1);
    eraseFromRecords(t2);
    Team &mergedTeam = m_teams.unionSets(root1, root2);
    if (mergedTeam.record != 0) {
        m_records[mergedTeam.record][mergedTeam.id] = mergedTeam.id;
    }
}

