#ifndef VOLE_MACHINE_SIMULATOR_MACHINE_H
#define VOLE_MACHINE_SIMULATOR_MACHINE_H
#include "CPU.h"

class Machine {
private:
    Byte Memory[256];
    ifstream inputFile;
    vector<Byte> instructions;
    CPU cpu;
    int start_address;

    bool Load_all_Instructions();
    bool Load_New_Program();
    bool Display();
    void Load_Instructions_Into_Memory();
    void Next_Step();
    void Reset();
public:
    void Run();
};


#endif //VOLE_MACHINE_SIMULATOR_MACHINE_H
