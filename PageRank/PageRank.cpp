// PageRank.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<list>
#include<iomanip>
#include<time.h>
#include <map>
#include <stdio.h>
#include<math.h>
using namespace std;

const double STOPLOSS =0.01;
typedef unsigned short Node_ID;
typedef vector<Node_ID> Links;
typedef map<Node_ID, Links> Page;
 vector<int>dgree;
 Page P;
 typedef vector<double>Rank;
 Rank New;
 Rank Old;
 int max(unsigned short a, unsigned short b)
 {
	 return a > b ? a : b;
 }
 //得到初始链接和节点度数
int Init(const char* filename)
{
	fstream input(filename,ios::in);
	Node_ID from = 0; 
	Node_ID to=0;
	//Node_ID maxID = 0;

	int maxID=0;
	while (!input.eof())
	{
		input >> from >> to;
		P[to].push_back(from);
		maxID = max(maxID, max(from, to));
	}
	input.close();
	dgree.resize(maxID+1);
	for (int i = 0; i < maxID+1; i++)
	{
		dgree[i]=P[i].size();
	}
	return maxID+1;
}
////得到初始化矩阵
//void InitMatrix(int n,double teleport,double** M)
//{
//	for (auto i : P)
//	{
//		for (auto j : i.second)
//		{
//			M[j][i.first] = teleport * (Old[i.first] / dgree[i.first]);
//		}
//	}
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < n; j++)
//		{
//			M[i][j] +=(1.0-teleport)*(1.0/n);
//		}
//	}
//	return ;
//}
//计算PageRank
void ComputeRank(int n,double teleport,double** M)
{
	New.resize(n, 0);
	Old.resize(n,1);
	double loss = 0.0;
	do {
		loss = 0.0;
		for (auto i : P)
		{
			New[i.first] = 0.0;
			for (auto j : i.second)
			{
				New[i.first] += teleport * (Old[j] / dgree[j]) ;
			}
			New[i.first] += (1.0 / n);
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
		//cout << "curr loss:" << loss << endl;
	} while (loss>STOPLOSS);
	cout << "finale loss:" << loss << endl;
	return;
}
int main()
{
	const char* filename = "WikiData.txt";
	double teleport = 0.85;
	int count = Init(filename);
	double** M = new double* [count];

	for (int i = 0; i < count; i++)
	{
		M[i] = new double[count];
	}
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			M[i][j] = 0;
		}
	}
	ComputeRank(count, teleport, M);
	ofstream out("result.txt");
	for (auto i : P)
	{
		out << i.first <<"\t"<< New[i.first] << endl;
	}
	out.close();
	
	return 0;
}




