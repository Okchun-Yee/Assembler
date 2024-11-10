#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctype.h>
#include <map>
#include <cctype>  // for isalpha
#include "Assembler.h"

using namespace std;

#define SIZE 1024 //1KB
map<string, int> label_map;  // ���̺��� ������ ��ųʸ�

const char* registers[] = {
    "AX", "BX", "CX", "DX",  // 16-bit �Ϲ� �������͵�
    "AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH",  // 8-bit ���� �������͵�
    "SI", "DI", "SP", "BP",  // �ε���, ���÷���, ���� ������, ���̽� ������
    "IP", "FLAGS",  // ���α׷� ī����, �÷��� ��������
    "CS", "DS", "SS", "ES", "FS", "GS",  // ���׸�Ʈ �������͵�
};

const char* instruc[] = {
    "MOV", "ADD", "DIV", "INC", "DEC", "CMP", "SUB", "INT", // ����� ��ɾ�
    "JA", "JAE", "JB", "JBE", "JC", "JE", "JNE",  // ���Ǻб� ��ɾ�
    "DW", "DB"  // ����
};

// ��ųʸ��� ���̺��� �����ϴ� �Լ�
void storeLabel(const char* label, int line_number) {
    // ���̺��� ':' ����
    string clean_label = label;
    if (clean_label.back() == ':') {
        clean_label.pop_back();  // ':' ����
    }

    // ���̺��� �̹� map�� �������� ������ �߰�
    if (label_map.find(clean_label) == label_map.end()) {
        label_map[clean_label] = line_number;  // ���̺�� �ش� �� ��ȣ�� ����
    }
}

// ���ڿ����� �ܾ �����Ͽ� ���̺��� ã�� ��ųʸ��� �����ϴ� �Լ�
int counter(FILE* fp) {
    char buffer[SIZE];  // �ӽ÷� ���ڿ� ���� ����
    int word_cnt = 0;   // �ܾ� �� �� ����

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {  // ���ۿ� ���ڿ� ��ü�� ����
        int instruction_length = sizeof(instruc) / sizeof(instruc[0]);
        bool check = true;

        // ���ڿ��� ������ �������� �и�
        char* token = strtok(buffer, "\t\n,+[] ");
        while (token != NULL) {
            // ���̺� ã��: ':'�� �������� Ȯ���ϰ� ':' ����
            if (token[strlen(token) - 1] == ':') {
                token[strlen(token) - 1] = '\0';  // ':' ����
                cout << "Label : " << token << " " << word_cnt << endl;
                storeLabel(token, word_cnt);  // ���̺�� �ش� �� ��ȣ ����
            }

            // ��ɾ����� Ȯ��
            bool is_instruction = false;
            for (int i = 0; i < instruction_length; i++) {
                if (strcmp(token, instruc[i]) == 0) {
                    is_instruction = true;
                    break;
                }
            }

            if (!is_instruction) {
                find_Registers(token, &word_cnt);  // �������� Ȯ��
            }

            token = strtok(NULL, "\t\n,+[] ");  // ���� ��ū
        }
        word_cnt++;
    }

    return 1;
}

// �������� Ȯ��
void find_Registers(char* token, int* cnt) {
    int register_count = sizeof(registers) / sizeof(registers[0]);
    bool is_register = false;

    for (int i = 0; i < register_count; i++) {
        if (strcmp(token, registers[i]) == 0) {
            is_register = true;
            break;
        }
    }

    // ��ɾ �ƴϰ� �������͵� �ƴ� ���, IP �ּ� ����
    if (!is_register && isalpha(token[0])) {
        (*cnt)++;
    }
}

// ���̺��� �ش� �� ��ȣ�� ġȯ�ϴ� �Լ�
void replaceLabel(FILE* inputFile, FILE* outputFile) {
    char buffer[SIZE];  // ���Ͽ��� �� �پ� ���� ����
    while (fgets(buffer, sizeof(buffer), inputFile)) {
        char* token = strtok(buffer, "\t\n,+[] ");  // �����ڷ� ���ڿ��� �и�
        while (token != NULL) {
            // ���̺� ó��: ':'�� ������ ��ū�� Ȯ���ϰ� ':' ����
            if (token[strlen(token) - 1] == ':') {
                // ':' ����
                token[strlen(token) - 1] = '\0';
            }

            // ���̺��� ã����, map���� �ش� �� ��ȣ�� ġȯ
            if (label_map.find(token) != label_map.end()) {
                int line_number = label_map[token];
                fprintf(outputFile, "%d ", line_number);  // ���̺��� ���ڷ� ġȯ
            }
            else {
                fprintf(outputFile, "%s ", token);  // ���̺��� �ƴϸ� �״�� ���
            }
            token = strtok(NULL, "\t\n,+[] ");  // ���� ��ū
        }
        fprintf(outputFile, "\n");
    }
}

