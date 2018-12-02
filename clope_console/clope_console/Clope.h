#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Cluster.h"
#include "Data.h"

using namespace std;

// ����� ���������
class CClope
{
public:
	CClope(void);
	virtual ~CClope(void);
	// ����� ���������� ��������� � ������ ������ � ���� filename
	// ����� ������ - ������� a-z, ����������� ��������. ���������� ����������� ����� �������
	void run(string filename, double r);
protected:
	// ������� ����� ���������� �� ���� ������
	// transaction - ������ �����, � ������� ���������� ��������������� ������
	// cluster_out - ��������� �� ��������� ���������� ������ ��������
	//               NULL - ������ ������� �� ��������� ������ ������ (�� �������������)
	//               �� NULL - ������ ������� �� ��������������� �� ������ ���� ��������� ������ ������ 
	//               �� ������ *cluster_out ������������ ��������� �� ������ ������ ��������, � �������� ��������� ����������
	int read_next_transaction(vector<int>& transaction, CCluster** cluster_out);
	// ���������������� ��������� ������ � ������ �������� ������ ������
	int rewind_database(void);
	// ������� ���������� ������ ������ ������
	int end_of_database(void);
	// ����� �������, ���������� ���������� transaction � ������� ��������� ���������� ������
	CCluster* find_cluster_maxprofit(vector<int>& transaction, double r, CCluster* cur_cluster);
	// ������� ������ �������
	CCluster* create_empty_cluster(void);
	// �������� ���������� transaction �� ��������������� ���������	� ��������� ������������� �������� cluster_id
	int write_transaction(vector<int>& transaction, int cluster_id);
	// ����� ���������� �������
	double calc_profit(double r);
	// ������� ����� ������ � ������ filename
	int open_database(string filename);
	// �������� cluster_id � ��������� ����������, ��������� ������� read_next_transaction 
	int write_clusterid(int cluster_id);
	// ����� ���������
	vector<CCluster*> clusters;
	// �������� r
	double r;
protected:
	CData db;
    // ����� �������������� ������
	void print_data(ofstream& fout);
};

