// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include "rabbit_turtle.h"
#include "program.h"
#include "variables.h"

// Template class for debugging program execution
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
class DebugExecutor {
public:
    using ProgramType = Program<InstructionSet, N, K, T>;
    using InputVariablesType = InputVariables<N>;
    using OutputVariablesType = OutputVariables<K>;

    // Constructors
    explicit DebugExecutor(const ProgramType& program_arg, const InputVariablesType& input_arg);

    // Execute program to completion or until infinite loop detected
    void execute();

    // Get output variables
    const OutputVariablesType& getOutput() const;

    // Check if infinite loop was detected
    bool isInfiniteLoopDetected() const;

    // Get execution step count
    std::size_t getStepCount() const;

private:
    RabbitTurtle<InstructionSet, N, K, T> rabbit_turtle;
    std::size_t step_count = 0;
};

