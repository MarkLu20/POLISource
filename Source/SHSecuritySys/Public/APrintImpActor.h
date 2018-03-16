// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EditDataHandle.h"
#include "APrintImpActor.generated.h"

UCLASS()
class SHSECURITYSYS_API AAPrintImpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAPrintImpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "ExhConfig")
		TArray<FExh_Editor_MemberInfo> GetMemberInfoByTeamName(int teamIndx, const FString&  teamName);
	
};
