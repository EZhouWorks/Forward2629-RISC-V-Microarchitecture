//
// Created by zhoue on 2026/7/17.
//

#ifndef RISC_V_CPU_SIMULATOR_PIPELINEREGISTERS_H
#define RISC_V_CPU_SIMULATOR_PIPELINEREGISTERS_H
#include <cstdint>
#include "Controller.h"
#include "L1Cache.h"
#include "BranchUnit.h"
#include "JumpUnit.h"

struct IF_ID_data {
    int valid;
    int enable;
    uint32_t machine_code;
    uint32_t command_PC_value;
    int branch_prediction;
    uint32_t branch_command_addr;
    uint32_t PHT_value;
};
struct ID_EX_data {
    int valid;
    int enable;
    ALU_op ALU_operation;
    ALU_source ALU_source1;
    ALU_source ALU_source2;
    uint32_t rs1_addr;
    uint32_t rs2_addr;
    uint32_t rd_addr;
    uint32_t rs1_val;
    uint32_t rs2_val;
    int32_t I_12bit_imm;
    uint32_t I_shamt_imm;
    uint32_t Store_imm;
    Store_op Store_op;
    Memory_op Memory_op;
    Memory_data_type Memory_data_type;
    RegFile_op RegFile_op;
    Branch_op Branch_op;
    int32_t B_imm;
    uint32_t command_PC_value;
    Jump_op jump_op;
    uint32_t J_imm;
    int branch_prediction;
    uint32_t branch_command_addr;
    uint32_t PHT_value;
};
struct EX_MEM_data {
    int valid;
    int enable;
    uint32_t ALU_result;
    Memory_op Memory_op;
    Memory_data_type Memory_data_type;
    uint32_t Store_imm;
    Store_op Store_op;
    uint32_t rd_addr;
    uint32_t rs1_val;
    uint32_t rs2_val;
    RegFile_op RegFile_op;
    uint32_t command_PC_value;
    int prev_store_op;
    uint32_t store_op_target_addr;
};
struct MEM_WB_data {
    int valid;
    int enable;
    uint32_t ALU_result;
    uint32_t Store_imm;
    Store_op Store_op;
    uint32_t rd_addr;
    uint32_t rs1_val;
    uint32_t rs2_val;
    uint32_t data;
    uint32_t store_data;
    RegFile_op RegFile_op;
    Memory_op Memory_op;
    uint32_t command_PC_value;

};

class PipelineRegisters {
public:
    IF_ID_data IF_ID_register;
    ID_EX_data ID_EX_register;
    EX_MEM_data EX_MEM_register;
    MEM_WB_data MEM_WB_register;
    PipelineRegisters() {
        IF_ID_register.valid = 0;
        ID_EX_register.valid = 0;
        EX_MEM_register.valid = 0;
        MEM_WB_register.valid = 0;
        EX_MEM_register.Memory_op = READBYTE;
        MEM_WB_register.Store_op = STOREBYTE;

        IF_ID_register.enable = 1;
        ID_EX_register.enable = 1;
        EX_MEM_register.enable = 1;
        MEM_WB_register.enable = 1;
    }
};

#endif //RISC_V_CPU_SIMULATOR_PIPELINEREGISTERS_H
