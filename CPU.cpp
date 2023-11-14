#include "CPU.h"

CPU::CPU() {
}

int CPU::toDec(string hex) {
    int decimal = 0, power = 1;
    for (int i = 0;i < hex.size();++i){
        decimal *= power;
        if (hex[i] >= '0' && hex[i] <= '9') decimal += hex[i] - '0';
        else decimal += hex[i] - 'A' + 10;
        power *= 16;
    }
    return decimal;
}

string CPU::toHex(int decimal) {
    string result;
    while (decimal){
        int m = decimal % 16;
        if (m >= 10) result.push_back('A' + m - 10);
        else result.push_back(m + '0');
        decimal /= 16;
    }
    reverse(result.begin(), result.end());
    if(result.empty()) result.push_back('0');
    return result;
}

void CPU::Load_From_Address(Byte Memory[]) {
    string Register, Address;
    Register.push_back(IR[1]);
    Address.push_back(IR[2]);
    Address.push_back(IR[3]);
    int register1 = toDec(Register);
    int address = toDec(Address);
    Registers[register1] = *(Memory + (address));
    return;
}

void CPU::Load_With_Pattern(Byte Memory[]) {
    string Register;
    Byte Pattern;
    Register.push_back(IR[1]);
    Pattern.nibble[0] = IR[2];
    Pattern.nibble[1] = IR[3];
    int register1 = toDec(Register);
    Registers[register1] = Pattern;
}

void CPU::Store(Byte Memory[]) {
    string Register, Address;
    Register.push_back(IR[1]);
    Address.push_back(IR[2]);
    Address.push_back(IR[3]);
    int register1 = toDec(Register);
    int address = toDec(Address);
    if(IR[2] == '0' && IR[3] == '0'){
        cout << Registers[register1].nibble[0] << Registers[register1].nibble[1] << endl;
        return;
    }
    (*(Memory+address)) = Registers[register1];
}

void CPU::Move(Byte Memory[]) {
    string register1, register2;
    register1.push_back(IR[2]);
    register2.push_back(IR[3]);
    int n1 = toDec(register1);
    int n2 = toDec(register2);
    Registers[n2] = Registers[n1];
}

Byte CPU::add(Byte byte1, Byte byte2) {
    string Hn1, Hn2;
    Hn1.push_back(byte1.nibble[0]);
    Hn1.push_back(byte1.nibble[1]);
    Hn2.push_back(byte2.nibble[0]);
    Hn2.push_back(byte2.nibble[1]);

    int n1 = toDec(Hn1), n2 = toDec(Hn2);
    int res = n1+n2;
    res&=((1<<8)-1);
    string hex = toHex(res);
    Byte f_result;
    if(hex.length()==1){
        f_result.nibble[0] = '0';
        f_result.nibble[1] = hex[0];
    }
    else{
        f_result.nibble[0] = hex[0];
        f_result.nibble[1] = hex[1];
    }
    return f_result;
}

bool CPU::compare() {
    string register1, address;
    register1.push_back(IR[1]);
    address+=(IR.substr(2,2));
    int n1 = toDec(register1), newAddress = toDec(address);
    if(Registers[n1].nibble[0] == Registers[0].nibble[0] && Registers[n1].nibble[1] == Registers[0].nibble[1]){
        ProgramCounter = newAddress;
        return true;
    }
    else
        return false;
}


void CPU::Fetch_Instruction(Byte Memory[]) {
    IR[0] = (Memory + ProgramCounter)->nibble[0];
    IR[1] = (Memory + ProgramCounter)->nibble[1];
    IR[2] = (Memory + ProgramCounter+1)->nibble[0];
    IR[3] = (Memory + ProgramCounter+1)->nibble[1];
}

void CPU::Decode_and_Excute_Instruction(Byte Memory[]) {
    if(IR[0] == '1'){
        Load_From_Address(Memory);
        ProgramCounter+=2;
    }
    else if(IR[0] == '2'){
        Load_With_Pattern(Memory);
        ProgramCounter+=2;
    }
    else if(IR[0] == '3'){
        Store(Memory);
        ProgramCounter+=2;
    }
    else if(IR[0] == '4'){
        Move(Memory);
        ProgramCounter+=2;
    }
    else if(IR[0] == '5'){
        string register1, register2, register3;
        register1+=IR[1];
        register2+=IR[2];
        register3+=IR[3];
        int n1 = toDec(register1), n2 = toDec(register2), n3 = toDec(register3);
        Registers[n1] = add(Registers[n2], Registers[n3]);
        ProgramCounter+=2;
    }
    else if(IR[0] == 'B'){
        if(!compare())
            ProgramCounter+=2;
    }
    else if(IR[0] == 'C'){
        Halted = true;
        return;
    }
    return;
}

void CPU::ResetCPU() {
    for (int i = 0; i < 16; ++i)
        Registers[i] = {{'0','0'}};
    ProgramCounter = 0;
    IR = "0000";
}