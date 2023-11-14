#include "Machine.h"

int readInt(int low, int high, bool cancel = false){
    cout << "\nEnter number in range " << low << " - " << high << ": ";
    int choice; cin >> choice;
    if(cancel && choice == -1)
        return choice;
    if (low <= choice && choice <= high)
        return choice;
    cout << "ERROR: invalid number...Try again\n";
    return readInt(low, high);
}

int readmenu(const vector<string> &choices, string header = "Menu") {
    cout << "\n"<<header<<":\n";
    for (int ch = 0; ch < (int) choices.size(); ++ch) {
        cout << "\t" << ch + 1 << ": " << choices[ch] << "\n";
    }
    return readInt(1, choices.size());
}

void Machine::Run() {
    vector<string> menu;
    menu.push_back("Load New Program");
    menu.push_back("Execute Next Step");
    menu.push_back("Execute Entire Program");
    menu.push_back("Display");
    menu.push_back("Exit");
    Reset();
    while (true){
        int choice = readmenu(menu);
        if(choice==1){
            Load_New_Program();
            cpu.ProgramCounter = start_address;
        }
        else if(choice==2){
            Next_Step();
            if(cpu.Halted){
                cout << "Program Halted" << endl;
            }
        }
        else if(choice==3){
            while (true){
                Next_Step();
                if(cpu.Halted){
                    cout << "Program Halted" << endl;
                    break;
                }
            }
        }
        else if(choice==4){
            Display();
        }
        else if(choice==5){
            break;
        }
    }
}

bool Machine::Load_New_Program() {
    Reset();
    string filename;
    cout << "Enter File Name: ";
    cin >> filename;
    inputFile.open(filename);
    if(!inputFile.is_open()){
        cout << "Error in opening file" << endl;
        return false;
    }
    else{
        if(Load_all_Instructions()){
            start_address = readInt(0, 256-instructions.size());
        }
        else{
            cout << "Couldn't load instructions" << endl;
            return false;
        }
    }
    Load_Instructions_Into_Memory();
    return true;
}

bool Machine::Load_all_Instructions() {
    string line;
    Byte first, second;
    while (getline(inputFile, line)){
        string stringFilter = "([1-5]|[B-C])[0-9A-F][0-9A-F][0-9A-F]";
        regex regexFilter(stringFilter);
        if (regex_match(line, regexFilter)){
            first.nibble[0] = line[0];
            first.nibble[1] = line[1];

            second.nibble[0] = line[2];
            second.nibble[1] = line[3];

            instructions.push_back(first);
            instructions.push_back(second);
        }
        else{
            instructions.clear();
            cout << "Invalid instructions" << std::endl;
            return false;
        }

        if (instructions.size() > 256){
            instructions.clear();
            cout << "Too many instructions" << std::endl;
            return false;
        }
    }
    return true;
}

void Machine::Load_Instructions_Into_Memory() {
    for (int i = start_address ; i < start_address+instructions.size() ; ++i) {
        Memory[i] = instructions[i-start_address];
    }
}

void Machine::Next_Step() {
    cpu.Fetch_Instruction(this->Memory);
    cpu.Decode_and_Excute_Instruction(this->Memory);
}

void Machine::Reset() {
    for (int i = 0; i < 256 ; ++i) {
        Memory[i] = {{'0','0'}};
    }
    cpu.ResetCPU();
}

bool Machine::Display() {
    fstream file;
    string filename;
    cout << "Enter File Name: ";
    cin >> filename;
    file.open(filename);
    if(!file.is_open()){
        cout << "Error in opening file" << endl;
        return false;
    }
    for (int i = 0; i < 16 ; ++i) {
        string s;
        s.push_back(cpu.Registers[i].nibble[0]);
        s.push_back(cpu.Registers[i].nibble[1]);
        file << "Register(" << cpu.toHex(i) << "): " << s << endl;
    }
    file << "--------------------------------" << endl;
    for (int i = 0; i < 256 ; ++i) {
        string s;
        s.push_back(Memory[i].nibble[0]);
        s.push_back(Memory[i].nibble[1]);
        file << "Memory Address(" << cpu.toHex(i) << "): " << s << endl;
    }
    return true;
}