// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <cstdint>
#include <sstream>
#include <string>
#include "address.hpp"

// Helper functions moved to address.hpp

// Add instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::Add<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    const std::uint8_t result_value = value1 + value2;
    result.setValue(state, result_value);
    ++state.instructionPointer();
}

// Sub instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::Sub<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    const std::uint8_t result_value = value1 - value2;
    result.setValue(state, result_value);
    ++state.instructionPointer();
}

// Mul instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::Mul<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    const std::uint8_t result_value = value1 * value2;
    result.setValue(state, result_value);
    ++state.instructionPointer();
}

// Div instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::Div<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    const std::uint8_t result_value = (value2 != 0) ? (value1 / value2) : 0;
    result.setValue(state, result_value);
    ++state.instructionPointer();
}

// Move instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::Move<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t source_value = source.getValue(state);
    destination.setValue(state, source_value);
    ++state.instructionPointer();
}

// Swap instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::Swap<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = address1.getValue(state);
    const std::uint8_t value2 = address2.getValue(state);
    address1.setValue(state, value2);
    address2.setValue(state, value1);
    ++state.instructionPointer();
}

// Goto instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::Goto<N, K, T>::execute(FullState<N, K, T>& state) {
    state.instructionPointer() = target;
}

// JumpIfGreater instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::JumpIfGreater<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    if (value1 > value2) {
        state.instructionPointer() = target;
    } else {
        ++state.instructionPointer();
    }
}

// JumpIfLess instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::JumpIfLess<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    if (value1 < value2) {
        state.instructionPointer() = target;
    } else {
        ++state.instructionPointer();
    }
}

// JumpIfGreaterOrEqual instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::JumpIfGreaterOrEqual<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    if (value1 >= value2) {
        state.instructionPointer() = target;
    } else {
        ++state.instructionPointer();
    }
}

// JumpIfLessOrEqual instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::JumpIfLessOrEqual<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    if (value1 <= value2) {
        state.instructionPointer() = target;
    } else {
        ++state.instructionPointer();
    }
}

// JumpIfEqual instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::JumpIfEqual<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    if (value1 == value2) {
        state.instructionPointer() = target;
    } else {
        ++state.instructionPointer();
    }
}

// JumpIfZero instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::JumpIfZero<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value = operand.getValue(state);
    if (value == 0) {
        state.instructionPointer() = target;
    } else {
        ++state.instructionPointer();
    }
}

// LoadIndirect instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::LoadIndirect<N, K, T>::execute(FullState<N, K, T>& state) {
    // Get the index from index_address
    const std::uint8_t index = index_address.getValue(state);
    
    // Check bounds based on array type
    bool index_valid = false;
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            index_valid = (index < N);
            break;
        case Address<N, K, T>::EAddressType::Output:
            index_valid = (index < K);
            break;
        case Address<N, K, T>::EAddressType::Temp:
            index_valid = (index < T);
            break;
    }
    
    // If index is out of bounds, load 0
    std::uint8_t value = 0;
    if (index_valid) {
        // Create a temporary address for indirect access
        Address<N, K, T> indirect_address;
        indirect_address.address_type = array_type;
        indirect_address.address = index;
        
        // Load value from indirect address
        value = indirect_address.getValue(state);
    }
    
    // Store to result_address (0 if index was out of bounds)
    result_address.setValue(state, value);
    ++state.instructionPointer();
}

// StoreIndirect instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::StoreIndirect<N, K, T>::execute(FullState<N, K, T>& state) {
    // Get the value to store from value_source
    const std::uint8_t value = value_source.getValue(state);
    
    // Get the index from index_address
    const std::uint8_t index = index_address.getValue(state);
    
    // Check bounds based on array type
    bool index_valid = false;
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            index_valid = (index < N);
            break;
        case Address<N, K, T>::EAddressType::Output:
            index_valid = (index < K);
            break;
        case Address<N, K, T>::EAddressType::Temp:
            index_valid = (index < T);
            break;
    }
    
    // Only store if index is valid
    if (index_valid) {
        // Create a temporary address for indirect access
        Address<N, K, T> indirect_address;
        indirect_address.address_type = array_type;
        indirect_address.address = index;
        
        // Store value to indirect address
        indirect_address.setValue(state, value);
    }
    // If index is out of bounds, silently ignore the store operation
    
    ++state.instructionPointer();
}

// getCombinationCount implementations for B1 instructions
// All B0 instructions have the same implementation as in B0
template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::Add<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * addr_count; // operand1 * operand2 * result
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::Sub<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * addr_count; // operand1 * operand2 * result
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::Mul<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * addr_count; // operand1 * operand2 * result
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::Div<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * addr_count; // operand1 * operand2 * result
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::Move<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count; // source * destination
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::Swap<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count; // address1 * address2
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::Goto<N, K, T>::getCombinationCount(unsigned programLen) {
    return static_cast<std::uint64_t>(programLen); // target can be 0 to programLen-1
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::JumpIfGreater<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::JumpIfLess<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::JumpIfGreaterOrEqual<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::JumpIfLessOrEqual<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::JumpIfEqual<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::JumpIfZero<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * static_cast<std::uint64_t>(programLen); // operand * target
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::LoadIndirect<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    return addr_count * array_type_count * addr_count; // index_address * array_type * result_address
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::StoreIndirect<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    return addr_count * addr_count * array_type_count; // value_source * index_address * array_type
}

// Helper functions moved to address.hpp

// Helper function to decode array type from index
template<unsigned N, unsigned K, unsigned T>
inline typename Address<N, K, T>::EAddressType decodeArrayType(std::uint64_t index) {
    switch (index) {
        case 0:
            return Address<N, K, T>::EAddressType::Input;
        case 1:
            return Address<N, K, T>::EAddressType::Output;
        case 2:
            return Address<N, K, T>::EAddressType::Temp;
        default:
            return Address<N, K, T>::EAddressType::Input;
    }
}

// getCombination implementations for B1 instructions
// All B0 instructions have the same implementation as in B0
template<unsigned N, unsigned K, unsigned T>
inline B1::Add<N, K, T> B1::Add<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t operand1_index = combinationIndex / (addr_count * addr_count);
    const std::uint64_t remainder1 = combinationIndex % (addr_count * addr_count);
    const std::uint64_t operand2_index = remainder1 / addr_count;
    const std::uint64_t result_index = remainder1 % addr_count;
    
    Add result;
    result.operand1 = decodeAddress<N, K, T>(operand1_index);
    result.operand2 = decodeAddress<N, K, T>(operand2_index);
    result.result = decodeAddress<N, K, T>(result_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::Sub<N, K, T> B1::Sub<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t operand1_index = combinationIndex / (addr_count * addr_count);
    const std::uint64_t remainder1 = combinationIndex % (addr_count * addr_count);
    const std::uint64_t operand2_index = remainder1 / addr_count;
    const std::uint64_t result_index = remainder1 % addr_count;
    
    Sub result;
    result.operand1 = decodeAddress<N, K, T>(operand1_index);
    result.operand2 = decodeAddress<N, K, T>(operand2_index);
    result.result = decodeAddress<N, K, T>(result_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::Mul<N, K, T> B1::Mul<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t operand1_index = combinationIndex / (addr_count * addr_count);
    const std::uint64_t remainder1 = combinationIndex % (addr_count * addr_count);
    const std::uint64_t operand2_index = remainder1 / addr_count;
    const std::uint64_t result_index = remainder1 % addr_count;
    
    Mul result;
    result.operand1 = decodeAddress<N, K, T>(operand1_index);
    result.operand2 = decodeAddress<N, K, T>(operand2_index);
    result.result = decodeAddress<N, K, T>(result_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::Div<N, K, T> B1::Div<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t operand1_index = combinationIndex / (addr_count * addr_count);
    const std::uint64_t remainder1 = combinationIndex % (addr_count * addr_count);
    const std::uint64_t operand2_index = remainder1 / addr_count;
    const std::uint64_t result_index = remainder1 % addr_count;
    
    Div result;
    result.operand1 = decodeAddress<N, K, T>(operand1_index);
    result.operand2 = decodeAddress<N, K, T>(operand2_index);
    result.result = decodeAddress<N, K, T>(result_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::Move<N, K, T> B1::Move<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t source_index = combinationIndex / addr_count;
    const std::uint64_t destination_index = combinationIndex % addr_count;
    
    Move result;
    result.source = decodeAddress<N, K, T>(source_index);
    result.destination = decodeAddress<N, K, T>(destination_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::Swap<N, K, T> B1::Swap<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t address1_index = combinationIndex / addr_count;
    const std::uint64_t address2_index = combinationIndex % addr_count;
    
    Swap result;
    result.address1 = decodeAddress<N, K, T>(address1_index);
    result.address2 = decodeAddress<N, K, T>(address2_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::Goto<N, K, T> B1::Goto<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    Goto result;
    result.target = static_cast<std::size_t>(combinationIndex);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::JumpIfGreater<N, K, T> B1::JumpIfGreater<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t total_per_target = addr_count * addr_count;
    const std::uint64_t target = combinationIndex / total_per_target;
    const std::uint64_t remainder = combinationIndex % total_per_target;
    const std::uint64_t operand1_index = remainder / addr_count;
    const std::uint64_t operand2_index = remainder % addr_count;
    
    JumpIfGreater result;
    result.operand1 = decodeAddress<N, K, T>(operand1_index);
    result.operand2 = decodeAddress<N, K, T>(operand2_index);
    result.target = static_cast<std::size_t>(target);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::JumpIfLess<N, K, T> B1::JumpIfLess<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t total_per_target = addr_count * addr_count;
    const std::uint64_t target = combinationIndex / total_per_target;
    const std::uint64_t remainder = combinationIndex % total_per_target;
    const std::uint64_t operand1_index = remainder / addr_count;
    const std::uint64_t operand2_index = remainder % addr_count;
    
    JumpIfLess result;
    result.operand1 = decodeAddress<N, K, T>(operand1_index);
    result.operand2 = decodeAddress<N, K, T>(operand2_index);
    result.target = static_cast<std::size_t>(target);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::JumpIfGreaterOrEqual<N, K, T> B1::JumpIfGreaterOrEqual<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t total_per_target = addr_count * addr_count;
    const std::uint64_t target = combinationIndex / total_per_target;
    const std::uint64_t remainder = combinationIndex % total_per_target;
    const std::uint64_t operand1_index = remainder / addr_count;
    const std::uint64_t operand2_index = remainder % addr_count;
    
    JumpIfGreaterOrEqual result;
    result.operand1 = decodeAddress<N, K, T>(operand1_index);
    result.operand2 = decodeAddress<N, K, T>(operand2_index);
    result.target = static_cast<std::size_t>(target);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::JumpIfLessOrEqual<N, K, T> B1::JumpIfLessOrEqual<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t total_per_target = addr_count * addr_count;
    const std::uint64_t target = combinationIndex / total_per_target;
    const std::uint64_t remainder = combinationIndex % total_per_target;
    const std::uint64_t operand1_index = remainder / addr_count;
    const std::uint64_t operand2_index = remainder % addr_count;
    
    JumpIfLessOrEqual result;
    result.operand1 = decodeAddress<N, K, T>(operand1_index);
    result.operand2 = decodeAddress<N, K, T>(operand2_index);
    result.target = static_cast<std::size_t>(target);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::JumpIfEqual<N, K, T> B1::JumpIfEqual<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t total_per_target = addr_count * addr_count;
    const std::uint64_t target = combinationIndex / total_per_target;
    const std::uint64_t remainder = combinationIndex % total_per_target;
    const std::uint64_t operand1_index = remainder / addr_count;
    const std::uint64_t operand2_index = remainder % addr_count;
    
    JumpIfEqual result;
    result.operand1 = decodeAddress<N, K, T>(operand1_index);
    result.operand2 = decodeAddress<N, K, T>(operand2_index);
    result.target = static_cast<std::size_t>(target);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::JumpIfZero<N, K, T> B1::JumpIfZero<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t total_per_target = addr_count;
    const std::uint64_t target = combinationIndex / total_per_target;
    const std::uint64_t operand_index = combinationIndex % total_per_target;
    
    JumpIfZero result;
    result.operand = decodeAddress<N, K, T>(operand_index);
    result.target = static_cast<std::size_t>(target);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::LoadIndirect<N, K, T> B1::LoadIndirect<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    const std::uint64_t total_per_array_type = addr_count * addr_count;
    const std::uint64_t array_type_index = combinationIndex / total_per_array_type;
    const std::uint64_t remainder = combinationIndex % total_per_array_type;
    const std::uint64_t index_address_index = remainder / addr_count;
    const std::uint64_t result_address_index = remainder % addr_count;
    
    LoadIndirect result;
    result.index_address = decodeAddress<N, K, T>(index_address_index);
    result.array_type = decodeArrayType<N, K, T>(array_type_index);
    result.result_address = decodeAddress<N, K, T>(result_address_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B1::StoreIndirect<N, K, T> B1::StoreIndirect<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    const std::uint64_t total_per_array_type = addr_count * addr_count;
    const std::uint64_t array_type_index = combinationIndex / total_per_array_type;
    const std::uint64_t remainder = combinationIndex % total_per_array_type;
    const std::uint64_t value_source_index = remainder / addr_count;
    const std::uint64_t index_address_index = remainder % addr_count;
    
    StoreIndirect result;
    result.value_source = decodeAddress<N, K, T>(value_source_index);
    result.index_address = decodeAddress<N, K, T>(index_address_index);
    result.array_type = decodeArrayType<N, K, T>(array_type_index);
    return result;
}

// InstructionSet getCombinationCount implementation
template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::InstructionSet<N, K, T>::getCombinationCount(unsigned programLen) {
    std::uint64_t total = 0;
    total += Add<N, K, T>::getCombinationCount(programLen);
    total += Sub<N, K, T>::getCombinationCount(programLen);
    total += Mul<N, K, T>::getCombinationCount(programLen);
    total += Div<N, K, T>::getCombinationCount(programLen);
    total += Move<N, K, T>::getCombinationCount(programLen);
    total += Swap<N, K, T>::getCombinationCount(programLen);
    total += Goto<N, K, T>::getCombinationCount(programLen);
    total += JumpIfGreater<N, K, T>::getCombinationCount(programLen);
    total += JumpIfLess<N, K, T>::getCombinationCount(programLen);
    total += JumpIfGreaterOrEqual<N, K, T>::getCombinationCount(programLen);
    total += JumpIfLessOrEqual<N, K, T>::getCombinationCount(programLen);
    total += JumpIfEqual<N, K, T>::getCombinationCount(programLen);
    total += JumpIfZero<N, K, T>::getCombinationCount(programLen);
    total += LoadIndirect<N, K, T>::getCombinationCount(programLen);
    total += StoreIndirect<N, K, T>::getCombinationCount(programLen);
    total += Inc<N, K, T>::getCombinationCount(programLen);
    total += Dec<N, K, T>::getCombinationCount(programLen);
    return total;
}

// InstructionSet getCombination implementation
template<unsigned N, unsigned K, unsigned T>
inline B1::InstructionSet<N, K, T> B1::InstructionSet<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    std::uint64_t accumulated_count = 0;
    
    // Check Add
    {
        std::uint64_t count = Add<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Add<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check Sub
    {
        std::uint64_t count = Sub<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Sub<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check Mul
    {
        std::uint64_t count = Mul<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Mul<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check Div
    {
        std::uint64_t count = Div<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Div<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check Move
    {
        std::uint64_t count = Move<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Move<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check Swap
    {
        std::uint64_t count = Swap<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Swap<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check Goto
    {
        std::uint64_t count = Goto<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Goto<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check JumpIfGreater
    {
        std::uint64_t count = JumpIfGreater<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return JumpIfGreater<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check JumpIfLess
    {
        std::uint64_t count = JumpIfLess<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return JumpIfLess<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check JumpIfGreaterOrEqual
    {
        std::uint64_t count = JumpIfGreaterOrEqual<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return JumpIfGreaterOrEqual<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check JumpIfLessOrEqual
    {
        std::uint64_t count = JumpIfLessOrEqual<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return JumpIfLessOrEqual<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check JumpIfEqual
    {
        std::uint64_t count = JumpIfEqual<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return JumpIfEqual<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check JumpIfZero
    {
        std::uint64_t count = JumpIfZero<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return JumpIfZero<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check LoadIndirect
    {
        std::uint64_t count = LoadIndirect<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return LoadIndirect<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check StoreIndirect
    {
        std::uint64_t count = StoreIndirect<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return StoreIndirect<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check Inc
    {
        std::uint64_t count = Inc<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Inc<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check Dec
    {
        std::uint64_t count = Dec<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Dec<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Should never reach here if combinationIndex is valid
    return InstructionSet{};
}

// Dump methods for instructions (same as B0 for common instructions)
template<unsigned N, unsigned K, unsigned T>
inline std::string B1::Add<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Add " << result.toString() << " = " 
        << operand1.toString() << " + " << operand2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::Sub<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Sub " << result.toString() << " = " 
        << operand1.toString() << " - " << operand2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::Mul<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Mul " << result.toString() << " = " 
        << operand1.toString() << " * " << operand2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::Div<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Div " << result.toString() << " = " 
        << operand1.toString() << " / " << operand2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::Move<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Move " << destination.toString() << " = " 
        << source.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::Swap<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Swap " << address1.toString() << " <-> " 
        << address2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::Goto<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Goto " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::JumpIfGreater<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfGreater " << operand1.toString() << " > " 
        << operand2.toString() << " -> " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::JumpIfLess<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfLess " << operand1.toString() << " < " 
        << operand2.toString() << " -> " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::JumpIfGreaterOrEqual<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfGreaterOrEqual " << operand1.toString() << " >= " 
        << operand2.toString() << " -> " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::JumpIfLessOrEqual<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfLessOrEqual " << operand1.toString() << " <= " 
        << operand2.toString() << " -> " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::JumpIfEqual<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfEqual " << operand1.toString() << " == " 
        << operand2.toString() << " -> " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::JumpIfZero<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfZero " << operand.toString() << " == 0 -> " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::LoadIndirect<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    std::string array_type_str;
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            array_type_str = "input";
            break;
        case Address<N, K, T>::EAddressType::Output:
            array_type_str = "output";
            break;
        case Address<N, K, T>::EAddressType::Temp:
            array_type_str = "temp";
            break;
        default:
            array_type_str = "unknown";
            break;
    }
    oss << line_number << ": LoadIndirect " << result_address.toString() << " = " 
        << array_type_str << "[" << index_address.toString() << "]";
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::StoreIndirect<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    std::string array_type_str;
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            array_type_str = "input";
            break;
        case Address<N, K, T>::EAddressType::Output:
            array_type_str = "output";
            break;
        case Address<N, K, T>::EAddressType::Temp:
            array_type_str = "temp";
            break;
        default:
            array_type_str = "unknown";
            break;
    }
    oss << line_number << ": StoreIndirect " << array_type_str << "[" 
        << index_address.toString() << "] = " << value_source.toString();
    return oss.str();
}

// Inc instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::Inc<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t current_value = address.getValue(state);
    const std::uint8_t new_value = current_value + 1;
    address.setValue(state, new_value);
    ++state.instructionPointer();
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::Inc<N, K, T>::getCombinationCount(unsigned programLen) {
    return getAddressCombinationCount<N, K, T>(); // Only address
}

template<unsigned N, unsigned K, unsigned T>
inline B1::Inc<N, K, T> B1::Inc<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    Inc result;
    result.address = decodeAddress<N, K, T>(combinationIndex);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::Inc<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Inc " << address.toString();
    return oss.str();
}

// Dec instruction
template<unsigned N, unsigned K, unsigned T>
inline void B1::Dec<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t current_value = address.getValue(state);
    const std::uint8_t new_value = current_value - 1;
    address.setValue(state, new_value);
    ++state.instructionPointer();
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B1::Dec<N, K, T>::getCombinationCount(unsigned programLen) {
    return getAddressCombinationCount<N, K, T>(); // Only address
}

template<unsigned N, unsigned K, unsigned T>
inline B1::Dec<N, K, T> B1::Dec<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    Dec result;
    result.address = decodeAddress<N, K, T>(combinationIndex);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B1::Dec<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Dec " << address.toString();
    return oss.str();
}

// Dump method for InstructionSet
template<unsigned N, unsigned K, unsigned T>
inline std::string B1::InstructionSet<N, K, T>::dump(unsigned line_number) const {
    return std::visit([line_number](const auto& inst) {
        return inst.dump(line_number);
    }, static_cast<const Base&>(*this));
}

