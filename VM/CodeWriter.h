#pragma once
#include <string>
#include <fstream>
#include "../VM/Utility/RAM.h"
#include "../VM/VirtualMachine.h"

#define SP_DEC "@SP\nAM=M-1\n"
#define SP_INC "@SP\nM=M+1\n"
#define LOAD_SP_TO_D "D=M\n"
#define LOAD_A_TO_D "A=M\nD=M\n"
#define WRITE_D_TO_SP "@SP\nA=M\nM=D\n"
#define A_DEC "A=A-1\n"

class CodeWriter {
private:
	ofstream& output;
	RAM& ram;

public:
	CodeWriter(ofstream& output, RAM& ram) : output(output), ram(ram) {};

	void writePushPop(const string& command, const string& segment, const int& index) {
		// argument(ARG).
		if (command == "C_PUSH" && segment == "argument") {
			output << "@ARG" << "\n"
				<< "D=M" << "\n"
				<< "@" << index << "\n"
				<< "A=D+A" << "\n"
				<< "D=M" << "\n"
				<< WRITE_D_TO_SP
				<< SP_INC;
			int baseAddr = ram.getAddress("ARG");
			int value = ram.getMemory(baseAddr + index);
			ram.pushStack(value);
		}
		else if (command == "C_POP" && segment == "argument") {
			output << "@ARG" << "\n"
				<< "D=M" << "\n"
				<< "@" << index << "\n"
				<< "D=D+A" << "\n"
				<< "@R13" << "\n"
				<< "M=D" << "\n"
				<< SP_DEC
				<< LOAD_SP_TO_D
				<< "@R13" << "\n"
				<< "A=M" << "\n"
				<< "M=D" << "\n";
			int baseAddr = ram.getAddress("ARG");
			int value = ram.popStack();
			ram.setMemory(baseAddr + index, value);
		}

		// local(LCL).
		if (command == "C_PUSH" && segment == "local") {
			output << "@LCL" << "\n"
				<< "D=M" << "\n"
				<< "@" << index << "\n"
				<< "A=D+A" << "\n"
				<< "D=M" << "\n"
				<< WRITE_D_TO_SP
				<< SP_INC;
			int baseAddr = ram.getAddress("LCL");
			int value = ram.getMemory(baseAddr + index);
			ram.pushStack(value);
		}
		else if (command == "C_POP" && segment == "local") {
			output << "@LCL" << "\n"
				<< "D=M" << "\n"
				<< "@" << index << "\n"
				<< "D=D+A" << "\n"
				<< "@R13" << "\n"
				<< "M=D" << "\n"
				<< SP_DEC
				<< LOAD_SP_TO_D
				<< "@R13" << "\n"
				<< "A=M" << "\n"
				<< "M=D" << "\n";
			int baseAddr = ram.getAddress("LCL");
			int value = ram.popStack();
			ram.setMemory(baseAddr + index, value);
		}

		// static.
		if (command == "C_PUSH" && segment == "static") {
			output << "@Static" << index << "\n"
				<< "D=M" << "\n"
				<< WRITE_D_TO_SP
				<< SP_INC;
			int value = ram.getMemory(16 + index);
			ram.pushStack(value);
		}
		else if (command == "C_POP" && segment == "static") {
			output << SP_DEC
				<< LOAD_SP_TO_D
				<< "@Static" << index << "\n"
				<< "M=D" << "\n";
			int value = ram.popStack();
			ram.setMemory(16 + index, value);
		}

		// constant.
		if (command == "C_PUSH" && segment == "constant") {
			output << "@" << index << "\n"
				<< "D=A" << "\n"
				<< WRITE_D_TO_SP
				<< SP_INC;
			ram.pushStack(index);
		}

		// this(THIS).
		if (command == "C_PUSH" && segment == "this") {
			output << "@THIS" << "\n"
				<< "D=M" << "\n"
				<< "@" << index << "\n"
				<< "A=D+A" << "\n"
				<< "D=M" << "\n"
				<< WRITE_D_TO_SP
				<< SP_INC;
			int baseAddr = ram.getAddress("THIS");
			int value = ram.getMemory(baseAddr + index);
			ram.pushStack(value);
		}
		else if (command == "C_POP" && segment == "this") {
			output << "@THIS" << "\n"
				<< "D=M" << "\n"
				<< "@" << index << "\n"
				<< "D=D+A" << "\n"
				<< "@R13" << "\n"
				<< "M=D" << "\n"
				<< SP_DEC
				<< LOAD_SP_TO_D
				<< "@R13" << "\n"
				<< "A=M" << "\n"
				<< "M=D" << "\n";
			int baseAddr = ram.getAddress("THIS");
			int value = ram.popStack();
			ram.setMemory(baseAddr + index, value);
		}

		// that(THAT).
		if (command == "C_PUSH" && segment == "that") {
			output << "@THAT" << "\n"
				<< "D=M" << "\n"
				<< "@" << index << "\n"
				<< "A=D+A" << "\n"
				<< "D=M" << "\n"
				<< WRITE_D_TO_SP
				<< SP_INC;
			int baseAddr = ram.getAddress("THAT");
			int value = ram.getMemory(baseAddr + index);
			ram.pushStack(value);
		}
		else if (command == "C_POP" && segment == "that") {
			output << "@THAT" << "\n"
				<< "D=M" << "\n"
				<< "@" << index << "\n"
				<< "D=D+A" << "\n"
				<< "@R13" << "\n"
				<< "M=D" << "\n"
				<< SP_DEC
				<< LOAD_SP_TO_D
				<< "@R13" << "\n"
				<< "A=M" << "\n"
				<< "M=D" << "\n";
			int baseAddr = ram.getAddress("THAT");
			int value = ram.popStack();
			ram.setMemory(baseAddr + index, value);
		}

		// pointer.
		if (command == "C_PUSH" && segment == "pointer") {
			if (index == 0) {
				output << "@THIS" << "\n"
					<< "D=M" << "\n"
					<< WRITE_D_TO_SP
					<< SP_INC;
				ram.pushStack(ram.getAddress("THIS"));
			}
			else if (index == 1) {
				output << "@THAT" << "\n"
					<< "D=M" << "\n"
					<< WRITE_D_TO_SP
					<< SP_INC;
				ram.pushStack(ram.getAddress("THAT"));
			}
			else {
				throw runtime_error("Not valid pointer must be either 0 or 1 current -> index: " + to_string(index));
			}
		}
		else if (command == "C_POP" && segment == "pointer") {
			if (index == 0) {
				output << SP_DEC
					<< LOAD_SP_TO_D
					<< "@THIS" << "\n"
					<< "M=D" << "\n";
				int value = ram.popStack();
				ram.setAddress("THIS", value);
			}
			else if (index == 1) {
				output << SP_DEC
					<< LOAD_SP_TO_D
					<< "@THAT" << "\n"
					<< "M=D" << "\n";
				int value = ram.popStack();
				ram.setAddress("THAT", value);
			}
			else {
				throw runtime_error("Not valid pointer must be either 0 or 1 current -> index: " + to_string(index));
			}
		}

		// temp(Temp).
		if (command == "C_PUSH" && segment == "temp") {
			int tempAddress = 5 + index;
			output << "@" << tempAddress << "\n"
				<< "D=M" << "\n"
				<< WRITE_D_TO_SP
				<< SP_INC;
			int value = ram.getMemory(tempAddress);
			ram.pushStack(value);
		}
		else if (command == "C_POP" && segment == "temp") {
			int tempAddress = 5 + index;
			output << SP_DEC
				<< LOAD_SP_TO_D
				<< "@" << tempAddress << "\n"
				<< "M=D" << "\n";
			int value = ram.popStack();
			ram.setMemory(tempAddress, value);
		}
	}

	void writeArithmetic(const string& command) {
		static int labelIndex = 0;

		if (command == "add") {
			output << SP_DEC  
				<< "D=M\n"    
				<< "A=A-1\n"         
				<< "M=D+M\n"; 
			int firstVal = ram.popStack();
			int secondVal = ram.popStack();
			ram.pushStack(secondVal + firstVal);
		}
		else if (command == "sub") {
			output << SP_DEC             
				<< "D=M\n"          
				<< "A=A-1\n"         
				<< "M=M-D\n"; 
			int firstVal = ram.popStack();
			int secondVal = ram.popStack();
			ram.pushStack(secondVal - firstVal);
		}
		else if (command == "neg") {
			output << "@SP\n"
				<< "A=M-1\n"          
				<< "M=-M\n";
			int value = ram.popStack();
			ram.pushStack(-value);
		}
		else if (command == "eq" || command == "gt" || command == "lt") {
			string jumpCommand;
			if (command == "eq") jumpCommand = "JEQ";
			if (command == "gt") jumpCommand = "JGT";
			if (command == "lt") jumpCommand = "JLT";

			string trueLabel = "TRUE_" + to_string(labelIndex);
			string endLabel = "END_" + to_string(labelIndex);
			labelIndex++;

			output << SP_DEC          
				<< "D=M\n"           
				<< "A=A-1\n"          
				<< "D=M-D\n"          
				<< "@" << trueLabel << "\n"
				<< "D;" << jumpCommand << "\n"
				<< "@SP\n"
				<< "A=M-1\n"
				<< "M=0\n"           
				<< "@" << endLabel << "\n"
				<< "0;JMP\n"          
				<< "(" << trueLabel << ")\n"
				<< "@SP\n"
				<< "A=M-1\n"
				<< "M=-1\n"         
				<< "(" << endLabel << ")\n";

			int firstVal = ram.popStack();
			int secondVal = ram.popStack();
			bool result = false;
			if (command == "eq") result = (secondVal == firstVal);
			if (command == "gt") result = (secondVal > firstVal);
			if (command == "lt") result = (secondVal < firstVal);
			ram.pushStack(result ? -1 : 0);
		}
		else if (command == "and") {
			output << SP_DEC            
				<< "D=M\n"          
				<< "A=A-1\n"         
				<< "M=D&M\n";   
			int firstVal = ram.popStack();
			int secondVal = ram.popStack();
			ram.pushStack(secondVal & firstVal ? -1 : 0);
		}
		else if (command == "or") {
			output << SP_DEC           
				<< "D=M\n"           
				<< "A=A-1\n"        
				<< "M=D|M\n";
			int firstVal = ram.popStack();
			int secondVal = ram.popStack();
			ram.pushStack(secondVal | firstVal ? -1 : 0);
		}
		else if (command == "not") {
			output << "@SP\n"
				<< "A=M-1\n"        
				<< "M=!M\n";
			int value = ram.popStack();
			ram.pushStack(~value);
		}
	}

};
