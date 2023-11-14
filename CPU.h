#ifndef VOLE_MACHINE_SIMULATOR_CPU_H
#define VOLE_MACHINE_SIMULATOR_CPU_H
#include <bits/stdc++.h>
#include "Byte.h"
using namespace std;

class CPU {
private:
    void Load_From_Address(Byte Memory[]);
    void Load_With_Pattern(Byte Memory[]);
    void Store(Byte Memory[]);
    void Move(Byte Memory[]);
    Byte add(Byte byte1, Byte byte2);
    bool compare();
public:
    Byte Registers[16];
    int ProgramCounter{};
    string IR;
    string Valid[8];
    bool Halted = false;
    CPU();
    int toDec(string hex);
    string toHex(int decimal);
    void Fetch_Instruction(Byte Memory[]);
    void Decode_and_Excute_Instruction(Byte Memory[]);
    void ResetCPU();
};


#endif //VOLE_MACHINE_SIMULATOR_CPU_H
