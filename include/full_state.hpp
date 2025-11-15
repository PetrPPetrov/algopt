// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

// Constructors
template<unsigned N, unsigned K, unsigned T>
inline FullState<N, K, T>::FullState(const VariablesType& variables_arg)
    : variables(variables_arg), instruction_pointer(0) {
}

template<unsigned N, unsigned K, unsigned T>
inline FullState<N, K, T>::FullState(const VariablesType& variables_arg, InstructionPointer instruction_pointer_arg)
    : variables(variables_arg), instruction_pointer(instruction_pointer_arg) {
}

// Access to variables
template<unsigned N, unsigned K, unsigned T>
inline typename FullState<N, K, T>::VariablesType& FullState<N, K, T>::getVariables() {
    return variables;
}

template<unsigned N, unsigned K, unsigned T>
inline const typename FullState<N, K, T>::VariablesType& FullState<N, K, T>::getVariables() const {
    return variables;
}

// Access to instruction pointer
template<unsigned N, unsigned K, unsigned T>
inline typename FullState<N, K, T>::InstructionPointer FullState<N, K, T>::getInstructionPointer() const noexcept {
    return instruction_pointer;
}

template<unsigned N, unsigned K, unsigned T>
inline void FullState<N, K, T>::setInstructionPointer(InstructionPointer instruction_pointer_arg) {
    instruction_pointer = instruction_pointer_arg;
}

template<unsigned N, unsigned K, unsigned T>
inline typename FullState<N, K, T>::InstructionPointer& FullState<N, K, T>::instructionPointer() noexcept {
    return instruction_pointer;
}

template<unsigned N, unsigned K, unsigned T>
inline const typename FullState<N, K, T>::InstructionPointer& FullState<N, K, T>::instructionPointer() const noexcept {
    return instruction_pointer;
}

// Compare with another full state
template<unsigned N, unsigned K, unsigned T>
inline bool FullState<N, K, T>::isSame(const FullState& other) const {
    return instruction_pointer == other.instruction_pointer &&
           variables.input.values == other.variables.input.values &&
           variables.output.values == other.variables.output.values &&
           variables.temp.values == other.variables.temp.values;
}

