#include "stdafx.h"



DWORD WINAPI ThreadProc(LPVOID pParam)
{
	std::cout << "test";
}

int main()
{
	DWORD threadId;
	auto handle = CreateThread(NULL, NULL, ThreadProc, 0, 0, &threadId)
}
