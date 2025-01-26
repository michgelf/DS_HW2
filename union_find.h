#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <cassert>
#include "hash_table.h"

template<class T>
class UnionFind {
protected:
    typedef int set_id_t;
    HashTable<T> m_sets; // maps roots to set descriptions
    HashTable<size_t> m_sizes; // maps roots to sizes of their trees
    HashTable<set_id_t> m_parents; // maps each set to its parent in the trees

public:

    UnionFind() = default;

    virtual ~UnionFind() = default;

    // Copy constructor
    UnionFind(const UnionFind& other) = default;

    // Move constructor
    UnionFind(UnionFind&& other) noexcept = default;

    // Copy assignment operator
    UnionFind& operator=(const UnionFind& other) = default;

    // Move assignment operator
    UnionFind& operator=(UnionFind&& other) noexcept = default;

    // Creates a new set with the given ID and description.
    void makeSet(set_id_t setId, const T& set);

    // Merges two sets and returns the description of the new root set.
    virtual T& unionSets(set_id_t root1, set_id_t root2);

    // get the set description of the root
    T& getRootSet(set_id_t root);

    // find the root of the tree containing setId
    set_id_t findRoot(set_id_t setId);

    // find the set description of the tree containing setId
    T& findSet(set_id_t setId);

    // check if a set has ever existed
    bool hasEverExisted(set_id_t member) const;

};

// Implementation

template<class T>
void UnionFind<T>::makeSet(set_id_t setId, const T& set) {
    if (!hasEverExisted(setId)) {
        m_parents[setId] = setId;
        m_sets[setId] = set;
        m_sizes[setId] = 1;
    }
}

template<class T>
T& UnionFind<T>::unionSets(set_id_t root1, set_id_t root2) {
    assert(m_sizes.contains(root1) && m_sizes.contains(root2));
    if (m_sizes[root1] < m_sizes[root2]) {
        std::swap(root1, root2);
    }
    // root1 holds the bigger tree root
    m_parents[root2] = root1;
    m_sizes[root1] += m_sizes[root2];
    m_sets.remove(root2);
    m_sizes.remove(root2);
    return getRootSet(root1);
}

template<class T>
T& UnionFind<T>::getRootSet(set_id_t root) {
    assert(m_sets.contains(root));
    return m_sets[root];
}

template<class T>
typename UnionFind<T>::set_id_t UnionFind<T>::findRoot(set_id_t setId) {
    if (m_parents[setId] != setId) {
        m_parents[setId] = findRoot(m_parents[setId]); // compress paths
    }
    return m_parents[setId];
}

template<class T>
T& UnionFind<T>::findSet(set_id_t setId) {
    return getRootSet(findRoot(setId));
}

template<class T>
bool UnionFind<T>::hasEverExisted(set_id_t member) const {
    return m_parents.contains(member);
}

#endif // UNION_FIND_H
