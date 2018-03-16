// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FileDictOperationCollection.h"
#include "Sys_PC.generated.h"

/**
 * 
 */
UCLASS()
class SHSECURITYSYS_API ASys_PC : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recieved110warningMessage(const FSys_Warning110Array &arrayData);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recievedWifiBoxMessage(const FSys_WifiBoxArray &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recieved110PhoneSearchResultMessage(const FSys_Warning110Array &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recieved110KeywordResultMessage(const FSys_Warning110Array &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recieved110AreaInfoMsg(const FSys_110AreaDataArray &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recievedWarningPeopleInit(const FSys_WarningPeopleArray &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recievedWarningPeopleNew(const FSys_WarningPeopleArray &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recievedTrafficData(const FSys_TrafficStatus &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recievedWeatherData(const FSys_WeartherData &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recievedKaKouData(const FSys_KaKouStatus &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recievedPeopleWifiData(const FSys_PeopleWifiData &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void recievedPoliceGPSData(const FSys_PoliceGPSArray &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void Callback_ReturnHongWaiData(const FHongWaiData &arrayData);

	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void Callback_Return110Data(const FSys_Warning110Array &arrayData);
};
