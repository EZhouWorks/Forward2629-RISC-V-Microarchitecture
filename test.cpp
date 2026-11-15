//this is a test environment for each cpu core
#include "src/RegisterFile.h"
#include<iostream>
#include<string>
#include<bitset>
#include<array>
#include "src/CPUcore.h"
#include <vector>
#include"src/Probe.h"

using namespace std;
void dumpSelectedReg(CPUcore core, int reg1_index, int reg2_index, int reg3_index) {
    cout<<"Reg "<<reg1_index<<" :"<<core.registerFile.read(reg1_index)<<endl;
    cout<<"Reg "<<reg2_index<<" :"<<core.registerFile.read(reg2_index)<<endl;
    cout<<"Reg "<<reg3_index<<" :"<<core.registerFile.read(reg3_index)<<endl;
}

int RunCommands(CPUcore &core, L2Cache& l2cache, RAM &ram, int num_commands, int& total_cycles) {
    uint32_t commands[] = {
        0b00000000010000000000000011101111, // jal x1, +4
    0b00111111110000001000000010010011, // addi x1, x1, 1020
    0b00000000000100000000000100010011, // addi x2, x0, 1
    0b00000000111000010001000100010011, // slli x2, x2, 14
    0b00000000001000001000000100110011, // add x2, x1, x2

    0b00000000000100000000001000010011, // addi x4, x0, 1
    0b00000000010000001010000000100011, // sw x4, 0(x1)
    0b00000000001000000000001000010011, // addi x4, x0, 2
    0b00000000010000010010000000100011, // sw x4, 0(x2)

    0b00000000000000000000001010010011, // addi x5, x0, 0
    0b00000100000000000000001100010011, // addi x6, x0, 64

    // loop: PC = 0x2C
    0b00000000000000001010001110000011, // lw x7, 0(x1)
    0b00000000000000010010010000000011, // lw x8, 0(x2)
    0b00000000011100101000001010110011, // add x5, x5, x7
    0b00000000100000101000001010110011, // add x5, x5, x8
    0b11111111111100110000001100010011, // addi x6, x6, -1
    0b11111110000000110001011011100011  // bne x6, x0, -20
    };

    ram.loadCommands(commands, num_commands);

    for (int i=0;i<1000000;i++) {
        cout<<endl;
        cout<<"================================"<<endl;
        cout<<"->Cycle "<<i<<endl;
        core.Step(l2cache,ram);

        if (i != 0 and core.pipeline_registers_read.IF_ID_register.valid == 0
            and core.pipeline_registers_read.ID_EX_register.valid == 0
            and core.pipeline_registers_read.EX_MEM_register.valid == 0
            and core.pipeline_registers_read.MEM_WB_register.valid == 0) {
            total_cycles = i+1;
            break;
            }
    }
    core.registerFile.dumpRawValue();
    return total_cycles;
}

int main() {
    //component initialization
    Probe probes = Probe();
    RAM ram = RAM();
    L2Cache l2cache = L2Cache(ram);
    CPUcore core0 = CPUcore(1,67,probes);


    //Execution
    RunCommands(core0,l2cache,ram,17,probes.total_cycles);

    //Show statistics
    cout<<endl;
    cout<<"Statistics: "<<endl;
    cout<<"Total Cycles: "<<probes.total_cycles<<endl;
    cout<<"Retired instructions: "<<probes.retired_instructions<<endl;
    if (core0.branch_predictor.enable == 1) {
        cout<<"Branch Predictor Status: On"<<endl;
    }
    else {
        cout<<"Branch Predictor Status: Off"<<endl;
    }
    if (core0.set_associative_on == 1) {
        cout<<"Cache Mode: Set associative"<<endl;
    }
    else {cout<<"Cache Mode: Direct mapping"<<endl;}
    cout<<"Stall Count: "<<probes.stall_count<<endl;

    //code below are for debug purposes only
    // cout << unsigned(ram.readCell(4096))  << endl; // 0
    // cout << unsigned(ram.readCell(20480)) << endl; // 99
    // cout << unsigned(ram.readCell(36864)) << endl; // 0
    probes.calculate_BPA(1);
    probes.calculate_executed_BPA();
    probes.calculate_IPC();
    probes.calculate_CPI();
    probes.calculate_load_hit_count();
    probes.calculate_store_hit_count();
    probes.calculate_overall_hit();


}