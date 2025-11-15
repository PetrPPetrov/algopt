// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <cstdint>
#include "program.h"
#include "variables.h"

// Template class for program optimization
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
class Optimize {
public:
    using ProgramType = Program<InstructionSet, N, K, T>;
    using InputVariablesType = InputVariables<N>;
    using OutputVariablesType = OutputVariables<K>;

    // Constructor
    explicit Optimize(const ProgramType& program_arg);

    // Find optimized program that produces same output but with fewer average steps
    // maxProgramSize: maximum size of programs to search
    // Returns optimized program (or original if no better found)
    ProgramType speed(unsigned maxProgramSize);
    
    // Calculate total step count for all input combinations
    std::uint64_t calculateAverageSteps(const ProgramType& program) const;

private:
    const ProgramType& original_program;
    
    // Execute program and count steps, return output variables
    OutputVariablesType executeAndCountSteps(const ProgramType& program, 
                                             const InputVariablesType& input, 
                                             std::uint64_t& step_count) const;
    
    // Check if two programs produce same output for all input combinations
    // If candidate is valid, also calculate and return total steps via output parameter
    bool producesSameOutput(const ProgramType& candidate, std::uint64_t& candidate_total_steps) const;
    
    // Helper: iterate through all input combinations and call callback for each
    template<typename Callback>
    void forEachInputCombination(Callback&& callback) const;
};
