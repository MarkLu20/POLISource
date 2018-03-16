// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Classes/Curves/RichCurve.h"
#include "RichCurveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHSECURITYSYS_API URichCurveComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	FRichCurve curve;
public:	
	// Sets default values for this component's properties
	URichCurveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddKey(float x, float y);
		
	UFUNCTION(BlueprintCallable)
	float GetYByX(float x);

	
};
