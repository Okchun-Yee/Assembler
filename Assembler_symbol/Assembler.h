#pragma once
#include <map>

//�Լ� ����
FILE* File_Open(const char*);  //���� ����
FILE* File_Open_w(const char* name);
int counter(FILE*);
void find_Registers(char* token, int* cnt);
void storeLabel(const char* label, int line_number);
void replaceLabelsInFile(FILE* inputFile, FILE* outputFile);

extern const char* registers[];	//�������� ����
extern const char* instruc[];	//��ɾ� ����

extern std::map<std::string, int> label_map;  // ���̺��� ������ map