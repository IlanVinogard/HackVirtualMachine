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
	int labelCounter = 0;

public:
	CodeWriter(ofstream& output, RAM& ram) : output(output), ram(ram) {};

	void writePushPop(const string& command, const string& segment, const int& index) {
		if (command == "C_PUSH") {
			if (segment == "constant") {
				output << "@" << index << "\n"
					<< "D=A" << "\n"
					<< WRITE_D_TO_SP
					<< SP_INC;
				ram.pushStack(index);
			}
			else {
				string segmentBase;
				if (segment == "argument") segmentBase = "ARG";
				else if (segment == "local") segmentBase = "LCL";
				else if (segment == "this") segmentBase = "THIS";
				else if (segment == "that") segmentBase = "THAT";
				else if (segment == "temp") {
					output << "@" << (5 + index) << "\n";
					segmentBase = "";
				}
				else if (segment == "pointer") {
					segmentBase = (index == 0) ? "THIS" : "THAT";
				}
				else if (segment == "static") {
					output << "@Static" << index << "\n";
					segmentBase = "";
				}

				if (!segmentBase.empty()) {
					output << "@" << segmentBase << "\n"
						<< "D=M" << "\n"
						<< "@" << index << "\n"
						<< "A=D+A" << "\n"
						<< "D=M" << "\n"
						<< WRITE_D_TO_SP
						<< SP_INC;
				}
			}
		}
		else if (command == "C_POP") {
			string segmentBase;
			if (segment == "argument") segmentBase = "ARG";
			else if (segment == "local") segmentBase = "LCL";
			else if (segment == "this") segmentBase = "THIS";
			else if (segment == "that") segmentBase = "THAT";
			else if (segment == "temp") {
				output << "@" << (5 + index) << "\n";
				segmentBase = "";
			}
			else if (segment == "pointer") {
				segmentBase = (index == 0) ? "THIS" : "THAT";
			}
			else if (segment == "static") {
				output << "@Static" << index << "\n";
				segmentBase = "";
			}

			if (!segmentBase.empty()) {
				output << "@" << segmentBase << "\n"
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
			}
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

	void writeLabel(const string& label) {
		output << "(" << label << ")" << endl;
	}

	void writeGoto(const string& label) {
		output << "@" << label << endl;
		output << "0;JMP" << endl;
	}

	void writeIf(const string& label) {
		output << SP_DEC;
		output << "D=M" << endl;
		output << "@" << label << endl;
		output << "D;JNE" << endl;
	}

	void writeFunction(const string& functionName, int numLocals) {
		output << "(" << functionName << ")" << endl;
		for (int i = 0; i < numLocals; ++i) {
			writePushPop("C_PUSH", "constant", 0);
		}
	}

	void writeCall(const string& functionName, int numArgs) {
		string returnLabel = "RETURN_" + to_string(labelCounter++);

		// Push return address
		output << "@" << returnLabel << endl;
		output << "D=A" << endl;
		output << WRITE_D_TO_SP << SP_INC;
		ram.pushStack(ram.getAddress("SP"));
		cout << "Pushing return address: " << ram.getAddress("SP") << endl;

		// Push LCL
		pushRegisterToStack("LCL");
		// Push ARG
		pushRegisterToStack("ARG");
		// Push THIS
		pushRegisterToStack("THIS");
		// Push THAT
		pushRegisterToStack("THAT");

		// Reposition ARG
		output << "@SP" << endl;
		output << "D=M" << endl;
		output << "@5" << endl;
		output << "D=D-A" << endl;
		output << "@" << numArgs << endl;
		output << "D=D-A" << endl;
		output << "@ARG" << endl;
		output << "M=D" << endl;
		ram.setAddress("ARG", ram.getAddress("SP") - numArgs - 5);
		cout << "Setting ARG to: " << ram.getAddress("SP") - numArgs - 5 << endl;

		// Reposition LCL
		output << "@SP" << endl;
		output << "D=M" << endl;
		output << "@LCL" << endl;
		output << "M=D" << endl;
		ram.setAddress("LCL", ram.getAddress("SP"));
		cout << "Setting LCL to: " << ram.getAddress("SP") << endl;

		// Jump to function
		output << "@" << functionName << endl;
		output << "0;JMP" << endl;

		// Declare return label
		output << "(" << returnLabel << ")" << endl;
		ram.setAddress(returnLabel, ram.getAddress("SP"));
		cout << "Setting return label address: " << ram.getAddress("SP") << endl;
	}

	void pushRegisterToStack(const string& reg) {
		output << "@" << reg << endl;
		output << "D=M" << endl;
		output << WRITE_D_TO_SP << SP_INC;
		ram.pushStack(ram.getAddress(reg));
		cout << "Pushing register " << reg << " value: " << ram.getAddress(reg) << " onto stack" << endl;
	}

	void writeReturn() {
		// FRAME = LCL
		output << "@LCL" << endl;
		output << "D=M" << endl;
		output << "@R13" << endl;
		output << "M=D" << endl;
		int frame = ram.getAddress("LCL");
		ram.setMemory(13, frame);
		cout << "Setting FRAME (LCL) to: " << frame << endl;

		// RET = *(FRAME - 5)
		output << "@5" << endl;
		output << "A=D-A" << endl;
		output << "D=M" << endl;
		output << "@R14" << endl;
		output << "M=D" << endl;
		try {
			int retAddr = ram.getMemory(frame - 5);
			ram.setMemory(14, retAddr);
			cout << "Setting RET address (FRAME - 5) to: " << retAddr << endl;
		}
		catch (const runtime_error& e) {
			cerr << "Error: " << e.what() << " at RET address (FRAME - 5)" << endl;
		}

		// *ARG = pop()
		if (ram.getStackSize() > 0) {
			output << SP_DEC;
			output << "D=M" << endl;
			output << "@ARG" << endl;
			output << "A=M" << endl;
			output << "M=D" << endl;
			int popValue = ram.popStack();
			ram.setMemory(ram.getAddress("ARG"), popValue);
			cout << "Setting *ARG to popped value: " << popValue << endl;
		}
		else {
			cerr << "Error: Stack underflow during return" << endl;
		}

		// SP = ARG + 1
		output << "@ARG" << endl;
		output << "D=M+1" << endl;
		output << "@SP" << endl;
		output << "M=D" << endl;
		ram.setAddress("SP", ram.getAddress("ARG") + 1);
		cout << "Setting SP to: " << ram.getAddress("SP") << endl;

		// THAT = *(FRAME - 1)
		output << "@R13" << endl;
		output << "AM=M-1" << endl;
		output << "D=M" << endl;
		output << "@THAT" << endl;
		output << "M=D" << endl;
		try {
			int thatValue = ram.getMemory(frame - 1);
			ram.setAddress("THAT", thatValue);
			cout << "Setting THAT to: " << thatValue << endl;
		}
		catch (const runtime_error& e) {
			cerr << "Error: " << e.what() << " at THAT address (FRAME - 1)" << endl;
		}

		// THIS = *(FRAME - 2)
		output << "@R13" << endl;
		output << "AM=M-1" << endl;
		output << "D=M" << endl;
		output << "@THIS" << endl;
		output << "M=D" << endl;
		try {
			int thisValue = ram.getMemory(frame - 2);
			ram.setAddress("THIS", thisValue);
			cout << "Setting THIS to: " << thisValue << endl;
		}
		catch (const runtime_error& e) {
			cerr << "Error: " << e.what() << " at THIS address (FRAME - 2)" << endl;
		}

		// ARG = *(FRAME - 3)
		output << "@R13" << endl;
		output << "AM=M-1" << endl;
		output << "D=M" << endl;
		output << "@ARG" << endl;
		output << "M=D" << endl;
		try {
			int argValue = ram.getMemory(frame - 3);
			ram.setAddress("ARG", argValue);
			cout << "Setting ARG to: " << argValue << endl;
		}
		catch (const runtime_error& e) {
			cerr << "Error: " << e.what() << " at ARG address (FRAME - 3)" << endl;
		}

		// LCL = *(FRAME - 4)
		output << "@R13" << endl;
		output << "AM=M-1" << endl;
		output << "D=M" << endl;
		output << "@LCL" << endl;
		output << "M=D" << endl;
		try {
			int lclValue = ram.getMemory(frame - 4);
			ram.setAddress("LCL", lclValue);
			cout << "Setting LCL to: " << lclValue << endl;
		}
		catch (const runtime_error& e) {
			cerr << "Error: " << e.what() << " at LCL address (FRAME - 4)" << endl;
		}

		// goto RET
		output << "@R14" << endl;
		output << "A=M" << endl;
		output << "0;JMP" << endl;
	}
};
