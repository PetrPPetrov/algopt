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
inline void B0::Add<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    const std::uint8_t result_value = value1 + value2;
    result.setValue(state, result_value);
    ++state.instructionPointer();
}

// Sub instruction
template<unsigned N, unsigned K, unsigned T>
inline void B0::Sub<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    const std::uint8_t result_value = value1 - value2;
    result.setValue(state, result_value);
    ++state.instructionPointer();
}

// Mul instruction
template<unsigned N, unsigned K, unsigned T>
inline void B0::Mul<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    const std::uint8_t result_value = value1 * value2;
    result.setValue(state, result_value);
    ++state.instructionPointer();
}

// Div instruction
template<unsigned N, unsigned K, unsigned T>
inline void B0::Div<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    const std::uint8_t result_value = (value2 != 0) ? (value1 / value2) : 0;
    result.setValue(state, result_value);
    ++state.instructionPointer();
}

// Move instruction
template<unsigned N, unsigned K, unsigned T>
inline void B0::Move<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t source_value = source.getValue(state);
    destination.setValue(state, source_value);
    ++state.instructionPointer();
}

// Swap instruction
template<unsigned N, unsigned K, unsigned T>
inline void B0::Swap<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = address1.getValue(state);
    const std::uint8_t value2 = address2.getValue(state);
    address1.setValue(state, value2);
    address2.setValue(state, value1);
    ++state.instructionPointer();
}

// Goto instruction
template<unsigned N, unsigned K, unsigned T>
inline void B0::Goto<N, K, T>::execute(FullState<N, K, T>& state) {
    state.instructionPointer() = target;
}

// JumpIfGreater instruction
template<unsigned N, unsigned K, unsigned T>
inline void B0::JumpIfGreater<N, K, T>::execute(FullState<N, K, T>& state) {
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
inline void B0::JumpIfLess<N, K, T>::execute(FullState<N, K, T>& state) {
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
inline void B0::JumpIfGreaterOrEqual<N, K, T>::execute(FullState<N, K, T>& state) {
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
inline void B0::JumpIfLessOrEqual<N, K, T>::execute(FullState<N, K, T>& state) {
    const std::uint8_t value1 = operand1.getValue(state);
    const std::uint8_t value2 = operand2.getValue(state);
    if (value1 <= value2) {
        state.instructionPointer() = target;
    } else {
        ++state.instructionPointer();
    }
}

// getCombinationCount implementations
template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::Add<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * addr_count; // operand1 * operand2 * result
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::Sub<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * addr_count; // operand1 * operand2 * result
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::Mul<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * addr_count; // operand1 * operand2 * result
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::Div<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * addr_count; // operand1 * operand2 * result
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::Move<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count; // source * destination
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::Swap<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count; // address1 * address2
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::Goto<N, K, T>::getCombinationCount(unsigned programLen) {
    return static_cast<std::uint64_t>(programLen); // target can be 0 to programLen-1
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::JumpIfGreater<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::JumpIfLess<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::JumpIfGreaterOrEqual<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::JumpIfLessOrEqual<N, K, T>::getCombinationCount(unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    return addr_count * addr_count * static_cast<std::uint64_t>(programLen); // operand1 * operand2 * target
}

// Helper functions moved to address.hpp

// getCombination implementations
template<unsigned N, unsigned K, unsigned T>
inline B0::Add<N, K, T> B0::Add<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline B0::Sub<N, K, T> B0::Sub<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline B0::Mul<N, K, T> B0::Mul<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline B0::Div<N, K, T> B0::Div<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline B0::Move<N, K, T> B0::Move<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t source_index = combinationIndex / addr_count;
    const std::uint64_t destination_index = combinationIndex % addr_count;
    
    Move result;
    result.source = decodeAddress<N, K, T>(source_index);
    result.destination = decodeAddress<N, K, T>(destination_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B0::Swap<N, K, T> B0::Swap<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    const std::uint64_t address1_index = combinationIndex / addr_count;
    const std::uint64_t address2_index = combinationIndex % addr_count;
    
    Swap result;
    result.address1 = decodeAddress<N, K, T>(address1_index);
    result.address2 = decodeAddress<N, K, T>(address2_index);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B0::Goto<N, K, T> B0::Goto<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
    Goto result;
    result.target = static_cast<std::size_t>(combinationIndex);
    return result;
}

template<unsigned N, unsigned K, unsigned T>
inline B0::JumpIfGreater<N, K, T> B0::JumpIfGreater<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline B0::JumpIfLess<N, K, T> B0::JumpIfLess<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline B0::JumpIfGreaterOrEqual<N, K, T> B0::JumpIfGreaterOrEqual<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
inline B0::JumpIfLessOrEqual<N, K, T> B0::JumpIfLessOrEqual<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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

// InstructionSet getCombinationCount implementation
template<unsigned N, unsigned K, unsigned T>
inline std::uint64_t B0::InstructionSet<N, K, T>::getCombinationCount(unsigned programLen) {
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
    return total;
}

// InstructionSet getCombination implementation
template<unsigned N, unsigned K, unsigned T>
inline B0::InstructionSet<N, K, T> B0::InstructionSet<N, K, T>::getCombination(std::uint64_t combinationIndex, unsigned programLen) {
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
    
    // Should never reach here if combinationIndex is valid
    return InstructionSet{};
}

// Dump methods for instructions
template<unsigned N, unsigned K, unsigned T>
inline std::string B0::Add<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Add " << result.toString() << " = " 
        << operand1.toString() << " + " << operand2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::Sub<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Sub " << result.toString() << " = " 
        << operand1.toString() << " - " << operand2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::Mul<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Mul " << result.toString() << " = " 
        << operand1.toString() << " * " << operand2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::Div<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Div " << result.toString() << " = " 
        << operand1.toString() << " / " << operand2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::Move<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Move " << destination.toString() << " = " 
        << source.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::Swap<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Swap " << address1.toString() << " <-> " 
        << address2.toString();
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::Goto<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": Goto " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::JumpIfGreater<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfGreater " << operand1.toString() << " > " 
        << operand2.toString() << " -> " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::JumpIfLess<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfLess " << operand1.toString() << " < " 
        << operand2.toString() << " -> " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::JumpIfGreaterOrEqual<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfGreaterOrEqual " << operand1.toString() << " >= " 
        << operand2.toString() << " -> " << target;
    return oss.str();
}

template<unsigned N, unsigned K, unsigned T>
inline std::string B0::JumpIfLessOrEqual<N, K, T>::dump(unsigned line_number) const {
    std::ostringstream oss;
    oss << line_number << ": JumpIfLessOrEqual " << operand1.toString() << " <= " 
        << operand2.toString() << " -> " << target;
    return oss.str();
}

// Dump method for InstructionSet is already implemented inline in instructions.h

