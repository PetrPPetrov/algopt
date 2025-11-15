// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include "full_state.h"
#include "program.h"
#include "variables.h"

// Template class for executing programs on variables
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
class Executor {
public:
    using ProgramType = Program<InstructionSet, N, K, T>;
    using FullStateType = FullState<N, K, T>;

    // Constructors
    explicit Executor(const ProgramType& program_arg, const InputVariables<N>& input_arg);

    // Access to program
    const ProgramType& getProgram() const;

    // Access to full state
    FullStateType& getFullState();
    const FullStateType& getFullState() const;

    // Execute current instruction
    // Returns false if program is finished, true otherwise
    bool execute();

    // Dump current state: variables and program with current instruction marked
    std::string dump() const;

private:
    const ProgramType& program;
    FullStateType full_state;
};

