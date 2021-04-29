// PageRank.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<time.h>
#include <map>
#include <stdio.h>
#include<math.h>
using namespace std;

const double STOPLOSS = 1e-10;//精度
typedef unsigned int Node_ID;
typedef vector<Node_ID> Links;//邻接表的每一行
typedef map<Node_ID, Links> Page;//邻接表
vector<int>dgree;//入度
vector<int>out_dgree;//出度
Page P;//入度邻接表
Page P_out;//出度邻接表
typedef vector<double>Rank;//rank值数组，下标即为对应Node_ID
Rank New;
Rank Old;
Node_ID maxID = 0;
typedef map<Node_ID, double>Node_Score;//Node的最终score
Node_Score node_score;
typedef pair<Node_ID, double>PAIR;

int max(unsigned short a, unsigned short b)
{
	return a > b ? a : b;
}
//得到初始链接和节点度数
int Init(const char* filename)
{
	fstream input(filename);
	Node_ID from = 0;
	Node_ID to = 0;

	while (!input.eof() && input >> from >> to)
	{
		P[to].push_back(from);
		P_out[from].push_back(to);
		maxID = max(maxID, max(from, to));
	}
	input.close();
	dgree.resize(maxID + 1);
	out_dgree.resize(maxID + 1);
	for (int i = 0; i < maxID + 1; i++)
	{
		dgree[i] = P[i].size();
		out_dgree[i] = P_out[i].size();
	}
	int count = maxID;
	for (int i = 0; i < maxID + 1; i++)
	{
		if (dgree[i] == 0 && out_dgree[i] == 0)
			count--;
	}
	return count + 1;
}

//计算PageRank
void ComputeRank(int n, double teleport)
{
	New.resize(maxID + 1, 0);
	Old.resize(maxID + 1, 1.0 / n);
	double loss = 0.0;
	do {
		loss = 0.0;
		for (auto i : P)
		{
			New[i.first] = 0.0;
			for (int j = 0; j < i.second.size(); j++)
			{
				if (out_dgree[i.second[j]] != 0)
					New[i.first] += teleport * (Old[i.second[j]] / out_dgree[i.second[j]]);
				else
					New[i.first] += 0;
			}
		}
		double S = 0.0;
		for (auto i : New)
		{
			S += i;
		}
		//cout << "curr S: " << S << endl;
		for (int i = 0; i < New.size(); i++)
		{
			New[i] += (1.0 - S) / n;
		}
		for (int i = 0; i < New.size(); i++)
		{
			double temp = New[i] - Old[i];
			if (temp > 0)
			{
				loss += temp;
			}
			else
			{
				loss += -temp;
			}
		}
		for (int i = 0; i < New.size(); i++)
		{
			Old[i] = New[i];
		}
		cout << "curr loss:" << loss << endl;
	} while (loss > STOPLOSS);
	cout << "finale loss:" << loss << endl;
	ofstream res("result.txt");
	for (auto i : P)
	{
		res << i.first << "\t" << New[i.first] << endl;
	}
	res.close();
	return;
}

bool cmp_by_value(const PAIR& lhs, const PAIR& rhs) {
	return lhs.second > rhs.second;
}
struct CmpByValue {
	bool operator()(const PAIR& lhs, const PAIR& rhs) {
		return lhs.second > rhs.second;
	}
};

void gettop100()
{
	for (int i = 0; i < New.size(); i++)
	{
		node_score[i] = New[i];
	}
	vector<PAIR>score_vec(node_score.begin(), node_score.end());
	sort(score_vec.begin(), score_vec.end(), CmpByValue());
	ofstream topres("top100.txt");
	for (int i = 0; i < 100; i++)
	{
		topres << score_vec[i].first << "\t" << score_vec[i].second << endl;
	}
	topres.close();
	return;
}

int main()
{
	const char* filename = "WikiData.txt";
	double teleport = 0.85;
	int count = Init(filename);
	cout << endl;
	ComputeRank(count, teleport);
	ofstream out("dgree.txt");
	for (auto i : P)
	{
		out << i.first << "\t" << "out_dgree: " << out_dgree[i.first] << "\tin_dgree: " << dgree[i.first] << endl;
	}
	out.close();
	gettop100();
	return 0;
}




