#include <iostream>
#include <Windows.h>
using namespace std;

BOOL adjusttoken()
{
	HANDLE htoken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &htoken))
	{
		size_t           s = sizeof(TOKEN_PRIVILEGES) + 2 * sizeof(LUID_AND_ATTRIBUTES);
		TOKEN_PRIVILEGES* p = (PTOKEN_PRIVILEGES)malloc(s);

		if (!LookupPrivilegeValue(NULL, SE_SYSTEM_ENVIRONMENT_NAME, &(p->Privileges[0].Luid)) ||
			!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &(p->Privileges[1].Luid)) ||
			!LookupPrivilegeValue(NULL, SE_RESTORE_NAME, &(p->Privileges[2].Luid)))
		{
			printf("failed to LookupPrivilegeValue! Error code: %d\r\n", GetLastError());
			printf("LookupPrivilegeValue 失败！错误代码：%d\r\n", GetLastError());
			printf("\r\n");

			free(p);
			return FALSE;
		}
		p->PrivilegeCount = 3;

		for (int i = 0; i < 3; ++i)
		{
			p->Privileges[i].Attributes = SE_PRIVILEGE_ENABLED;
		}

		if (!AdjustTokenPrivileges(htoken, FALSE, p, s, NULL, NULL) || GetLastError() != ERROR_SUCCESS)
		{
			printf("AdjustTokenPrivileges failed! Error code: %d\r\n", GetLastError());
			printf("AdjustTokenPrivileges 失败！错误代码：%d\r\n", GetLastError());
			printf("\r\n");

			free(p);
			return FALSE;
		}
		free(p);
	}
	else
	{
		printf("Open process token failed! Error code: %d\r\n", GetLastError());
		printf("打开进程令牌失败！错误代码：%d\r\n", GetLastError());
		printf("\r\n");
		return FALSE;
	}

	return TRUE;
}

void unlock()
{
	UCHAR c = 1;
	BOOL b = SetFirmwareEnvironmentVariableA("AmiSetupFormSetVar", "{E102C8AA-0F10-4A0E-90A7-A6F816089BB7}", &c, 1);
	if (b)
	{
		cout << "Unlock BIOS Hidden Menu Success!" << endl;
		cout << "成功解锁BIOS隐藏菜单！" << endl;
	}
	else
	{
		cout << "Unlock BIOS Hidden Menu Failed! Error Code: " << GetLastError() << endl;
		cout << "解锁BIOS隐藏菜单失败！错误代码：" << GetLastError() << endl;
	}
}

void lock()
{
	UCHAR c = 0;
	BOOL b = SetFirmwareEnvironmentVariableA("AmiSetupFormSetVar", "{E102C8AA-0F10-4A0E-90A7-A6F816089BB7}", &c, 1);
	if (b)
	{
		cout << "Lock BIOS Hidden Menu Success!" << endl;
		cout << "成功锁定BIOS隐藏菜单！" << endl;
	}
	else
	{
		cout << "Lock BIOS Hidden Menu Failed! Error Code: " << GetLastError() << endl;
		cout << "锁定BIOS隐藏菜单失败！错误代码：" << GetLastError() << endl;
	}
}

UCHAR buffer[0x1000] = { 0 };

int main()
{

	if (adjusttoken()) {
		cout << "Get Privilege Success!" << endl;
		cout << "已获得管理员权限！" << endl;
		system("cls");
	} else {
		cout << "Get Privilege Failed! Please run as Administrator!" << endl;
		cout << "获取管理员权限失败！请以管理员身份运行！" << endl << endl;
		cout << endl;

		system("pause");
		return 0;
	}

	cout << "Lenovo Legion T5 26AMR5 BIOS Hidden Menu Tool" << endl;
	cout << "联想拯救者刃7000P 2021 BIOS隐藏菜单工具" << endl;
	cout << "------------------------------------------------------------------------" << endl << endl;
	cout << "[1] Unlock Hidden Menu 解锁隐藏菜单" << endl;
	cout << "[2] Lock Hidden Menu 锁定隐藏菜单" << endl << endl;
	cout << "Choose an option 选择需要进行的操作: ";
	int mode;
	cin >> mode;
	switch (mode) {
	case 1:
		unlock();
		break;
	case 2:
		lock();
		break;
	default:
		printf("Please input correct number. 请输入正确数字。\r\n");
	}
	
	system("pause");
	return 0;
}