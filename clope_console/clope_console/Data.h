#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

// класс набора данных
class CData
{
public:
	CData(void);
	~CData(void);
	// считать очередную транзацию из исходного набора данных
	// возвращает транзакцию в векторе transaction. ѕри отсутствии данных возвращаетс€ пустой вектор и устанавливаетс€ признак конца данных в наборе данных
	void get_transaction(vector<int>& transaction);
	// открыть набор данных с именем filename
	// в случае успеха возвращает 0
	// в случае ошибки возвращает -1
	int open_database(string filename);
	// признак конца данных в наборе данных
	int is_eof();
	// считать транзакцию transaction и соответствующий идентификатор кластера cluster из рабочего набора данных
	void get_transaction_workdb(vector<int>& transaction, int& cluster);
	// записать транзакцию transaction и соответствующий индентификатор кластера cluster из в рабочий набор данных
	void put_transaction_workdb(vector<int>& transaction, int cluster);
	// записать идентификатор кластера cluster в последнюю транзацию, считанную методом get_transaction_workdb
	void put_clusterid_workdb(int cluster);
	// инициализировать указатели чтени€ и записи рабочего набора данных
	int rewind(void);
	// вывести результат в тектосвый файл
	void result_out(void);
protected:
	ifstream in;
	fstream inout;
	vector<int> transaction;
	int flag_eof;
	streamoff read_position_start;
	streamoff read_position_stop;
};

