/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

The lexer scans the input file and returns the tokens - the basic elements of the scene description

*/
#ifndef HELLLEXER
#define HELLLEXER

#include <string>
#include <stack>
#include <cstdio>

class Lexer
{
public:
	//avalible tokens
	enum TokenType {LBRACKET,RBRACKET,WORD,SEMICOLON,END};
private:
	int line; //current line
	FILE* file;
	
	//sometimes we have a situation, when one token occurs just after another (without a whitespace between them)
	//so we need to return first token and remember the next - here is the variable responsible for this
	TokenType next;

	void EatComments(); //as the name says
public:
	struct Token
	{
		TokenType type;
		std::string value;
		int line; //number of line where the token occured

		Token() {}
		Token(TokenType t,std::string v,int l) : type(t),value(v),line(l) {}
	};

	Lexer(std::string fileName);
	~Lexer();

	Token GetToken(); //get next token
};

#endif