// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include "debug_executor.h"
#include <iostream>
#include <string>
#include <limits>

// Constructors
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline DebugExecutor<InstructionSet, N, K, T>::DebugExecutor(const typename DebugExecutor<InstructionSet, N, K, T>::ProgramType& program_arg, const typename DebugExecutor<InstructionSet, N, K, T>::InputVariablesType& input_arg)
    : rabbit_turtle(program_arg, input_arg) {
}

// Execute program to completion or until infinite loop detected
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline void DebugExecutor<InstructionSet, N, K, T>::execute() {
    // Start execution and dump initial state
    std::cout << rabbit_turtle.startDump() << std::endl;
    std::cout << "Press Enter to continue..." << std::flush;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    step_count = 0;
    constexpr std::size_t max_steps = 1000000; // Safety limit

    std::string dump1, dump2;
    while (rabbit_turtle.executeDump(dump1, dump2) && step_count < max_steps) {
        step_count++;
        
        // Output dump after first rabbit step
        std::cout << dump1 << std::endl;
        std::cout << "Press Enter to continue..." << std::flush;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // Output dump after second rabbit step
        std::cout << dump2 << std::endl;
        std::cout << "Press Enter to continue..." << std::flush;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (rabbit_turtle.isInfiniteLoopDetected()) {
            std::cout << "Infinite loop detected!" << std::endl;
            break;
        }
    }

    if (step_count >= max_steps) {
        std::cout << "Reached maximum step limit!" << std::endl;
    }
}

// Get output variables
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline const typename DebugExecutor<InstructionSet, N, K, T>::OutputVariablesType& DebugExecutor<InstructionSet, N, K, T>::getOutput() const {
    return rabbit_turtle.getOutput();
}

// Check if infinite loop was detected
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool DebugExecutor<InstructionSet, N, K, T>::isInfiniteLoopDetected() const {
    return rabbit_turtle.isInfiniteLoopDetected();
}

// Get execution step count
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline std::size_t DebugExecutor<InstructionSet, N, K, T>::getStepCount() const {
    return step_count;
}

