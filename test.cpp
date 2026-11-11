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
        0b00000010000000000000000010010011, // 0x00  addi x1, x0, 32    循环计数器
        0b00000000000000000000000100010011, // 0x04  addi x2, x0, 0     累加器
        0b00000000000100010000000100110011, // 0x08  add  x2, x2, x1    loop: 累加当前 x1
        0b11111111111100001000000010010011, // 0x0C  addi x1, x1, -1    计数器减 1
        0b11111110000000001001110011100011, // 0x10  bne  x1, x0, -8    x1 != 0 时跳回 0x08
        0b00000000000100010000000110010011, // 0x14  addi x3, x2, 1     退出后执行一次
    };

    ram.loadCommands(commands, num_commands);

    for (int i=0;i<1000;i++) {
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
    CPUcore core0 = CPUcore(0,23,probes);


    //Execution
    RunCommands(core0,l2cache,ram,6,probes.total_cycles);

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
    probes.calculate_IPC();
    probes.calculate_CPI();
    probes.calculate_load_hit_count();
    probes.calculate_store_hit_count();
    probes.calculate_overall_hit();


}