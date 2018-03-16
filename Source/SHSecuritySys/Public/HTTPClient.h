// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Http.h"
#include "ProjectJsonData.h"
#include "Sys_PC.h"
#include "Runnable.h"
#include "RunnableThread.h"
#include "FileDictOperationCollection.h"
#include "HTTPClient.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHSECURITYSYS_API UHTTPClient : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHTTPClient();

public:
	FHttpModule* Http;

	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void MyHttpCall(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void MyHttpWifiCall(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseWifiReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void Http110PhoneSearchCall(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponse110PhoneSearchReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void Http110KeywordSearchCall(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponse110KeywordSearchReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void HttpWarningPeopleInit(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseWarningPeopleInitReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void HttpWarningPeopleNew(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseWarningPeopleNewReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);



	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void HttpArea110Info(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponse110AreaInfoReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	//Traffic----->
	/* The actual HTTP call */
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
	void HttpTrafficInfo(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseTrafficInfoReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	//Weather---->
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
	void HttpTWeatherInfo(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseWeatherInfoReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	//KaKou--->
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
	void HttpKaKouInfo(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseKaKouInfoReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	//WifiData--->
	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void HttpPeopleWifiInfo(FString url);

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponsePeopleWifiReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SysFunc")
		ASys_PC* pc;


	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void ParseSiMiaoData(FString jsonData);

	UFUNCTION(BlueprintCallable, Category = "SysFunc")
		void Parse110Data(FString jsonData);
};


class ParseSiMiaoDataAsyncTask :public FNonAbandonableTask
{
	FString data;
	ASys_PC* pc;
public:
	ParseSiMiaoDataAsyncTask(FString jsonContent,ASys_PC* _pc)
	{
		this->data = jsonContent;
		this->pc = _pc;
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(ParseSiMiaoDataAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork()
	{
		pc->Callback_ReturnHongWaiData(UFileDictOperationCollection::ReadHongWaiData(data));	
		//GLOG->Log("Do work!!!");
	}
};

class Parse110DataAsyncTask :public FNonAbandonableTask
{
	FString data;
	ASys_PC* pc;
public:
	Parse110DataAsyncTask(FString jsonContent, ASys_PC* _pc)
	{
		this->data = jsonContent;
		this->pc = _pc;
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(Parse110DataAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork()
	{
		FSys_Warning110Array data2 = UFileDictOperationCollection::Parse110Data(data);
		pc->Callback_Return110Data(data2);
		//pc->recieved110warningMessage();
		//pc->recieved110warningMessage(this.);
		//GLOG->Log("Do work!!!");
	}
};