#include "Sequence.h"
#include "Split.h"
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <sstream>
#include <string>
#include <utility>

int main() {
    using IntSequence = Sequence<int, std::string>;

    // A new sequence starts empty.
    IntSequence sequence;
    assert(sequence.empty());
    assert(sequence.size() == 0);

    // Add values and read them by key.
    sequence.push_back(10, "");
    sequence.push_back(20, "");
    sequence.push_back(30, "");
    assert(!sequence.empty());
    assert(sequence.size() == 3);
    assert(sequence.get(10) == 10);
    assert(sequence.get(20) == 20);
    assert(sequence.get(30) == 30);
    // Accessing a missing key must throw an exception.
    bool threw = false;
    try {
        sequence.get(3);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    // Add key-info pairs at either end of the sequence.
    IntSequence pairs;
    pairs.push_back(20, "back");
    pairs.push_front(10, "front");
    pairs.push_back(30, "back again");
    assert(!pairs.empty());
    assert(pairs.size() == 3);
    assert(pairs.get(10) == 10);
    assert(pairs.get(20) == 20);
    assert(pairs.get(30) == 30);
    assert(pairs.insert_after(25, "inserted", 20));
    assert(pairs.getInfo(25) == "inserted");
    assert(pairs.size() == 4);
    assert(!pairs.insert_after(35, "not inserted", 99));
    assert(pairs.size() == 4);

    IntSequence removed;
    removed.push_back(1, "first");
    removed.push_back(2, "middle");
    removed.push_back(3, "last");
    assert(removed.remove_at_key(2));
    assert(removed.size() == 2);
    assert(removed.get(1) == 1);
    assert(removed.get(3) == 3);
    assert(removed.remove_at_key(1));
    assert(removed.remove_at_key(3));
    assert(removed.empty());
    assert(!removed.remove_at_key(99));

    IntSequence appended;
    appended.push_back(40, "appended");
    assert(appended.get(40) == 40);
    assert(appended.getInfo(40) == "appended");

    IntSequence sameValues;
    sameValues.push_back(40, "appended");
    assert(appended == sameValues);
    assert(!(appended != sameValues));
    sameValues.clear();
    sameValues.push_back(40, "different");
    assert(!(appended == sameValues));
    assert(appended != sameValues);

    // Remove values from either end and report whether an element was removed.
    IntSequence popped;
    assert(!popped.pop_front());
    assert(!popped.pop_back());
    popped.push_back(1, "first");
    popped.push_back(2, "middle");
    popped.push_back(3, "last");
    assert(popped.pop_front());
    assert(popped.size() == 2);
    assert(popped.get(2) == 2);
    assert(popped.get(3) == 3);
    assert(popped.pop_back());
    assert(popped.size() == 1);
    assert(popped.get(2) == 2);
    assert(popped.pop_back());
    assert(popped.empty());
    assert(!popped.pop_back());

    // Print each key-info pair on its own line.
    IntSequence printable;
    printable.push_back(1, "one");
    printable.push_back(2, "two");
    std::ostringstream printedOutput;
    std::streambuf* originalOutput = std::cout.rdbuf(printedOutput.rdbuf());
    printable.print();
    std::cout.rdbuf(originalOutput);
    assert(printedOutput.str() == "1: one\n2: two\n");

    // Copying creates an independent sequence with the same values.
    IntSequence copied(sequence);
    assert(copied == sequence);
    assert(copied.size() == sequence.size());

    // Copy assignment replaces the destination contents.
    IntSequence copyAssigned;
    copyAssigned.push_back(99, "");
    copyAssigned = sequence;
    assert(copyAssigned == sequence);

    // Moving transfers the nodes and leaves the source empty.
    IntSequence moved(std::move(sequence));
    assert(sequence.empty());
    assert(moved.size() == 3);
    assert(moved.get(10) == 10);

    IntSequence moveAssigned;
    moveAssigned.push_back(99, "");
    moveAssigned = std::move(moved);
    assert(moved.empty());
    assert(moveAssigned == copied);

    // Swapping exchanges the contents of two sequences.
    IntSequence first;
    IntSequence second;
    first.push_back(1, "");
    first.push_back(2, "");
    second.push_back(3, "");
    second.push_back(4, "");
    first.swap(second);
    assert(first.get(3) == 3);
    assert(second.get(1) == 1);

    // Clearing removes every element.
    first.clear();
    assert(first.empty());
    assert(first.size() == 0);

    // Split the remaining values into alternating chunks.
    IntSequence source;
    for (int value = 1; value <= 10; ++value) {
        source.push_back(value, std::to_string(value));
    }

    IntSequence res1;
    IntSequence res2;
    res1.push_back(100, "");
    res2.push_back(200, "");
    split(source, res1, 2, res2, 3, 2);

    IntSequence expectedRes1;
    expectedRes1.push_back(3, "3");
    expectedRes1.push_back(4, "4");
    expectedRes1.push_back(8, "8");
    expectedRes1.push_back(9, "9");

    IntSequence expectedRes2;
    expectedRes2.push_back(5, "5");
    expectedRes2.push_back(6, "6");
    expectedRes2.push_back(7, "7");
    expectedRes2.push_back(10, "10");

    assert(res1 == expectedRes1);
    assert(res2 == expectedRes2);
    assert(res1.getInfo(3) == "3");
    assert(res2.getInfo(5) == "5");

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