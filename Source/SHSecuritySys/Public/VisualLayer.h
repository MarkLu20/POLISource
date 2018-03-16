// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VisualLayer.generated.h"


UENUM(BlueprintType)
enum class EExhBrushType : uint8 {
	Rect = 0,
	Circle,
	Count UMETA(Hidden)
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHSECURITYSYS_API UVisualLayer : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blueprints)
		class UTexture2D* Texture;
public:
	// Sets default values for this component's properties
	UVisualLayer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void Initialize(int _width, int _height);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void SetColor(int x, int y, FLinearColor color);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void PaintRect(int x, int y, int brush_size, FLinearColor color);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void PaintCircle(int x, int y, int brush_size, FLinearColor color);
		
	
};
