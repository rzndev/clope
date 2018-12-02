#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

// ����� ������ ������
class CData
{
public:
	CData(void);
	~CData(void);
	// ������� ��������� ��������� �� ��������� ������ ������
	// ���������� ���������� � ������� transaction. ��� ���������� ������ ������������ ������ ������ � ��������������� ������� ����� ������ � ������ ������
	void get_transaction(vector<int>& transaction);
	// ������� ����� ������ � ������ filename
	// � ������ ������ ���������� 0
	// � ������ ������ ���������� -1
	int open_database(string filename);
	// ������� ����� ������ � ������ ������
	int is_eof();
	// ������� ���������� transaction � ��������������� ������������� �������� cluster �� �������� ������ ������
	void get_transaction_workdb(vector<int>& transaction, int& cluster);
	// �������� ���������� transaction � ��������������� �������������� �������� cluster �� � ������� ����� ������
	void put_transaction_workdb(vector<int>& transaction, int cluster);
	// �������� ������������� �������� cluster � ��������� ���������, ��������� ������� get_transaction_workdb
	void put_clusterid_workdb(int cluster);
	// ���������������� ��������� ������ � ������ �������� ������ ������
	int rewind(void);
	// ������� ��������� � ��������� ����
	void result_out(void);
protected:
	ifstream in;
	fstream inout;
	vector<int> transaction;
	int flag_eof;
	streamoff read_position_start;
	streamoff read_position_stop;
};

