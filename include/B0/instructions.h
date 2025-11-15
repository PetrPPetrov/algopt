// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <new>
#include <algorithm>
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
struct InstructionSet {
    enum class Type : std::uint8_t {
        Add = 0,
        Sub = 1,
        Mul = 2,
        Div = 3,
        Move = 4,
        Swap = 5,
        Goto = 6,
        JumpIfGreater = 7,
        JumpIfLess = 8,
        JumpIfGreaterOrEqual = 9,
        JumpIfLessOrEqual = 10
    };
    
    Type type;
    
    union Storage {
        Add<N, K, T> add;
        Sub<N, K, T> sub;
        Mul<N, K, T> mul;
        Div<N, K, T> div;
        Move<N, K, T> move;
        Swap<N, K, T> swap;
        Goto<N, K, T> goto_;
        JumpIfGreater<N, K, T> jump_if_greater;
        JumpIfLess<N, K, T> jump_if_less;
        JumpIfGreaterOrEqual<N, K, T> jump_if_greater_or_equal;
        JumpIfLessOrEqual<N, K, T> jump_if_less_or_equal;
        
        Storage() {}
        ~Storage() {}
    } storage;
    
    // Default constructor
    InstructionSet() : type(Type::Add) {
        new (&storage.add) Add<N, K, T>();
    }
    
    // Constructors for each instruction type
    InstructionSet(const Add<N, K, T>& inst) : type(Type::Add) {
        new (&storage.add) Add<N, K, T>(inst);
    }
    
    InstructionSet(const Sub<N, K, T>& inst) : type(Type::Sub) {
        new (&storage.sub) Sub<N, K, T>(inst);
    }
    
    InstructionSet(const Mul<N, K, T>& inst) : type(Type::Mul) {
        new (&storage.mul) Mul<N, K, T>(inst);
    }
    
    InstructionSet(const Div<N, K, T>& inst) : type(Type::Div) {
        new (&storage.div) Div<N, K, T>(inst);
    }
    
    InstructionSet(const Move<N, K, T>& inst) : type(Type::Move) {
        new (&storage.move) Move<N, K, T>(inst);
    }
    
    InstructionSet(const Swap<N, K, T>& inst) : type(Type::Swap) {
        new (&storage.swap) Swap<N, K, T>(inst);
    }
    
    InstructionSet(const Goto<N, K, T>& inst) : type(Type::Goto) {
        new (&storage.goto_) Goto<N, K, T>(inst);
    }
    
    InstructionSet(const JumpIfGreater<N, K, T>& inst) : type(Type::JumpIfGreater) {
        new (&storage.jump_if_greater) JumpIfGreater<N, K, T>(inst);
    }
    
    InstructionSet(const JumpIfLess<N, K, T>& inst) : type(Type::JumpIfLess) {
        new (&storage.jump_if_less) JumpIfLess<N, K, T>(inst);
    }
    
    InstructionSet(const JumpIfGreaterOrEqual<N, K, T>& inst) : type(Type::JumpIfGreaterOrEqual) {
        new (&storage.jump_if_greater_or_equal) JumpIfGreaterOrEqual<N, K, T>(inst);
    }
    
    InstructionSet(const JumpIfLessOrEqual<N, K, T>& inst) : type(Type::JumpIfLessOrEqual) {
        new (&storage.jump_if_less_or_equal) JumpIfLessOrEqual<N, K, T>(inst);
    }
    
    // Copy constructor
    InstructionSet(const InstructionSet& other) : type(other.type) {
        switch (type) {
            case Type::Add: new (&storage.add) Add<N, K, T>(other.storage.add); break;
            case Type::Sub: new (&storage.sub) Sub<N, K, T>(other.storage.sub); break;
            case Type::Mul: new (&storage.mul) Mul<N, K, T>(other.storage.mul); break;
            case Type::Div: new (&storage.div) Div<N, K, T>(other.storage.div); break;
            case Type::Move: new (&storage.move) Move<N, K, T>(other.storage.move); break;
            case Type::Swap: new (&storage.swap) Swap<N, K, T>(other.storage.swap); break;
            case Type::Goto: new (&storage.goto_) Goto<N, K, T>(other.storage.goto_); break;
            case Type::JumpIfGreater: new (&storage.jump_if_greater) JumpIfGreater<N, K, T>(other.storage.jump_if_greater); break;
            case Type::JumpIfLess: new (&storage.jump_if_less) JumpIfLess<N, K, T>(other.storage.jump_if_less); break;
            case Type::JumpIfGreaterOrEqual: new (&storage.jump_if_greater_or_equal) JumpIfGreaterOrEqual<N, K, T>(other.storage.jump_if_greater_or_equal); break;
            case Type::JumpIfLessOrEqual: new (&storage.jump_if_less_or_equal) JumpIfLessOrEqual<N, K, T>(other.storage.jump_if_less_or_equal); break;
        }
    }
    
    // Move constructor
    InstructionSet(InstructionSet&& other) noexcept : type(other.type) {
        switch (type) {
            case Type::Add: new (&storage.add) Add<N, K, T>(std::move(other.storage.add)); break;
            case Type::Sub: new (&storage.sub) Sub<N, K, T>(std::move(other.storage.sub)); break;
            case Type::Mul: new (&storage.mul) Mul<N, K, T>(std::move(other.storage.mul)); break;
            case Type::Div: new (&storage.div) Div<N, K, T>(std::move(other.storage.div)); break;
            case Type::Move: new (&storage.move) Move<N, K, T>(std::move(other.storage.move)); break;
            case Type::Swap: new (&storage.swap) Swap<N, K, T>(std::move(other.storage.swap)); break;
            case Type::Goto: new (&storage.goto_) Goto<N, K, T>(std::move(other.storage.goto_)); break;
            case Type::JumpIfGreater: new (&storage.jump_if_greater) JumpIfGreater<N, K, T>(std::move(other.storage.jump_if_greater)); break;
            case Type::JumpIfLess: new (&storage.jump_if_less) JumpIfLess<N, K, T>(std::move(other.storage.jump_if_less)); break;
            case Type::JumpIfGreaterOrEqual: new (&storage.jump_if_greater_or_equal) JumpIfGreaterOrEqual<N, K, T>(std::move(other.storage.jump_if_greater_or_equal)); break;
            case Type::JumpIfLessOrEqual: new (&storage.jump_if_less_or_equal) JumpIfLessOrEqual<N, K, T>(std::move(other.storage.jump_if_less_or_equal)); break;
        }
    }
    
    // Copy assignment
    InstructionSet& operator=(const InstructionSet& other) {
        if (this != &other) {
            destroy();
            type = other.type;
            switch (type) {
                case Type::Add: new (&storage.add) Add<N, K, T>(other.storage.add); break;
                case Type::Sub: new (&storage.sub) Sub<N, K, T>(other.storage.sub); break;
                case Type::Mul: new (&storage.mul) Mul<N, K, T>(other.storage.mul); break;
                case Type::Div: new (&storage.div) Div<N, K, T>(other.storage.div); break;
                case Type::Move: new (&storage.move) Move<N, K, T>(other.storage.move); break;
                case Type::Swap: new (&storage.swap) Swap<N, K, T>(other.storage.swap); break;
                case Type::Goto: new (&storage.goto_) Goto<N, K, T>(other.storage.goto_); break;
                case Type::JumpIfGreater: new (&storage.jump_if_greater) JumpIfGreater<N, K, T>(other.storage.jump_if_greater); break;
                case Type::JumpIfLess: new (&storage.jump_if_less) JumpIfLess<N, K, T>(other.storage.jump_if_less); break;
                case Type::JumpIfGreaterOrEqual: new (&storage.jump_if_greater_or_equal) JumpIfGreaterOrEqual<N, K, T>(other.storage.jump_if_greater_or_equal); break;
                case Type::JumpIfLessOrEqual: new (&storage.jump_if_less_or_equal) JumpIfLessOrEqual<N, K, T>(other.storage.jump_if_less_or_equal); break;
            }
        }
        return *this;
    }
    
    // Move assignment
    InstructionSet& operator=(InstructionSet&& other) noexcept {
        if (this != &other) {
            destroy();
            type = other.type;
            switch (type) {
                case Type::Add: new (&storage.add) Add<N, K, T>(std::move(other.storage.add)); break;
                case Type::Sub: new (&storage.sub) Sub<N, K, T>(std::move(other.storage.sub)); break;
                case Type::Mul: new (&storage.mul) Mul<N, K, T>(std::move(other.storage.mul)); break;
                case Type::Div: new (&storage.div) Div<N, K, T>(std::move(other.storage.div)); break;
                case Type::Move: new (&storage.move) Move<N, K, T>(std::move(other.storage.move)); break;
                case Type::Swap: new (&storage.swap) Swap<N, K, T>(std::move(other.storage.swap)); break;
                case Type::Goto: new (&storage.goto_) Goto<N, K, T>(std::move(other.storage.goto_)); break;
                case Type::JumpIfGreater: new (&storage.jump_if_greater) JumpIfGreater<N, K, T>(std::move(other.storage.jump_if_greater)); break;
                case Type::JumpIfLess: new (&storage.jump_if_less) JumpIfLess<N, K, T>(std::move(other.storage.jump_if_less)); break;
                case Type::JumpIfGreaterOrEqual: new (&storage.jump_if_greater_or_equal) JumpIfGreaterOrEqual<N, K, T>(std::move(other.storage.jump_if_greater_or_equal)); break;
                case Type::JumpIfLessOrEqual: new (&storage.jump_if_less_or_equal) JumpIfLessOrEqual<N, K, T>(std::move(other.storage.jump_if_less_or_equal)); break;
            }
        }
        return *this;
    }
    
    // Assignment operators for each instruction type
    InstructionSet& operator=(const Add<N, K, T>& inst) {
        destroy();
        type = Type::Add;
        new (&storage.add) Add<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Sub<N, K, T>& inst) {
        destroy();
        type = Type::Sub;
        new (&storage.sub) Sub<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Mul<N, K, T>& inst) {
        destroy();
        type = Type::Mul;
        new (&storage.mul) Mul<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Div<N, K, T>& inst) {
        destroy();
        type = Type::Div;
        new (&storage.div) Div<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Move<N, K, T>& inst) {
        destroy();
        type = Type::Move;
        new (&storage.move) Move<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Swap<N, K, T>& inst) {
        destroy();
        type = Type::Swap;
        new (&storage.swap) Swap<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Goto<N, K, T>& inst) {
        destroy();
        type = Type::Goto;
        new (&storage.goto_) Goto<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const JumpIfGreater<N, K, T>& inst) {
        destroy();
        type = Type::JumpIfGreater;
        new (&storage.jump_if_greater) JumpIfGreater<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const JumpIfLess<N, K, T>& inst) {
        destroy();
        type = Type::JumpIfLess;
        new (&storage.jump_if_less) JumpIfLess<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const JumpIfGreaterOrEqual<N, K, T>& inst) {
        destroy();
        type = Type::JumpIfGreaterOrEqual;
        new (&storage.jump_if_greater_or_equal) JumpIfGreaterOrEqual<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const JumpIfLessOrEqual<N, K, T>& inst) {
        destroy();
        type = Type::JumpIfLessOrEqual;
        new (&storage.jump_if_less_or_equal) JumpIfLessOrEqual<N, K, T>(inst);
        return *this;
    }
    
    // Destructor
    ~InstructionSet() {
        destroy();
    }
    
    // Execute instruction
    void execute(FullState<N, K, T>& state) {
        switch (type) {
            case Type::Add: storage.add.execute(state); break;
            case Type::Sub: storage.sub.execute(state); break;
            case Type::Mul: storage.mul.execute(state); break;
            case Type::Div: storage.div.execute(state); break;
            case Type::Move: storage.move.execute(state); break;
            case Type::Swap: storage.swap.execute(state); break;
            case Type::Goto: storage.goto_.execute(state); break;
            case Type::JumpIfGreater: storage.jump_if_greater.execute(state); break;
            case Type::JumpIfLess: storage.jump_if_less.execute(state); break;
            case Type::JumpIfGreaterOrEqual: storage.jump_if_greater_or_equal.execute(state); break;
            case Type::JumpIfLessOrEqual: storage.jump_if_less_or_equal.execute(state); break;
        }
    }
    
    // Dump instruction
    std::string dump(unsigned line_number) const {
        switch (type) {
            case Type::Add: return storage.add.dump(line_number);
            case Type::Sub: return storage.sub.dump(line_number);
            case Type::Mul: return storage.mul.dump(line_number);
            case Type::Div: return storage.div.dump(line_number);
            case Type::Move: return storage.move.dump(line_number);
            case Type::Swap: return storage.swap.dump(line_number);
            case Type::Goto: return storage.goto_.dump(line_number);
            case Type::JumpIfGreater: return storage.jump_if_greater.dump(line_number);
            case Type::JumpIfLess: return storage.jump_if_less.dump(line_number);
            case Type::JumpIfGreaterOrEqual: return storage.jump_if_greater_or_equal.dump(line_number);
            case Type::JumpIfLessOrEqual: return storage.jump_if_less_or_equal.dump(line_number);
        }
        return "";
    }
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static InstructionSet getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
private:
    void destroy() {
        switch (type) {
            case Type::Add: storage.add.~Add(); break;
            case Type::Sub: storage.sub.~Sub(); break;
            case Type::Mul: storage.mul.~Mul(); break;
            case Type::Div: storage.div.~Div(); break;
            case Type::Move: storage.move.~Move(); break;
            case Type::Swap: storage.swap.~Swap(); break;
            case Type::Goto: storage.goto_.~Goto(); break;
            case Type::JumpIfGreater: storage.jump_if_greater.~JumpIfGreater(); break;
            case Type::JumpIfLess: storage.jump_if_less.~JumpIfLess(); break;
            case Type::JumpIfGreaterOrEqual: storage.jump_if_greater_or_equal.~JumpIfGreaterOrEqual(); break;
            case Type::JumpIfLessOrEqual: storage.jump_if_less_or_equal.~JumpIfLessOrEqual(); break;
        }
    }
};

} // namespace B0
