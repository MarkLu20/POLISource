// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "MKGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class SHSECURITYSYS_API UMKGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	virtual bool RequiresHitProxyStorage() override { return 1; }
	
	
};
