// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

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
