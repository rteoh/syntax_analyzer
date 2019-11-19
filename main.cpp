/*
Jimmy Phong
Ryan Teoh
Assignment 2

CPSC 323

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
string lexer(char chVector, vector<char>buffer); //this checks EACH lexeme 
void production_rules(string type, char lexeme);
bool E(string token, char lexeme);
bool Q(string token, char lexeme);
bool T(string token, char lexeme);
bool R(string token, char lexeme);
bool F(string token, char lexeme);
int identifier_num;
int identifier_num_line;

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

	// Get count of identifier tokens
	int identifier_num = 0;

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

		bool comment = false;

		for (int a = 0; a < chVector.size(); a++)
		{

			// Run only if it is not a comment in the line
			if(comment == false) {

				cout << chVector[a] << " ";

				// Get Token of the character
				token = lexer(chVector[a],buffer);
				cout << token << endl;

				// Check if comment
				if(token == "comment") {
					comment = true;
					break;
				}

				// Open file
				outFile.open("output.txt", ios::app);


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


				// Close write file
				outFile.close();

				// 
				switch(chVector[a]) {

					case 'E':
						E(token, chVector[a]);
					case 'Q':
						Q(token, chVector[a]);
					case 'T':
						T(token, chVector[a]);
					case 'R':
						R(token, chVector[a]);
					case 'F':
						F(token, chVector[a]);
				}

				// Production Rule Function (Add production rules)
				production_rules(token, chVector[a]);

			}

		}

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

string lexer(char chVector,vector<char>buffer)
{
	//check for operators first 
	char operators[] = "+-*/%=<>";


	string wordInBuffer;
	//This goes through chVector and adds into buffer if needed



	//checks operator
	if (chVector == '+' || chVector == '-' || chVector == '*' || chVector == '/' ||
		chVector == '%' || chVector == '=' || chVector == '<' || chVector == '>' || chVector == ';')
	{
		return "operator";

	}//checks seperator
	else if (chVector == '[' || chVector == ']' || chVector == '(' || chVector == ')' ||
		chVector == '{' || chVector == '}' || chVector == '[' || chVector == '.' ||
		chVector == ',' || chVector == ':')
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
	else if (isalnum(chVector)|| chVector == '$')
	{
		buffer.push_back(chVector);

		if (isalpha(chVector)) {
			return "identifier";
		} else if (isdigit(chVector)) {
			return "number";
		}

	}
	else if ((chVector == ' ' || chVector == '\n') && (!buffer.empty())) {

		string wordInBuffer(buffer.begin(), buffer.end());
		
		if (isKeyword(buffer) == true)
		{
			return "keyword";
		} else {

			if (isalpha(chVector)) {
				return "identifier";
			} else if (isdigit(chVector)) {
				return "number";
			} else {
				return "identifier";
			}

		}
		buffer.clear();

	} else if (chVector == '!') {
		return "comment";
	} else if (isalpha(chVector)) {
		return "identifier";
	} else if (isdigit(chVector)) {
		return "number";
	} else if (chVector == ' ' || chVector == '\n') {
		return "whitepace";
	} else {
		return "identifier";
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

					// Clear identifier_num_line if line ends with ';'
					identifier_num_line = 0;
				}

			}

		}

	// For identifiers
	} else if(type == "identifier") {

		if(isalnum(chVector)) {

			// First Identifier in code (Must be a statement list)
			if(identifier_num == 0) {
				outFile << "	<Statement List> -> <Statement> | <Statement> <Statement List>" << endl;
			}

			// Non-terminal first (Must add T (Term Prime) for defined identifier)
			if(identifier_num_line == 0 && identifier_num != 0) {
				outFile << "	<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While> " << endl;
				outFile << "	<Assign> -> <Identifier> = <Expression>;" << endl;
				
			} else {

				// Add Expression if it is the first identifier when defining another identifier
				if(identifier_num_line == 1) {
					outFile << "	<Expression> -> <Term> <ExpressionPrime>" << endl;
				}

				outFile << "	<Term> -> <Factor> <TermPrime>" << endl;
				outFile << "	<Factor> -> - <Primary> | <Primary>" << endl;
				outFile << "	<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false" << endl;
			
			}

		} else {

			outFile << "<Expression> -> <Term> <ExpressionPrime>" << endl;
			outFile << "	<Term> -> <Factor> <TermPrime>" << endl;
			outFile << "	<Factor> -> - <Primary> | <Primary>" << endl;
			outFile << "	<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false" << endl;

		}

		// Identifier counter
		identifier_num++;
		identifier_num_line++;

	} else if(type == "error") {

		// If lexer function could not find the token
		outFile << "ERROR: Tokens could not be found." << endl;

	} else if(type == "") {

		// IGNORE

	} else {

		// If the type is not defined in the production rules functions
		//outFile << "ERROR: Production Rules has not been added for this token yet." << endl;
	}


}


// Left recursion, CHECK TO SEE IF SYNTAX IS VALID


//expression
//<Expression>  -> <Term> <Expression Prime>
bool E(string token, char lexeme)
{
	bool condition = false;//assume the worst

	ofstream outFile;
	outFile.open("output.txt", ios::app);

	if (T(token, lexeme))
	{
		if (Q(token, lexeme))
		{

			ofstream outFile;
			outFile.open("output.txt", ios::app);

			outFile << "\t<Expression> -> <Term> <Expression Prime>\n";
			condition = true;//everything works

			outFile.close();
		}

	}
	


	/*
	T(token, lexeme, outFile);//term
	Q(token, lexeme, outFile);//expression prime
	*/


	return condition;
}

//expression prime
/*<Expression Prime>  -> + <Term> <Expression Prime>  |
	- <Term> <Expression Prime> | <Empty>*/
bool Q(string token, char lexeme)
{
	bool condition = false;

	if (lexeme == '+')
	{ 
		if (T(token, lexeme))
		{
			if (Q(token, lexeme))
			{ 
				ofstream outFile;
				outFile.open("output.txt", ios::app);

				outFile << "<Expression Prime> -> + <Term> <Expression Prime>";
				condition = true;

				outFile.close();
			}
		}
	}else if (lexeme == '-')
	{
		if (T(token, lexeme))
		{
			if (Q(token, lexeme))
			{
				ofstream outFile;
				outFile.open("output.txt", ios::app);

				outFile << "\t<Expression Prime> -> - <Term> <Expression Prime>\n";
				condition = true;

				outFile.close();
			}
		}
	}

	return condition;
	/*
	if (lexeme == '+')
	{
		outFile << "\t<Expression Prime> -> + <Term> <Expression Prime>\n";
		T(token, lexeme, outFile);
		Q(token, lexeme, outFile);
	}
	else if (lexeme == '-')
	{
		outFile << "\t<Expression Prime> -> - <Term> <Expression Prime>\n";
		T(token, lexeme, outFile);
		Q(token, lexeme, outFile);
	}
	*/

}

//term
//<Term>    ->  <Factor> <Term Prime>
bool T(string token, char lexeme)
{
	bool condition = false;

	ofstream outFile;
	outFile.open("output.txt", ios::app);

	if (F(token, lexeme))
	{
		if (R(token, lexeme))
		{

			ofstream outFile;
			outFile.open("output.txt", ios::app);

			outFile << "\t<Term> -> <Factor> <Term Prime>\n";
			condition = true;

			outFile.close();
		}
	}


	return condition;
}

//term prime
/*<Term Prime> ->  * <Factor> <Term Prime> |  / <Factor> <Term Prime> |
	<Empty>*/
bool R(string token, char lexeme)
{
	bool condition = false;

	ofstream outFile;
	outFile.open("output.txt", ios::app);

	if (lexeme == '*')
	{
		if (F(token, lexeme))
		{
			if (R(token, lexeme))
			{

				ofstream outFile;
				outFile.open("output.txt", ios::app);

				outFile << "\t<Term Prime> -> * <Factor> <Term Prime>\n";
				condition = true;

				outFile.close();
			}
		}
	}


	return condition;
}

//factor
bool F(string token, char lexeme)
{
	bool condition = false;

	ofstream outFile;
	outFile.open("output.txt", ios::app);

	if (token == "identifier")
	{

		ofstream outFile;
		outFile.open("output.txt", ios::app);

		outFile << "<Factor> -> <id>" << endl;
		condition = true;

		outFile.close();
	}
	else if (lexeme == '(')
	{
		if (E(token, lexeme))
		{
			if (lexeme == ')')
			{

				ofstream outFile;
				outFile.open("output.txt", ios::app);

				outFile << "<Factor> -> <(Expression)>" << endl;
				condition = true;

				outFile.close();
			}
		}
	}


	return condition;
}

