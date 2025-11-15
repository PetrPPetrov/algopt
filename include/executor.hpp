// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <sstream>
#include <string>

// Constructors
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline Executor<InstructionSet, N, K, T>::Executor(const typename Executor<InstructionSet, N, K, T>::ProgramType& program_arg, const InputVariables<N>& input_arg)
    : program(program_arg) {
    // Initialize variables with input variables and zero-initialized output and temp
    Variables<N, K, T> variables(input_arg);
    
    // Initialize full state with variables and instruction pointer 0
    full_state = FullStateType(variables, 0);
}

// Access to program
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline const typename Executor<InstructionSet, N, K, T>::ProgramType& Executor<InstructionSet, N, K, T>::getProgram() const {
    return program;
}

// Access to full state
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Executor<InstructionSet, N, K, T>::FullStateType& Executor<InstructionSet, N, K, T>::getFullState() {
    return full_state;
}

template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline const typename Executor<InstructionSet, N, K, T>::FullStateType& Executor<InstructionSet, N, K, T>::getFullState() const {
    return full_state;
}

// Execute current instruction
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool Executor<InstructionSet, N, K, T>::execute() {
    return program.execute(full_state);
}

// Dump current state: variables and program with current instruction marked
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline std::string Executor<InstructionSet, N, K, T>::dump() const {
    std::ostringstream oss;
    
    // Dump input variables
    oss << "Input variables: [";
    const auto& input = full_state.getVariables().input;
    for (unsigned i = 0; i < N; ++i) {
        oss << static_cast<unsigned>(input.values[i]);
        if (i < N - 1) oss << ", ";
    }
    oss << "]\n";
    
    // Dump output variables
    oss << "Output variables: [";
    const auto& output = full_state.getVariables().output;
    for (unsigned i = 0; i < K; ++i) {
        oss << static_cast<unsigned>(output.values[i]);
        if (i < K - 1) oss << ", ";
    }
    oss << "]\n";
    
    // Dump temp variables
    oss << "Temp variables: [";
    const auto& temp = full_state.getVariables().temp;
    for (unsigned i = 0; i < T; ++i) {
        oss << static_cast<unsigned>(temp.values[i]);
        if (i < T - 1) oss << ", ";
    }
    oss << "]\n";
    
    // Dump instruction pointer
    oss << "Instruction pointer: " << full_state.getInstructionPointer() << "\n\n";
    
    // Dump program with current instruction marked
    oss << "Program:\n";
    const std::size_t current_ip = full_state.getInstructionPointer();
    for (std::size_t i = 0; i < program.size(); ++i) {
        if (i == current_ip) {
            oss << "=> ";
        } else {
            oss << "   ";
        }
        oss << program[i].dump(i) << "\n";
    }
    
    return oss.str();
}
