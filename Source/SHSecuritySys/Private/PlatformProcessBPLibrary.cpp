// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformProcessBPLibrary.h"
//#include <Windows.h>
#include "PlatformProcess.h"

UPlatformProcessBPLibrary::UPlatformProcessBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPlatformProcessBPLibrary::OpenFile(const FString& URL)
{
	FPlatformProcess::LaunchFileInDefaultExternalApplication(*URL);
	//ShellExecute(NULL, _T("open"), *URL, NULL, NULL, SW_SHOW);
}
     
