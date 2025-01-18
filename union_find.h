//
// Created by michg on 1/17/2025.
//


#include <unordered_map>

using namespace std;

#ifndef C_SCRIPTS_UNION_FIND_H
#define C_SCRIPTS_UNION_FIND_H

template<class Set>
class UnionFind {
protected:
    unordered_map<int, Set> sets; // maps roots to sets descriptions
    unordered_map<int, unsigned int> sizes; // maps roots to sizes of sets
    unordered_map<int, int> parents;
public:

    virtual ~UnionFind() = default;

    void make_set(int setId, Set set) {
        if (!hasEverExisted(setId)) {
            parents[setId] = setId;
            sets.insert({setId, set});
            sizes[setId] = 1;
        }
    }

    virtual Set& union_sets(int root1, int root2) {
        if (sizes[root1] < sizes[root2]) {
            swap(root1, root2);
        }
        // root1 holds the bigger tree root
        parents[root2] = root1;
        sizes[root1] += sizes[root2];
        sets.erase(root2);
        sizes.erase(root2);

        return getRootSet(root1);
    }

    Set& getRootSet(int root) {
        return sets.at(root);
    }

    int findRootId(int setId) {
        if (parents[setId] != setId) {
            parents[setId] = findRootId(parents[setId]); // compress paths
        }
        return parents[setId];
    }

    Set& find_set(int setId) {
        return getRootSet(findRootId(setId));
    }

    bool hasEverExisted(int member) {
        return parents.find(member) != parents.end();
    }


    virtual bool contains(int setId) {
        return hasEverExisted(setId) && find_set(setId).id == setId;
    }


};

#endif //C_SCRIPTS_UNION_FIND_H
