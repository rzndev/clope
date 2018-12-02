#pragma once
#include <fstream>
#include <vector>
#include <map>

using namespace std;
// ����� ��������
class CCluster
{
public:
	CCluster(void);
	virtual ~CCluster(void);
protected:
	// ���������� ��������� ������� i � ������� C
	map<int, int> Occ;
	// ������ ��������
	int W;
	// ���������� ���������� � ��������
	int N;
	// ���������� �������� � ��������
	double S;
public:
	// ��������� ������� ��� ���������� ���������� transaction � ���������� r
	double delta_add(vector<int>& transaction, double r);
	// ��������� ������� ��� �������� ���������� transaction � ���������� r
	double delta_del(vector<int>& transaction, double r);
	// �������� ���������� transaction � �������
	void add_transaction(vector<int>& transaction);
	// �������� ������������� ��������
	int get_id(void);
	// ������� ���������� �� ��������
	void del_transaction(vector<int>& transaction);
	// �������� ���������� ���������� � ��������
	double get_n(void);
protected:
	// ����������� ������� ���������� ���������� (������������ ��� ��������� �������������� ����������)
	static int g_id;
	// ������������� ������� ����������
	int id;
public:
	// �������� �������������� S ��������
	double get_S(void);
	// �������� �������������� W ��������
	double get_W(void);
	// ������� ������ �������� � ����
	void print_data(ofstream& fout);
};

