#include "stdafx.h"
#include "Generation.h"


ofstream outfile("GenJava.txt");
bool isFunction = false;
bool isMain = false;
int idFunc;
int buf;


void Generation(LT::LexTable &Lextable, In::StToken *tokens, IT::IdTable &idtable)
{
	outfile << Head;
	for (int i = 0; i < Lextable.size; i++)
	{
		switch (Lextable.table[i].lexema)
		{
		case LEX_NUM:
		{
			if (strcmp(tokens[i + 1].token, LEX_TYPE_FUNCTION))
			{
				if (!strcmp(tokens[i].token, LEX_TYPE_NUM))
					outfile << "int ";
				if (!strcmp(tokens[i].token, LEX_TYPE_STR))
					outfile << "String ";
				if (!strcmp(tokens[i].token, LEX_TYPE_DOUBLE))
					outfile << "double ";

			}			
			
			break;
		}

		case LEX_GLAVN:
		{
			isMain = true;
			outfile << "\npublic static void main(String[] args)";
			break;
		}
		case LEX_FUNCTION:
		{
			isFunction = true;
			idFunc = Lextable.table[i + 1].idxTI;
			buf = Lextable.table[i + 1].idxTI;
			string tmp;
			if (!strcmp(tokens[i - 1].token, LEX_TYPE_NUM))
				tmp = "int ";
			else if (!strcmp(tokens[i - 1].token, LEX_TYPE_STR))
				tmp = "string ";
			else
				tmp = "double ";
			outfile << "static " << tmp << idtable.table[buf].id;
			i++;
			break;
		}
		case LEX_LEFTBRACE:
		{
			outfile << " {\n" << "    ";
			break;
		}
		case LEX_BRACELET:
		{
			if (isFunction) 
			    outfile << "\n" << "\r" <<"}"<<"\n";
			break;
		}
		case  LEX_DEF:
		{
			if ((Lextable.table[i + 1].lexema == LEX_NUM || Lextable.table[i + 1].lexema == LEX_STR || Lextable.table[i + 1].lexema == LEX_DOUBLE) && Lextable.table[i + 3].lexema == LEX_EQUAL)
			{
				if (!(Lextable.table[i + 4].lexema == LEX_STRLEN || Lextable.table[i + 4].lexema == LEX_SUBSTR))
				{
					if (!strcmp(tokens[i + 1].token, LEX_TYPE_NUM))
						outfile << "int " << tokens[i + 2].token << '=' << tokens[i + 4].token;
					if (!strcmp(tokens[i + 1].token, LEX_TYPE_STR))
						outfile << "String " << tokens[i + 2].token << '=' << tokens[i + 4].token;
					if (!strcmp(tokens[i + 1].token, LEX_TYPE_DOUBLE))
						outfile << "double " << tokens[i + 2].token << '=' << tokens[i + 4].token;
					i += 4;
				}						
				else
				{
					if (!strcmp(tokens[i + 1].token, LEX_TYPE_NUM))
						outfile << "int " << tokens[i + 2].token << '=';
					if (!strcmp(tokens[i + 1].token, LEX_TYPE_STR))
						outfile << "String " << tokens[i + 2].token << '=';
					if (!strcmp(tokens[i + 1].token, LEX_TYPE_DOUBLE))
						outfile << "double " << tokens[i + 2].token << '=';
					i += 3;
				}
					
				break;
			}
			if ((Lextable.table[i + 1].lexema == LEX_NUM || Lextable.table[i + 1].lexema == LEX_STR || Lextable.table[i + 1].lexema == LEX_DOUBLE) && Lextable.table[i + 3].lexema == LEX_SEPARATOR)
			{
				if(!strcmp(tokens[i+1].token,LEX_TYPE_NUM))
					outfile << "int "  << tokens[i + 2].token << tokens[i+3].token << "\n    ";
				if(!strcmp(tokens[i+1].token,LEX_TYPE_STR))
					outfile << "String " << tokens[i + 2].token << tokens[i+3].token << "\n    ";
				if (!strcmp(tokens[i + 1].token, LEX_TYPE_DOUBLE))
					outfile << "double " << tokens[i + 2].token << tokens[i + 3].token << "\n    ";
				i += 3;
				break;
			}
			while (Lextable.table[i].lexema != LEX_SEPARATOR)
				i++;

			break;
		}
		case LEX_STANDART:
		{
			outfile << "\r";
			break;
		}
		case LEX_ID:
		{
			int y;
			y = Lextable.table[i].idxTI;

			if (Lextable.table[i - 2].lexema == LEX_STRLEN)
				break;
			if (Lextable.table[i - 2].lexema == LEX_SUBSTR)
			{
				i++;
				break;
			}

			if (Lextable.table[i - 2].lexema == LEX_OUT)
				outfile << idtable.table[y].id;
			else
			{
				if (idtable.table[y].iddatatype == IT::STR || IT::NUM || IT::DBL && Lextable.table[i - 2].lexema != LEX_OUT)
					outfile << idtable.table[y].id;
				
			}
			break;
		}
		case LEX_EQUAL:
		{
			outfile << "=";
			break;
		}
		case LEX_SEPARATOR:
		{
			if (Lextable.table[i - 1].lexema == LEX_STANDART)
				outfile << "\n    ";
			else
				outfile << ";\n    ";
			break;
		}
		case LEX_STRLEN:
		{
			outfile << tokens[i + 2].token << ".length";
			break;

		}
		case LEX_SUBSTR:
		{
			outfile << tokens[i+2].token << ".substring";
			break;
		}
		case LEX_LITERAL:
		{
			if (tokens[i].isLiteral ==true)
			{
				if (tokens[i].isStr == true)
					outfile << tokens[i].token; 
				else
				{
					int tp;
					tp = atoi(tokens[i].token);
					if (tp > INT_MAXSIZE)
					{
						throw ERROR_THROW(6);
					}
					else
					{
						if (tp < INT_MIN)
							throw ERROR_THROW(6);
					}
					outfile << tp;
					break;
				}
			}
			break;
		}
		case LEX_OUT:
		{
			outfile << "System.out.println";
			break;
		}
		case LEX_LEFTHESIS:
		{
			outfile << "(";
			break;
		}
		case LEX_RIGHTTHESIS:
		{
			outfile << ")";	
			break;
		}
		case LEX_PLUS:
		{
			outfile << "+";
			break;
		}
		case LEX_STAR:
		{
			outfile << "*";
			break;
		}
		case LEX_MINUS:
		{
			outfile << "-";
			break;
		}
		case LEX_DIRSLASH:
		{
			outfile << "/";
			break;
		}
		case LEX_COMMA:
		{
			outfile << ",";
			break;
		}
		case LEX_BACK:
		{
			if (isMain == true)
			{
				i+=2;
				break;
			}
			else
			{
				if (isFunction)
					outfile << "return ";
			}
				
			break;
		}
		case LEX_STEPEN:
		{
			outfile << "Math.pow";
			break;
		}
		default:
		{
			outfile << tokens[i].token;			
			break;
		}
		}
	}
	outfile << "\n}";

	outfile.close();
}