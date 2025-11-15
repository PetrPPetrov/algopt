// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "program.h"

// Template class Fabric for program generation/manipulation
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
class Fabric {
public:
    using ProgramType = Program<InstructionSet, N, K, T>;
    using InstructionSetType = InstructionSet<N, K, T>;

    // Constructors
    explicit Fabric(unsigned programLen);

    // Access to program length
    unsigned getProgramLen() const noexcept;

    // Generate program from current combination_indices state
    ProgramType generate() const;

    // Move to next combination, returns false if no more combinations
    bool next();

    // Get string representation of current combination
    const std::string& getCombinationStrId() const noexcept;
    
    // Get string representation of the last possible combination
    const std::string& getLastProgramStrId() const noexcept;

private:
    // Combination indices for each program position
    std::vector<std::uint64_t> combination_indices;
    
    // String representation of current combination
    std::string combination_id;
    
    // String representation of the last possible combination
    std::string last_program_str_id;
    
    // Update combination_id from combination_indices
    void updateCombinationId();
    
    // Initialize last_program_str_id
    void initializeLastProgramStrId();
};
