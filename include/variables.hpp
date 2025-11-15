// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#pragma once

// Constructors
template<unsigned N, unsigned K, unsigned T>
inline Variables<N, K, T>::Variables(const InputVariables<N>& input_arg)
    : input(input_arg), output{}, temp{} {
    // Output and temp variables are explicitly zero-initialized
}

