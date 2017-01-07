#pragma once

#include <memory>

struct PartyBase;

struct Party {
    Party() : isRequested_(false), isKicked_(false) {}
    Party(std::shared_ptr<PartyBase> party) : isRequested_(false), isKicked_(false), party_(party) {}
    Party(const Party &other) : isRequested_(false), isKicked_(false), party_(other.party_) {}

    bool isRequested_;
    bool isKicked_;
    std::shared_ptr<PartyBase> party_;
};
