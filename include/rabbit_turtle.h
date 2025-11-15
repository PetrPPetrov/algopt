// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include "executor.h"
#include "program.h"
#include "variables.h"

// Template class for rabbit executor with constant reference to program
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
class RabbitTurtle {
public:
    using ProgramType = Program<InstructionSet, N, K, T>;
    using InputVariablesType = InputVariables<N>;
    using OutputVariablesType = OutputVariables<K>;
    using ExecutorType = Executor<InstructionSet, N, K, T>;

    // Constructors
    explicit RabbitTurtle(const ProgramType& program_arg, const InputVariablesType& input_arg);

    // Access to program
    const ProgramType& getProgram() const;

    // Access to input variables
    const InputVariablesType& getInput() const;

    // Access to output variables
    const OutputVariablesType& getOutput() const;

    // Start execution
    void start();

    // Start execution and dump rabbit executor state
    std::string startDump();

    // Execute one iteration: rabbit makes 2 steps, turtle makes 1 step
    // Returns false if program is finished, true otherwise
    bool execute();

    // Execute one iteration and dump rabbit executor state after each rabbit step
    // Returns false if program is finished, true otherwise
    bool executeDump(std::string& dump_after_first_step, std::string& dump_after_second_step);

    // Check if infinite loop was detected
    bool isInfiniteLoopDetected() const;

private:
    const ProgramType& program;
    const InputVariablesType& input;
    OutputVariablesType output;
    ExecutorType rabbit;
    ExecutorType turtle;
    bool infinite_loop_detected = false;
};

