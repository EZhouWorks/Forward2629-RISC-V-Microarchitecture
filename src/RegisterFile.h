//This file defines the 32 register
//the values are stored in 32bit binary and the value passed in will be automatically converted to binary
#ifndef RISC_V_CPU_SIMULATOR_REGISTER_H
#define RISC_V_CPU_SIMULATOR_REGISTER_H
#include <cstdint>
#include <array>
#include <iostream>
#include <bitset>
using namespace std;
enum RegFile_op {
    SAVE_DATA, //save data loaded from cache/mem
    SAVE_ALU_RESULT,
    SAVE_PC_VAL, //save PC value for JAL/JALR
    LOAD_DATA,
    NO_RegFile_OP
};
class RegisterFile {
public:
    array<uint32_t,32> registers;

    RegisterFile() {
        for (int i=0; i<32; i++) {
            registers[i] = 00000000000000000000000000000000;
        }
    };

    uint32_t read(uint32_t reg_index) {
        return registers[reg_index];
    }

    void write(uint8_t index, uint32_t value) {
        if (index != 0) {
            registers[index] = value;
        }
    }

    void reset() {
        for (int i=0; i<32; i++) {
            registers[i] = 0;
        }
    }

    void dump() {
        for (int i=0; i<32; i++) {
            cout<<registers[i]<<" ";
        }
    }

    void operate(RegFile_op RegFile_op,uint32_t data, uint32_t ALU_result,uint32_t addr,uint32_t command_PC_value) {
        switch (RegFile_op) {
            case SAVE_DATA:write(addr,data);break;
            case SAVE_ALU_RESULT:write(addr,ALU_result);break;
            case SAVE_PC_VAL:write(addr,command_PC_value);break;
            // case LOAD_DATA: read(reg_index);break;
            case NO_RegFile_OP:return;
            default: throw runtime_error("Unknown Register File Operation");
        }
    }

    //functions below are for debug purposes only
    void toHex(uint32_t reg_index) {
        cout<<"0x"<<hex<<registers[reg_index];
    }

    void dumpRawValue() {
        for (int i=0; i<32; i++) {
            cout << "reg " << i << ": 0x" << hex << registers[i] << dec << endl;
        }
    }

};
#endif //RISC_V_CPU_SIMULATOR_REGISTER_H



