// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <sstream>
#include <string>

// Constructors
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline Program<InstructionSet, N, K, T>::Program(typename Program<InstructionSet, N, K, T>::size_type capacity) {
    instructions.reserve(capacity);
}

// Add instruction
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
template<typename Instruction>
inline void Program<InstructionSet, N, K, T>::add(Instruction&& inst) {
    instructions.emplace_back(std::forward<Instruction>(inst));
}

// Access to instructions
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Program<InstructionSet, N, K, T>::InstructionSetType& Program<InstructionSet, N, K, T>::operator[](typename Program<InstructionSet, N, K, T>::size_type index) {
    assert(index < instructions.size());
    return instructions[index];
}

template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline const typename Program<InstructionSet, N, K, T>::InstructionSetType& Program<InstructionSet, N, K, T>::operator[](typename Program<InstructionSet, N, K, T>::size_type index) const {
    assert(index < instructions.size());
    return instructions[index];
}

// Program size
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Program<InstructionSet, N, K, T>::size_type Program<InstructionSet, N, K, T>::size() const noexcept {
    return instructions.size();
}

template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool Program<InstructionSet, N, K, T>::empty() const noexcept {
    return instructions.empty();
}

// Iterators
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Program<InstructionSet, N, K, T>::iterator Program<InstructionSet, N, K, T>::begin() noexcept {
    return instructions.begin();
}

template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Program<InstructionSet, N, K, T>::iterator Program<InstructionSet, N, K, T>::end() noexcept {
    return instructions.end();
}

template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Program<InstructionSet, N, K, T>::const_iterator Program<InstructionSet, N, K, T>::begin() const noexcept {
    return instructions.begin();
}

template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Program<InstructionSet, N, K, T>::const_iterator Program<InstructionSet, N, K, T>::end() const noexcept {
    return instructions.end();
}

template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Program<InstructionSet, N, K, T>::const_iterator Program<InstructionSet, N, K, T>::cbegin() const noexcept {
    return instructions.cbegin();
}

template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Program<InstructionSet, N, K, T>::const_iterator Program<InstructionSet, N, K, T>::cend() const noexcept {
    return instructions.cend();
}

// Clear
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline void Program<InstructionSet, N, K, T>::clear() noexcept {
    instructions.clear();
}

// Reserve memory
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline void Program<InstructionSet, N, K, T>::reserve(typename Program<InstructionSet, N, K, T>::size_type capacity) {
    instructions.reserve(capacity);
}

// Swap
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline void Program<InstructionSet, N, K, T>::swap(Program& other) noexcept {
    instructions.swap(other.instructions);
}

// Execute current instruction
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool Program<InstructionSet, N, K, T>::execute(FullState<N, K, T>& full_state) const {
    if (full_state.instructionPointer() >= instructions.size()) {
        return false;
    }
    
    auto& instruction = const_cast<InstructionSetType&>(instructions[full_state.instructionPointer()]);
    std::visit([&full_state](auto& inst) {
        inst.execute(full_state);
    }, static_cast<typename InstructionSetType::Base&>(instruction));
    
    // Check instruction pointer after execution, as instruction may have modified it
    return full_state.instructionPointer() < instructions.size();
}

// Dump program as text representation
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline std::string Program<InstructionSet, N, K, T>::dump() const {
    std::ostringstream oss;
    for (size_type i = 0; i < instructions.size(); ++i) {
        oss << instructions[i].dump(i) << "\n";
    }
    return oss.str();
}
