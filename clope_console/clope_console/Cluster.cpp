#include "StdAfx.h"
#include "Cluster.h"

#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;


int CCluster::g_id = 0;

CCluster::CCluster(void) : W(0), S(0), N(0)
{
	id = g_id++;
}


CCluster::~CCluster(void)
{
}


double CCluster::delta_add(vector<int>& transaction, double r)
{
	double result = 0.0;
	const map<int,int>::iterator end_Occ = Occ.end();
	double S_new = S + static_cast<double>(transaction.size());
	int W_new = W;
	
	for_each(transaction.begin(), transaction.end(), [this, &W_new, &end_Occ](int item) {
		map<int,int>::iterator elem = this->Occ.find(item);
		if (elem == end_Occ)
		{
			++W_new;
		}
	});
	if (0 == W)
		result = S_new * (N + 1) / pow(static_cast<double>(W_new), r);
	else
		result = S_new * (N + 1) / pow(static_cast<double>(W_new), r) - S * N / pow(static_cast<double>(W), r);
	return result;
}


double CCluster::delta_del(vector<int>& transaction, double r)
{
	double result = 0.0;
	const map<int,int>::iterator end_Occ = Occ.end();
	double S_new = S - static_cast<double>(transaction.size());
	int W_new = W;
	
	for_each(transaction.begin(), transaction.end(), [this, &W_new, &end_Occ](char item) {
		
		map<int,int>::iterator elem = this->Occ.find(item);
		if (elem != end_Occ)
		{
			--W_new;
		}
	});
	if (0 == W_new)
		result = -S * N / pow(static_cast<double>(W), r);
	else
		result = S_new * (N - 1)/ pow(static_cast<double>(W_new), r) - S * N / pow(static_cast<double>(W), r);
	return result;
}





void CCluster::add_transaction(vector<int>& transaction)
{
	const map<int, int>::iterator Occ_end = Occ.end();
	++N;

	S += transaction.size();
	for_each(transaction.begin(), transaction.end(), [this, &Occ_end](int item) {
		map<int, int>::iterator Occ_item = this->Occ.find(item);
		if (Occ_item == Occ_end)
		{
			this->Occ[item] = 1;
			++this->W;
		} else
		{
			this->Occ[item] = this->Occ[item] + 1;
		}
	});
}


int CCluster::get_id(void)
{
	return id;
}


void CCluster::del_transaction(vector<int>& transaction)
{
	const map<int, int>::iterator Occ_end = Occ.end();
	--N;
	assert(N>=0);	
	S -= transaction.size();
	for_each(transaction.begin(), transaction.end(), [this, &Occ_end](int item) {
	map<int, int>::iterator Occ_item = this->Occ.find(item);
	assert (Occ_item != Occ_end);
	this->Occ[item] = this->Occ[item] - 1;
	if (!this->Occ[item]) --W;
	int val = this->Occ[item];
	assert(this->Occ[item] >= 0);
});
}


double CCluster::get_n(void)
{
	return static_cast<double>(N);
}


double CCluster::get_S(void)
{
	return S;
}


double CCluster::get_W(void)
{
	return static_cast<double>(W);
}


void CCluster::print_data(ofstream& fout)
{
	fout << "==========" << endl;
	fout << "cluster id = " << id << endl;
	fout << "S = " << S << endl;
	fout << "W = " << W << endl;
	fout << "N = " << N << endl;
}
