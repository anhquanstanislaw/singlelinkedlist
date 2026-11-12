#include "Sequence.h"
#include "Split.h"
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>

int main() {
    using IntSequence = Sequence<int, std::string>;

    // A new sequence starts empty.
    IntSequence sequence;
    assert(sequence.empty());
    assert(sequence.size() == 0);

    // Add values and read them by index.
    sequence.append(10);
    sequence.append(20);
    sequence.append(30);
    assert(!sequence.empty());
    assert(sequence.size() == 3);
    assert(sequence.get(0) == 10);  
    assert(sequence.get(1) == 20);
    assert(sequence.get(2) == 30);
    // Accessing an invalid index must throw an exception.
    bool threw = false;
    try {
        sequence.get(3);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    // Copying creates an independent sequence with the same values.
    IntSequence copied(sequence);
    assert(copied == sequence);
    assert(copied.size() == sequence.size());

    // Copy assignment replaces the destination contents.
    IntSequence copyAssigned;
    copyAssigned.append(99);
    copyAssigned = sequence;
    assert(copyAssigned == sequence);

    // Moving transfers the nodes and leaves the source empty.
    IntSequence moved(std::move(sequence));
    assert(sequence.empty());
    assert(moved.size() == 3);
    assert(moved.get(0) == 10);

    IntSequence moveAssigned;
    moveAssigned.append(99);
    moveAssigned = std::move(moved);
    assert(moved.empty());
    assert(moveAssigned == copied);

    // Swapping exchanges the contents of two sequences.
    IntSequence first;
    IntSequence second;
    first.append(1);
    first.append(2);
    second.append(3);
    second.append(4);
    first.swap(second);
    assert(first.get(0) == 3);
    assert(second.get(0) == 1);

    // Clearing removes every element.
    first.clear();
    assert(first.empty());
    assert(first.size() == 0);

    // Split the remaining values into alternating chunks.
    IntSequence source;
    for (int value = 1; value <= 10; ++value) {
        source.append(value);
    }

    IntSequence res1;
    IntSequence res2;
    res1.append(100);
    res2.append(200);
    split(source, res1, 2, res2, 3, 2);

    IntSequence expectedRes1;
    expectedRes1.append(3);
    expectedRes1.append(4);
    expectedRes1.append(8);
    expectedRes1.append(9);

    IntSequence expectedRes2;
    expectedRes2.append(5);
    expectedRes2.append(6);
    expectedRes2.append(7);
    expectedRes2.append(10);

    assert(res1 == expectedRes1);
    assert(res2 == expectedRes2);

    // Split clears the output sequences when no source values remain.
    split(source, res1, 2, res2, 3, 10);
    assert(res1.empty());
    assert(res2.empty());

    // Invalid chunk lengths and start indexes are rejected.
    bool invalidArgumentsThrew = false;
    try {
        split(source, res1, 0, res2, 2, 0);
    } catch (const std::invalid_argument&) {
        invalidArgumentsThrew = true;
    }
    assert(invalidArgumentsThrew);

    std::cout << "All tests passed!\n";
    return 0;
}