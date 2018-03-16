// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sys_SaveLoadBinary.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHSECURITYSYS_API USys_SaveLoadBinary : public UActorComponent
{
	GENERATED_BODY()

private:
	template <class T>
	void SaveLoadData(FArchive& Ar, T& data)
	{
		Ar << data;
	};

public:
	UFUNCTION(BlueprintCallable, Category = SaveLoad)
		bool SaveData(float data, FString saveName);

	UFUNCTION(BlueprintCallable, Category = SaveLoad)
		bool LoadData(float& data, FString saveName);

public:	
	// Sets default values for this component's properties
	USys_SaveLoadBinary();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
