//
// Created by Enze Zhou on 2026/8/9.
//

#ifndef RISCV_CPU_JUMPUNIT_H
#define RISCV_CPU_JUMPUNIT_H
#include <cstdint>
#include <stdexcept>

#include "ProgramCounter.h"
using namespace std;
enum Jump_op {
    JAL,
    JALR,
    NO_JUMP_OP
};

class JumpUnit {
public:
    JumpUnit(){};
    uint32_t JAL(uint32_t command_PC_value, uint32_t target_addr, ProgramCounter& program_counter) {
        program_counter.jump_taken = 1;
        program_counter.Jump(target_addr);
        return command_PC_value+4;
    }
    uint32_t JALR(uint32_t command_PC_value, uint32_t target_command_addr, ProgramCounter& program_counter) {
        program_counter.jump_taken = 1;
        program_counter.Jump(target_command_addr & ~uint32_t{1});
        return command_PC_value+4;
    }
    uint32_t operate(Jump_op jump_op, uint32_t command_PC_value, uint32_t target_command_addr, ProgramCounter& program_counter) {
        switch (jump_op) {
            case Jump_op::JAL: cout<<"JUMP TRGD"<<endl; return JAL(command_PC_value, target_command_addr, program_counter);
            case Jump_op::JALR: cout<<"JUMP TRGD"<<endl; return JALR(command_PC_value, target_command_addr, program_counter);
            case Jump_op::NO_JUMP_OP: return command_PC_value;
            default: throw runtime_error("Invalid Jump_op");
        }
    }
};

#endif //RISCV_CPU_JUMPUNIT_H