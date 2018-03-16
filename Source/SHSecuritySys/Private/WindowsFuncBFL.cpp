// Fill out your copyright notice in the Description page of Project Settings.

#include "WindowsFuncBFL.h"

#include <iostream>
#include <windows.h>
#include "shellapi.h"



//打开文件夹
void UWindowsFuncBFL::OpenFolder(const FString &_Path)
{
	ShellExecute(NULL, L"open", *_Path, NULL, NULL, SW_SHOW);
	//ShellExecute(NULL, L"explore", L"D:\\", NULL, NULL, SW_SHOW);
}
