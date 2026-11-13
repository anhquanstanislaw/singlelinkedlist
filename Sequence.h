#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdexcept>
#include <utility>

template<typename Key, typename Info>
class Sequence {
private:
    struct Node {
        Key key;
        Info info;
        Node* next;

        explicit Node(const Key& value)
            : key(value), info(), next(nullptr) {}

        Node(const Key& value, const Info& nodeInfo)
            : key(value), info(nodeInfo), next(nullptr) {}
    };

    unsigned int count;
    Node* head;
    Node* nodeAt(unsigned int index) const;

public:
    Sequence();

    Sequence(const Sequence& other);

    Sequence(Sequence&& other) noexcept;

    ~Sequence();

    Sequence& operator=(const Sequence& other);

    Sequence& operator=(Sequence&& other) noexcept;

    bool operator==(const Sequence& other) const;

    bool operator!=(const Sequence& other) const;

    void append(const Key& key, const Info& info);

    void push_front(const Key& key, const Info& info);

    void push_back(const Key& key, const Info& info);

    bool pop_front();

    bool pop_back();

    void print() const;

    Key get(unsigned int index) const;

    Info getInfo(unsigned int index) const;

    bool empty() const;

    void clear();

    unsigned int size() const;

    void swap(Sequence& other) noexcept;
};

#include "Sequence.tpp"

#endif
