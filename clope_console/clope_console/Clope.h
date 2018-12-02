#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Cluster.h"
#include "Data.h"

using namespace std;

// класс алгоритма
class CClope
{
public:
	CClope(void);
	virtual ~CClope(void);
	// метод применени€ алгоритма к набору данных в файл filename
	// набор данных - символы a-z, разделенные зап€тыми. “ранзакции раздел€ютс€ новой строкой
	void run(string filename, double r);
protected:
	// считать новую транзакцию из базы данных
	// transaction - вектор целых, в который помещаютс€ нормализованные данные
	// cluster_out - указатель на указатель экземпл€ра класса кластера
	//               NULL - данные берутс€ из исходного набора данных (до кластеризации)
	//               не NULL - данные берутс€ из подготовленного на первом шаге алгоритма набора данных 
	//               по адресу *cluster_out возвращаетс€ указатель на объект класса кластера, к которому относитс€ транзакци€
	int read_next_transaction(vector<int>& transaction, CCluster** cluster_out);
	// инициализировать указатели чтени€ и записи рабочего набора данных
	int rewind_database(void);
	// признак завершени€ чтени€ набора данных
	int end_of_database(void);
	// найти кластер, добавление транзакции transaction в который обеспечит наибольший профит
	CCluster* find_cluster_maxprofit(vector<int>& transaction, double r, CCluster* cur_cluster);
	// создать пустой кластер
	CCluster* create_empty_cluster(void);
	// записать транзакцию transaction во вспомогательное хранилище	и назначить идентификатор кластера cluster_id
	int write_transaction(vector<int>& transaction, int cluster_id);
	// метод вычислени€ профита
	double calc_profit(double r);
	// открыть набор данных с именем filename
	int open_database(string filename);
	// обновить cluster_id в последней транзакции, считанной методом read_next_transaction 
	int write_clusterid(int cluster_id);
	// набор кластеров
	vector<CCluster*> clusters;
	// параметр r
	double r;
protected:
	CData db;
    // вывод статистических данных
	void print_data(ofstream& fout);
};

