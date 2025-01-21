//
// Created by michg on 1/17/2025.
//

#ifndef C_SCRIPTS_UNION_FIND_TEAMS_H
#define C_SCRIPTS_UNION_FIND_TEAMS_H

#include "union_find.h"


struct Team {
    int id;
    int record;

    Team() : id(0), record(0) {}

    Team(int id) : id(id), record(0) {}
};


class UnionFindTeams : public UnionFind<Team> {

public:

    void make_set(int teamId) {
        UnionFind<Team>::make_set(teamId, Team(teamId));
    }

    Team& union_sets(int root1, int root2) override {
        Team& t1 = getRootSet(root1);
        Team& t2 = getRootSet(root2);
        int newId = t2.record > t1.record ? t2.id : t1.id;
        int newRecord = t1.record + t2.record;
        Team& mergedTeam = UnionFind<Team>::union_sets(root1, root2);
        mergedTeam.id = newId;
        mergedTeam.record = newRecord;
        // compresses paths and ensures that finding the team with the id of the mergedTeam is O(1)
        findRoot(newId);
        return mergedTeam;
    }

    // todo consider make it const
    bool contains(int setId) {
        if (!hasEverExisted(setId)) {
            return false;
        }

        // check if the set is a root
        if (sets.contains(setId) && sets[setId].id == setId) {
            return true;
        }

        // check if the parent is a root
        int parentId = parents[setId];
        if (sets.contains(parentId) && sets[parentId].id == setId) {
            return true;
        }

        return false;
    }


};


#endif //C_SCRIPTS_UNION_FIND_TEAMS_H
