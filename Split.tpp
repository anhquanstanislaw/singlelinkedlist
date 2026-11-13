#include <stdexcept>

template<typename Key, typename Info>
void split(const Sequence<Key, Info>& source,
           Sequence<Key, Info>& res1,
           int length1,
           Sequence<Key, Info>& res2,
           int length2,
           int start) {
    if (length1 <= 0 || length2 <= 0) {
        throw std::invalid_argument("Chunk lengths must be positive");
    }
    if (start < 0) {
        throw std::invalid_argument("Start index cannot be negative");
    }

    // Copy first so the function also works when an output aliases source.
    Sequence<Key, Info> input(source);
    res1.clear();
    res2.clear();

    unsigned int sourceIndex = static_cast<unsigned int>(start);
    if (sourceIndex >= input.size()) {
        sourceIndex = input.size();
    }
    bool writingToFirst = true;

    while (sourceIndex < input.size()) {
        int chunkLength;
        if (writingToFirst) {
            chunkLength = length1;
        } else {
            chunkLength = length2;
        }

        for (int i = 0; i < chunkLength && sourceIndex < input.size(); ++i) {
            if (writingToFirst) {
                res1.push_back(input.get(sourceIndex), input.getInfo(sourceIndex));
            } else {
                res2.push_back(input.get(sourceIndex), input.getInfo(sourceIndex));
            }
            ++sourceIndex;
        }

        writingToFirst = !writingToFirst;
    }
}
