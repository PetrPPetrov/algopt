// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>
#include "address.h"
#include "full_state.h"

namespace B0 {

// Basic instructions for B0 instruction set
// Convention: each instruction is a simple structure with fields

template<unsigned N, unsigned K, unsigned T>
struct Add {
    Address<N, K, T> operand1;
    Address<N, K, T> operand2;
    Address<N, K, T> result;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Add getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct Sub {
    Address<N, K, T> operand1;
    Address<N, K, T> operand2;
    Address<N, K, T> result;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Sub getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct Mul {
    Address<N, K, T> operand1;
    Address<N, K, T> operand2;
    Address<N, K, T> result;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Mul getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct Div {
    Address<N, K, T> operand1;
    Address<N, K, T> operand2;
    Address<N, K, T> result;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Div getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct Move {
    Address<N, K, T> source;
    Address<N, K, T> destination;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Move getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct Swap {
    Address<N, K, T> address1;
    Address<N, K, T> address2;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Swap getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct Goto {
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Goto getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct JumpIfGreater {
    Address<N, K, T> operand1;
    Address<N, K, T> operand2;
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static JumpIfGreater getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct JumpIfLess {
    Address<N, K, T> operand1;
    Address<N, K, T> operand2;
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static JumpIfLess getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct JumpIfGreaterOrEqual {
    Address<N, K, T> operand1;
    Address<N, K, T> operand2;
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static JumpIfGreaterOrEqual getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct JumpIfLessOrEqual {
    Address<N, K, T> operand1;
    Address<N, K, T> operand2;
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static JumpIfLessOrEqual getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

// B0 instruction set as variant type
template<unsigned N, unsigned K, unsigned T>
struct InstructionSet : public std::variant<
    Add<N, K, T>,
    Sub<N, K, T>,
    Mul<N, K, T>,
    Div<N, K, T>,
    Move<N, K, T>,
    Swap<N, K, T>,
    Goto<N, K, T>,
    JumpIfGreater<N, K, T>,
    JumpIfLess<N, K, T>,
    JumpIfGreaterOrEqual<N, K, T>,
    JumpIfLessOrEqual<N, K, T>
> {
    using Base = std::variant<
        Add<N, K, T>,
        Sub<N, K, T>,
        Mul<N, K, T>,
        Div<N, K, T>,
        Move<N, K, T>,
        Swap<N, K, T>,
        Goto<N, K, T>,
        JumpIfGreater<N, K, T>,
        JumpIfLess<N, K, T>,
        JumpIfGreaterOrEqual<N, K, T>,
        JumpIfLessOrEqual<N, K, T>
    >;
    
    using Base::Base;
    using Base::operator=;
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static InstructionSet getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

} // namespace B0

// Specialization of std::variant_size and variant_alternative for B0::InstructionSet
namespace std {
    template<unsigned N, unsigned K, unsigned T>
    struct variant_size<B0::InstructionSet<N, K, T>> : variant_size<typename B0::InstructionSet<N, K, T>::Base> {};
    
    template<std::size_t I, unsigned N, unsigned K, unsigned T>
    struct variant_alternative<I, B0::InstructionSet<N, K, T>> : variant_alternative<I, typename B0::InstructionSet<N, K, T>::Base> {};
}
