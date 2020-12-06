#include<stdio.h>

#include<Windows.h>

int main()
{
    //步骤
	//1.查找游戏
	//spy++
	//窗口句柄：窗口ID,相当于身份证号码
	HWND hGameWnd = FindWindow(L"MainWindow",L"植物大战僵尸中文版");

	if (hGameWnd == NULL)
	{
		printf("没有运行该游戏！\n");
		return 0;
	}


	//2.根据窗口句柄获取进程的PID
	DWORD dwPID = 0;
	GetWindowThreadProcessId(hGameWnd, &dwPID);

	if (dwPID == 0)
	{
		printf("获取进程PID失败！错误代码：%d\n",GetLastError());
		return 0;
	}


	//3.根据PID获取进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hProcess == NULL)
	{
		printf("打开进程失败!错误代码：%d\n", GetLastError());
		return 0;
	}

	//4.从指定进程内存区域读取数据
	DWORD dwSunshineBaseAddress = 0x006A9EC0;
	DWORD dwSunshineAddressValue = 0;
	DWORD dwSize = 0;
	if (FALSE == ReadProcessMemory(
		hProcess,                          //哪一个进程
		(LPVOID)dwSunshineBaseAddress,     //要读取的基地址
		&dwSunshineAddressValue,           //接受进程地址数据
		sizeof(DWORD),                      //要读取的进程字节数
		& dwSize
	))
	{
		printf("获取静态地址失败!错误代码：%d\n", GetLastError());
		return 0;
	}
	
	//5.获取一级偏移地址
	DWORD dwSunshineOffsetFirst = 0x768;
	DWORD dwSunshineOffsetValue = 0;
	if (FALSE == ReadProcessMemory(
		hProcess,
		(LPVOID)(dwSunshineAddressValue + dwSunshineOffsetFirst),
		&dwSunshineOffsetValue,
		sizeof(DWORD),
		&dwSize
	))
	{
		printf("获取一级偏移地址失败！错误代码：%d\n", GetLastError());
		return 0;
	}
	//6.二级偏移
	DWORD dwSunshineOffsetSecond = 0x5560;
	DWORD dwSunshine = 0;
	if (FALSE == ReadProcessMemory(
		hProcess,
		(LPVOID)(dwSunshineOffsetValue + dwSunshineOffsetSecond),
		&dwSunshine,
		sizeof(DWORD),
		&dwSize
	))
	{
		printf("获取二级偏移地址失败！错误代码：%d\n", GetLastError());
		return 0;
	}

	printf("阳光值：%d\n",dwSunshine);
	printf("请输入要修改的阳光值:\n");
	int nModifySunshine;
	scanf_s("%d", &nModifySunshine);
	WriteProcessMemory(
		hProcess,
		(LPVOID)(dwSunshineOffsetValue + dwSunshineOffsetSecond),
		&nModifySunshine,
		sizeof(DWORD),
		&dwSize
	);
	CloseHandle(hProcess);
	return 0;
}