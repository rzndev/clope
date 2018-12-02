#pragma once
#include <fstream>
#include <vector>
#include <map>

using namespace std;
// класс кластера
class CCluster
{
public:
	CCluster(void);
	virtual ~CCluster(void);
protected:
	// количество вхождений объекта i в кластер C
	map<int, int> Occ;
	// ширина кластера
	int W;
	// количество транзакций в кластере
	int N;
	// количество объектов в кластере
	double S;
public:
	// изменение профита при добавлении транзакции transaction с параметром r
	double delta_add(vector<int>& transaction, double r);
	// изменение профита при удалении транзакции transaction с параметром r
	double delta_del(vector<int>& transaction, double r);
	// добавить транзакцию transaction в кластер
	void add_transaction(vector<int>& transaction);
	// получить идентификатор кластера
	int get_id(void);
	// удалить транзакцию из кластера
	void del_transaction(vector<int>& transaction);
	// получить количество транзакций в кластере
	double get_n(void);
protected:
	// статический счетчик количества транзакций (используется для генерации идентификатора транзакции)
	static int g_id;
	// идентификатор текущей транзакции
	int id;
public:
	// получить характеристику S кластера
	double get_S(void);
	// получить характеристику W кластера
	double get_W(void);
	// Вывести данные кластера в файл
	void print_data(ofstream& fout);
};

