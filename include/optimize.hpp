// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdint>
#include <functional>
#include <limits>
#include "executor.h"
#include "executor.hpp"
#include "fabric.h"
#include "full_state.h"
#include "full_state.hpp"
#include "program.hpp"
#include "rabbit_turtle.h"
#include "rabbit_turtle.hpp"
#include "variables.hpp"

// Constructor
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline Optimize<InstructionSet, N, K, T>::Optimize(const ProgramType& program_arg)
    : original_program(program_arg) {
}

// Execute program and count steps, return output variables
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Optimize<InstructionSet, N, K, T>::OutputVariablesType
Optimize<InstructionSet, N, K, T>::executeAndCountSteps(const ProgramType& program,
                                                          const InputVariablesType& input,
                                                          std::uint64_t& step_count) const {
    RabbitTurtle<InstructionSet, N, K, T> rt(program, input);
    rt.start();
    step_count = 0;
    
    // Maximum step limit to prevent infinite loops (fallback)
    const std::uint64_t max_steps = 1000000;
    
    while (rt.execute()) {
        ++step_count;
        
        // Check if infinite loop detected by RabbitTurtle
        if (rt.isInfiniteLoopDetected()) {
            break;
        }
        
        // Fallback: prevent infinite loops by setting a maximum step limit
        if (step_count > max_steps) {
            break;
        }
    }
    
    return rt.getOutput();
}

// Helper: iterate through all input combinations and call callback for each
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
template<typename Callback>
inline void Optimize<InstructionSet, N, K, T>::forEachInputCombination(Callback&& callback) const {
    InputVariablesType current;
    
    // Initialize all values to 0
    for (unsigned i = 0; i < N; ++i) {
        current.values[i] = 0;
    }
    
    // Generate all combinations iteratively (like a multi-digit counter in base 256)
    // Start with all zeros
    callback(current);
    
    // Increment counter until overflow
    while (true) {
        // Increment the least significant "digit"
        unsigned pos = 0;
        while (pos < N) {
            if (current.values[pos] < 255) {
                ++current.values[pos];
                break;
            } else {
                // Overflow: reset to 0 and carry to next position
                current.values[pos] = 0;
                ++pos;
            }
        }
        
        // If we overflowed all positions, we're done
        if (pos >= N) {
            break;
        }
        
        callback(current);
    }
}

// Check if two programs produce same output for all input combinations
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool Optimize<InstructionSet, N, K, T>::producesSameOutput(const ProgramType& candidate) const {
    bool all_match = true;
    
    forEachInputCombination([&](const InputVariablesType& input) {
        if (!all_match) {
            return; // Early exit optimization
        }
        
        std::uint64_t original_steps, candidate_steps;
        
        OutputVariablesType original_output = executeAndCountSteps(original_program, input, original_steps);
        OutputVariablesType candidate_output = executeAndCountSteps(candidate, input, candidate_steps);
        
        // Compare output variables (ignore temp variables)
        for (unsigned i = 0; i < K; ++i) {
            if (original_output.values[i] != candidate_output.values[i]) {
                all_match = false;
                return;
            }
        }
    });
    
    return all_match;
}

// Calculate average step count for all input combinations
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline double Optimize<InstructionSet, N, K, T>::calculateAverageSteps(const ProgramType& program) const {
    std::uint64_t total_steps = 0;
    std::uint64_t valid_runs = 0;
    
    forEachInputCombination([&](const InputVariablesType& input) {
        std::uint64_t step_count;
        executeAndCountSteps(program, input, step_count);
        total_steps += step_count;
        ++valid_runs;
    });
    
    if (valid_runs == 0) {
        return std::numeric_limits<double>::max();
    }
    
    return static_cast<double>(total_steps) / static_cast<double>(valid_runs);
}

// Find optimized program
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Optimize<InstructionSet, N, K, T>::ProgramType
Optimize<InstructionSet, N, K, T>::speed(unsigned maxProgramSize) {
    // Calculate average steps for original program
    double original_avg_steps = calculateAverageSteps(original_program);
    
    ProgramType best_program = original_program;
    double best_avg_steps = original_avg_steps;
    
    // Search through all possible program sizes from 1 to maxProgramSize
    for (unsigned program_size = 1; program_size <= maxProgramSize; ++program_size) {
        std::cout << "Searching programs of size " << program_size << "..." << std::endl;
        Fabric<InstructionSet, N, K, T> fabric(program_size);
        std::uint64_t checked_count = 0;
        std::uint64_t valid_count = 0;
        
        // Iterate through all possible programs of this size
        do {
            ProgramType candidate = fabric.generate();
            ++checked_count;
            
            // Check if candidate produces same output
            if (producesSameOutput(candidate)) {
                ++valid_count;
                // Calculate average steps for candidate
                double candidate_avg_steps = calculateAverageSteps(candidate);
                
                // If candidate is better, update best
                if (candidate_avg_steps < best_avg_steps) {
                    std::cout << "Found better program (size " << program_size 
                              << ", avg steps: " << candidate_avg_steps 
                              << " < " << best_avg_steps << ")" << std::endl;
                    best_program = candidate;
                    best_avg_steps = candidate_avg_steps;
                }
            }
            
            // Print progress every 100 programs
            if (checked_count % 100 == 0) {
                std::cout << "  Checked " << checked_count << " programs, found " 
                          << valid_count << " valid, best avg steps: " << best_avg_steps << std::endl;
            }
        } while (fabric.next());
        
        std::cout << "Size " << program_size << " complete: checked " << checked_count 
                  << " programs, found " << valid_count << " valid" << std::endl;
    }
    
    return best_program;
}

