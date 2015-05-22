#include <iostream>
#pragma warning(disable:4996)
using namespace std;


class FIFO
{
private:
	int size;
	int missCnt;
	int Cnt;
	double missPercent;
	int *data;
public:
	FIFO(int s)
	{
		size = s;
		missCnt = 0;
		Cnt = 0;
		missPercent = 0;
		data = new int[size];
		memset(data, -1, size*sizeof(int));
	}
	~FIFO()
	{
		delete[] data;
	}
	void showdata(void)
	{
		for (int i = 0; i < size; i++)
		{
			cout.width(2);
			cout << data[i] << " ";
		}
		cout << endl;
	}

	bool find(int elem)
	{
		cout << "�û�ǰ:";
		showdata();
		for (int i = 0; i < size; i++)
		if (data[i] == elem)
		{
			Cnt++;
			missPercent = 1.0*missCnt / Cnt;
			cout << "�û���:";
			showdata();
			return true;
		}
		for (int i = size - 1; i >0; i--)
			data[i] = data[i - 1];
		data[0] = elem;
		Cnt++;
		missCnt++;
		missPercent = 1.0*missCnt / Cnt;
		cout << "�û���:";
		showdata();
		return false;
	}


	int getCnt(void)
	{
		return Cnt;
	}
	int getMissCnt(void)
	{
		return missCnt;
	}
	double getMissPercent(void)
	{
		return missPercent;
	}
};

class LRU
{
private:
	int size;
	int *data;
	int missCnt;
	int Cnt;
	double missPercent;

public:
	LRU(int s)
	{
		size = s;
		data = new int[size];
		missCnt = 0;
		Cnt = 0;
		missPercent = 0;
		memset(data, -1, size*sizeof(int));
	}
	~LRU()
	{
		delete[] data;
	}
	void showdata(void)
	{
		for (int i = 0; i < size; i++)
		{
			cout.width(2);
			cout << data[i] << " ";
		}
		cout << endl;
	}

	bool find(int elem)
	{
		cout << "�û�ǰ:";
		showdata();
		for (int i = 0; i < size; i++)
		if (data[i] == elem)
		{
			Cnt++;
			for (int j = i; j>0; j--)
			{
				data[j] = data[j - 1];
			}
			data[0] = elem;
			missPercent = 1.0*missCnt / Cnt;
			cout << "�û���:"; 
			showdata();
			return true;
		}
		for (int j = size-1; j > 0; j--)
			data[j] = data[j - 1];
		data[0] = elem;
		Cnt++;
		missCnt++;
		missPercent = 1.0*missCnt / Cnt;
		cout << "�û���:";
		showdata();
		return false;
	}
	
	int getCnt(void)
	{
		return Cnt;
	}
	int getMissCnt(void)
	{
		return missCnt;
	}
	double getMissPercent(void)
	{
		return missPercent;
	}

};

int main()
{
	freopen("in.txt", "r", stdin);
	int t;
	cout << "������ҳ���ڴ��С:";
	cin >> t;
	cout << t << endl;
	LRU lru(t);
	FIFO fifo(t);
	cout << endl << "������Ҫ��ѯ��ֵ:";
	while (cin >> t&&t != -1)
	{
		cout << t << endl << "LRU:" << endl;
		if (lru.find(t))
			cout << t << " ��ҳ���б�����\t";
		else
			cout << t << " ��ҳ����δ����\t";
		cout << "��ǰ�ܹ���ѯ���� " << lru.getCnt() << " ȱҳ���� " << lru.getMissCnt()
			<< " ȱҳ�� ";
		cout.precision(3);
		cout<<lru.getMissPercent()*100<<"%" << endl;
		cout << endl << "FIFO:" << endl;
		if (fifo.find(t))
			cout << t << " ��ҳ���б�����\t";
		else
			cout << t << " ��ҳ����δ����\t";
		cout << "��ǰ�ܹ���ѯ���� " << fifo.getCnt() << " ȱҳ���� " << fifo.getMissCnt()
			<< " ȱҳ�� ";
		cout.precision(3);
		cout << fifo.getMissPercent() * 100 << "%" << endl;
		cout << endl << "������Ҫ��ѯ��ֵ:";
	}
	return 0;
}