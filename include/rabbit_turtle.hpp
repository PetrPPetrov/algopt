// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

// Constructors
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline RabbitTurtle<InstructionSet, N, K, T>::RabbitTurtle(const typename RabbitTurtle<InstructionSet, N, K, T>::ProgramType& program_arg, const typename RabbitTurtle<InstructionSet, N, K, T>::InputVariablesType& input_arg)
    : program(program_arg), input(input_arg), rabbit(program_arg, input_arg), turtle(program_arg, input_arg) {
}

// Access to program
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline const typename RabbitTurtle<InstructionSet, N, K, T>::ProgramType& RabbitTurtle<InstructionSet, N, K, T>::getProgram() const {
    return program;
}

// Access to input variables
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline const typename RabbitTurtle<InstructionSet, N, K, T>::InputVariablesType& RabbitTurtle<InstructionSet, N, K, T>::getInput() const {
    return input;
}

// Access to output variables
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline const typename RabbitTurtle<InstructionSet, N, K, T>::OutputVariablesType& RabbitTurtle<InstructionSet, N, K, T>::getOutput() const {
    return output;
}

// Start execution
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline void RabbitTurtle<InstructionSet, N, K, T>::start() {
    // Create variables with input from input and zero-initialized output and temp
    Variables<N, K, T> variables(input);

    // Set full state for rabbit executor
    rabbit.getFullState().getVariables() = variables;
    rabbit.getFullState().instructionPointer() = 0;

    // Set full state for turtle executor
    turtle.getFullState().getVariables() = variables;
    turtle.getFullState().instructionPointer() = 0;

    infinite_loop_detected = false;
}

// Start execution and dump rabbit executor state
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline std::string RabbitTurtle<InstructionSet, N, K, T>::startDump() {
    start();
    return rabbit.dump();
}

// Execute one iteration: rabbit makes 2 steps, turtle makes 1 step
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool RabbitTurtle<InstructionSet, N, K, T>::execute() {
    // Rabbit makes first step
    if (!rabbit.execute()) {
        // Program finished - copy output variables from rabbit
        output = rabbit.getFullState().getVariables().output;
        return false; // Program finished
    }

    // Rabbit makes second step
    if (!rabbit.execute()) {
        // Program finished - copy output variables from rabbit
        output = rabbit.getFullState().getVariables().output;
        return false; // Program finished
    }

    // Turtle makes one step
    if (!turtle.execute()) {
        // Program finished - copy output variables from turtle
        output = turtle.getFullState().getVariables().output;
        return false; // Program finished
    }

    // Compare states: if rabbit and turtle are at the same state, infinite loop detected
    if (rabbit.getFullState().isSame(turtle.getFullState())) {
        infinite_loop_detected = true;
    }

    return true; // Continue execution
}

// Execute one iteration and dump rabbit executor state after each rabbit step
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool RabbitTurtle<InstructionSet, N, K, T>::executeDump(std::string& dump_after_first_step, std::string& dump_after_second_step) {
    // Rabbit makes first step
    if (!rabbit.execute()) {
        // Program finished - copy output variables from rabbit
        output = rabbit.getFullState().getVariables().output;
        dump_after_first_step = rabbit.dump();
        dump_after_second_step = ""; // No second step
        return false; // Program finished
    }
    dump_after_first_step = rabbit.dump();

    // Rabbit makes second step
    if (!rabbit.execute()) {
        // Program finished - copy output variables from rabbit
        output = rabbit.getFullState().getVariables().output;
        dump_after_second_step = rabbit.dump();
        return false; // Program finished
    }
    dump_after_second_step = rabbit.dump();

    // Turtle makes one step
    if (!turtle.execute()) {
        // Program finished - copy output variables from turtle
        output = turtle.getFullState().getVariables().output;
        return false; // Program finished
    }

    // Compare states: if rabbit and turtle are at the same state, infinite loop detected
    if (rabbit.getFullState().isSame(turtle.getFullState())) {
        infinite_loop_detected = true;
    }

    return true; // Continue execution
}

// Check if infinite loop was detected
template<template<unsigned, unsigned, unsigned> class InstructionSet, unsigned N, unsigned K, unsigned T>
inline bool RabbitTurtle<InstructionSet, N, K, T>::isInfiniteLoopDetected() const {
    return infinite_loop_detected;
}

