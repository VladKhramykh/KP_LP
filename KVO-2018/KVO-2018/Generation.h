#pragma once
#define Head "public class Main {\n" //"шапка" нашей генерации

void Generation(LT::LexTable &Lextable, In::StToken *tokens, IT::IdTable & idtable); //создаём прототип для функции генерации