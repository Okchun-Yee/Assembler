#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <ctype.h>
#include "Assembler.h"

#define SIZE 1024 //1KB
using namespace std;

//Read String FILE & Sort dictionary
int counter(FILE* fp) {
	//���Ͽ��� ���ڿ��� �о arr ������ �迭�� ����
	char buffer[(SIZE)];	//�ӽ÷� ���ڿ� ���� ����	(����ũ�⿡ ���缭 ũ�� ���� 5KB)

	int word_cnt = 0; // �ܾ� �� �� ����

	//word �����ϱ� => ������ (, . " " ! ? \n ���)
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {	//���ۿ� ���ڿ� ��ü�� ����
		// ������ ������ �������� ���ڿ� �и�
		char* token = strtok(buffer, "\t\n,+[]: ");	//������ ����
		while (token != NULL) {
			word_cnt++;
			cout << token << endl;
			token = strtok(NULL, ", \n");
		}
	}
	return 1;
}