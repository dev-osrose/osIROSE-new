#include "fuzzy_matching.h"
#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fts.h"

#include <algorithm>

std::vector<Core::FuzzyMatch> Core::fuzzy_match(const std::vector<std::string>& strings, const std::string& pattern) {
    std::vector<Core::FuzzyMatch> scores;

    for (size_t i = 0; i < strings.size(); ++i) {
        int score = 0;
        if (fts::fuzzy_match(pattern.c_str(), strings[i].c_str(), score))
            scores.push_back({i, score});
    }

    std::sort(scores.begin(), scores.end(), [](const Core::FuzzyMatch& a, const Core::FuzzyMatch& b) { return a.score > b.score; });
    return scores;
}
