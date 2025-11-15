// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <cstddef>
#include "variables.h"

// Template class representing full state with variables and instruction pointer
template<unsigned N, unsigned K, unsigned T>
class FullState {
public:
    using VariablesType = Variables<N, K, T>;
    using InstructionPointer = std::size_t;

    // Constructors
    FullState() = default;
    explicit FullState(const VariablesType& variables_arg);
    FullState(const VariablesType& variables_arg, InstructionPointer instruction_pointer_arg);

    // Access to variables
    VariablesType& getVariables();
    const VariablesType& getVariables() const;

    // Access to instruction pointer
    InstructionPointer getInstructionPointer() const noexcept;
    void setInstructionPointer(InstructionPointer instruction_pointer_arg);
    InstructionPointer& instructionPointer() noexcept;
    const InstructionPointer& instructionPointer() const noexcept;

    // Compare with another full state
    bool isSame(const FullState& other) const;

private:
    VariablesType variables;
    InstructionPointer instruction_pointer = 0;
};

