// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include "full_state.h"

// Address structure
template<unsigned N, unsigned K, unsigned T>
struct Address {
    enum class EAddressType {
        Input,
        Output,
        Temp
    };

    EAddressType address_type;
    unsigned address;

    std::uint8_t getValue(const FullState<N, K, T>& state) const;
    void setValue(FullState<N, K, T>& state, std::uint8_t value) const;
    
    std::string toString() const;
};
