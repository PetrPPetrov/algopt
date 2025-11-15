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

namespace S0 {

// Sorting instructions for S0 instruction set
// All operations use indirect addressing with array type selection

template<unsigned N, unsigned K, unsigned T>
struct SwapIndirect {
    Address<N, K, T> index1_address;
    Address<N, K, T> index2_address;
    typename Address<N, K, T>::EAddressType array_type;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static SwapIndirect getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct JumpIfLessIndirect {
    Address<N, K, T> index1_address;
    Address<N, K, T> index2_address;
    typename Address<N, K, T>::EAddressType array_type;
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static JumpIfLessIndirect getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct JumpIfGreaterIndirect {
    Address<N, K, T> index1_address;
    Address<N, K, T> index2_address;
    typename Address<N, K, T>::EAddressType array_type;
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static JumpIfGreaterIndirect getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct JumpIfEqualIndirect {
    Address<N, K, T> index1_address;
    Address<N, K, T> index2_address;
    typename Address<N, K, T>::EAddressType array_type;
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static JumpIfEqualIndirect getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct LoadIndirect {
    Address<N, K, T> index_address;
    typename Address<N, K, T>::EAddressType array_type;
    Address<N, K, T> result_address;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static LoadIndirect getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

template<unsigned N, unsigned K, unsigned T>
struct StoreIndirect {
    Address<N, K, T> value_source;
    Address<N, K, T> index_address;
    typename Address<N, K, T>::EAddressType array_type;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static StoreIndirect getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

// Increment instruction: increases value at address by 1
template<unsigned N, unsigned K, unsigned T>
struct Inc {
    Address<N, K, T> address;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Inc getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

// Decrement instruction: decreases value at address by 1
template<unsigned N, unsigned K, unsigned T>
struct Dec {
    Address<N, K, T> address;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Dec getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

// JumpIfEqual instruction: jumps to target if operand1 == operand2
template<unsigned N, unsigned K, unsigned T>
struct JumpIfEqual {
    Address<N, K, T> operand1;
    Address<N, K, T> operand2;
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static JumpIfEqual getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

// JumpIfZero instruction: jumps to target if operand == 0
template<unsigned N, unsigned K, unsigned T>
struct JumpIfZero {
    Address<N, K, T> operand;
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static JumpIfZero getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

// SetC instruction: sets value at address to a constant
template<unsigned N, unsigned K, unsigned T>
struct SetC {
    Address<N, K, T> address;
    std::uint8_t constant;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static SetC getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

// Goto instruction: unconditional jump to target
template<unsigned N, unsigned K, unsigned T>
struct Goto {
    std::size_t target;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Goto getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

// Move instruction: copies value from source to destination
template<unsigned N, unsigned K, unsigned T>
struct Move {
    Address<N, K, T> source;
    Address<N, K, T> destination;

    void execute(FullState<N, K, T>& state);
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static Move getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
    std::string dump(unsigned line_number) const;
};

// S0 instruction set as custom variant type
template<unsigned N, unsigned K, unsigned T>
struct InstructionSet {
    enum class Type : std::uint8_t {
        SwapIndirect = 0,
        JumpIfLessIndirect = 1,
        JumpIfGreaterIndirect = 2,
        JumpIfEqualIndirect = 3,
        LoadIndirect = 4,
        StoreIndirect = 5,
        Inc = 6,
        Dec = 7,
        JumpIfEqual = 8,
        JumpIfZero = 9,
        SetC = 10,
        Goto = 11,
        Move = 12
    };
    
    Type type;
    
    union Storage {
        SwapIndirect<N, K, T> swap_indirect;
        JumpIfLessIndirect<N, K, T> jump_if_less_indirect;
        JumpIfGreaterIndirect<N, K, T> jump_if_greater_indirect;
        JumpIfEqualIndirect<N, K, T> jump_if_equal_indirect;
        LoadIndirect<N, K, T> load_indirect;
        StoreIndirect<N, K, T> store_indirect;
        Inc<N, K, T> inc;
        Dec<N, K, T> dec;
        JumpIfEqual<N, K, T> jump_if_equal;
        JumpIfZero<N, K, T> jump_if_zero;
        SetC<N, K, T> set_c;
        Goto<N, K, T> goto_inst;
        Move<N, K, T> move;
        
        Storage() {}
        ~Storage() {}
    } storage;
    
    // Default constructor
    InstructionSet() : type(Type::SwapIndirect) {
        new (&storage.swap_indirect) SwapIndirect<N, K, T>();
    }
    
    // Constructors for each instruction type
    InstructionSet(const SwapIndirect<N, K, T>& inst) : type(Type::SwapIndirect) {
        new (&storage.swap_indirect) SwapIndirect<N, K, T>(inst);
    }
    
    InstructionSet(const JumpIfLessIndirect<N, K, T>& inst) : type(Type::JumpIfLessIndirect) {
        new (&storage.jump_if_less_indirect) JumpIfLessIndirect<N, K, T>(inst);
    }
    
    InstructionSet(const JumpIfGreaterIndirect<N, K, T>& inst) : type(Type::JumpIfGreaterIndirect) {
        new (&storage.jump_if_greater_indirect) JumpIfGreaterIndirect<N, K, T>(inst);
    }
    
    InstructionSet(const JumpIfEqualIndirect<N, K, T>& inst) : type(Type::JumpIfEqualIndirect) {
        new (&storage.jump_if_equal_indirect) JumpIfEqualIndirect<N, K, T>(inst);
    }
    
    InstructionSet(const LoadIndirect<N, K, T>& inst) : type(Type::LoadIndirect) {
        new (&storage.load_indirect) LoadIndirect<N, K, T>(inst);
    }
    
    InstructionSet(const StoreIndirect<N, K, T>& inst) : type(Type::StoreIndirect) {
        new (&storage.store_indirect) StoreIndirect<N, K, T>(inst);
    }
    
    InstructionSet(const Inc<N, K, T>& inst) : type(Type::Inc) {
        new (&storage.inc) Inc<N, K, T>(inst);
    }
    
    InstructionSet(const Dec<N, K, T>& inst) : type(Type::Dec) {
        new (&storage.dec) Dec<N, K, T>(inst);
    }
    
    InstructionSet(const JumpIfEqual<N, K, T>& inst) : type(Type::JumpIfEqual) {
        new (&storage.jump_if_equal) JumpIfEqual<N, K, T>(inst);
    }
    
    InstructionSet(const JumpIfZero<N, K, T>& inst) : type(Type::JumpIfZero) {
        new (&storage.jump_if_zero) JumpIfZero<N, K, T>(inst);
    }
    
    InstructionSet(const SetC<N, K, T>& inst) : type(Type::SetC) {
        new (&storage.set_c) SetC<N, K, T>(inst);
    }
    
    InstructionSet(const Goto<N, K, T>& inst) : type(Type::Goto) {
        new (&storage.goto_inst) Goto<N, K, T>(inst);
    }
    
    InstructionSet(const Move<N, K, T>& inst) : type(Type::Move) {
        new (&storage.move) Move<N, K, T>(inst);
    }
    
    // Copy constructor
    InstructionSet(const InstructionSet& other) : type(other.type) {
        switch (type) {
            case Type::SwapIndirect: new (&storage.swap_indirect) SwapIndirect<N, K, T>(other.storage.swap_indirect); break;
            case Type::JumpIfLessIndirect: new (&storage.jump_if_less_indirect) JumpIfLessIndirect<N, K, T>(other.storage.jump_if_less_indirect); break;
            case Type::JumpIfGreaterIndirect: new (&storage.jump_if_greater_indirect) JumpIfGreaterIndirect<N, K, T>(other.storage.jump_if_greater_indirect); break;
            case Type::JumpIfEqualIndirect: new (&storage.jump_if_equal_indirect) JumpIfEqualIndirect<N, K, T>(other.storage.jump_if_equal_indirect); break;
            case Type::LoadIndirect: new (&storage.load_indirect) LoadIndirect<N, K, T>(other.storage.load_indirect); break;
            case Type::StoreIndirect: new (&storage.store_indirect) StoreIndirect<N, K, T>(other.storage.store_indirect); break;
            case Type::Inc: new (&storage.inc) Inc<N, K, T>(other.storage.inc); break;
            case Type::Dec: new (&storage.dec) Dec<N, K, T>(other.storage.dec); break;
            case Type::JumpIfEqual: new (&storage.jump_if_equal) JumpIfEqual<N, K, T>(other.storage.jump_if_equal); break;
            case Type::JumpIfZero: new (&storage.jump_if_zero) JumpIfZero<N, K, T>(other.storage.jump_if_zero); break;
            case Type::SetC: new (&storage.set_c) SetC<N, K, T>(other.storage.set_c); break;
            case Type::Goto: new (&storage.goto_inst) Goto<N, K, T>(other.storage.goto_inst); break;
            case Type::Move: new (&storage.move) Move<N, K, T>(other.storage.move); break;
        }
    }
    
    // Move constructor
    InstructionSet(InstructionSet&& other) noexcept : type(other.type) {
        switch (type) {
            case Type::SwapIndirect: new (&storage.swap_indirect) SwapIndirect<N, K, T>(std::move(other.storage.swap_indirect)); break;
            case Type::JumpIfLessIndirect: new (&storage.jump_if_less_indirect) JumpIfLessIndirect<N, K, T>(std::move(other.storage.jump_if_less_indirect)); break;
            case Type::JumpIfGreaterIndirect: new (&storage.jump_if_greater_indirect) JumpIfGreaterIndirect<N, K, T>(std::move(other.storage.jump_if_greater_indirect)); break;
            case Type::JumpIfEqualIndirect: new (&storage.jump_if_equal_indirect) JumpIfEqualIndirect<N, K, T>(std::move(other.storage.jump_if_equal_indirect)); break;
            case Type::LoadIndirect: new (&storage.load_indirect) LoadIndirect<N, K, T>(std::move(other.storage.load_indirect)); break;
            case Type::StoreIndirect: new (&storage.store_indirect) StoreIndirect<N, K, T>(std::move(other.storage.store_indirect)); break;
            case Type::Inc: new (&storage.inc) Inc<N, K, T>(std::move(other.storage.inc)); break;
            case Type::Dec: new (&storage.dec) Dec<N, K, T>(std::move(other.storage.dec)); break;
            case Type::JumpIfEqual: new (&storage.jump_if_equal) JumpIfEqual<N, K, T>(std::move(other.storage.jump_if_equal)); break;
            case Type::JumpIfZero: new (&storage.jump_if_zero) JumpIfZero<N, K, T>(std::move(other.storage.jump_if_zero)); break;
            case Type::SetC: new (&storage.set_c) SetC<N, K, T>(std::move(other.storage.set_c)); break;
            case Type::Goto: new (&storage.goto_inst) Goto<N, K, T>(std::move(other.storage.goto_inst)); break;
            case Type::Move: new (&storage.move) Move<N, K, T>(std::move(other.storage.move)); break;
        }
    }
    
    // Copy assignment
    InstructionSet& operator=(const InstructionSet& other) {
        if (this != &other) {
            destroy();
            type = other.type;
            switch (type) {
                case Type::SwapIndirect: new (&storage.swap_indirect) SwapIndirect<N, K, T>(other.storage.swap_indirect); break;
                case Type::JumpIfLessIndirect: new (&storage.jump_if_less_indirect) JumpIfLessIndirect<N, K, T>(other.storage.jump_if_less_indirect); break;
                case Type::JumpIfGreaterIndirect: new (&storage.jump_if_greater_indirect) JumpIfGreaterIndirect<N, K, T>(other.storage.jump_if_greater_indirect); break;
                case Type::JumpIfEqualIndirect: new (&storage.jump_if_equal_indirect) JumpIfEqualIndirect<N, K, T>(other.storage.jump_if_equal_indirect); break;
                case Type::LoadIndirect: new (&storage.load_indirect) LoadIndirect<N, K, T>(other.storage.load_indirect); break;
                case Type::StoreIndirect: new (&storage.store_indirect) StoreIndirect<N, K, T>(other.storage.store_indirect); break;
                case Type::Inc: new (&storage.inc) Inc<N, K, T>(other.storage.inc); break;
                case Type::Dec: new (&storage.dec) Dec<N, K, T>(other.storage.dec); break;
                case Type::JumpIfEqual: new (&storage.jump_if_equal) JumpIfEqual<N, K, T>(other.storage.jump_if_equal); break;
                case Type::JumpIfZero: new (&storage.jump_if_zero) JumpIfZero<N, K, T>(other.storage.jump_if_zero); break;
                case Type::SetC: new (&storage.set_c) SetC<N, K, T>(other.storage.set_c); break;
                case Type::Goto: new (&storage.goto_inst) Goto<N, K, T>(other.storage.goto_inst); break;
                case Type::Move: new (&storage.move) Move<N, K, T>(other.storage.move); break;
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
                case Type::SwapIndirect: new (&storage.swap_indirect) SwapIndirect<N, K, T>(std::move(other.storage.swap_indirect)); break;
                case Type::JumpIfLessIndirect: new (&storage.jump_if_less_indirect) JumpIfLessIndirect<N, K, T>(std::move(other.storage.jump_if_less_indirect)); break;
                case Type::JumpIfGreaterIndirect: new (&storage.jump_if_greater_indirect) JumpIfGreaterIndirect<N, K, T>(std::move(other.storage.jump_if_greater_indirect)); break;
                case Type::JumpIfEqualIndirect: new (&storage.jump_if_equal_indirect) JumpIfEqualIndirect<N, K, T>(std::move(other.storage.jump_if_equal_indirect)); break;
                case Type::LoadIndirect: new (&storage.load_indirect) LoadIndirect<N, K, T>(std::move(other.storage.load_indirect)); break;
                case Type::StoreIndirect: new (&storage.store_indirect) StoreIndirect<N, K, T>(std::move(other.storage.store_indirect)); break;
                case Type::Inc: new (&storage.inc) Inc<N, K, T>(std::move(other.storage.inc)); break;
                case Type::Dec: new (&storage.dec) Dec<N, K, T>(std::move(other.storage.dec)); break;
                case Type::JumpIfEqual: new (&storage.jump_if_equal) JumpIfEqual<N, K, T>(std::move(other.storage.jump_if_equal)); break;
                case Type::JumpIfZero: new (&storage.jump_if_zero) JumpIfZero<N, K, T>(std::move(other.storage.jump_if_zero)); break;
                case Type::SetC: new (&storage.set_c) SetC<N, K, T>(std::move(other.storage.set_c)); break;
                case Type::Goto: new (&storage.goto_inst) Goto<N, K, T>(std::move(other.storage.goto_inst)); break;
                case Type::Move: new (&storage.move) Move<N, K, T>(std::move(other.storage.move)); break;
            }
        }
        return *this;
    }
    
    // Assignment operators for each instruction type
    InstructionSet& operator=(const SwapIndirect<N, K, T>& inst) {
        destroy();
        type = Type::SwapIndirect;
        new (&storage.swap_indirect) SwapIndirect<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const JumpIfLessIndirect<N, K, T>& inst) {
        destroy();
        type = Type::JumpIfLessIndirect;
        new (&storage.jump_if_less_indirect) JumpIfLessIndirect<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const JumpIfGreaterIndirect<N, K, T>& inst) {
        destroy();
        type = Type::JumpIfGreaterIndirect;
        new (&storage.jump_if_greater_indirect) JumpIfGreaterIndirect<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const JumpIfEqualIndirect<N, K, T>& inst) {
        destroy();
        type = Type::JumpIfEqualIndirect;
        new (&storage.jump_if_equal_indirect) JumpIfEqualIndirect<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const LoadIndirect<N, K, T>& inst) {
        destroy();
        type = Type::LoadIndirect;
        new (&storage.load_indirect) LoadIndirect<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const StoreIndirect<N, K, T>& inst) {
        destroy();
        type = Type::StoreIndirect;
        new (&storage.store_indirect) StoreIndirect<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Inc<N, K, T>& inst) {
        destroy();
        type = Type::Inc;
        new (&storage.inc) Inc<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Dec<N, K, T>& inst) {
        destroy();
        type = Type::Dec;
        new (&storage.dec) Dec<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const JumpIfEqual<N, K, T>& inst) {
        destroy();
        type = Type::JumpIfEqual;
        new (&storage.jump_if_equal) JumpIfEqual<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const JumpIfZero<N, K, T>& inst) {
        destroy();
        type = Type::JumpIfZero;
        new (&storage.jump_if_zero) JumpIfZero<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const SetC<N, K, T>& inst) {
        destroy();
        type = Type::SetC;
        new (&storage.set_c) SetC<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Goto<N, K, T>& inst) {
        destroy();
        type = Type::Goto;
        new (&storage.goto_inst) Goto<N, K, T>(inst);
        return *this;
    }
    
    InstructionSet& operator=(const Move<N, K, T>& inst) {
        destroy();
        type = Type::Move;
        new (&storage.move) Move<N, K, T>(inst);
        return *this;
    }
    
    // Destructor
    ~InstructionSet() {
        destroy();
    }
    
    // Execute instruction
    void execute(FullState<N, K, T>& state) {
        switch (type) {
            case Type::SwapIndirect: storage.swap_indirect.execute(state); break;
            case Type::JumpIfLessIndirect: storage.jump_if_less_indirect.execute(state); break;
            case Type::JumpIfGreaterIndirect: storage.jump_if_greater_indirect.execute(state); break;
            case Type::JumpIfEqualIndirect: storage.jump_if_equal_indirect.execute(state); break;
            case Type::LoadIndirect: storage.load_indirect.execute(state); break;
            case Type::StoreIndirect: storage.store_indirect.execute(state); break;
            case Type::Inc: storage.inc.execute(state); break;
            case Type::Dec: storage.dec.execute(state); break;
            case Type::JumpIfEqual: storage.jump_if_equal.execute(state); break;
            case Type::JumpIfZero: storage.jump_if_zero.execute(state); break;
            case Type::SetC: storage.set_c.execute(state); break;
            case Type::Goto: storage.goto_inst.execute(state); break;
            case Type::Move: storage.move.execute(state); break;
        }
    }
    
    // Dump instruction
    std::string dump(unsigned line_number) const {
        switch (type) {
            case Type::SwapIndirect: return storage.swap_indirect.dump(line_number);
            case Type::JumpIfLessIndirect: return storage.jump_if_less_indirect.dump(line_number);
            case Type::JumpIfGreaterIndirect: return storage.jump_if_greater_indirect.dump(line_number);
            case Type::JumpIfEqualIndirect: return storage.jump_if_equal_indirect.dump(line_number);
            case Type::LoadIndirect: return storage.load_indirect.dump(line_number);
            case Type::StoreIndirect: return storage.store_indirect.dump(line_number);
            case Type::Inc: return storage.inc.dump(line_number);
            case Type::Dec: return storage.dec.dump(line_number);
            case Type::JumpIfEqual: return storage.jump_if_equal.dump(line_number);
            case Type::JumpIfZero: return storage.jump_if_zero.dump(line_number);
            case Type::SetC: return storage.set_c.dump(line_number);
            case Type::Goto: return storage.goto_inst.dump(line_number);
            case Type::Move: return storage.move.dump(line_number);
        }
        return "";
    }
    
    static std::uint64_t getCombinationCount(unsigned programLen);
    static InstructionSet getCombination(std::uint64_t combinationIndex, unsigned programLen);
    
private:
    void destroy() {
        switch (type) {
            case Type::SwapIndirect: storage.swap_indirect.~SwapIndirect(); break;
            case Type::JumpIfLessIndirect: storage.jump_if_less_indirect.~JumpIfLessIndirect(); break;
            case Type::JumpIfGreaterIndirect: storage.jump_if_greater_indirect.~JumpIfGreaterIndirect(); break;
            case Type::JumpIfEqualIndirect: storage.jump_if_equal_indirect.~JumpIfEqualIndirect(); break;
            case Type::LoadIndirect: storage.load_indirect.~LoadIndirect(); break;
            case Type::StoreIndirect: storage.store_indirect.~StoreIndirect(); break;
            case Type::Inc: storage.inc.~Inc(); break;
            case Type::Dec: storage.dec.~Dec(); break;
            case Type::JumpIfEqual: storage.jump_if_equal.~JumpIfEqual(); break;
            case Type::JumpIfZero: storage.jump_if_zero.~JumpIfZero(); break;
            case Type::SetC: storage.set_c.~SetC(); break;
            case Type::Goto: storage.goto_inst.~Goto(); break;
            case Type::Move: storage.move.~Move(); break;
        }
    }
};

} // namespace S0

