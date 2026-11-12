#ifndef SPLIT_H
#define SPLIT_H

#include "Sequence.h"

template<typename Key, typename Info>
void split(const Sequence<Key, Info>& source,
           Sequence<Key, Info>& res1,
           int length1,
           Sequence<Key, Info>& res2,
           int length2,
           int start);

#include "Split.tpp"

#endif
