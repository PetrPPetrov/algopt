// Copyright 2025 Petr Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

#include <iostream>
#include "B0/instructions.h"
#include "B1/instructions.h"
#include "S0/instructions.h"
#include "B0/instructions.hpp"
#include "B1/instructions.hpp"
#include "S0/instructions.hpp"
#include "address.hpp"
#include "fabric.h"
#include "fabric.hpp"
#include "optimize.h"
#include "optimize.hpp"
#include "rabbit_turtle.h"
#include "rabbit_turtle.hpp"
#include "debug_executor.h"
#include "debug_executor.hpp"
#include "executor.hpp"
#include "full_state.hpp"
#include "program.hpp"
#include "variables.h"
#include "variables.hpp"

// Forward declaration
void demonstrateOptimization();
void demonstrateBubbleSort();

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
    
    // Demonstrate bubble sort
    demonstrateBubbleSort();

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

// Function to demonstrate bubble sort
void demonstrateBubbleSort() {
    std::cout << "\n=== Bubble Sort Demo ===" << std::endl;
    
    // Program parameters: 16 input variables, 16 output variables, 5 temporary variables
    constexpr unsigned N = 16;
    constexpr unsigned K = 16;
    constexpr unsigned T = 6;
    
    // Create input variables with test values (unsorted array)
    InputVariables<N> input;
    // Initialize with some test values: [5, 2, 8, 1, 9, 3, 7, 4, 6, 10, 15, 12, 11, 14, 13, 16]
    input.values[0] = 5;
    input.values[1] = 2;
    input.values[2] = 8;
    input.values[3] = 1;
    input.values[4] = 9;
    input.values[5] = 3;
    input.values[6] = 7;
    input.values[7] = 4;
    input.values[8] = 6;
    input.values[9] = 10;
    input.values[10] = 15;
    input.values[11] = 12;
    input.values[12] = 11;
    input.values[13] = 14;
    input.values[14] = 13;
    input.values[15] = 16;
    
    // Create program for bubble sort
    Program<S0::InstructionSet, N, K, T> program;

    // Use temp[2] for constant 15
    S0::SetC<N, K, T> set_limit_15;
    set_limit_15.address.address_type = Address<N, K, T>::EAddressType::Temp;
    set_limit_15.address.address = 2;
    set_limit_15.constant = 15;
    program.add(set_limit_15);

    // Use temp[2] for constant 16
    S0::SetC<N, K, T> set_limit_16;
    set_limit_16.address.address_type = Address<N, K, T>::EAddressType::Temp;
    set_limit_16.address.address = 5;
    set_limit_16.constant = 16;
    program.add(set_limit_16);

    // LoadIndirect: load input[temp[1]] into temp[0] (temporary storage)
    S0::LoadIndirect<N, K, T> load_input;
    load_input.index_address.address_type = Address<N, K, T>::EAddressType::Temp;
    load_input.index_address.address = 1;
    load_input.array_type = Address<N, K, T>::EAddressType::Input;
    load_input.result_address.address_type = Address<N, K, T>::EAddressType::Temp;
    load_input.result_address.address = 0; // Temporary storage
    program.add(load_input);

    // StoreIndirect: store temp[0] to output[temp[1]]
    S0::StoreIndirect<N, K, T> store_output;
    store_output.value_source.address_type = Address<N, K, T>::EAddressType::Temp;
    store_output.value_source.address = 0;
    store_output.index_address.address_type = Address<N, K, T>::EAddressType::Temp;
    store_output.index_address.address = 1;
    store_output.array_type = Address<N, K, T>::EAddressType::Output;
    program.add(store_output);

    // Inc temp[1] (i++)
    S0::Inc<N, K, T> inc_i;
    inc_i.address.address_type = Address<N, K, T>::EAddressType::Temp;
    inc_i.address.address = 1;
    program.add(inc_i);

    size_t copy_loop_end = program.size() + 2;
    S0::JumpIfEqual<N, K, T> jump_if_copy_done;
    jump_if_copy_done.operand1.address_type = Address<N, K, T>::EAddressType::Temp;
    jump_if_copy_done.operand1.address = 1; // i
    jump_if_copy_done.operand2.address_type = Address<N, K, T>::EAddressType::Temp;
    jump_if_copy_done.operand2.address = 5; // 16
    jump_if_copy_done.target = copy_loop_end;
    program.add(jump_if_copy_done);

    // Goto copy loop start
    S0::Goto<N, K, T> goto_copy_loop;
    goto_copy_loop.target = 2;
    program.add(goto_copy_loop);

    S0::SetC<N, K, T> reset_i;
    reset_i.address.address_type = Address<N, K, T>::EAddressType::Temp;
    reset_i.address.address = 1;
    reset_i.constant = 0;
    program.add(reset_i);

    // loop
    size_t outer_loop_start = program.size();
    // Move: temp[3] = temp[1],  j = i
    S0::Move<N, K, T> move_j;
    move_j.source.address_type = Address<N, K, T>::EAddressType::Temp;
    move_j.source.address = 1;
    move_j.destination.address_type = Address<N, K, T>::EAddressType::Temp;
    move_j.destination.address = 3;
    program.add(move_j);

    // loop
    S0::Move<N, K, T> move_j2;
    move_j2.source.address_type = Address<N, K, T>::EAddressType::Temp;
    move_j2.source.address = 3;
    move_j2.destination.address_type = Address<N, K, T>::EAddressType::Temp;
    move_j2.destination.address = 4;
    program.add(move_j2);

    size_t inner_loop_start = program.size();
    // Inc temp[4] (j++)
    S0::Inc<N, K, T> inc_j;
    inc_j.address.address_type = Address<N, K, T>::EAddressType::Temp;
    inc_j.address.address = 4;
    program.add(inc_j);

    // Calculate target: after swap instruction (1 instruction ahead)
    size_t after_swap = program.size() + 2;
    S0::JumpIfLessIndirect<N, K, T> compare_swap;
    compare_swap.index1_address.address_type = Address<N, K, T>::EAddressType::Temp;
    compare_swap.index1_address.address = 3; // i
    compare_swap.index2_address.address_type = Address<N, K, T>::EAddressType::Temp;
    compare_swap.index2_address.address = 4; // j
    compare_swap.array_type = Address<N, K, T>::EAddressType::Output;
    compare_swap.target = after_swap;
    program.add(compare_swap);

    // Swap output[j] and output[j+1]
    S0::SwapIndirect<N, K, T> swap_elems;
    swap_elems.index1_address.address_type = Address<N, K, T>::EAddressType::Temp;
    swap_elems.index1_address.address = 3; // j
    swap_elems.index2_address.address_type = Address<N, K, T>::EAddressType::Temp;
    swap_elems.index2_address.address = 4; // j+1
    swap_elems.array_type = Address<N, K, T>::EAddressType::Output;
    program.add(swap_elems);
    
    // After swap (or if no swap needed)
    // Inc j: temp[2]++
    S0::Inc<N, K, T> inc_j2;
    inc_j2.address.address_type = Address<N, K, T>::EAddressType::Temp;
    inc_j2.address.address = 3;
    program.add(inc_j2);

    size_t inner_loop_end = program.size() + 2;
    S0::JumpIfEqual<N, K, T> jump_if_inner_done;
    jump_if_inner_done.operand1.address_type = Address<N, K, T>::EAddressType::Temp;
    jump_if_inner_done.operand1.address = 3; // i
    jump_if_inner_done.operand2.address_type = Address<N, K, T>::EAddressType::Temp;
    jump_if_inner_done.operand2.address = 2; // 15
    jump_if_inner_done.target = inner_loop_end;
    program.add(jump_if_inner_done);
    
    // Goto inner start
    S0::Goto<N, K, T> goto_inner_loop;
    goto_inner_loop.target = inner_loop_start;
    program.add(goto_inner_loop);

    S0::Inc<N, K, T> inc_i2;
    inc_i2.address.address_type = Address<N, K, T>::EAddressType::Temp;
    inc_i2.address.address = 1;
    program.add(inc_i2);

    size_t outer_loop_end = program.size() + 2;
    S0::JumpIfEqual<N, K, T> jump_if_outer_done;
    jump_if_outer_done.operand1.address_type = Address<N, K, T>::EAddressType::Temp;
    jump_if_outer_done.operand1.address = 1; // i
    jump_if_outer_done.operand2.address_type = Address<N, K, T>::EAddressType::Temp;
    jump_if_outer_done.operand2.address = 2; // 15
    jump_if_outer_done.target = outer_loop_end;
    program.add(jump_if_outer_done);

    // Goto outer start
    S0::Goto<N, K, T> goto_outer_loop;
    goto_outer_loop.target = outer_loop_start;
    program.add(goto_outer_loop);
   
    std::cout << "Program created with " << program.size() << " instructions" << std::endl;
    std::cout << "Program dump:" << std::endl;
    std::cout << program.dump() << std::endl;
    
    // Execute the program using DebugExecutor
    DebugExecutor<S0::InstructionSet, N, K, T> debug_executor(program, input);
    debug_executor.execute();
    
    // Print results
    std::cout << "\n=== Bubble Sort Results ===" << std::endl;
    std::cout << "Original input array: ";
    for (unsigned i = 0; i < N; ++i) {
        std::cout << static_cast<unsigned>(input.values[i]);
        if (i < N - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "Sorted output array: ";
    for (unsigned i = 0; i < K; ++i) {
        std::cout << static_cast<unsigned>(debug_executor.getOutput().values[i]);
        if (i < K - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    std::cout << "Execution completed in " << debug_executor.getStepCount() << " steps" << std::endl;
    if (debug_executor.isInfiniteLoopDetected()) {
        std::cout << "Warning: Infinite loop was detected during execution!" << std::endl;
    }
}
