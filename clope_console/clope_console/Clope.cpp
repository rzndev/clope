#include "StdAfx.h"
#include "Clope.h"
#include <cassert>

CClope::CClope(void)
	: r(2.6)
{
}


CClope::~CClope(void)
{
	vector<CCluster*>::iterator iter;
	for(iter = clusters.begin(); iter != clusters.end(); ++iter) delete *iter;
}


void CClope::run(string filename, double r)
{
	bool moved;
	vector<int> transaction;
	ofstream statfile("log.txt");
	CCluster *cur_cluster, *next_cluster;
	this->r = r;
	if (open_database(filename) != 0) return;
	// создаем начальный кластер
	create_empty_cluster();
	// 1-я итерация
	int debug_trans = 0;
	while(1)
	{
		read_next_transaction(transaction, NULL);
		if (end_of_database()) break;
		cur_cluster = find_cluster_maxprofit(transaction, r, NULL);
		if (cur_cluster->get_n() == 0)
		{
			// в списке кластеров обязательно должен быть пустой кластер
			create_empty_cluster();
		}
		cur_cluster->add_transaction(transaction);
		write_transaction(transaction, cur_cluster->get_id());
		++debug_trans;
	}
	double profit = calc_profit(r);
	cout << "initial profit = " << profit << endl;
	statfile << "init stage" << endl; 
	print_data(statfile);

	// уточнающие итерации
	int debug_iter = 0;
	int iter_trans = 0;
	do
	{
		rewind_database();
		moved = false;
		while(1)
		{
			read_next_transaction(transaction, &cur_cluster);
			if (end_of_database()) break;
			iter_trans++;
			next_cluster = find_cluster_maxprofit(transaction, r, cur_cluster);
			if (next_cluster == NULL)
			{
				continue;
			}
			if (next_cluster->get_id() != cur_cluster->get_id())
			{
				if (next_cluster->get_n() == 0)
				{
					if (cur_cluster->get_n() > 1)
					{
						// необходимо создать пустой кластер, так как в результате операции перемещения транзакции между кластерами пустых кластеров не останется
						create_empty_cluster();
					}
				}
				cur_cluster->del_transaction(transaction);
				next_cluster->add_transaction(transaction);
				write_clusterid(next_cluster->get_id());
				moved = true;
			}
//			write_transaction(transaction, next_cluster->get_id());
		}
		iter_trans = 0;
		debug_iter++;
		double profit = calc_profit(r);
		cout << "iteration = " << debug_iter << " profit = " << profit << endl;
		statfile << "iteration = " << debug_iter << endl; 
		print_data(statfile);
	} while (moved);
	db.result_out();
}


int CClope::read_next_transaction(vector<int>& transaction, CCluster** cluster_out)
{
	int cluster_id = 0;
	if (NULL == cluster_out)
	{
		db.get_transaction(transaction);
	} else
	{
		db.get_transaction_workdb(transaction, cluster_id);
		if (db.is_eof())
		{
			*cluster_out = NULL;
			return -1;
		}
		assert(cluster_id >= 0);
		assert((vector<CCluster*>::size_type)cluster_id < clusters.size());
		*cluster_out = clusters[cluster_id];
	}
	return 0;
}


int CClope::rewind_database(void)
{
	db.rewind();
	return 0;
}


int CClope::end_of_database(void)
{
	return db.is_eof();
}


CCluster* CClope::find_cluster_maxprofit(vector<int>& transaction, double r, CCluster* cur_cluster)
{
	double max_cost = 0.0;
	double rem_cost = (cur_cluster == NULL) ? 0.0 : cur_cluster->delta_del(transaction, r);
	double add_cost;
	vector<CCluster*>::iterator iter;
	CCluster* best_cluster = NULL;
	for(iter = clusters.begin(); iter != clusters.end(); ++iter)
	{
		add_cost = (*iter)->delta_add(transaction, r);
		if ((add_cost + rem_cost) > max_cost)
		{
			max_cost = add_cost + rem_cost;
			best_cluster = *iter;
		}
	}
	return best_cluster;
}


CCluster* CClope::create_empty_cluster(void)
{
	CCluster* cluster = new CCluster();
	clusters.push_back(cluster);
	return cluster;
}


int CClope::write_transaction(vector<int>& transaction, int cluster_id)
{
	db.put_transaction_workdb(transaction, cluster_id);
	return 0;
}


double CClope::calc_profit(double r)
{
	double num = 0.0;
	double den = 0.0;
	double result;
	for_each(clusters.begin(), clusters.end(), [this, &num, &den, r](CCluster* cluster) {
		assert(cluster->get_n() >= 0);
		if (cluster->get_n())
		{
			den = den + cluster->get_n();
			num = num + cluster->get_S() / pow(cluster->get_W(), r) * cluster->get_n();
		}
	});
	result = (abs(den) > 1e-7) ?	den / num : 0.0;
	return result;
}


int CClope::open_database(string filename)
{
	return db.open_database(filename);
}


int CClope::write_clusterid(int cluster_id)
{
	db.put_clusterid_workdb(cluster_id);
	return 0;
}


void CClope::print_data(ofstream& fout)
{
	for_each(clusters.begin(), clusters.end(), [this, &fout](CCluster* item) {
		if (item->get_n() > 0)
		{
			item->print_data(fout);
		}
	});
}
