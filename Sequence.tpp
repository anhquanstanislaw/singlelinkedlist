#include <iostream>

template<typename Key, typename Info>
// Creates an empty sequence.
Sequence<Key, Info>::Sequence() : count(0), head(nullptr) {}

template<typename Key, typename Info>
// Creates a deep copy of another sequence.
Sequence<Key, Info>::Sequence(const Sequence& other) : count(0), head(nullptr) {
    for (Node* node = other.head; node != nullptr; node = node->next) {
        Node* copiedNode = new Node(node->key, node->info);
        if (head == nullptr) {
            head = copiedNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = copiedNode;
        }
        ++count;
    }
}

template<typename Key, typename Info>
// Transfers ownership of the nodes from another sequence.
Sequence<Key, Info>::Sequence(Sequence&& other) noexcept
    : count(other.count), head(other.head) {
    other.count = 0;
    other.head = nullptr;
}

template<typename Key, typename Info>
// Releases all nodes owned by the sequence.
Sequence<Key, Info>::~Sequence() {
    clear();
}

template<typename Key, typename Info>
// Replaces this sequence with a deep copy of another sequence.
Sequence<Key, Info>& Sequence<Key, Info>::operator=(const Sequence& other) {
    if (this == &other) {
        return *this;
    }

    Sequence temp(other);
    swap(temp);
    return *this;
}

template<typename Key, typename Info>
// Releases the current nodes and takes ownership of another sequence's nodes.
Sequence<Key, Info>& Sequence<Key, Info>::operator=(Sequence&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    clear();
    count = other.count;
    head = other.head;
    other.count = 0;
    other.head = nullptr;
    return *this;
}

template<typename Key, typename Info>
// Compares the keys, info values, and lengths of two sequences.
bool Sequence<Key, Info>::operator==(const Sequence& other) const {
    Node* thisNode = head;
    Node* otherNode = other.head;

    while (thisNode != nullptr && otherNode != nullptr) {
        if (thisNode->key != otherNode->key ||
            thisNode->info != otherNode->info) {
            return false;
        }

        thisNode = thisNode->next;
        otherNode = otherNode->next;
    }

    return thisNode == nullptr && otherNode == nullptr;
}

template<typename Key, typename Info>
// Compares two sequences for inequality.
bool Sequence<Key, Info>::operator!=(const Sequence& other) const {
    return !(*this == other);
}

template<typename Key, typename Info>
// Adds a key-info pair to the end of the sequence.
void Sequence<Key, Info>::append(const Key& key, const Info& info) {
    Node* node = new Node(key, info);

    if (head == nullptr) {
        head = node;
    } else {
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = node;
    }
    ++count;
}

template<typename Key, typename Info>
// Adds a key-info pair to the beginning of the sequence.
void Sequence<Key, Info>::push_front(const Key& key, const Info& info) {
    Node* node = new Node(key, info);
    node->next = head;
    head = node;
    ++count;
}

template<typename Key, typename Info>
// Adds a key-info pair to the end of the sequence.
void Sequence<Key, Info>::push_back(const Key& key, const Info& info) {
    append(key, info);
}

template<typename Key, typename Info>
// Inserts a key-info pair after the first node with the requested key.
bool Sequence<Key, Info>::insert_after(const Key& key,
                                       const Info& info,
                                       const Key& afterKey) {
    Node* target = nodeAtKey(afterKey);
    if (target == nullptr) {
        return false;
    }

    Node* node = new Node(key, info);
    node->next = target->next;
    target->next = node;
    ++count;
    return true;
}

template<typename Key, typename Info>
// Removes the first node with the requested key.
bool Sequence<Key, Info>::remove_at_key(const Key& key) {
    Node* previous = nullptr;
    Node* current = head;

    while (current != nullptr && current->key != key) {
        previous = current;
        current = current->next;
    }
    if (current == nullptr) {
        return false;
    }

    if (previous == nullptr) {
        head = current->next;
    } else {
        previous->next = current->next;
    }

    delete current;
    --count;
    return true;
}

template<typename Key, typename Info>
// Removes the first node, returning false when the sequence is empty.
bool Sequence<Key, Info>::pop_front() {
    if (head == nullptr) {
        return false;
    }

    Node* node = head;
    head = head->next;
    delete node;
    --count;
    return true;
}

template<typename Key, typename Info>
// Removes the last node, returning false when the sequence is empty.
bool Sequence<Key, Info>::pop_back() {
    if (head == nullptr) {
        return false;
    }

    if (head->next == nullptr) {
        delete head;
        head = nullptr;
        --count;
        return true;
    }

    Node* current = head;
    while (current->next->next != nullptr) {
        current = current->next;
    }

    delete current->next;
    current->next = nullptr;
    --count;
    return true;
}

template<typename Key, typename Info>
// Prints each key-info pair on its own line.
void Sequence<Key, Info>::print() const {
    for (Node* current = head; current != nullptr; current = current->next) {
        std::cout << current->key << ": " << current->info << '\n';
    }
}

template<typename Key, typename Info>
// Returns the key identified by the requested key or throws if it is missing.
Key Sequence<Key, Info>::get(const Key& key) const {
    Node* node = nodeAtKey(key);
    if (node == nullptr) {
        throw std::out_of_range("Key not found");
    }
    return node->key;
}

template<typename Key, typename Info>
// Returns the info identified by the requested key or throws if it is missing.
Info Sequence<Key, Info>::getInfo(const Key& key) const {
    Node* node = nodeAtKey(key);
    if (node == nullptr) {
        throw std::out_of_range("Key not found");
    }
    return node->info;
}

template<typename Key, typename Info>
// Checks whether the sequence contains no elements.
bool Sequence<Key, Info>::empty() const {
    return count == 0;
}

template<typename Key, typename Info>
// Deletes all nodes and resets the sequence to empty.
void Sequence<Key, Info>::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    count = 0;
}

template<typename Key, typename Info>
// Returns the number of elements in the sequence.
unsigned int Sequence<Key, Info>::size() const {
    return count;
}

template<typename Key, typename Info>
// Exchanges the contents of two sequences.
void Sequence<Key, Info>::swap(Sequence& other) noexcept {
    std::swap(count, other.count);
    std::swap(head, other.head);
}

template<typename Key, typename Info>
// Finds the first node with the requested key, or returns nullptr if it is missing.
typename Sequence<Key, Info>::Node*
Sequence<Key, Info>::nodeAtKey(const Key& key) const {
    for (Node* current = head; current != nullptr; current = current->next) {
        if (current->key == key) {
            return current;
        }
    }
    return nullptr;
}

template<typename Key, typename Info>
// Finds the node at a positional offset for internal sequence operations.
typename Sequence<Key, Info>::Node*
Sequence<Key, Info>::nodeAtIndex(unsigned int index) const {
    if (index >= count) {
        return nullptr;
    }

    Node* current = head;
    for (unsigned int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current;
}
