// FCAI-CU OOP Assignment-2 task-2
// Authors:
//Abdelrhman Amr Fathy_20210228_bondok735489@gmail.com
//Abdelrhman Ashraf El-Sayed_20210198_Abdelrhmanashrafelsayed1@gmail.com
//Mohamed Samy Sayed_20210339_Samy45790@gmail.com
// Date: 15/11/2023

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>  

using namespace std;

int hexToDecimal(char hexValue) {
    int decimalValue = 0;

    if (isdigit(hexValue)) {
        decimalValue = hexValue - '0';
    } else {
        decimalValue = toupper(hexValue) - 'A' + 10;
    }

    return decimalValue;
}

int hexToDecimal(string hexValue) {
    int decimalValue = 0;
    int hexLength = hexValue.length();

    for (int i = 0; i < hexLength; ++i) {
        char currentHexDigit = hexValue[i];
        int digitValue;

        if (isdigit(currentHexDigit)) {
            digitValue = currentHexDigit - '0';
        } else {
            digitValue = toupper(currentHexDigit) - 'A' + 10;
        }

        decimalValue += digitValue * pow(16, hexLength - 1 - i);
    }

    return decimalValue;
}

class Register {
protected:
    int result;

public:
    Register() : result(0) {}

    void setresult(int newresult) {
        result = newresult;
    }

    int getresult() const {
        return result;
    }
};

class Memory {
protected:
    string instructionset;
    ifstream file;
    vector<string> instructions;
    vector<int> memory;  // Memory cells
    int Counter;
    Register registers[16];
public:
    Memory(const string &instructionset) : instructionset(instructionset), memory(256) {
        file.open(instructionset);

        if (!file.is_open()) {
            cerr << "Error: Can't open file " << instructionset << endl;
            exit(EXIT_FAILURE);
        }
    }

    ~Memory() {
        if (file.is_open()) {
            file.close();
        }
    }

    void ReadData() {
        if (!file.is_open()) {
            cerr << "Error: File is not open." << endl;
            return;
        }

        string line;
        int memoryIndex = 0;

        while (getline(file, line) && memoryIndex < 256) {
            instructions.push_back(line);

            istringstream iss(line);
            int instructionValue;
            iss >> dec >> instructionValue;
            memory[memoryIndex++] = instructionValue;
        }
        Counter = 0;

}
    const vector<int>& getMemory() const {
    return memory;
}

int getCounter() const {
    return Counter;
}

const Register* getRegisters() const {
    return registers;
}

};

class Instruction : public Memory {
public:
Instruction(const string &instructionset) : Memory(instructionset) {}
    void ExecuteInstruction(char opcode, int operand1, int operand2, string sOperand2);

    void load(int operand1, int operand2) {
        registers[operand1].setresult(memory[operand2]);
    }


    void store(int operand1, int operand2) {
        memory[operand2] = registers[operand1].getresult();
        if (operand2 == 0) {
            cout << memory[0];
        }
    }

    void Transfer(int operand1, string sOperand2) {
        registers[sOperand2[1]].setresult(registers[sOperand2[0]].getresult());
    }

    void Add(int operand1, string sOperand2) {
       // registers[operand1].setresult(memory[sOperand2[0]] + memory[sOperand2[1]]);
        int address1 = hexToDecimal(sOperand2.substr(0, 1));
    int address2 = hexToDecimal(sOperand2.substr(1, 1));
    registers[operand1].setresult(memory[address1] + memory[address2]);
    }

    void UP(int operand1, int operand2) {
        if (registers[operand1].getresult() == registers[0].getresult()) {
             Counter = operand2 - 1;
        }
    }

    void H() {}
};

void Instruction::ExecuteInstruction(char opcode, int operand1, int operand2, string sOperand2) {
}

class Machine : public Instruction {
public:

    Machine(const string &instructionset) : Instruction(instructionset) {}

    void ExecuteProgram() {
        int Counter = 0;

        while (getCounter() < memory.size()) {
            string sInstruction = instructions[getCounter()];
            char opcode = sInstruction[2];

            if (opcode == 'C') {
                break;
            }

            char sOperand1 = sInstruction[6];
            int operand1 = hexToDecimal(sOperand1);
            string sOperand2 = sInstruction.substr(10);
            int operand2 = hexToDecimal(sOperand2);

            ExecuteInstruction(opcode, operand1, operand2, sOperand2);
            Counter++;
        }
    }
     void displayStatus() const {
            cout << "Program Counter: " << getCounter() << endl;

            cout << "Registers:\n";
            for (int i = 0; i < 16; ++i) {
                cout << "R" << i << ": " << setw(4) << setfill('0') << getRegisters()[i].getresult() << "   ";
                if ((i + 1) % 4 == 0) {
                    cout << endl;
                }
            }

            cout << "Memory Content:\n";
            for (int i = 0; i < getMemory().size(); ++i) {
                cout << setw(4) << setfill('0') << hex << i << ": " << setw(4) << setfill('0') << getMemory()[i] << "   ";
                if ((i + 1) % 4 == 0) {
                    cout << endl;
                }
            }
        }

};
int main() {
    Machine machine("textfile.txt");

    Memory memory("textfile.txt");
    memory . ReadData();
        int test;

        cout << "1- Execute one Step\n";
        cout << "2- Display Status\n";
        cout << "3- Exit\n";
        cout << "Enter your choice: \n";
        cin>>test;
           switch (test) {
            case 1:
                machine.ExecuteProgram();
                break;
            case 2:
                machine.displayStatus();
                break;
            case 3:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                main();
        }
           
}
