#include <stdlib.h>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>
#include <sqltypes.h>

#define Name_Len 20
#define Pwd_Len 10

SQLCHAR ID[Name_Len];
SQLCHAR PWD[Pwd_Len];

SQLINTEGER userID = SQL_NTS, userName = SQL_NTS, userPassword = SQL_NTS;


int main()
{
	//������
	SQLHENV serverhenv;
	SQLHDBC serverhdbc;
	SQLHSTMT serverstmt;
	SQLRETURN ret;

	//��ʼ������
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &serverhenv);
	ret = SQLSetEnvAttr(serverhenv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	if (ret == SQL_SUCCESS)
	{
		printf(" ------------------------ - \n");
		printf(" ��������ɹ�\n");
		printf(" ------------------------ - \n");
	}

	//��������
	ret = SQLAllocHandle(SQL_HANDLE_DBC, serverhenv, &serverhdbc);
	ret = SQLConnectW(serverhdbc, (SQLWCHAR *)"David_MySQL" , SQL_NTS, (SQLWCHAR*)"root" , SQL_NTS, (SQLWCHAR*)"Dyc19930418", SQL_NTS);
	if (!SQL_SUCCEEDED(ret))
	{
		printf(" ��������ʧ��\n");
		return -1;
	}

	//��ʼ�������

	ret = SQLAllocHandle(SQL_HANDLE_STMT, serverhdbc, &serverstmt);
	ret = SQLExecDirect(serverstmt, (SQLWCHAR*)" select * from my_test" , SQL_NTS);

	//��������

	ret = SQLBindCol(serverstmt, 1, SQL_C_CHAR, ID, Name_Len, &userID);
	ret = SQLBindCol(serverstmt, 2, SQL_C_CHAR, PWD, Pwd_Len, &userPassword);

	printf(" userID\t userPassword\t\n");
	while ((ret = SQLFetch(serverstmt)) != SQL_NO_DATA_FOUND)
	{
		if (ret == SQL_ERROR)
		{
			printf(" Fetch data error!\n");
		}
		else
		{

			printf(" %s\t %s\t\n" , ID, PWD);
		}
	}

	//��ֹ����
	SQLFreeHandle(SQL_HANDLE_STMT, serverstmt);
	SQLDisconnect(serverhdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, serverhdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, serverhenv);
	printf(" ---------------------------- - \n");
	return 0;
}

