#include "StdAfx.h"
#include "Data.h"

#include <cassert>


CData::CData(void)
	: flag_eof(0), read_position_start(0), read_position_stop(0)
{
}


CData::~CData(void)
{
}


void CData::get_transaction(vector<int>& transaction)
{
	vector<char> result;
	string line;
	char c;
	const unsigned int start_char = static_cast<unsigned int>('a');
	int pos = 0;
	getline(in, line);
	istringstream iss(line);
	transaction.clear();
	while(1)
	{
		iss >> c;
		if (iss.eof()) break;
		if (c == ',')  
		{
			++pos;
		} else
		{
			result.push_back(c);
			if (c != '?' && c >= 'a' && c <= 'z')
			{
				unsigned int data = static_cast<int>(c) - start_char + 1;
				data = data + 26 * pos;
				transaction.push_back(data);
			}
		}
		
	};
	if (transaction.size() == 0) 
	{
			flag_eof = 1;
	}
	return;
}


int CData::open_database(string filename)
{
	flag_eof = 1;
	in.open(filename, ios::in);
	if (!in) return -1;
	inout.open("tmp.bin", ios::out | ios::binary);
	if (!inout) return -1;
	inout.close();
	inout.open("tmp.bin", ios::out | ios::in | ios::binary);
	if (!inout) return -1;
	// первые символы в тестовых данных - заголовок UTF
	char c;
	in >> c;
	if (c == -17)
	{
		in >> c;
		if (c == -69)
		{
			in >> c;
			if (c != -65)
			{
				in.seekg(0);
			}
		}
	}
	flag_eof = 0;
	return 0;
}

int CData::is_eof()
{
	return flag_eof;
}

void CData::get_transaction_workdb(vector<int>& transaction, int& cluster)
{
	assert(!inout.fail());
	read_position_start = inout.tellg();
	transaction.clear();
	int val_read;
	inout.read((char*)&val_read, sizeof(int));
	if (inout.eof())
	{
		cluster = -1;
		flag_eof = 1;
		inout.clear();
		return;
	}
	cluster = val_read;
	do
	{
		inout.read((char*)&val_read, sizeof(int));
		if (inout.eof()) {
			flag_eof = 1;
			inout.clear();
			break;
		}
		if (val_read)
			transaction.push_back(val_read);
	} while (!inout.eof() && (val_read != 0));
	read_position_stop = inout.tellg();
	if (inout.eof()) {
		inout.clear();
		flag_eof = 1;
	}
}


void CData::put_transaction_workdb(vector<int>& transaction, int cluster)
{
	const int transaction_delimiter = 0;
	assert(!inout.fail());
	inout.write((char*)&cluster, sizeof(int));
	for_each(transaction.begin(), transaction.end(), [this](int item) {
		inout.write((char*)&item, sizeof(int));
	});
	inout.write((char*)&transaction_delimiter, sizeof(int));
}


void CData::put_clusterid_workdb(int cluster)
{
	assert(!inout.fail());
	inout.seekp(read_position_start);
	inout.write((char*)&cluster, sizeof(int));
	inout.seekp(read_position_stop);
	inout.seekg(read_position_stop);
}


int CData::rewind(void)
{
	assert(!inout.fail());
	inout.seekg(0);
	inout.seekp(0);
	flag_eof = 0;
	return 0;
}


void CData::result_out(void)
{
	vector<int> transaction;
	int cluster_id;
	rewind();
	ofstream fout("result.txt");
	unsigned int current_position;
	const unsigned int start_char = static_cast<unsigned int>('a');
	do
	{
		get_transaction_workdb(transaction, cluster_id);	
		if (flag_eof) break;
		current_position = 0;
		if (transaction.size())
		{
			fout << cluster_id << ", ";
			for_each(transaction.begin(), transaction.end(), [this, &fout, start_char, &current_position](int item){
			    unsigned int pos = item / 26;
				unsigned int c = item % 26;
				c = c + start_char - 1;
				assert(pos >= current_position);
				while (pos != current_position)
				{
					if (current_position) fout << ", ";
					fout << "?";
					++current_position;
				}
				if (current_position) fout << ", ";
				fout << static_cast<char>(c);
				++current_position;
			});
			fout << endl;
		}
	} while(1);
	fout.close();
}
