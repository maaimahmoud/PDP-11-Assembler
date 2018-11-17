#include <iostream>
#include <map>
#include <iterator>
#include <bitset>
#include <string>
#include <vector>

using namespace std;


int main()
{
	map<string, pair<int, int> > instructions;

	instructions.insert(pair <string, pair<int, int>>("MOV", pair<int, int>(0, 0)));
	instructions.insert(pair <string, pair<int, int>>("ADD", pair<int, int>(1, 0)));
	instructions.insert(pair <string, pair<int, int>>("ADC", pair<int, int>(2, 0)));
	instructions.insert(pair <string, pair<int, int>>("SUB", pair<int, int>(3, 0)));
	instructions.insert(pair <string, pair<int, int>>("SUBC", pair<int, int>(4, 0)));
	instructions.insert(pair <string, pair<int, int>>("SBC", pair<int, int>(5, 0)));
	instructions.insert(pair <string, pair<int, int>>("AND", pair<int, int>(6, 0)));
	instructions.insert(pair <string, pair<int, int>>("OR", pair<int, int>(7, 0)));
	instructions.insert(pair <string, pair<int, int>>("XNOR", pair<int, int>(8, 0)));

	instructions.insert(pair <string, pair<int, int>>("INC", pair<int, int>(9, 0)));
	instructions.insert(pair <string, pair<int, int>>("DEC", pair<int, int>(9, 1)));
	instructions.insert(pair <string, pair<int, int>>("CLR", pair<int, int>(9, 2)));
	instructions.insert(pair <string, pair<int, int>>("INV", pair<int, int>(9, 3)));
	instructions.insert(pair <string, pair<int, int>>("LSR", pair<int, int>(9, 4)));
	instructions.insert(pair <string, pair<int, int>>("ROR", pair<int, int>(9, 5)));
	instructions.insert(pair <string, pair<int, int>>("RRC", pair<int, int>(9, 6)));
	instructions.insert(pair <string, pair<int, int>>("ASR", pair<int, int>(9, 7)));
	instructions.insert(pair <string, pair<int, int>>("LSL", pair<int, int>(9, 8)));
	instructions.insert(pair <string, pair<int, int>>("ROL", pair<int, int>(9, 9)));
	instructions.insert(pair <string, pair<int, int>>("RLC", pair<int, int>(9, 10)));


	instructions.insert(pair <string, pair<int, int>>("BR", pair<int, int>(10, 0)));
	instructions.insert(pair <string, pair<int, int>>("BEQ", pair<int, int>(10, 1)));
	instructions.insert(pair <string, pair<int, int>>("BNE", pair<int, int>(10, 2)));
	instructions.insert(pair <string, pair<int, int>>("BLO", pair<int, int>(10, 3)));
	instructions.insert(pair <string, pair<int, int>>("BLS", pair<int, int>(10, 4)));
	instructions.insert(pair <string, pair<int, int>>("BHI", pair<int, int>(10, 5)));
	instructions.insert(pair <string, pair<int, int>>("BHS", pair<int, int>(10, 6)));

	instructions.insert(pair <string, pair<int, int>>("HLT", pair<int, int>(11, 0)));
	instructions.insert(pair <string, pair<int, int>>("NOP", pair<int, int>(11, 1)));

	string input;

	getline(cin, input);

	while (input != "q")
	{

		string opCode = "", firstOperand = "", SecondOperand = "";




		int space = input.find(' ');
		int comma = input.find(',');

		opCode = input.substr(0, space);

		if (space != -1)
			firstOperand = input.substr(space + 1, comma - space - 1);
		if (comma != -1)
			SecondOperand = input.substr(comma + 1, input.size() - comma);

		//Capitalize all letters
		for (auto & c : opCode) c = toupper(c);
		for (auto & c : firstOperand) c = toupper(c);
		for (auto & c : SecondOperand) c = toupper(c);


		map<string, pair<int, int>>::iterator instructionIt = instructions.find(opCode);

		if (instructionIt == instructions.end())
			cout << "Instruction is not included in instruction set" << endl;
		else
		{
			if (instructionIt->second.first <= 8)
			{
				//2 Operand Instructions
				bitset<4> x(instructionIt->second.first);
				cout << x << endl;
			}
			else
			{
				//Other instructions
				bitset<4> x(instructionIt->second.first);
				bitset<4> y(instructionIt->second.second);
				cout << x << " " << y << endl;

			}
		}
		getline(cin, input);
	}
}