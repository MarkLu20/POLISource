// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MatrixFuncLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SHSECURITYSYS_API UMatrixFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Math|Matrix")
		static FMatrix ProjectionMatrix(float InWidth, float InHeight, float InFOV, float InMinZ);
	
	UFUNCTION(BlueprintPure, Category = "Math|Matrix")
		static FMatrix ViewMatrix(const FVector& InViewLocation, const FRotator& InViewRotation);

	UFUNCTION(BlueprintPure, Category = "Math|Matrix")
		static FMatrix Multiply(const FMatrix& InLeft, const FMatrix& InRight);

	UFUNCTION(BlueprintPure, Category = "Math|Matrix")
		static FMatrix Transpose(const FMatrix& InMatrix, bool InTranspose = true);
};
