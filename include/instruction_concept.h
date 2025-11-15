// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <concepts>
#include "full_state.h"

// Concept for instruction classes
// Instruction must have a method that takes FullState by non-const reference
// and modifies it directly
template<typename Instruction, unsigned N, unsigned K, unsigned T>
concept InstructionConcept = requires(Instruction instruction, FullState<N, K, T>& state) {
    { instruction.execute(state) } -> std::same_as<void>;
};


