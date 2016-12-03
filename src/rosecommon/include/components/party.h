#pragma once

#include <memory>

struct PartyBase;

struct Party {
    Party() : isRequested_(false) {}
    Party(std::shared_ptr<PartyBase> party) : isRequested_(false), party_(party) {}
    Party(const Party &other) : isRequested_(false), party_(other.party_) {}

    bool isRequested_;
    std::shared_ptr<PartyBase> party_;
};
