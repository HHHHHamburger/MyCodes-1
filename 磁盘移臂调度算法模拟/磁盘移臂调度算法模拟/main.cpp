/*
�����Ʊ۵����㷨SSTF��SCANģ��
*/
#include <iostream>
#pragma warning(disable:4996)
using namespace std;

class SSTF
{
private:
	struct indexDistance
	{
		int distance;
		int index;
	};
	int nowindex;
	int size;
	int maxSize;
	indexDistance *list;
	int total;
	int *seq;
	double average;
	int outNumber;
	int inNumber;
public:
	SSTF(int s, int n)
	{
		size = s;
		maxSize = s;
		list = new indexDistance[size];
		seq = new int[size];
		memset(list, 0, sizeof(list)*size);
		memset(seq, 0, sizeof(seq)*size);
		total = 0;
		outNumber = 0;
		inNumber = 0;
		average = 0;
		nowindex = n;
	}
	~SSTF()
	{
		delete[] list;
		list = nullptr;
		delete[] seq;
		seq = nullptr;
	}
	void push(int index)
	{
		list[inNumber++].index = index;
	}
	int getTotal(void)
	{
		return total;
	}

	void printSequence(void)
	{
		cout << "��ǰ���������Ϊ:";
		for (int i = 0; i < maxSize - size; i++)
			cout << seq[i] << " ";
		cout << endl;
	}
	void setDistance()
	{
		for (int i = 0; i < size; i++)
			list[i].distance = abs(list[i].index - nowindex);
	}



	bool out()
	{
		if (size == 0)return false;
		int low = 0;
		for (int i = 0; i < size; i++)
		{
			if (list[low].distance>list[i].distance)
				low = i;
		}
		seq[outNumber++] = list[low].index;
		total += list[low].distance;
		nowindex = list[low].index;
		list[low].index = list[size - 1].index;
		size--;
		average = 1.0*total / (maxSize - size);
		cout << endl << "�����ʵ���һ���ŵ���:";
		cout.width(3);
		cout << nowindex << "\t�ƶ�����:";
		cout.width(3);
		cout << list[low].distance;
		cout << "\tƽ��Ѱ������:";
		cout.width(3);
		cout.precision(3);
		cout << average << endl;
		for (int i = 0; i < size; i++)
		{
			list[i].distance = abs(list[i].index - nowindex);
		}
		return true;
	}

};

class SCAN
{
private:
	struct indexDistance
	{
		int distance;
		int index;
	};
	int nowindex;
	int size;
	int maxSize;
	indexDistance *list, *start;
	int total;
	int *seq;
	double average;
	int outNumber;
	int inNumber;
public:
	SCAN(int s, int n)
	{
		size = s;
		maxSize = s;
		list = new indexDistance[size];
		start = list;
		seq = new int[size];
		memset(list, 0, sizeof(list)*size);
		memset(seq, 0, sizeof(seq)*size);
		total = 0;
		outNumber = 0;
		inNumber = 0;
		average = 0;
		nowindex = n;
	}
	~SCAN()
	{
		delete[] start;
		list = nullptr;
		delete[] seq;
		seq = nullptr;
	}
	void push(int index)
	{
		list[inNumber++].index = index;
	}
	int getTotal(void)
	{
		return total;
	}

	void printSequence(void)
	{
		cout << "��ǰ���������Ϊ:";
		for (int i = 0; i < maxSize - size; i++)
			cout << seq[i] << " ";
		cout << endl;
	}

	void setDistance()
	{
		for (int i = 0; i < size; i++)
			list[i].distance = abs(list[i].index - nowindex);
	}

	void sort()
	{
		int tmp;
		int i = 0, j = size - 1;
		while (i < j)
		{
			if (list[i].index > nowindex)i++;
			if (list[j].index < nowindex)j--;
			tmp = list[i].index;
			list[i].index = list[j].index;
			list[j].index = tmp;
		}
		int key = 0;
		while (list[key].index>nowindex)key++;
		//��0~k-1ð������
		for (i = 0; i < key; i++)
		for (j = i + 1; j < key; j++)
		{
			if (list[i].index>list[j].index)
			{
				tmp = list[i].index;
				list[i].index = list[j].index;
				list[j].index = tmp;
			}
		}
		//��k~size-1ð������
		for (i = key; i < size; i++)
		for (j = i + 1; j < size; j++)
		{
			if (list[i].index < list[j].index)
			{
				tmp = list[i].index;
				list[i].index = list[j].index;
				list[j].index = tmp;
			}
		}
		setDistance();
	}

	bool out()
	{
		if (size == 0)
			return false;
		cout << endl << "�����ʵ���һ���ŵ���:";
		cout.width(3);
		cout << list[0].index << "\t�ƶ�����:";
		cout.width(3);
		cout << list[0].distance;
		total += list[0].distance;
		seq[outNumber++] = list[0].index;
		size--;
		average = 1.0*total / (maxSize - size);
		cout << "\tƽ��Ѱ������:";
		cout.width(3);
		cout.precision(3);
		cout << average << endl;
		nowindex = list[0].index;
		list = list + 1;
		sort();
		return true;
	}

};

int main()
{
	freopen("in.txt", "r", stdin);
	cout << "������ŵ�������:";
	int t;
	cin >> t;
	cout << t << endl;
	cout << "�����뿪ʼ�ŵ���:";
	int n;
	cin >> n;
	cout << n << endl;
	SCAN scan(t, n);
	SSTF sstf(t, n);
	for (int i = 0; i < t; i++)
	{
		cin >> n;
		cout << "�������" << i + 1 << "���ŵ���:";
		scan.push(n);
		sstf.push(n);
		cout << n << endl;
	}
	scan.sort();
	sstf.setDistance();
	cout << endl << "SCAN:" << endl;
	while (scan.out());
	cout << endl << "Total:" << scan.getTotal() << endl;
	scan.printSequence();

	cout << endl << "SSTF:" << endl;
	while (sstf.out());
	cout << endl << "Total:" << sstf.getTotal() << endl;
	sstf.printSequence();
	return 0;
}