// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficDataReflect.generated.h"

UCLASS()
class SHSECURITYSYS_API ATrafficDataReflect : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrafficDataReflec)
		TArray<FTrafficCrowdedMeshData> trafficData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrafficDataReflec)
		FTrafficCrowdedRoad roadData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrafficDataReflec)
		FVector2D cameraDirNor;
public:	
	// Sets default values for this actor's properties
	ATrafficDataReflect();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "TrafficCrowded")
		void InitData();

	UFUNCTION(BlueprintCallable, Category = "TrafficCrowded")
		void SetData(TArray<FTrafficCrowdedMeshData> data);

	
	
};
