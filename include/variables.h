// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

// Input variables structure
template<unsigned N>
struct InputVariables {
    std::array<std::uint8_t, N> values;
};

// Output variables structure
template<unsigned K>
struct OutputVariables {
    std::array<std::uint8_t, K> values;
};

// Temporary variables structure
template<unsigned T>
struct TempVariables {
    std::array<std::uint8_t, T> values;
};

// Variables structure using InputVariables and OutputVariables
template<unsigned N, unsigned K, unsigned T>
struct Variables {
    InputVariables<N> input;
    OutputVariables<K> output;
    TempVariables<T> temp;

    // Constructors
    Variables() = default;
    explicit Variables(const InputVariables<N>& input_arg);
};
