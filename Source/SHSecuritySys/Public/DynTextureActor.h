// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynTextureActor.generated.h"

UCLASS()
class SHSECURITYSYS_API ADynTextureActor : public AActor
{
	GENERATED_BODY()
	
	struct FPixelData 
	{
		UINT8 B;
		UINT8 G;
		UINT8 R;
		UINT8 A;
	};
	
public:	
	// Sets default values for this actor's properties
	ADynTextureActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Blueprints)
		class UTexture2D* pTexture;

protected:
	int32 LineWidth;
	FRichCurve* pRichCurve;
	FColor	CurveColor;
	FColor  CurveAreaColor;
	ERichCurveInterpMode CurveInterpMode;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		UTexture2D*		CreateDynTexture(int32 x, int32 y);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void			UpdateDyntexture(UTexture2D* pDynTexture, TArray<float> values, float totalTime);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void			SetCurveMode(int32 lineWidth, FColor curveColor, float fillWeight, ERichCurveInterpMode curveMode);

	UFUNCTION(BlueprintCallable, Category = Blueprints)
		void			SetCurvesValue(TArray<float> timeAry, TArray<float> valueAry);
};
