#ifndef UNION_FIND_TEAMS_H
#define UNION_FIND_TEAMS_H

#include "union_find.h"

struct Team {
    int id;
    int record;

    Team() = default;

    Team(int id) : id(id), record(0) {}
};


class UnionFindTeams : public UnionFind<Team> {

public:
    void makeSet(set_id_t teamId);

    Team& unionSets(set_id_t root1, set_id_t root2) override;

    bool contains(int setId) const;
};


#endif // UNION_FIND_TEAMS_H
