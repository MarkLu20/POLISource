// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WinPrinterActorComponent.generated.h"

enum class EPrintScreenState : uint8 {
	kIdle = 0,
	kPrepare
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHSECURITYSYS_API UWinPrinterActorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	EPrintScreenState PrintScreenState;
	FString PrintScreenPath;
	ULONG_PTR gdiplusToken;

public:
	// Sets default values for this component's properties
	UWinPrinterActorComponent();
	~UWinPrinterActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void PrintScreenCheck();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, Category = "ExhSH")
		void PrintScreen(FString path);

	UFUNCTION(BlueprintCallable, Category = "ExhSH")
		void PrintItem(FString content);

	void GetDefaultPrinterMy(FString& csPrinter);

};
