#pragma once

#include <vector>
#include <string>

namespace Core {

struct FuzzyMatch {
    size_t index;
    int score;
};

// returns a sorted vector of matches
std::vector<FuzzyMatch> fuzzy_match(const std::vector<std::string>& strings, const std::string& pattern);

}
