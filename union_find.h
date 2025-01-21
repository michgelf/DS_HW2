//
// Created by michg on 1/17/2025.
//

#include <cassert>
#include "hash_table.h"

#ifndef C_SCRIPTS_UNION_FIND_H
#define C_SCRIPTS_UNION_FIND_H

template<class Set>
class UnionFind {
protected:
    typedef int set_id_t;
    HashTable<Set> sets; // maps roots to sets descriptions
    HashTable<size_t> sizes; // maps roots to sizes of their trees
    HashTable<set_id_t> parents; // maps each set to its parent in the tree
public:

    UnionFind() = default;

    virtual ~UnionFind() = default;

    UnionFind(const UnionFind& other) = default;

    UnionFind(UnionFind&& other) noexcept = default;

    UnionFind& operator=(const UnionFind& other) = default;

    UnionFind& operator=(UnionFind&& other) noexcept = default;

    void make_set(set_id_t setId, Set set) {
        if (!hasEverExisted(setId)) {
            parents[setId] = setId;
            sets[setId] = set;
            sizes[setId] = 1;
        }
    }

    virtual Set& union_sets(set_id_t root1, set_id_t root2) {
        assert(sizes.contains(root1) && sizes.contains(root2));
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

    Set& getRootSet(set_id_t root) {
        assert(sets.contains(root));
        return sets[root];
    }

    int findRoot(set_id_t setId) {
        if (parents[setId] != setId) {
            parents[setId] = findRoot(parents[setId]); // compress paths
        }
        return parents[setId];
    }

    Set& find_set(set_id_t setId) {
        return getRootSet(findRoot(setId));
    }

    bool hasEverExisted(set_id_t member) const {
        return parents.contains(member);
    }


};

#endif //C_SCRIPTS_UNION_FIND_H
