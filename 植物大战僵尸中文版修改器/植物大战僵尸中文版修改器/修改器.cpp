#include<stdio.h>

#include<Windows.h>

int main()
{
    //����
	//1.������Ϸ
	//spy++
	//���ھ��������ID,�൱�����֤����
	HWND hGameWnd = FindWindow(L"MainWindow",L"ֲ���ս��ʬ���İ�");

	if (hGameWnd == NULL)
	{
		printf("û�����и���Ϸ��\n");
		return 0;
	}


	//2.���ݴ��ھ����ȡ���̵�PID
	DWORD dwPID = 0;
	GetWindowThreadProcessId(hGameWnd, &dwPID);

	if (dwPID == 0)
	{
		printf("��ȡ����PIDʧ�ܣ�������룺%d\n",GetLastError());
		return 0;
	}


	//3.����PID��ȡ���̾��
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hProcess == NULL)
	{
		printf("�򿪽���ʧ��!������룺%d\n", GetLastError());
		return 0;
	}

	//4.��ָ�������ڴ������ȡ����
	DWORD dwSunshineBaseAddress = 0x006A9EC0;
	DWORD dwSunshineAddressValue = 0;
	DWORD dwSize = 0;
	if (FALSE == ReadProcessMemory(
		hProcess,                          //��һ������
		(LPVOID)dwSunshineBaseAddress,     //Ҫ��ȡ�Ļ���ַ
		&dwSunshineAddressValue,           //���ܽ��̵�ַ����
		sizeof(DWORD),                      //Ҫ��ȡ�Ľ����ֽ���
		& dwSize
	))
	{
		printf("��ȡ��̬��ַʧ��!������룺%d\n", GetLastError());
		return 0;
	}
	
	//5.��ȡһ��ƫ�Ƶ�ַ
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
		printf("��ȡһ��ƫ�Ƶ�ַʧ�ܣ�������룺%d\n", GetLastError());
		return 0;
	}
	//6.����ƫ��
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
		printf("��ȡ����ƫ�Ƶ�ַʧ�ܣ�������룺%d\n", GetLastError());
		return 0;
	}

	printf("����ֵ��%d\n",dwSunshine);
	printf("������Ҫ�޸ĵ�����ֵ:\n");
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