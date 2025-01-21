#include "union_find_teams.h"


void UnionFindTeams::makeSet(set_id_t teamId) {
    UnionFind<Team>::makeSet(teamId, Team(teamId));
}

Team& UnionFindTeams::unionSets(set_id_t root1, set_id_t root2) {
    Team& t1 = getRootSet(root1);
    Team& t2 = getRootSet(root2);
    int newId = t2.record > t1.record ? t2.id : t1.id;
    int newRecord = t1.record + t2.record;
    Team& mergedTeam = UnionFind<Team>::unionSets(root1, root2);
    mergedTeam.id = newId;
    mergedTeam.record = newRecord;
    // Compress paths and ensure the merged team's id can be found in O(1)
    findRoot(newId);
    return mergedTeam;
}

bool UnionFindTeams::contains(set_id_t setId) const {
    if (!hasEverExisted(setId)) {
        return false;
    }

    // Check if the set is a root
    if (m_sets.contains(setId) && m_sets[setId].id == setId) {
        return true;
    }

    // Check if the parent is a root
    int parentId = m_parents[setId];
    if (m_sets.contains(parentId) && m_sets[parentId].id == setId) {
        return true;
    }

    return false;
}
