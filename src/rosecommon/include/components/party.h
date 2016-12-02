#pragma once

#include <memory>

struct PartyBase;

struct Party {
    Party(std::shared_ptr<PartyBase> party) : party_(party) {}
    Party(const Party &other) : party_(other.party_) {}

    std::shared_ptr<PartyBase> party_;
};
