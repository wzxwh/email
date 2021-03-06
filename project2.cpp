// project2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

struct word
{
	string str;
	int n;
};

inline bool islowword(string &str);	//判断一个字符串是不是单词并且把它改成小写， 如果前面部分和后面部分是非字符并且去除后剩下的是单词，那么返回true并且修改字符串。
void adddata(vector<word> &data, string str);
void creatof();
int vectorfind(const vector<string> &data, int low, int high, const string &str);;	//二分查找
bool checkspam(const string &str, const vector<string> &hamword, const vector<double> &hamnum, const vector<string> &spamword, const vector<double> &spamnum);

int main()
{
	ifstream hamif;
	ifstream spamif;
	fstream newemail("E:\\myvscpp\\project2\\project2\\NewEmail.txt");
	fstream test("E:\\myvscpp\\project2\\project2\\TestingSet.txt");
	vector<string> hamword;
	vector<string> spamword;
	vector<double> hamnum;
	vector<double> spamnum;
	vector<bool> hs;
	string str;
	int index = 0;
	int ham = 0, spam = 0;
	double num;
	double sp = 1.0;
	double hp = 1.0;

	hamif.open("E:\\myvscpp\\project2\\project2\\hamdictionary.txt");
	spamif.open("E:\\myvscpp\\project2\\project2\\spamdictionary.txt");
	if (!hamif || !spamif)
	{
		creatof();
		hamif.open("E:\\myvscpp\\project2\\project2\\hamdictionary.txt");
		spamif.open("E:\\myvscpp\\project2\\project2\\spamdictionary.txt");
	}

	while (hamif >> str >> num)
	{
		hamword.push_back(str);
		hamnum.push_back(num);
	}
	while (spamif >> str >> num)
	{
		spamword.push_back(str);
		spamnum.push_back(num);
	}
	hamif.close();
	spamif.close();

	getline(newemail, str);

	if (checkspam(str, hamword, hamnum, spamword, spamnum))
		cout << "spam" << endl;
	else
		cout << "ham" << endl;

	while (getline(test, str))
	{
		if (checkspam(str, hamword, hamnum, spamword, spamnum))
			hs.push_back(true);
		else
			hs.push_back(false);
	}

	for (int i = 0; i < 45; ++i)
	{
		if (!hs[i])
			ham += 1;
	}
	for (int i = hs.size() - 45; i < hs.size(); ++i)
	{
		if (hs[i])
			spam += 1;
	}
	cout << ham << spam << endl;

	test.close();

	fstream testin("E:\\myvscpp\\project2\\project2\\TestingSet.txt");
	fstream newtrain("TrainingSet.txt", ofstream::app);

	int i = 0;
	while (getline(testin, str))
	{
		if (hs[i])
			newtrain << "spam " << str << '\n';
		else
			newtrain << "ham" << str << '\n';
		i++;
	}

	testin.close();
	newtrain.close();

	creatof();
	ifstream newhamif;
	ifstream newspamif;
	vector<string> newhamword;
	vector<string> newspamword;
	vector<double> newhamnum;
	vector<double> newspamnum;
	newhamif.open("E:\\myvscpp\\project2\\project2\\hamdictionary.txt");
	newspamif.open("E:\\myvscpp\\project2\\project2\\spamdictionary.txt");

	while (newhamif >> str >> num)
	{
		newhamword.push_back(str);
		newhamnum.push_back(num);
	}
	while (newspamif >> str >> num)
	{
		newspamword.push_back(str);
		newspamnum.push_back(num);
	}
	newhamif.close();
	newspamif.close();

	getline(newemail, str);

	if (checkspam(str, newhamword, newhamnum, newspamword, newspamnum))
		cout << "spam" << endl;
	else
		cout << "ham" << endl;



	return 0;
}

bool islowword(string &str)
{
	bool till = false;	//是否开始读取字符
	bool ntill = false;	//是否字符已经读取完毕
	int count = 0;
	int size = str.size();
	for (int i = 0; i < size; ++i)
	{
		if (!isalpha(str[i - count]))
		{
			if (!till)
			{
				str.erase(i - count, 1);
				count += 1;
			}
			if (ntill)
			{
				str.erase(i - count, 1);
				count += 1;
			}
			if (till && !ntill)
			{
				ntill = true;
				str.erase(i - count, 1); count += 1;
			}
		}
		else
		{
			if (!till)
				till = true;
			if (ntill)
				return false;
			if (str[i - count] >= 'A' && str[i - count] <= 'Z')
				str[i - count] -= 'A' - 'a';
			
		}
	}
	if (str.size() != 0)
		return true;
	else
		return false;
}

void adddata(vector<word> &data, string str)
{
	if (data.size() == 0)
	{
		word *x;
		x = new word;
		x->n = 1;
		x->str = str;
		data.push_back(*x);
	}
	else
	{
		for (int i = 0; i < data.size(); ++i)
		{
			if (str == data[i].str)
			{
				data[i].n += 1;
				break;
			}
			if (str < data[i].str)
			{
				word *x;
				x = new word;
				x->n = 1;
				x->str = str;
				data.insert(data.begin() + i, *x);
				break;
			}
			if (str > data[i].str && i == data.size() - 1)
			{
				word *x;
				x = new word;
				x->n = 1;
				x->str = str;
				data.push_back(*x);
				break;
			}
		}
	}
}

void creatof()
{
	fstream f("TrainingSet.txt");
	ofstream ofham("E:\\myvscpp\\project2\\project2\\hamdictionary.txt");
	ofstream ofspam("E:\\myvscpp\\project2\\project2\\spamdictionary.txt");
	string str;
	vector<word> hamdata;
	vector<word> spamdata;
	int hamcount = 0;
	int spamcount = 0;
	bool flag = true;

	while (f >> str)
	{
		if (islowword(str))
		{
			if (str == "ham")
			{
				flag = true;
				hamcount += 1;
				continue;
			}
			if (str == "spam")
			{
				flag = false;
				spamcount += 1;
				continue;
			}
			if (flag)
				adddata(hamdata, str);
			else
				adddata(spamdata, str);
		}
	}

	for (int i = 0; i < hamdata.size(); ++i)
		ofham << hamdata[i].str << ' ' << hamdata[i].n / double(hamcount) << endl;
	for (int i = 0; i < spamdata.size(); ++i)
		ofspam << spamdata[i].str << ' ' << spamdata[i].n / double(spamcount) << endl;

	f.close();
	ofham.close();
	ofspam.close();
}

int vectorfind(const vector<string> &data,int low, int high, const string &str)
{
	int mid = (low + high) / 2;

	if (low > high)
		return -1;
	if (data[mid] == str)
		return mid;
	if (data[mid] > str)
		return vectorfind(data, low, mid - 1, str);
	if (data[mid] < str)
		return vectorfind(data, mid + 1, high, str);
}

bool checkspam(const string &str, const vector<string> &hamword, const vector<double> &hamnum, const vector<string> &spamword, const vector<double> &spamnum)
{
	stringstream ss(str);
	string sub;
	int index;
	double sp = 1.0;
	double hp = 1.0;

	while (ss >> sub)
	{
		if (islowword(sub))
		{
			index = vectorfind(hamword, 0, hamword.size() - 1, sub);
			if (index == -1)
				hp *= 0.0001;
			else
				hp *= hamnum[index];
			index = vectorfind(spamword, 0, spamword.size() - 1, sub);
			if (index == -1)
				sp *= 0.0001;
			else
				sp *= spamnum[index];
		}
	}
	if (sp > hp)
		return true;
	return false;
}