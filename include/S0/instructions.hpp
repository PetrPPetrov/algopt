// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include "S0/instructions.h"
#include "address.hpp"
#include <sstream>

namespace S0 {

// Helper function to decode array type
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

// Helper function to get array size based on type
template<unsigned N, unsigned K, unsigned T>
inline unsigned getArraySize(typename Address<N, K, T>::EAddressType array_type) {
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            return N;
        case Address<N, K, T>::EAddressType::Output:
            return K;
        case Address<N, K, T>::EAddressType::Temp:
            return T;
        default:
            return 0;
    }
}

// Helper function to get value from array by index
template<unsigned N, unsigned K, unsigned T>
inline std::uint8_t getArrayValue(const FullState<N, K, T>& state, 
                                   typename Address<N, K, T>::EAddressType array_type, 
                                   std::uint8_t index) {
    const auto& variables = state.getVariables();
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            if (index < N) {
                return variables.input.values[index];
            }
            break;
        case Address<N, K, T>::EAddressType::Output:
            if (index < K) {
                return variables.output.values[index];
            }
            break;
        case Address<N, K, T>::EAddressType::Temp:
            if (index < T) {
                return variables.temp.values[index];
            }
            break;
    }
    return 0;
}

// Helper function to set value in array by index
template<unsigned N, unsigned K, unsigned T>
inline void setArrayValue(FullState<N, K, T>& state, 
                          typename Address<N, K, T>::EAddressType array_type, 
                          std::uint8_t index, 
                          std::uint8_t value) {
    auto& variables = state.getVariables();
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            if (index < N) {
                variables.input.values[index] = value;
            }
            break;
        case Address<N, K, T>::EAddressType::Output:
            if (index < K) {
                variables.output.values[index] = value;
            }
            break;
        case Address<N, K, T>::EAddressType::Temp:
            if (index < T) {
                variables.temp.values[index] = value;
            }
            break;
    }
}

// SwapIndirect implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::SwapIndirect<N, K, T>::execute(FullState<N, K, T>& state) {
    // Get indices from addresses
    const std::uint8_t index1 = index1_address.getValue(state);
    const std::uint8_t index2 = index2_address.getValue(state);
    
    // Get array size
    const unsigned array_size = getArraySize<N, K, T>(array_type);
    
    // Check bounds
    if (index1 < array_size && index2 < array_size) {
        // Get values
        const std::uint8_t value1 = getArrayValue<N, K, T>(state, array_type, index1);
        const std::uint8_t value2 = getArrayValue<N, K, T>(state, array_type, index2);
        
        // Swap values
        setArrayValue<N, K, T>(state, array_type, index1, value2);
        setArrayValue<N, K, T>(state, array_type, index2, value1);
    }
    // If indices are out of bounds, silently ignore
    
    ++state.instructionPointer();
}

// JumpIfLessIndirect implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::JumpIfLessIndirect<N, K, T>::execute(FullState<N, K, T>& state) {
    // Get indices from addresses
    const std::uint8_t index1 = index1_address.getValue(state);
    const std::uint8_t index2 = index2_address.getValue(state);
    
    // Get array size
    const unsigned array_size = getArraySize<N, K, T>(array_type);
    
    // Check bounds
    if (index1 < array_size && index2 < array_size) {
        // Get values
        const std::uint8_t value1 = getArrayValue<N, K, T>(state, array_type, index1);
        const std::uint8_t value2 = getArrayValue<N, K, T>(state, array_type, index2);
        
        // Compare and jump
        if (value1 < value2) {
            state.instructionPointer() = target;
            return;
        }
    }
    
    ++state.instructionPointer();
}

// JumpIfGreaterIndirect implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::JumpIfGreaterIndirect<N, K, T>::execute(FullState<N, K, T>& state) {
    // Get indices from addresses
    const std::uint8_t index1 = index1_address.getValue(state);
    const std::uint8_t index2 = index2_address.getValue(state);
    
    // Get array size
    const unsigned array_size = getArraySize<N, K, T>(array_type);
    
    // Check bounds
    if (index1 < array_size && index2 < array_size) {
        // Get values
        const std::uint8_t value1 = getArrayValue<N, K, T>(state, array_type, index1);
        const std::uint8_t value2 = getArrayValue<N, K, T>(state, array_type, index2);
        
        // Compare and jump
        if (value1 > value2) {
            state.instructionPointer() = target;
            return;
        }
    }
    
    ++state.instructionPointer();
}

// JumpIfEqualIndirect implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::JumpIfEqualIndirect<N, K, T>::execute(FullState<N, K, T>& state) {
    // Get indices from addresses
    const std::uint8_t index1 = index1_address.getValue(state);
    const std::uint8_t index2 = index2_address.getValue(state);
    
    // Get array size
    const unsigned array_size = getArraySize<N, K, T>(array_type);
    
    // Check bounds
    if (index1 < array_size && index2 < array_size) {
        // Get values
        const std::uint8_t value1 = getArrayValue<N, K, T>(state, array_type, index1);
        const std::uint8_t value2 = getArrayValue<N, K, T>(state, array_type, index2);
        
        // Compare and jump
        if (value1 == value2) {
            state.instructionPointer() = target;
            return;
        }
    }
    
    ++state.instructionPointer();
}

// LoadIndirect implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::LoadIndirect<N, K, T>::execute(FullState<N, K, T>& state) {
    // Get the index from index_address
    const std::uint8_t index = index_address.getValue(state);
    
    // Get array size
    const unsigned array_size = getArraySize<N, K, T>(array_type);
    
    // Check bounds
    std::uint8_t value = 0;
    if (index < array_size) {
        value = getArrayValue<N, K, T>(state, array_type, index);
    }
    
    // Store value to result_address
    result_address.setValue(state, value);
    
    ++state.instructionPointer();
}

// StoreIndirect implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::StoreIndirect<N, K, T>::execute(FullState<N, K, T>& state) {
    // Get value from value_source
    const std::uint8_t value = value_source.getValue(state);
    
    // Get the index from index_address
    const std::uint8_t index = index_address.getValue(state);
    
    // Get array size
    const unsigned array_size = getArraySize<N, K, T>(array_type);
    
    // Check bounds and store
    if (index < array_size) {
        setArrayValue<N, K, T>(state, array_type, index, value);
    }
    // If index is out of bounds, silently ignore
    
    ++state.instructionPointer();
}

// getCombinationCount implementations
template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::SwapIndirect<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    return addr_count * addr_count * array_type_count; // index1 * index2 * array_type
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::JumpIfLessIndirect<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    const std::uint64_t total_per_target = addr_count * addr_count * array_type_count;
    return static_cast<std::uint64_t>(programLen) * total_per_target; // target * (index1 * index2 * array_type)
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::JumpIfGreaterIndirect<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    const std::uint64_t total_per_target = addr_count * addr_count * array_type_count;
    return static_cast<std::uint64_t>(programLen) * total_per_target; // target * (index1 * index2 * array_type)
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::JumpIfEqualIndirect<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    const std::uint64_t total_per_target = addr_count * addr_count * array_type_count;
    return static_cast<std::uint64_t>(programLen) * total_per_target; // target * (index1 * index2 * array_type)
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::LoadIndirect<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    return addr_count * addr_count * array_type_count; // index_address * result_address * array_type
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::StoreIndirect<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    return addr_count * addr_count * array_type_count; // value_source * index_address * array_type
}

// Inc implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::Inc<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t current_value = address.getValue(state);
    const std::uint8_t new_value = current_value + 1;
    address.setValue(state, new_value);
    ++state.instructionPointer();
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::Inc<N, K, T>::getCombinationCount(unsigned programLen) {
    return getAddressCombinationCount<N, K, T>(); // Only address
}

template<unsigned N, unsigned K, unsigned T>
inline S0::Inc<N, K, T> S0::Inc<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    Inc result;
    result.address = decodeAddress<N, K, T>(combinationIndex);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::Inc<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Inc " << address.toString();
    return oss.str();
}

// Dec implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::Dec<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t current_value = address.getValue(state);
    const std::uint8_t new_value = current_value - 1;
    address.setValue(state, new_value);
    ++state.instructionPointer();
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::Dec<N, K, T>::getCombinationCount(unsigned programLen) {
    return getAddressCombinationCount<N, K, T>(); // Only address
}

template<unsigned N, unsigned K, unsigned T>
inline S0::Dec<N, K, T> S0::Dec<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    Dec result;
    result.address = decodeAddress<N, K, T>(combinationIndex);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::Dec<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Dec " << address.toString();
    return oss.str();
}

// JumpIfEqual implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::JumpIfEqual<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    if (value1 == value2) {
        state.instructionPointer() = target;
    } else {
        ++state.instructionPointer();
    }
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::JumpIfEqual<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

template<unsigned N, unsigned K, unsigned T>
inline S0::JumpIfEqual<N, K, T> S0::JumpIfEqual<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline std::string S0::JumpIfEqual<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfEqual " << operand1.toString() 
        << " " << operand2.toString() << " " << target;
    return oss.str();
}

// JumpIfZero implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::JumpIfZero<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value = operand.getValue(state);
    if (value == 0) {
        state.instructionPointer() = target;
    } else {
        ++state.instructionPointer();
    }
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::JumpIfZero<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * static_cast<std::uint64_t>(programLen); // operand * target
}

template<unsigned N, unsigned K, unsigned T>
inline S0::JumpIfZero<N, K, T> S0::JumpIfZero<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline std::string S0::JumpIfZero<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfZero " << operand.toString() << " " << target;
    return oss.str();
}

// SetC implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::SetC<N, K, T>::execute(FullState<N, K, T>& state) {
    address.setValue(state, constant);
    ++state.instructionPointer();
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::SetC<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t constant_count = 256; // All possible uint8_t values (0-255)
    return addr_count * constant_count; // address * constant
}

template<unsigned N, unsigned K, unsigned T>
inline S0::SetC<N, K, T> S0::SetC<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t constant_count = 256; // All possible uint8_t values (0-255)
    const std::uint64_t address_index = combinationIndex / constant_count;
    const std::uint64_t constant_index = combinationIndex % constant_count;
    
    SetC result;
    result.address = decodeAddress<N, K, T>(address_index);
    result.constant = static_cast<std::uint8_t>(constant_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::SetC<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": SetC " << address.toString() << " " << static_cast<unsigned>(constant);
    return oss.str();
}

// Goto implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::Goto<N, K, T>::execute(FullState<N, K, T>& state) {
    state.instructionPointer() = target;
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::Goto<N, K, T>::getCombinationCount(unsigned programLen) {
    return static_cast<std::uint64_t>(programLen); // target can be 0 to programLen-1
}

template<unsigned N, unsigned K, unsigned T>
inline S0::Goto<N, K, T> S0::Goto<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    Goto result;
    result.target = static_cast<std::size_t>(combinationIndex);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::Goto<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Goto " << target;
    return oss.str();
}

// Move implementation
template<unsigned N, unsigned K, unsigned T>
inline void S0::Move<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t source_value = source.getValue(state);
    destination.setValue(state, source_value);
    ++state.instructionPointer();
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::Move<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count; // source * destination
}

template<unsigned N, unsigned K, unsigned T>
inline S0::Move<N, K, T> S0::Move<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t source_index = combinationIndex / addr_count;
    const std::uint64_t destination_index = combinationIndex % addr_count;
    
    Move result;
    result.source = decodeAddress<N, K, T>(source_index);
    result.destination = decodeAddress<N, K, T>(destination_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::Move<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Move " << destination.toString() << " = " 
        << source.toString();
    return oss.str();
}

// getCombination implementations
template<unsigned N, unsigned K, unsigned T>
inline S0::SwapIndirect<N, K, T> S0::SwapIndirect<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    const std::uint64_t total_per_array_type = addr_count * addr_count;
    const std::uint64_t array_type_index = combinationIndex / total_per_array_type;
    const std::uint64_t remainder = combinationIndex % total_per_array_type;
    const std::uint64_t index1_index = remainder / addr_count;
    const std::uint64_t index2_index = remainder % addr_count;
    
    SwapIndirect result;
    result.index1_address = decodeAddress<N, K, T>(index1_index);
    result.index2_address = decodeAddress<N, K, T>(index2_index);
    result.array_type = decodeArrayType<N, K, T>(array_type_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline S0::JumpIfLessIndirect<N, K, T> S0::JumpIfLessIndirect<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    const std::uint64_t total_per_array_type = addr_count * addr_count;
    const std::uint64_t total_per_target = total_per_array_type * array_type_count;
    const std::uint64_t target = combinationIndex / total_per_target;
    const std::uint64_t remainder = combinationIndex % total_per_target;
    const std::uint64_t array_type_index = remainder / total_per_array_type;
    const std::uint64_t remainder2 = remainder % total_per_array_type;
    const std::uint64_t index1_index = remainder2 / addr_count;
    const std::uint64_t index2_index = remainder2 % addr_count;
    
    JumpIfLessIndirect result;
    result.index1_address = decodeAddress<N, K, T>(index1_index);
    result.index2_address = decodeAddress<N, K, T>(index2_index);
    result.array_type = decodeArrayType<N, K, T>(array_type_index);
    result.target = static_cast<std::size_t>(target);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline S0::JumpIfGreaterIndirect<N, K, T> S0::JumpIfGreaterIndirect<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    const std::uint64_t total_per_array_type = addr_count * addr_count;
    const std::uint64_t total_per_target = total_per_array_type * array_type_count;
    const std::uint64_t target = combinationIndex / total_per_target;
    const std::uint64_t remainder = combinationIndex % total_per_target;
    const std::uint64_t array_type_index = remainder / total_per_array_type;
    const std::uint64_t remainder2 = remainder % total_per_array_type;
    const std::uint64_t index1_index = remainder2 / addr_count;
    const std::uint64_t index2_index = remainder2 % addr_count;
    
    JumpIfGreaterIndirect result;
    result.index1_address = decodeAddress<N, K, T>(index1_index);
    result.index2_address = decodeAddress<N, K, T>(index2_index);
    result.array_type = decodeArrayType<N, K, T>(array_type_index);
    result.target = static_cast<std::size_t>(target);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline S0::JumpIfEqualIndirect<N, K, T> S0::JumpIfEqualIndirect<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t array_type_count = 3; // Input, Output, Temp
    const std::uint64_t total_per_array_type = addr_count * addr_count;
    const std::uint64_t total_per_target = total_per_array_type * array_type_count;
    const std::uint64_t target = combinationIndex / total_per_target;
    const std::uint64_t remainder = combinationIndex % total_per_target;
    const std::uint64_t array_type_index = remainder / total_per_array_type;
    const std::uint64_t remainder2 = remainder % total_per_array_type;
    const std::uint64_t index1_index = remainder2 / addr_count;
    const std::uint64_t index2_index = remainder2 % addr_count;
    
    JumpIfEqualIndirect result;
    result.index1_address = decodeAddress<N, K, T>(index1_index);
    result.index2_address = decodeAddress<N, K, T>(index2_index);
    result.array_type = decodeArrayType<N, K, T>(array_type_index);
    result.target = static_cast<std::size_t>(target);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline S0::LoadIndirect<N, K, T> S0::LoadIndirect<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline S0::StoreIndirect<N, K, T> S0::StoreIndirect<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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

// dump implementations
template<unsigned N, unsigned K, unsigned T>
inline std::string S0::SwapIndirect<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": SwapIndirect " << index1_address.toString() 
        << " " << index2_address.toString() << " ";
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            oss << "Input";
            break;
        case Address<N, K, T>::EAddressType::Output:
            oss << "Output";
            break;
        case Address<N, K, T>::EAddressType::Temp:
            oss << "Temp";
            break;
    }
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::JumpIfLessIndirect<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfLessIndirect " << index1_address.toString() 
        << " " << index2_address.toString() << " ";
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            oss << "Input";
            break;
        case Address<N, K, T>::EAddressType::Output:
            oss << "Output";
            break;
        case Address<N, K, T>::EAddressType::Temp:
            oss << "Temp";
            break;
    }
    oss << " " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::JumpIfGreaterIndirect<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfGreaterIndirect " << index1_address.toString() 
        << " " << index2_address.toString() << " ";
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            oss << "Input";
            break;
        case Address<N, K, T>::EAddressType::Output:
            oss << "Output";
            break;
        case Address<N, K, T>::EAddressType::Temp:
            oss << "Temp";
            break;
    }
    oss << " " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::JumpIfEqualIndirect<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfEqualIndirect " << index1_address.toString() 
        << " " << index2_address.toString() << " ";
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            oss << "Input";
            break;
        case Address<N, K, T>::EAddressType::Output:
            oss << "Output";
            break;
        case Address<N, K, T>::EAddressType::Temp:
            oss << "Temp";
            break;
    }
    oss << " " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::LoadIndirect<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": LoadIndirect " << index_address.toString() << " ";
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            oss << "Input";
            break;
        case Address<N, K, T>::EAddressType::Output:
            oss << "Output";
            break;
        case Address<N, K, T>::EAddressType::Temp:
            oss << "Temp";
            break;
    }
    oss << " " << result_address.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string S0::StoreIndirect<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": StoreIndirect " << value_source.toString() 
        << " " << index_address.toString() << " ";
    switch (array_type) {
        case Address<N, K, T>::EAddressType::Input:
            oss << "Input";
            break;
        case Address<N, K, T>::EAddressType::Output:
            oss << "Output";
            break;
        case Address<N, K, T>::EAddressType::Temp:
            oss << "Temp";
            break;
    }
    return oss.str();
}

// InstructionSet getCombinationCount implementation
template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t S0::InstructionSet<N, K, T>::getCombinationCount(unsigned programLen) {
    std::uint64_t total = 0;
    total += SwapIndirect<N, K, T>::getCombinationCount(programLen);
    total += JumpIfLessIndirect<N, K, T>::getCombinationCount(programLen);
    total += JumpIfGreaterIndirect<N, K, T>::getCombinationCount(programLen);
    total += JumpIfEqualIndirect<N, K, T>::getCombinationCount(programLen);
    total += LoadIndirect<N, K, T>::getCombinationCount(programLen);
    total += StoreIndirect<N, K, T>::getCombinationCount(programLen);
    total += Inc<N, K, T>::getCombinationCount(programLen);
    total += Dec<N, K, T>::getCombinationCount(programLen);
    total += JumpIfEqual<N, K, T>::getCombinationCount(programLen);
    total += JumpIfZero<N, K, T>::getCombinationCount(programLen);
    total += SetC<N, K, T>::getCombinationCount(programLen);
    total += Goto<N, K, T>::getCombinationCount(programLen);
    total += Move<N, K, T>::getCombinationCount(programLen);
    return total;
}

// InstructionSet getCombination implementation
template<unsigned N, unsigned K, unsigned T>
inline S0::InstructionSet<N, K, T> S0::InstructionSet<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    std::uint64_t accumulated_count = 0;
    
    // Check SwapIndirect
    {
        std::uint64_t count = SwapIndirect<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return SwapIndirect<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check JumpIfLessIndirect
    {
        std::uint64_t count = JumpIfLessIndirect<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return JumpIfLessIndirect<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check JumpIfGreaterIndirect
    {
        std::uint64_t count = JumpIfGreaterIndirect<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return JumpIfGreaterIndirect<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Check JumpIfEqualIndirect
    {
        std::uint64_t count = JumpIfEqualIndirect<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return JumpIfEqualIndirect<N, K, T>::getCombination(relative_index, programLen);
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
    
    // Check SetC
    {
        std::uint64_t count = SetC<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return SetC<N, K, T>::getCombination(relative_index, programLen);
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
    
    // Check Move
    {
        std::uint64_t count = Move<N, K, T>::getCombinationCount(programLen);
        if (combinationIndex < accumulated_count + count) {
            std::uint64_t relative_index = combinationIndex - accumulated_count;
            return Move<N, K, T>::getCombination(relative_index, programLen);
        }
        accumulated_count += count;
    }
    
    // Default fallback
    return SwapIndirect<N, K, T>::getCombination(0, programLen);
}

} // namespace S0

