// TestConnectOracle.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ocilib.h"
//错误处理
void err_handler(OCI_Error *err)
{
	int   err_type = OCI_ErrorGetType(err);
	char *err_msg = (char*)OCI_ErrorGetString(err);

	printf("%s - %s\n", err_type == OCI_ERR_WARNING ? "warning" : "error", err_msg);
}

int main()
{
    std::cout << "Hello World!\n"; int nRet = 0;
	OCI_Connection* cn = NULL;
	OCI_Statement* st = NULL;
	OCI_Resultset* rs = NULL;

	//初始化OCI库
	nRet = OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);
	if (!nRet)
	{
		std::cout << "OCI_Initialize fail" << std::endl;
		return -1;
	}

	do
	{
		std::cout << "compile version:" << OCI_GetOCICompileVersion() << std::endl;

		std::cout << "runtime version:" << OCI_GetOCIRuntimeVersion() << std::endl;

		//建立联接:tns:cctt1;username:d10086;password:d10086

		

		cn = OCI_ConnectionCreate(L"orcl", L"WdxjPR", L"1234", OCI_SESSION_DEFAULT);
		if (NULL == cn)
		{
			std::cout << "OCI_ConnectionCreate fail" << std::endl;
			err_handler(OCI_GetLastError());
			nRet = -2;
			break;
		}

		printf("Server major    version : %i\n", OCI_GetServerMajorVersion(cn));
		printf("Server minor    version : %i\n", OCI_GetServerMinorVersion(cn));
		printf("Server revision version : %i\n\n", OCI_GetServerRevisionVersion(cn));
		printf("Connection     version : %i\n\n", OCI_GetVersionConnection(cn));

		//建立声明
		st = OCI_StatementCreate(cn);
		if (NULL == st)
		{
			std::cout << "OCI_StatementCreate fail" << std::endl;
			err_handler(OCI_GetLastError());
			nRet = -3;
			break;
		}

		//绑定变量
		const	char *value = "1";
		//准备SQL语句，用 :value 占位符占位
		if (!OCI_Prepare(st, L"select t.* from BIZTBCOMMANDMANAGE t where code like '%'||:value||'%'"))
		{
			std::cout << "OCI_Prepare fail" << std::endl;
			err_handler(OCI_GetLastError());
			nRet = -4;
			break;
		}
		//绑定变量，用变量替换占位符
		if (!OCI_BindString(st, L":value",(otext *) value, strlen(value)))
		{
			std::cout << "OCI_BindString fail" << std::endl;
			err_handler(OCI_GetLastError());
			nRet = -5;
			break;
		}
		//执行声明
		if (!OCI_Execute(st))
		{
			std::cout << "OCI_Execute fail" << std::endl;
			err_handler(OCI_GetLastError());
			nRet = -6;
			break;
		}



		//得到返回值集
		rs = OCI_GetResultset(st);
		if (NULL == rs)
		{
			std::cout << "OCI_GetResultset fail" << std::endl;
			err_handler(OCI_GetLastError());
			nRet = -5;
			break;
		}

		//枚举返回值集
		while (OCI_FetchNext(rs))
		{
			printf("%i - %s\n", OCI_GetString(rs, 1), OCI_GetString(rs, 2));
		}
	} while (0);

	//释放返回集
	if (rs)
		OCI_ReleaseResultsets(st);

	//清除声明
	if (st)
		OCI_StatementFree(st);

	//清除连接
	if (cn)
		OCI_ConnectionFree(cn);

	//清除OCI库
	OCI_Cleanup();

	//return EXIT_SUCCESS;
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
