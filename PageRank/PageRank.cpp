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
using namespace std;

typedef unsigned short Node_ID;
typedef vector<Node_ID> Links;
typedef map<Node_ID, Links> Page;
 vector<int>dgree;
 Page P;
 int max(unsigned short a, unsigned short b)
 {
	 return a > b ? a : b;
 }
 //得到初始链接和节点度数
int Init(const char* filename)
{
	ifstream input(filename);
	Node_ID from = 0; 
	Node_ID to=0;
	//Node_ID maxID = 0;

	int maxID=0;
	while (!input.eof() && (input >> from >> to))
	{
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
//得到初始化矩阵
void InitMatrix(int n,double teleport,double** M)
{
	
	for (auto i : P)
	{
		for (auto j : i.second)
		{
			M[j][i.first] = teleport * (1.0 / dgree[i.first]);
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			M[i][j] +=(1.0-teleport)*(1.0/n);
		}
	}
	return ;
}
int main()
{
	const char* filename = "Data.txt";
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
	for (auto i : P)
	{
		cout << i.first << " " << "Links:";
		for (int j = 0; j < i.second.size(); j++)
		{
			cout << i.second[j];
		}
		cout << endl;
	}
	for (int i = 0; i < count; i++)
	{
		cout << dgree[i] << " ";
	}
	InitMatrix(count, teleport, M);

	cout << endl;
	cout << "the matrix:" << endl;
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			cout << M[i][j] << " ";
		}
		cout << endl;
	}
	
	return 0;
}




