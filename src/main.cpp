// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#include <iostream>
#include "B0/instructions.h"
#include "B1/instructions.h"
#include "B0/instructions.hpp"
#include "B1/instructions.hpp"
#include "address.hpp"
#include "fabric.h"
#include "fabric.hpp"
#include "optimize.h"
#include "optimize.hpp"
#include "rabbit_turtle.h"
#include "rabbit_turtle.hpp"
#include "executor.hpp"
#include "full_state.hpp"
#include "program.hpp"
#include "variables.h"
#include "variables.hpp"

// Forward declaration
void demonstrateOptimization();

int main() {
    // Program parameters: 3 input variables, 1 output variable, 0 temporary variables
    constexpr unsigned N = 3;
    constexpr unsigned K = 1;
    constexpr unsigned T = 0;

    // Create input variables with test values
    InputVariables<N> input;
    for (unsigned i = 0; i < N; ++i) {
        input.values[i] = static_cast<std::uint8_t>(i + 1); // Values 1, 2, 3
    }

    // Create program
    Program<B0::InstructionSet, N, K, T> program;

    // Initialize output[0] with input[0]
    B0::Move<N, K, T> move_init;
    move_init.source.address_type = Address<N, K, T>::EAddressType::Input;
    move_init.source.address = 0;
    move_init.destination.address_type = Address<N, K, T>::EAddressType::Output;
    move_init.destination.address = 0;
    program.add(move_init);

    // Add input[1] and input[2] to output[0]
    for (unsigned i = 1; i < N; ++i) {
        B0::Add<N, K, T> add_inst;
        add_inst.operand1.address_type = Address<N, K, T>::EAddressType::Output;
        add_inst.operand1.address = 0;
        add_inst.operand2.address_type = Address<N, K, T>::EAddressType::Input;
        add_inst.operand2.address = i;
        add_inst.result.address_type = Address<N, K, T>::EAddressType::Output;
        add_inst.result.address = 0;
        program.add(add_inst);
    }

    // Create RabbitTurtle executor
    RabbitTurtle<B0::InstructionSet, N, K, T> rabbit_turtle(program, input);

    // Start execution
    rabbit_turtle.start();

    // Execute until program finishes or infinite loop detected
    while (rabbit_turtle.execute()) {
        if (rabbit_turtle.isInfiniteLoopDetected()) {
            std::cout << "Infinite loop detected!" << std::endl;
            break;
        }
    }

    // Print results
    std::cout << "Input values: ";
    for (unsigned i = 0; i < N; ++i) {
        std::cout << static_cast<unsigned>(input.values[i]);
        if (i < N - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "Sum (output[0]): " << static_cast<unsigned>(rabbit_turtle.getOutput().values[0]) << std::endl;

    // Calculate expected sum for verification
    unsigned expected_sum = 0;
    for (unsigned i = 0; i < N; ++i) {
        expected_sum += static_cast<unsigned>(input.values[i]);
    }
    std::cout << "Expected sum: " << expected_sum << std::endl;
    std::cout << "Note: Result may differ due to uint8_t overflow (max 255)" << std::endl;

    // Print program text representation
    std::cout << "\n=== Program Text Representation ===" << std::endl;
    std::cout << program.dump();

    // Create Fabric with combination indices
    constexpr unsigned program_length = 3;
    Fabric<B0::InstructionSet, N, K, T> fabric(program_length);
    std::cout << "\n=== Fabric Demo ===" << std::endl;
    std::cout << "Fabric created with program length: " << fabric.getProgramLen() << std::endl;
    std::cout << "First combination ID: " << fabric.getCombinationStrId() << std::endl;
    std::cout << "Last combination ID: " << fabric.getLastProgramStrId() << std::endl;
    
    // Demonstrate next() method and dump programs
    std::cout << "\nFirst few combinations with program dumps:" << std::endl;
    
    // Reset fabric to first combination
    Fabric<B0::InstructionSet, N, K, T> fabric_for_dump(program_length);
    
    for (int i = 0; i < 5; ++i) {
        std::cout << "\n--- Combination " << i << " (ID: " << fabric_for_dump.getCombinationStrId() << ") ---" << std::endl;
        auto generated_program = fabric_for_dump.generate();
        std::cout << generated_program.dump();
        
        if (!fabric_for_dump.next()) {
            break;
        }
    }

    // Demonstrate optimization
    demonstrateOptimization();

    return 0;
}

// Function to demonstrate optimization
void demonstrateOptimization() {
    std::cout << "\n=== Optimization Demo ===" << std::endl;
    
    // Program parameters: 2 input variables, 1 output variable, 0 temporary variables
    constexpr unsigned N = 2;
    constexpr unsigned K = 1;
    constexpr unsigned T = 0;
    
    // Create reference program that computes sum using two loops with Inc/Dec
    // Algorithm:
    // 1. First loop: while input[0] != 0:
    //    - Inc output[0]
    //    - Dec input[0]
    // 2. Second loop: while input[1] != 0:
    //    - Dec input[1]
    //    - Inc output[0]
    // Result: output[0] = input[0] + input[1]
    Program<B1::InstructionSet, N, K, T> reference_program;
    
    // First loop: while input[0] != 0
    // Step 1: Loop start (position 0)
    // Step 2: Jump to second loop if input[0] == 0 (position 1)
    B1::JumpIfZero<N, K, T> jump_if_input0_zero;
    jump_if_input0_zero.operand.address_type = Address<N, K, T>::EAddressType::Input;
    jump_if_input0_zero.operand.address = 0;
    jump_if_input0_zero.target = 4; // Jump to second loop start (position 4) if input[0] == 0
    reference_program.add(jump_if_input0_zero);
    
    // Step 3: Inc output[0] (position 2)
    B1::Inc<N, K, T> inc_output1;
    inc_output1.address.address_type = Address<N, K, T>::EAddressType::Output;
    inc_output1.address.address = 0;
    reference_program.add(inc_output1);
    
    // Step 4: Dec input[0] (position 3)
    B1::Dec<N, K, T> dec_input0;
    dec_input0.address.address_type = Address<N, K, T>::EAddressType::Input;
    dec_input0.address.address = 0;
    reference_program.add(dec_input0);
    
    // Step 5: Goto first loop start (position 0) (position 4)
    B1::Goto<N, K, T> goto_loop1;
    goto_loop1.target = 0;
    reference_program.add(goto_loop1);
    
    // Second loop: while input[1] != 0
    // Step 6: Loop start (position 4)
    // Step 7: Jump to end if input[1] == 0 (position 5)
    B1::JumpIfZero<N, K, T> jump_if_input1_zero;
    jump_if_input1_zero.operand.address_type = Address<N, K, T>::EAddressType::Input;
    jump_if_input1_zero.operand.address = 1;
    jump_if_input1_zero.target = 8; // Jump to end (position 8) if input[1] == 0
    reference_program.add(jump_if_input1_zero);
    
    // Step 8: Dec input[1] (position 6)
    B1::Dec<N, K, T> dec_input1;
    dec_input1.address.address_type = Address<N, K, T>::EAddressType::Input;
    dec_input1.address.address = 1;
    reference_program.add(dec_input1);
    
    // Step 9: Inc output[0] (position 7)
    B1::Inc<N, K, T> inc_output2;
    inc_output2.address.address_type = Address<N, K, T>::EAddressType::Output;
    inc_output2.address.address = 0;
    reference_program.add(inc_output2);
    
    // Step 10: Goto second loop start (position 4) (position 8)
    B1::Goto<N, K, T> goto_loop2;
    goto_loop2.target = 4;
    reference_program.add(goto_loop2);
    
    // Program ends here (position 10)
    
    // Program ends here
    
    // Create Fabric to get last program ID
    Fabric<B1::InstructionSet, N, K, T> fabric(reference_program.size());
    std::cout << "Reference program length: " << reference_program.size() << std::endl;
    std::cout << "Last program ID: " << fabric.getLastProgramStrId() << std::endl;
    
    std::cout << "Reference program (sum using loop with Inc/Dec):" << std::endl;
    std::cout << reference_program.dump() << std::endl;
    
    // Test the reference program
    InputVariables<N> test_input;
    test_input.values[0] = 3;
    test_input.values[1] = 5;
    
    RabbitTurtle<B1::InstructionSet, N, K, T> rt(reference_program, test_input);
    rt.start();
    
    while (rt.execute()) {
        if (rt.isInfiniteLoopDetected()) {
            std::cout << "Warning: Infinite loop detected in reference program!" << std::endl;
            break;
        }
    }
    
    std::cout << "Test input: [" << static_cast<unsigned>(test_input.values[0]) 
              << ", " << static_cast<unsigned>(test_input.values[1]) << "]" << std::endl;
    std::cout << "Reference program result: " << static_cast<unsigned>(rt.getOutput().values[0]) << std::endl;
    std::cout << "Expected sum: " << (static_cast<unsigned>(test_input.values[0]) + static_cast<unsigned>(test_input.values[1])) << std::endl;
    
    // Calculate and display total steps for reference program
    Optimize<B1::InstructionSet, N, K, T> optimizer(reference_program);
    std::uint64_t reference_total_steps = optimizer.calculateAverageSteps(reference_program);
    std::cout << "Reference program total steps: " << reference_total_steps << std::endl;
    
    // Display last program ID for program length = 1 (optimization search space)
    constexpr unsigned max_program_size = 1;
    Fabric<B1::InstructionSet, N, K, T> fabric_opt(max_program_size);
    std::cout << "Last program ID for length " << max_program_size << ": " << fabric_opt.getLastProgramStrId() << std::endl;
    
    // Now try to optimize
    std::cout << "\nAttempting optimization (max program size: 1 instruction)..." << std::endl;
    std::cout << "This may take a while..." << std::endl;
    
    // Search for programs up to size 1
    Program<B1::InstructionSet, N, K, T> optimized_program = optimizer.speed(max_program_size);
    
    std::cout << "\nOptimized program:" << std::endl;
    std::cout << optimized_program.dump() << std::endl;
    
    // Test the optimized program
    InputVariables<N> test_input_opt;
    test_input_opt.values[0] = 3;
    test_input_opt.values[1] = 5;
    
    RabbitTurtle<B1::InstructionSet, N, K, T> rt_opt(optimized_program, test_input_opt);
    rt_opt.start();
    
    while (rt_opt.execute()) {
        if (rt_opt.isInfiniteLoopDetected()) {
            std::cout << "Warning: Infinite loop detected in optimized program!" << std::endl;
            break;
        }
    }
    
    std::cout << "Optimized program result: " << static_cast<unsigned>(rt_opt.getOutput().values[0]) << std::endl;
    std::cout << "Expected sum: " << (static_cast<unsigned>(test_input_opt.values[0]) + static_cast<unsigned>(test_input_opt.values[1])) << std::endl;
}

