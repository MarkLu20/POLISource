// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Map.h"
#include "SharedPointer.h"
#include "GridLayer.h"
#include "GridPalette.h"
#include "GridManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHSECURITYSYS_API UGridManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blueprints)
		int Width;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blueprints)
		int Height;
public:
	// Sets default values for this component's properties
	UGridManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void Initialize(int _width, int _height);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		UGridLayer* NewLayer();

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		UGridPalette* NewPalette();
	
};
