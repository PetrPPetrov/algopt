// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#include "fabric.h"
#include <variant>
#include <sstream>
#include <string>

// Constructors
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline Fabric<InstructionSet, N, K, T>::Fabric(unsigned programLen)
    : combination_indices(programLen, 0) {
    updateCombinationId();
    initializeLastProgramStrId();
}

// Access to program length
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline unsigned Fabric<InstructionSet, N, K, T>::getProgramLen() const noexcept {
    return static_cast<unsigned>(combination_indices.size());
}

// Generate program from current combination_indices state
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Fabric<InstructionSet, N, K, T>::ProgramType Fabric<InstructionSet, N, K, T>::generate() const {
    ProgramType program;
    
    // For each position in the program
    for (std::uint64_t combination_index : combination_indices) {
        // Use InstructionSet::getCombination to generate the instruction
        InstructionSetType instruction = InstructionSetType::getCombination(combination_index, getProgramLen());
        
        // Add the instruction to the program
        std::visit([&program](auto&& inst) {
            program.add(inst);
        }, static_cast<typename InstructionSetType::Base&>(instruction));
    }
    
    return program;
}

// Move to next combination
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool Fabric<InstructionSet, N, K, T>::next() {
    if (combination_indices.empty()) {
        return false;
    }
    
    const std::uint64_t max_combinations = InstructionSetType::getCombinationCount(getProgramLen());
    
    // Increment combination indices like a number in base max_combinations
    // Start from the last position
    for (std::size_t i = combination_indices.size(); i > 0; --i) {
        std::size_t pos = i - 1;
        ++combination_indices[pos];
        
        // If this position hasn't overflowed, we're done
        if (combination_indices[pos] < max_combinations) {
            updateCombinationId();
            return true;
        }
        
        // Otherwise, reset this position to 0 and carry over to the next position
        combination_indices[pos] = 0;
    }
    
    // All positions have overflowed, no more combinations
    updateCombinationId();
    return false;
}

// Get string representation of current combination
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline const std::string& Fabric<InstructionSet, N, K, T>::getCombinationStrId() const noexcept {
    return combination_id;
}

// Update combination_id from combination_indices
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline void Fabric<InstructionSet, N, K, T>::updateCombinationId() {
    std::ostringstream oss;
    oss << "[";
    for (std::size_t i = 0; i < combination_indices.size(); ++i) {
        oss << "0x" << std::hex << combination_indices[i];
        if (i < combination_indices.size() - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    combination_id = oss.str();
}

// Get string representation of the last possible combination
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline const std::string& Fabric<InstructionSet, N, K, T>::getLastProgramStrId() const noexcept {
    return last_program_str_id;
}

// Initialize last_program_str_id
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline void Fabric<InstructionSet, N, K, T>::initializeLastProgramStrId() {
    if (combination_indices.empty()) {
        last_program_str_id = "[]";
        return;
    }
    
    const std::uint64_t max_combinations = InstructionSetType::getCombinationCount(getProgramLen());
    const std::uint64_t last_index = (max_combinations > 0) ? max_combinations - 1 : 0;
    
    std::ostringstream oss;
    oss << "[";
    for (std::size_t i = 0; i < combination_indices.size(); ++i) {
        oss << "0x" << std::hex << last_index;
        if (i < combination_indices.size() - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    last_program_str_id = oss.str();
}

