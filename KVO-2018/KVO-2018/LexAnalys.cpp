#include "stdafx.h"

namespace LeX
{
	Graph graph[] =                                       
	{   //массив содержащий лексемы и соответствующие им графы
		{ LEX_STANDART, FST::FST(GRAPH_STANDART) },
		{ LEX_NOT_EQUALS, FST::FST(GRAPH_NOT_EQUALS) },
		{ LEX_NUM, FST::FST(GRAPH_NUM) },
		{ LEX_DOUBLE, FST::FST(GRAPH_DOUBLE) },
		{ LEX_STRLEN, FST::FST(GRAPH_STRLEN) },
		{ LEX_SUBSTR, FST::FST(GRAPH_SUBSTR) },
		{ LEX_STEPEN, FST::FST(GRAPH_STEPEN) },
		{ LEX_IF, FST::FST(GRAPH_IF) },
		{ LEX_STR, FST::FST(GRAPH_STR) },
		{ LEX_FUNCTION, FST::FST(GRAPH_FUNCTION) },
		{ LEX_DEF, FST::FST(GRAPH_DEF) },
		{ LEX_BACK, FST::FST(GRAPH_BACK) },
		{ LEX_OUT, FST::FST(GRAPH_OUT) },
		{ LEX_GLAVN, FST::FST(GRAPH_GLAVN) },
		{ LEX_LITERAL, FST::FST(GRAPH_NUM_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_STR_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_DBL_LITERAL) },
		{ LEX_ID, FST::FST(GRAPH_ID) },
		{ LEX_SEPARATOR, FST::FST(GRAPH_SEPARATOR) },
		{ LEX_COMMA, FST::FST(GRAPH_COMMA) },
		{ LEX_LEFTBRACE, FST::FST(GRAPH_LEFT_CURLY_BRACE) },
		{ LEX_BRACELET, FST::FST(GRAPH_RIGHT_CURLY_BRACE) },
		{ LEX_LEFTHESIS, FST::FST(GRAPH_OPEN_PARENTHESIS) },
		{ LEX_RIGHTTHESIS, FST::FST(GRAPH_CLOSE_PARENTHESIS) },
		{ LEX_PLUS, FST::FST(GRAPH_PLUS) },
		{ LEX_MINUS, FST::FST(GRAPH_MINUS) },
		{ LEX_STAR, FST::FST(GRAPH_ASTERISK) },
		{ LEX_DIRSLASH, FST::FST(GRAPH_FORWARD_SLASH) },
		{ LEX_EQUAL, FST::FST(GRAPH_EQUALS) }
	};
	LEX INITLEX(In::IN InStruct, Log::LOG &log) //функция для преобразования из токенов в лексемы
	{
		LEX Tables;
		Tables.Lextable = LT::Create(InStruct.TokenCount); //создаём таблицу лексем
		Tables.IDtable = IT::Create(65536); //создаём таблицу идентификаторов
		bool isDeclare, isParam, isMainFunc, isExecuted, isLiteral;
		isDeclare = isParam = isMainFunc = isExecuted = isLiteral = false;
		IT::IDDATATYPE funcType = IT::NUM;
		for (int i = 0; i < InStruct.TokenCount; i++) //цикл пробега по всем токенам(словам)
		{
			for (int j = 0; j < LENGTH(graph); j++) //цикл пробега по всем графам 
			{
				FST::FST fstex(InStruct.tokens[i].token, graph[j].graph);
				if (FST::execute(fstex))//проверка на совпадение токена
				{
					isExecuted = true;
					switch (graph[j].Lexema) //поиск нужной лексемы
					{
					case LEX_STANDART:
					{
						LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1);
						entrylt.tokenId = i;
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_SUBSTR:
					{
						IT::Entry entryit(InStruct.tokens[i].token, i, funcType = IT::STR, IT::S);
						for (int j = 0; j < Tables.IDtable.size; j++)
						{
							if (!strcmp(InStruct.tokens[i + 2].token, Tables.IDtable.table[j].id))
							{

								for (int k = 0; k < Tables.IDtable.size; k++)
								{
									if (!strcmp(InStruct.tokens[i - 2].token, Tables.IDtable.table[k].id))
									{
										int a = atoi(InStruct.tokens[i + 4].token);
										int b = atoi(InStruct.tokens[i + 6].token);
										char str[TI_STR_MAXSIZE];
										strcpy(str, Tables.IDtable.table[j].value.vstr.str);
										char *c = str;
										c = c + a; // на a-й  символ в строке															
										char *buf = new char[b];
										strncpy(buf, c, b);
										strncpy(Tables.IDtable.table[k].value.vstr.str, buf, b);
									}
								}

							}
						}
						IT::Add(Tables.IDtable, entryit);
						LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line, Tables.IDtable.size);
						entrylt.tokenId = i;
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_STRLEN:
					{
						IT::Entry entryit(InStruct.tokens[i].token, i, funcType = IT::NUM, IT::S);
						for (int j = 0; j < Tables.IDtable.size; j++)
						{
							if (!strcmp(InStruct.tokens[i + 2].token, Tables.IDtable.table[j].id))
							{
								for (int k = 0; k < Tables.IDtable.size; k++)
								{
									if (!strcmp(InStruct.tokens[i - 2].token, Tables.IDtable.table[k].id))
									{
										Tables.IDtable.table[k].value.vint = strlen(Tables.IDtable.table[j].value.vstr.str) - 2;
										break;
									}
								}
								break;
							}
						}
						IT::Add(Tables.IDtable, entryit);

						IT::Add(Tables.IDtable, entryit);
						LT::Entry entrylt(LEX_STRLEN, InStruct.tokens[i].line, InStruct.tokens[i].token[0]);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_ID: //Вариант при подаче идентификатора
					{
						
						if (Tables.Lextable.table[i - 2].lexema == LEX_DEF && IT::IsId(Tables.IDtable, InStruct.tokens[i].token) != -1)
						{ 
							throw ERROR_THROW_IN(103, InStruct.tokens[i].line, NULL);
							break;
						}
						if (IT::IsId(Tables.IDtable, InStruct.tokens[i].token) != -1 && (Tables.Lextable.table[i - 2].lexema == LEX_IF))
						{
							if (IT::GetEntry(Tables.IDtable, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)).value.vint == 0)
								throw ERROR_THROW_IN(602, Tables.Lextable.table[i].sn, NULL);
						}
						if (IT::IsId(Tables.IDtable, InStruct.tokens[i].token) == -1)
						{
							if (Tables.Lextable.table[i - 1].lexema == LEX_SEPARATOR || Tables.Lextable.table[i - 1].lexema == LEX_LEFTHESIS || Tables.Lextable.table[i - 1].lexema == LEX_COMMA || Tables.Lextable.table[i - 1].lexema == LEX_BACK)
							{								
								throw ERROR_THROW_IN(105, InStruct.tokens[i].line, NULL);
								break;
							}
							if (Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION)
							{
								if (!strcmp(InStruct.tokens[i - 2].token, LEX_TYPE_NUM))
								{
									IT::Entry entryit(InStruct.tokens[i].token, i, funcType = IT::NUM, IT::F);
									IT::Add(Tables.IDtable, entryit);
								}
								else
									if (!strcmp(InStruct.tokens[i - 2].token, LEX_TYPE_STR))
									{
										IT::Entry entryit(InStruct.tokens[i].token, i, funcType = IT::STR, IT::F);
										IT::Add(Tables.IDtable, entryit);
									}
									else
										if (!strcmp(InStruct.tokens[i - 2].token, LEX_TYPE_DOUBLE))
										{
											IT::Entry entryit(InStruct.tokens[i].token, i, funcType = IT::DBL, IT::F);
											IT::Add(Tables.IDtable, entryit);
										}
								LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1);
								entrylt.tokenId = i;
								LT::Add(Tables.Lextable, entrylt);
								isParam = true;
								break;
							}
							else
								if (Tables.Lextable.table[i - 1].lexema == LEX_OBJECT)
								{
									if (isParam)
									{
										if (!strcmp(InStruct.tokens[i - 1].token, LEX_TYPE_NUM))
										{
											IT::Entry entryit(InStruct.tokens[i].token, i, IT::NUM, IT::P);
											IT::Add(Tables.IDtable, entryit);
										}
										else
											if (!strcmp(InStruct.tokens[i - 1].token, LEX_TYPE_STR))
											{
												IT::Entry entryit(InStruct.tokens[i].token, i, IT::STR, IT::P);
												IT::Add(Tables.IDtable, entryit);
											}
											else
												if (!strcmp(InStruct.tokens[i - 1].token, LEX_TYPE_DOUBLE))
												{
													IT::Entry entryit(InStruct.tokens[i].token, i, funcType = IT::DBL, IT::P);
													IT::Add(Tables.IDtable, entryit);
												}

										if (InStruct.tokens[i + 1].token[0] == LEX_RIGHTTHESIS)
											isParam = false;
									}
									else
										if (isDeclare)
										{
											if (!strcmp(InStruct.tokens[i - 1].token, LEX_TYPE_NUM))
											{		
												
												IT::Entry entryit(InStruct.tokens[i].token, i, IT::NUM, IT::V);
												bool flag = true;
												int re= atoi(InStruct.tokens[i + 2].token);
												int l = i;
												while (flag)
												{													
													if (!strcmp(InStruct.tokens[l + 3].token, "/"))
													{
														re /= atoi(InStruct.tokens[l + 4].token);
													}
													else if (!strcmp(InStruct.tokens[l + 3].token, "+"))
													{
														re += atoi(InStruct.tokens[l + 4].token);
													}
													else if (!strcmp(InStruct.tokens[l + 3].token, "*"))
													{
														re *= atoi(InStruct.tokens[l + 4].token);
													}
													else if (!strcmp(InStruct.tokens[l + 3].token, "-"))
													{
														re -= atoi(InStruct.tokens[l + 4].token);
													}
													else
													{
														flag = false;
														//cout << re;
														entryit.value.vint=re;
													}
													l += 2;
												}
												
												if (!strcmp(InStruct.tokens[i + 1].token, "strlen"))
												{
													
													for (int j = 0; j < Tables.IDtable.size; j++)
													{
														if (!strcmp(InStruct.tokens[i + 4].token, Tables.IDtable.table[j].id))
														{		
															entryit.value.vint = strlen(Tables.IDtable.table[j].value.vstr.str)-2;
															break;
														}
													}
												}
												//cout << entryit.value.vint;
												IT::Add(Tables.IDtable, entryit);																							
											}
											else if (!strcmp(InStruct.tokens[i - 1].token, LEX_TYPE_STR))
											{
												IT::Entry entryit(InStruct.tokens[i].token, i, IT::STR, IT::V);
												if (!strcmp(InStruct.tokens[i + 2].token, "substr"))
												{
													for (int j = 0; j < Tables.IDtable.size; j++)
													{
														if (!strcmp(InStruct.tokens[i + 4].token, Tables.IDtable.table[j].id))
														{
															int a = atoi(InStruct.tokens[i + 6].token);
															int b = atoi(InStruct.tokens[i + 8].token);
															char str[TI_STR_MAXSIZE];
															strcpy(str, Tables.IDtable.table[j].value.vstr.str);
															char *c = str;
															c = c + a; // на a-й  символ в строке															
															char *buf = new char[b];
															strncpy(buf, c, b);
															strncpy(entryit.value.vstr.str, buf,b);
															break;
														}
													}
													break;
												}												
													
												IT::Add(Tables.IDtable, entryit);
											}
											else if (!strcmp(InStruct.tokens[i - 1].token, LEX_TYPE_DOUBLE))
											{
												IT::Entry entryit(InStruct.tokens[i].token, i, IT::DBL, IT::V);
												if(strcmp(InStruct.tokens[i+1].token,";"))
													strcpy(entryit.value.vstr.str, InStruct.tokens[i+2].token);
												//entryit.value.vdbl = atof(InStruct.tokens[i + 2].token);
												//cout << entryit.value.vdbl;
												IT::Add(Tables.IDtable, entryit);
											}
											
											if (InStruct.tokens[i + 1].token[0] == LEX_RIGHTTHESIS)
												isDeclare = false;
										}

									LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1);
									entrylt.tokenId = i;
									LT::Add(Tables.Lextable, entrylt);
									break;
								}
						}
						else
						{
							LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token));
							entrylt.tokenId = i;
							LT::Add(Tables.Lextable, entrylt);
							break;
						}
					}
					case LEX_LITERAL: //Вариант при подаче лексемы
					{
						isLiteral = true;
						InStruct.tokens[i].isLiteral = true;
						break;
					}
					case LEX_STEPEN:
					{
						IT::Entry entryit(InStruct.tokens[i].token, i, funcType = IT::NUM, IT::S);
						int a, b;
						for (int j = 0; j < Tables.IDtable.size; j++)
						{
							if (!strcmp(InStruct.tokens[i-2].token, Tables.IDtable.table[j].id))
							{
								for (int k = 0; k < Tables.IDtable.size; k++)
								{
									if (!strcmp(InStruct.tokens[i + 2].token, Tables.IDtable.table[k].id))
									{
										a = Tables.IDtable.table[k].value.vint;										
									}
									if (!strcmp(InStruct.tokens[i + 4].token, Tables.IDtable.table[k].id))
									{
										b = Tables.IDtable.table[k].value.vint;										
									}
								}
								Tables.IDtable.table[j].value.vdbl = pow(a,b);
								break;
							}
						}
						IT::Add(Tables.IDtable, entryit);
						LT::Entry entrylt(LEX_STEPEN, InStruct.tokens[i].line, InStruct.tokens[i].token[0]);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_IF:
					{						
						if (IT::IsId(Tables.IDtable, InStruct.tokens[i+2].token) == -1)							
							throw ERROR_THROW_IN(603, InStruct.tokens[i+2].line, NULL);
						
						//if(Tables.Lextable.table[i+3].lexema != LEX_EQUALS_EQUAL || Tables.Lextable.table[i + 3].lexema != LEX_NOT_EQUAL)
							//throw ERROR_THROW_IN(603, InStruct.tokens[i + 2].line, NULL);
						
						LT::Entry entrylt(LEX_IF, InStruct.tokens[i].line, InStruct.tokens[i].token[0]);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_PLUS:
					{
						LT::Entry entrylt(LEX_PLUS, InStruct.tokens[i].line, InStruct.tokens[i].token[0]);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_MINUS:
					{
						LT::Entry entrylt(LEX_STAR, InStruct.tokens[i].line, InStruct.tokens[i].token[0]);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_STAR:
					{
						LT::Entry entrylt(LEX_STAR, InStruct.tokens[i].line, InStruct.tokens[i].token[0]);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_NOT_EQUALS:
					{
						LT::Entry entrylt(LEX_EQUAL, InStruct.tokens[i].line, InStruct.tokens[i].token[0]);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_DIRSLASH:
					{
						LT::Entry entrylt(LEX_DIRSLASH, InStruct.tokens[i].line, InStruct.tokens[i].token[0]);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_SEPARATOR:
					{
						InStruct.tokens[i].isLiteral = false;
						LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_GLAVN:
					{
						isMainFunc = true;
						funcType = IT::NUM;
						LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_DEF:
					{
						isDeclare = true;
						LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_OUT:
					{
						LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_COMMA:
					{
						LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					
					default:
					{
						LT::Entry entrylt(graph[j].Lexema, InStruct.tokens[i].line);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					}
					break;
				}
			}
			if (!isExecuted || isLiteral)
			{
				if (InStruct.tokens[i].isLiteral || isLiteral)
				{
					switch (Tables.Lextable.table[i - 1].lexema)
					{
					case LEX_NOT_EQUALS:
					{
						LT::Entry entrylt(LEX_NOT_EQUALS, InStruct.tokens[i].line, Tables.IDtable.size - 1);
						entrylt.tokenId = i;
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_EQUAL: //Вариант при подаче равно
					{
						InStruct.tokens[i].isLiteral = true;
						LT::Entry entrylt(LEX_LITERAL, InStruct.tokens[i].line, Tables.IDtable.size);
						LT::Add(Tables.Lextable, entrylt);
						if (IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token) != -1)
							if ((Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::V ||
								Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::P))
							{
								IT::SetValue(Tables.IDtable, IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token), InStruct.tokens[i].token);
							}
						break;
					}
					
					case LEX_OUT:
					{
						LT::Entry entrylt(LEX_LITERAL, InStruct.tokens[i].line, Tables.IDtable.size - 1);
						entrylt.tokenId = i;
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_BACK:
					{
						LT::Entry entrylt(LEX_LITERAL, InStruct.tokens[i].line, Tables.IDtable.size - 1);
						entrylt.tokenId = i;
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					case LEX_COMMA:
					{
						LT::Entry entrylt(LEX_LITERAL, InStruct.tokens[i].line, Tables.IDtable.size-1);
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
					default:
						LT::Entry entrylt(LEX_LITERAL, InStruct.tokens[i].line, Tables.IDtable.size-1);
						entrylt.tokenId = i;
						LT::Add(Tables.Lextable, entrylt);
						break;
					}
				}
				else
					throw ERROR_THROW_IN(124, Tables.Lextable.table[i - 2].sn, NULL);
			}
			isExecuted = false;
			isLiteral = false;
		}
		return Tables; //возвращаем таблицу лексем
	}
}
