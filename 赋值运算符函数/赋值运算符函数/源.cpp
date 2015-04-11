#include <iostream>
using namespace std;

class CMyString
{
public:
	CMyString(char*pData = NULL);
	CMyString(const CMyString &str);
	CMyString & operator=(const CMyString &str);
	~CMyString(void);
private:
	char* m_pData;
};

//����Ľⷨ�������ڳ�������Ա

/*
CMyString & CMyString::operator(const CMyString &str)
{
	if(this==&str)
		return *this;
	delete []m_pData;
	m_pData=NULL;
	m_pData=new char[strlen(str.m_pData)+1];
	strcpy(m_pData,str.m_pData);

	return *this;
*/



//�����쳣��ȫ�ԵĽⷨ���߼�����Ա�ر�
CMyString & CMyString::operator=(const CMyString &str)
{
	if (this != &str)
	{
		CMyString tmpStr(str);

		char* mTemp = tmpStr.m_pData;
		tmpStr.m_pData = this->m_pData;
		this->m_pData = mTemp;
	}
	return *this;
}