// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdint>
#include <functional>
#include <limits>
#include <list>
#include <iostream>
#include <utility>
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
// If candidate is valid, also calculate and return total steps via output parameter
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool Optimize<InstructionSet, N, K, T>::producesSameOutput(const ProgramType& candidate, std::uint64_t& candidate_total_steps) const {
    bool all_match = true;
    candidate_total_steps = 0;
    
    forEachInputCombination([&](const InputVariablesType& input) {
        if (!all_match) {
            return; // Early exit optimization
        }
        
        std::uint64_t original_steps, candidate_steps;
        
        // Execute original program
        RabbitTurtle<InstructionSet, N, K, T> rt_original(original_program, input);
        rt_original.start();
        original_steps = 0;
        const std::uint64_t max_steps = 1000000;
        bool original_infinite = false;
        
        while (rt_original.execute()) {
            ++original_steps;
            if (rt_original.isInfiniteLoopDetected() || original_steps > max_steps) {
                original_infinite = true;
                break;
            }
        }
        
        // Execute candidate program
        RabbitTurtle<InstructionSet, N, K, T> rt_candidate(candidate, input);
        rt_candidate.start();
        candidate_steps = 0;
        bool candidate_infinite = false;
        
        while (rt_candidate.execute()) {
            ++candidate_steps;
            if (rt_candidate.isInfiniteLoopDetected() || candidate_steps > max_steps) {
                candidate_infinite = true;
                break;
            }
        }
        
        // Accumulate candidate steps (only if program is valid)
        candidate_total_steps += candidate_steps;
        
        // If one program gets stuck but the other doesn't, they're not equivalent
        if (original_infinite != candidate_infinite) {
            all_match = false;
            return;
        }
        
        OutputVariablesType original_output = rt_original.getOutput();
        OutputVariablesType candidate_output = rt_candidate.getOutput();
        
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

// Calculate total step count for all input combinations
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline std::uint64_t Optimize<InstructionSet, N, K, T>::calculateAverageSteps(const ProgramType& program) const {
    std::uint64_t total_steps = 0;
    
    forEachInputCombination([&](const InputVariablesType& input) {
        std::uint64_t step_count;
        executeAndCountSteps(program, input, step_count);
        total_steps += step_count;
    });
    
    return total_steps;
}

// Find optimized program
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline typename Optimize<InstructionSet, N, K, T>::ProgramType
Optimize<InstructionSet, N, K, T>::speed(unsigned maxProgramSize) {
    // Calculate total steps for original program
    std::uint64_t original_total_steps = calculateAverageSteps(original_program);
    
    ProgramType best_program = original_program;
    std::uint64_t best_total_steps = original_total_steps;
    
    // Container to store all valid programs with their step counts
    std::list<std::pair<ProgramType, std::uint64_t>> valid_programs;
    
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
            
            // Check if candidate produces same output and get total steps
            std::uint64_t candidate_total_steps = 0;
            if (producesSameOutput(candidate, candidate_total_steps)) {
                ++valid_count;
                // Add to list of valid programs with step count
                valid_programs.push_back(std::make_pair(candidate, candidate_total_steps));
                
                // If candidate is better, update best
                if (candidate_total_steps < best_total_steps) {
                    std::cout << "Found better program (size " << program_size 
                              << ", total steps: " << candidate_total_steps 
                              << " < " << best_total_steps << ")" << std::endl;
                    best_program = candidate;
                    best_total_steps = candidate_total_steps;
                }
            }
            
            // Print progress every 100 programs
            if (checked_count % 100 == 0) {
                std::cout << "  Checked " << checked_count << " programs, found " 
                          << valid_count << " valid, best total steps: " << best_total_steps << std::endl;
            }
        } while (fabric.next());
        
        std::cout << "Size " << program_size << " complete: checked " << checked_count 
                  << " programs, found " << valid_count << " valid" << std::endl;
    }
    
    // Output all valid programs
    std::cout << "\n=== All Valid Programs (" << valid_programs.size() << " total) ===" << std::endl;
    std::uint64_t program_index = 0;
    for (const auto& program_pair : valid_programs) {
        const auto& program = program_pair.first;
        std::uint64_t program_steps = program_pair.second;
        std::cout << "\n--- Valid Program #" << program_index << " (total steps: " << program_steps << ") ---" << std::endl;
        std::cout << program.dump() << std::endl;
        ++program_index;
    }
    
    return best_program;
}

