// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FileDictOperationCollection.h"
#include "Http.h"
#include "HttpJsonComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHSECURITYSYS_API UHttpJsonComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHttpJsonComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Init(FString url, float deltaTime);
	
	UFUNCTION(BlueprintCallable)
	void Start();

	UFUNCTION(BlueprintImplementableEvent)
	void recievedTrafficStatusJson(const FSys_TrafficStatus& trafficStatus);

	void FetchURL();

	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

private:
	float dTime = 999.0f;
	FString URL = "";
	bool isStart = false;

	float tnowTime=0.0f;
};
