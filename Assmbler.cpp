#include <iostream>
#include <map>
#include <iterator>
#include <bitset>
#include <string>
#include <vector>
#include<math.h>
#include <fstream>




//limitations:
//1- if you add another operant after , to 1 op line, it will ignore it
//2- must end code file with end

//assumptions:
//1- memory is word addressable


using namespace std;

map<string, pair<int, int> > instructions;
map<string, int > registers;
map<string, int > addModes;
map<string, int > labels;
map<string, int> variables;

int PC=0; 
int l=0;
bool er=false;
int curr_pass=0;

ofstream out;
ifstream in;




bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int index_first_letter(string s){
	for(int i=0; i<s.length(); i++){
		if(s[i]!=' '){
			return i;
		}

	}
}

void eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
	size_t pos = std::string::npos;
 
	// Search for the substring in string in a loop untill nothing is found
	while ((pos  = mainStr.find(toErase) )!= std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}


bool printOperand(string& op,int &luggage,int opnum){

	map<string, int>::iterator It;
	 
	bool ind=false;
	if(op[0]=='@')
		ind=true;
			
	int r=op.find("R");

	if(r!=-1){
		string reg=op.substr(r,2);
		It=registers.find(reg);
		if(It!=registers.end()){
			eraseAllSubStr(op,reg);
			It=addModes.find(op);
			
			if (It == addModes.end()){
				if(ind){
					op.erase(0,1);
				}
				
				if( op.find("()")!=-1){// most probably indexed
					eraseAllSubStr(op,"()");
					if(is_number(op)){//definately indexed
						luggage=stoi(op);
						if(ind){
						bitset<3> x(addModes.find("@X()")->second);
						out << x ;}
						else{
						bitset<3> x(addModes.find("X()")->second);
						out << x;
						}
						bitset<3> y(registers.find(reg)->second);
						out<<y;
						PC++;
						return true;
					}
				}
					//fe moshkila el syntax
					return false;

			}
			else{
				bitset<3> x(It->second);
				out << x;
				bitset<3> y(registers.find(reg)->second);
				out<<y;
				return true;
				
			}

		}
		

	}
	PC++;

	if(curr_pass==1)
	{
		return true;
	}

	//search for variable
	It=variables.find(op);
	if(It!=variables.end()){
		luggage=It->second-(PC);
			if(ind){
			bitset<3> x(addModes.find("@X()")->second);
			out << x;
			}
			else{
			bitset<3> x(addModes.find("X()")->second);
			out << x;
			}
			bitset<3> y(registers.find("R7")->second);
			out<<y;
			return true;

	}

	if(opnum==2 && !ind){ //i can't save in a numbr basically
		return false;
	}
	//now there is a number involved, most probably
	if(ind){
		op.erase(0,1);
	}
	if(op[0]=='#'){
		op.erase(0,1);
		if(is_number(op)){
			if(ind){
				bitset<3> x(addModes.find("@()+")->second);
				out << x;
				}
			else{
				bitset<3> x(addModes.find("()+")->second);
				out << x;
				}

			bitset<3> y(registers.find("R7")->second);
			out<<y;

			luggage=stoi(op);

			return true;
			
		}

	}




	return false;


}

// void firstPass(){

// 	in.open("code.txt",ios::in);
// 	string input,label="";

// 	getline(in, input);

// 	for (auto & c : input) c = toupper(c);

// 	while (input.find("END")==-1)
// 	{
// 		cout<<input<<endl;
// 		int colon=input.find(':');

// 		if(colon!=-1){
// 			label=input.substr(0,colon);
// 			cout<<label<<endl;
// 			eraseAllSubStr(input,input.substr(0,colon+1));
// 			eraseAllSubStr(label," ");
// 			if(label!="")
// 				labels.insert(pair<string,int>(label,PC));
				
// 		}

// 		l++; PC++;
// 		getline(in, input);
// 		for (auto & c : input) c = toupper(c);

// 	}

// 	in.close();

// }
void removeComment(string &inp){

	int com=inp.find(';');
	if(com!=-1){
		// cout<<inp.substr(com,inp.length())<<endl;
		eraseAllSubStr(inp,inp.substr(com,inp.length()));
	}
}

bool getVal(string op){
	int space= op.find(" ");
	if(space==-1){
		space=op.find("	");//tab
	}
	if(space!=-1){
		op=op.substr(space,op.length());
		eraseAllSubStr(op," ");
		eraseAllSubStr(op,"	");//tab
		if(is_number(op)){
			bitset<16> y(stoi(op));
			
			out << y << endl;
			return true;


		}
	}

	return false;
}

void Pass(int num){
	in.open("code.txt",ios::in);
	if(num==2){
		out.open("binary.txt",ios::in);
	}
	cout<<"START PASS # "<<num<<endl;
	string input;

	getline(in, input);
	// cout<<is_number(input)<<endl;

	// eraseAllSubStr(input,"hi");

	// cout<<input;

	// while(1){}
	for (auto & c : input) c = toupper(c);
	//PC++;

	while (input.find("END")==-1)
	{
		l++;
		

		string opCode = "", firstOperand = "", SecondOperand = "",label="";

      //counts current line, assume i start at Zero and I inc 1 every line since memory is word addressable, max number of words in 2k, which is 2^11 words

		removeComment(input);
		int colon=input.find(':');

		if(colon!=-1){
			label=input.substr(0,colon);
			// cout<<label<<endl;
			eraseAllSubStr(input,input.substr(0,colon+1));
			eraseAllSubStr(label," ");
			eraseAllSubStr(label,"	");//tab
			if(label==""){
				cout<<": has no associated name, will be ignored"<<endl;
			}
			else
				{labels.insert(pair<string,int>(label,PC)); }
		}

		input.erase(0,index_first_letter(input));
		int space = input.find(' ');
		if(space==-1){
			space=input.find('	');//tab
		}
		int comma = input.find(',');

		
		if (comma != -1){
			SecondOperand = input.substr(comma + 1, input.size() - comma);
			

		}
		else{
			comma=input.length();
		}

		if (space != -1){
			firstOperand = input.substr(space + 1, comma - space - 1); //substr(pos=0,len)
			




		}
		else{
			space=input.length();
		}

	
		opCode = input.substr(0,space); 
		eraseAllSubStr(opCode," ");
		eraseAllSubStr(opCode,"	");//tab

		if(opCode=="") ///empty string
		{
			getline(in, input);
			for (auto & c : input) c = toupper(c);
			continue;
		}
		
		out<<PC<<": ";
		PC++;

		if(opCode=="DEFINE"){
			firstOperand.erase(0,index_first_letter(firstOperand));
			
			if(!getVal(firstOperand))
			{
				cout<<"Error at line "<<l<<" not a number man"<<endl;
				er=true;
			}
			else{
				int sp=firstOperand.find(" ");
				if(sp==-1){
					sp=firstOperand.find("	");//tab
				}
				string var=firstOperand.substr(0,sp);
				variables.insert(pair<string,int>(var,PC-1));

			}
			getline(in, input);
			for (auto & c : input) c = toupper(c);
			continue;
			
		}


		

		//Capitalize all letters
		// for (auto & c : opCode) c = toupper(c);
		// for (auto & c : firstOperand) c = toupper(c);
		// for (auto & c : SecondOperand) c = toupper(c);
		// cout<<opCode<<firstOperand<<SecondOperand<<endl;

		// getline(cin, input);
		// continue;


		map<string, pair<int, int>>::iterator instructionIt = instructions.find(opCode);
		eraseAllSubStr(firstOperand," ");
		eraseAllSubStr(firstOperand,"	");//tabs
		eraseAllSubStr(SecondOperand," ");
		eraseAllSubStr(SecondOperand,"	");//tabs

		// cout<<"OP"<<opCode<<"FIRST"<<firstOperand<<"SEC"<<SecondOperand<<"11"<<endl;
		// cout<<SecondOperand[2]<<int(SecondOperand[2])<<"space"<<int(' ')<<endl;

	
		if (instructionIt == instructions.end()){
			cout << "Instruction"<<opCode<<" is not included in instruction set at line " <<l<< endl;
			er=true;// break;
		}
			//check for labels and variables
		else
		{
			if (instructionIt->second.first <= 8)
			{
				//2 Operand Instructions
				bitset<4> x(instructionIt->second.first);
				out << x;
			}
			else
			{
				//Other instructions
				bitset<4> x(instructionIt->second.first);
				bitset<4> y(instructionIt->second.second);
				out << x  << y;

			}
			bool success; int lug1=2147483647, lug2=2147483647;

			if(instructionIt->second.first<9)// 2 op ins
			{
				success=printOperand(firstOperand,lug1,1);
				if(!success){
					cout<<"Error at line "<<l<<" first op"<<endl;
					er=true;// break;
				}

				success=printOperand(SecondOperand,lug2,2);
				if(!success){
					cout<<"Error at line "<<l<<" sec op"<<endl;
					er=true;// break;
				}

				

			}
			else{
				if(instructionIt->second.first==10){
					//breanch treatment
					map<string, int>::iterator It;
					It=labels.find(firstOperand);
					if(It!=labels.end()){
						if(abs(It->second-(PC))>pow(2,8)){
							cout<<"error at line "<<l<<"label too far"<<endl;
							er=true;
						}
						else{
							bitset<8> x(It->second-(PC));
							out<<x;

						}
					}	
					else{
						if(num==2){
						cout<<"error at line "<<l<<"label not found"<<endl;
						er=true;
						}
						

					}

				}
				else if(opCode=="JSR"){
					bitset<8> x(0);
					out<<x;
					map<string, int>::iterator It;
					It=labels.find(firstOperand);
					if(It!=labels.end()){
							out<<endl;
							bitset<16> x(It->second);
							out<<x;
							// PC++;
					}	
					else if(num==2){
						cout<<"error at line "<<l<<"label not found"<<endl;
						er=true;
						}
					PC++;
				}
				else// one operand inst
				if(instructionIt->second.first==9){
					success=printOperand(firstOperand,lug1,2);
					bitset<2> x(0);
					out<<x;
					if(!success){
						cout<<"Error at line "<<l<<" first op"<<endl;
						er=true;// break;
					}
				}
				else{
					bitset<8> x(0);
					out<<x;
				}
				


			}

				
			if(lug1!=2147483647){
				

				out<<endl;
				out<<PC-1<<": ";
				bitset<16> x(lug1);
				out<<x;
				// PC++;

			}
			if(lug2!=2147483647){


				out<<endl;
				out<<PC-1<<": ";

				bitset<16> y(lug2);
				out<<y;
				// PC++;

			}

			out<<endl;
	
			
		}
		
		getline(in, input);
		for (auto & c : input) c = toupper(c);

	}

	in.close();
	if(num==2){
		out.close();
	}
	if(er){
		cout<<"PASS "<< num<< " FAILED!"<<endl;
	}
	else
		cout<<"PASS "<< num<< " SUCCESSFUL!"<<endl;

}


int main(){

	//variable and label trials 

    
    //3 bits each
    registers.insert(pair<string,int>("R0",0));
    registers.insert(pair<string,int>("R1",1));
    registers.insert(pair<string,int>("R2",2));
    registers.insert(pair<string,int>("R3",3));
    registers.insert(pair<string,int>("R4",4));
    registers.insert(pair<string,int>("R5",5));
    registers.insert(pair<string,int>("R6",6));
    registers.insert(pair<string,int>("R7",7));

    //3 bits each 
    addModes.insert(pair<string,int>("",0));
    addModes.insert(pair<string,int>("()+",1));
    addModes.insert(pair<string,int>("-()",2));
    addModes.insert(pair<string,int>("X()",3));
    addModes.insert(pair<string,int>("@",4));
    addModes.insert(pair<string,int>("@()+",5));
    addModes.insert(pair<string,int>("@-()",6));
    addModes.insert(pair<string,int>("@X()",7));



    //4 bits each
	instructions.insert(pair <string, pair<int, int>>("MOV", pair<int, int>(0, 0)));
	instructions.insert(pair <string, pair<int, int>>("ADD", pair<int, int>(1, 0)));
	instructions.insert(pair <string, pair<int, int>>("ADC", pair<int, int>(2, 0)));
	instructions.insert(pair <string, pair<int, int>>("SUB", pair<int, int>(3, 0)));
	instructions.insert(pair <string, pair<int, int>>("SBC", pair<int, int>(4, 0)));
	instructions.insert(pair <string, pair<int, int>>("AND", pair<int, int>(5, 0)));
	instructions.insert(pair <string, pair<int, int>>("OR", pair<int, int>(6, 0)));
	instructions.insert(pair <string, pair<int, int>>("XNOR", pair<int, int>(7, 0)));
    instructions.insert(pair <string, pair<int, int>>("CMP", pair<int, int>(8, 0)));

    //8bits each
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
	instructions.insert(pair <string, pair<int, int>>("JSR", pair<int, int>(11, 2)));
	instructions.insert(pair <string, pair<int, int>>("RTS", pair<int, int>(11, 3)));
	instructions.insert(pair <string, pair<int, int>>("IRET", pair<int, int>(11, 4)));
	
     //hiii i am a comment that seems to be cusing trouble

	 curr_pass++;
	Pass(curr_pass);
	PC=0;
	l=0;
	curr_pass++;
	if(!er){
	Pass(curr_pass);
	}
	if(er){
		out.open("binary.txt", std::ofstream::out | std::ofstream::trunc);
		out.close();
	}


    return 0;

   
}
