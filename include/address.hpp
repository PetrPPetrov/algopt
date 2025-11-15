// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include "address.h"

// Helper function to calculate address combinations
// Each address can be: Input (N options), Output (K options), or Temp (T options)
template<unsigned N, unsigned K, unsigned T>
constexpr std::uint64_t getAddressCombinationCount() {
    return static_cast<std::uint64_t>(N) + K + T;
}

// Helper function to decode address from index
template<unsigned N, unsigned K, unsigned T>
inline Address<N, K, T> decodeAddress(std::uint64_t index) {
    Address<N, K, T> addr;
    const std::uint64_t addr_count = getAddressCombinationCount<N, K, T>();
    
    if (index < N) {
        addr.address_type = Address<N, K, T>::EAddressType::Input;
        addr.address = static_cast<unsigned>(index);
    } else if (index < static_cast<std::uint64_t>(N) + K) {
        addr.address_type = Address<N, K, T>::EAddressType::Output;
        addr.address = static_cast<unsigned>(index - N);
    } else {
        addr.address_type = Address<N, K, T>::EAddressType::Temp;
        addr.address = static_cast<unsigned>(index - N - K);
    }
    
    return addr;
}

// Get value from address
template<unsigned N, unsigned K, unsigned T>
inline std::uint8_t Address<N, K, T>::getValue(const FullState<N, K, T>& state) const {
    const auto& variables = state.getVariables();
    switch (address_type) {
        case EAddressType::Input:
            return variables.input.values[address];
        case EAddressType::Output:
            return variables.output.values[address];
        case EAddressType::Temp:
            return variables.temp.values[address];
        default:
            return 0;
    }
}

// Set value to address
template<unsigned N, unsigned K, unsigned T>
inline void Address<N, K, T>::setValue(FullState<N, K, T>& state, std::uint8_t value) const {
    auto& variables = state.getVariables();
    switch (address_type) {
        case EAddressType::Input:
            variables.input.values[address] = value;
            break;
        case EAddressType::Output:
            variables.output.values[address] = value;
            break;
        case EAddressType::Temp:
            variables.temp.values[address] = value;
            break;
        default:
            break;
    }
}

// Get string representation of address
template<unsigned N, unsigned K, unsigned T>
inline std::string Address<N, K, T>::toString() const {
    std::ostringstream oss;
    switch (address_type) {
        case EAddressType::Input:
            oss << "input[" << address << "]";
            break;
        case EAddressType::Output:
            oss << "output[" << address << "]";
            break;
        case EAddressType::Temp:
            oss << "temp[" << address << "]";
            break;
        default:
            oss << "unknown[" << address << "]";
            break;
    }
    return oss.str();
}

