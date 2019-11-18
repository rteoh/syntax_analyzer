/*
Jimmy Phong
Ryan Teoh
Assignment 2
*/

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

bool isKeyword(vector<char>buffer);
bool is_digits(string str);
string lexer(vector<char>chVector, vector<char>buffer); //this checks EACH lexeme 
void production_rules(string type, char);

int main()
{
	// Open Input file
	ifstream inputFile;
	inputFile.open("test1.txt");

	// Clear output.txt
    ofstream outFile;
    outFile.open("output.txt");
    outFile.close();

	//file error check
	if (!inputFile.is_open())
	{
		cout << "error in file opening" << endl;
		exit(0);
	}

	//vector initialization
	vector<char> chVector;	//hold the characters
	vector<char> buffer;	//pushes characters from chVector 

	while (!inputFile.eof())
	{
		string line;
		string token;

		getline(inputFile, line);
		cout << "...Testing getLine..." << endl;
		cout << line << endl;//here i am inputing a whole string into the function
		
		//converts string and puts each char into a vector
		cout << "...Testing Correct Vector Input..." << endl;
		copy(line.begin(), line.end(), back_inserter(chVector));

		// Open file
		outFile.open("output.txt", ios::app);

		for (int a = 0; a < chVector.size(); a++)
		{
			cout << chVector[a] << " ";

			// Get Token of the character
			token = lexer(chVector,buffer);

			// Output Token into file
			if(token == "operator") {
				outFile << endl << "Token: Operator       Lexeme:   " << chVector[a] << endl;
			} else if(token == "seperator") {
				outFile << endl << "Token: Seperator      Lexeme:   " << chVector[a] << endl;
			} else if(token == "identifier") {
				outFile << endl << "Token: Identifier     Lexeme:   " << chVector[a] << endl;
			} else if(token == "keyword") {
				outFile << endl << "Token: Keyword        Lexeme:   " << chVector[a] << endl;
			} else if(token == "number") {
				outFile << endl << "Token: Number         Lexeme:   " << chVector[a] << endl;
			}

			// Production Rule Function (Add production rules)
			production_rules(token, chVector[a]);

		}

		// Close write file
		outFile.close();

		cout << endl;

		//passing the vector into the lexer function for tokenization
			//call the production rule function with lexer
		chVector.clear();		//clears for next line
	}

	// Close read file
	inputFile.close();
	cout << "...Testing Complete..." << endl;
	cout << "...Terminating Program..." << endl;
	return 0;
}

//////////////////////Function Definitions////////////////////////////////
bool isKeyword(vector<char> buffer)
{
	//convert buffer to string
	string str(buffer.begin(), buffer.end());
	cout <<"Buffer in String Form: " <<str << endl;
	
	string keywords[45] = { "and","auto","bool","break","case","char","const","continue","default",
							"do","double","else","endif","enum","extern","false","float","for","forend","function","goto",
							"if","input","int","long","output","or","register","return","short","signed",
							"sizeof","static","string","struct","switch","then","true","typedef","union",
							"unsigned","void","volatile","while","whileend" };
	
	//check if the word in the buffer is a keyword
	bool indicator = false; // this is the initial response 
	for (int c = 0; c < 45; c++)
	{
		if (str == keywords[c])
		{
			indicator = true;//set true if found 
			cout << str << " is a keyword" << endl;
		}
		
	}
	return indicator;
}
bool is_digits(string str)
{
	//checks if the whole string contain numbers
	return all_of(str.begin(), str.end(), ::isdigit);
}

string lexer(vector<char>chVector,vector<char>buffer)
{
	//check for operators first 
	char operators[] = "+-*/%=<>";


	string wordInBuffer;
	//This goes through chVector and adds into buffer if needed
	for (int b = 0; b < chVector.size(); b++)
	{

		// Check to see if line starts with a valid character
		if(b == 0) {

			if (chVector[b] == '!') {
				//if( !isalnum(chArray[0]) || (chArray[0] != '{') || (chArray[0] != '}') || (chArray[0] != '#') || (chArray[0] != '/')  || (chArray[0] != '*')) {

					// If line does not with alpha-numeric or with certain characters, the line will be ignored.
				return "comment";
			}

		}


		//checks operator
		if (chVector[b] == '+' || chVector[b] == '-' || chVector[b] == '*' || chVector[b] == '/' ||
			chVector[b] == '%' || chVector[b] == '=' || chVector[b] == '<' || chVector[b] == '>')
		{
			return "operator";

		}//checks seperator
		else if (chVector[b] == '[' || chVector[b] == ']' || chVector[b] == '(' || chVector[b] == ')' ||
			chVector[b] == '{' || chVector[b] == '}' || chVector[b] == '[' || chVector[b] == '.' ||
			chVector[b] == ',' || chVector[b] == ';' || chVector[b] == ':' || chVector[b] == '!')
		{
			// If previous keyword/identifiers did not get called before operator
			if (!buffer.empty())
			{
				string wordInBuffer(buffer.begin(), buffer.end());
				if (isKeyword(buffer) == true)
					return "keyword";
				else
				{
					if (is_digits(wordInBuffer) == true)
					{
						return "number";
					}
					else
						return "identifier";
				}
				// Resets Buffer
				buffer.clear();
			}
			return "seperator";
		}
		else if (isalnum(chVector[b])|| chVector[b] == '$')
		{
			buffer.push_back(chVector[b]);
	
		}
		else if ((chVector[b] == ' ' || chVector[b] == '\n') && (!buffer.empty())) {

			string wordInBuffer(buffer.begin(), buffer.end());
			
			if (isKeyword(buffer) == true)
			{
				return "keyword";
			}
			else
			{
				if (is_digits(wordInBuffer) == true)
				{
					return "number";
				}
				else
					return "identifier";
			}
			// Resets Buffer
			buffer.clear();

		} else {
			return "identifier";
		}

	}

	return "error";

}

void production_rules(string type, char chVector) {

	ofstream outFile;
	outFile.open("output.txt", ios::app);

	// For operators
	if(type == "operator") {

		if(chVector == '+' || chVector == '-' || chVector == '*' || chVector == '/' || chVector == ';') {

			outFile << "	<Empty> -> Epsilon" << endl;
			outFile << "	<TermPrime> -> * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;

			if(chVector == '+' || chVector == '-' || chVector == ';') {
				outFile << "	<Empty> -> Epsilon" << endl;
				outFile << "	<ExpressionPrime> -> + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>   " << endl;

				if(chVector == ';') {
					outFile << "	<Empty> -> Epsilon" << endl;
				}

			}

		}

	// For identifiers
	} else if(type == "identifier") {

		if(isalnum(chVector)) {

			/*if(var_count == 0) {
				outFile << "<Term> -> <Factor> <TermPrime>" << endl;
			}*/

			outFile << "	<Factor> -> - <Primary> | <Primary>" << endl;
			outFile << "	<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false" << endl;

		} else {

			outFile << "<Expression> -> <Term> <ExpressionPrime>" << endl;
			outFile << "	<Term> -> <Factor> <TermPrime>" << endl;
			outFile << "	<Factor> -> - <Primary> | <Primary>" << endl;
			outFile << "	<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false" << endl;

		}

	} else if(type == "error") {

		// If lexer function could not find the token
		//outFile << "ERROR: Tokens could not be found." << endl;

	} else if(type == "") {

		// IGNORE

	} else {

		// If the type is not defined in the production rules functions
		//outFile << "ERROR: Production Rules has not been added for this token yet." << endl;
	}

	outFile.close();

}



















