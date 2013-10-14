/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

Implementation of lexer
*/

#include "lexer.h"
#include "log.h"
#include "utilities.h"
#include <stdio.h>

using namespace std;

Lexer::Lexer(std::string fileName)
{
	//open the file
	file=fopen(fileName.c_str(),"r");
	if(file==NULL)
		Log::AddMessage("Cannot open "+fileName,Log::ERR);

	//set some variables
	line=1;
	next=END;
}

Lexer::~Lexer()
{
	fclose(file);
}

void Lexer::EatComments()
{
	char last=' ';
	char c=fgetc(file);
	if(c!='/'&&c!='*') //slashes cannot occur alone
		Log::AddMessage("Wrong comment",Log::ERR);
	if(c=='/')
	{	//eating up all characters in the line
		for(char tmp=fgetc(file);tmp!='\n'&&tmp!=EOF;tmp=fgetc(file));
		++line;
	}
	if(c=='*')
	{	//more complex comments, can be nested also
		int level=1;
		for(char tmp=fgetc(file);tmp!=EOF&&level>0;tmp=fgetc(file))
		{
			if(tmp=='\n')
				++line;
			if(last=='/'&&tmp=='*')
				++level;
			if(last=='*'&&tmp=='/')
				--level;
			last=tmp;
		}
	}
}

Lexer::Token Lexer::GetToken()
{
	int currentLine=-1;
	if(next!=END) //any tokens pending?
	{
		Token temp(next,"",line);
		next=END;
		return temp;
	}

	string result;
	for(;;)
	{
		char x=fgetc(file);
		if(x==EOF)
			break;
		if((x>='a'&&x<='z')||(x>='0'&&x<='9')||(x>='A'&&x<='Z')||x=='.'||x=='-'||x==',')
		{ //yeah, we've got an element of the word!
			if(currentLine<0) //remember the line with the first character
				currentLine=line;
			result+=x;
			continue;
		}
		switch(x)
		{
			case '/':
			{
				EatComments();
				continue;
			}
			case ';':
			{
				next=SEMICOLON;
				break;
			}
			case '{':
			{
				next=LBRACKET;
				break;
			}
			case '}':
			{
				next=RBRACKET;
				break;
			}
			case '\n':
				++line;
			case '\t':
			case ' ':
			{
				if(result!="") //we do have a token 
					break;
				continue;
			}
			default:
				Log::AddMessage("Unrecognised character on line: "+ToString(line)+". Skipping...",Log::NORMAL);
		}
		break;
	}
	if(result!="")
		return Token(WORD,result,currentLine);
	TokenType temp=next;
	next=END;
	return Token(temp,"",line);
}

