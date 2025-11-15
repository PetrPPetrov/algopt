// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <cassert>
#include <cstddef>
#include <variant>
#include <vector>
#include "full_state.h"

// Template class for program working with any variant type of instructions
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
class Program {
public:
    using InstructionSetType = InstructionSet<N, K, T>;
    using Instructions = std::vector<InstructionSetType>;
    using size_type = typename Instructions::size_type;
    using iterator = typename Instructions::iterator;
    using const_iterator = typename Instructions::const_iterator;

    // Constructors
    Program() = default;
    explicit Program(size_type capacity);

    // Add instruction
    template<typename Instruction>
    void add(Instruction&& inst);

    // Access to instructions
    InstructionSetType& operator[](size_type index);
    const InstructionSetType& operator[](size_type index) const;

    // Program size
    size_type size() const noexcept;
    bool empty() const noexcept;

    // Iterators
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    // Clear
    void clear() noexcept;

    // Reserve memory
    void reserve(size_type capacity);

    // Swap
    void swap(Program& other) noexcept;

    // Execute current instruction
    // Returns false if program is finished (instruction_pointer >= size), true otherwise
    bool execute(FullState<N, K, T>& full_state) const;
    
    // Dump program as text representation
    std::string dump() const;

private:
    Instructions instructions;
};
