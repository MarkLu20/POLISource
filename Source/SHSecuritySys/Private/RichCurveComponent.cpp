// Fill out your copyright notice in the Description page of Project Settings.

#include "RichCurveComponent.h"


// Sets default values for this component's properties
URichCurveComponent::URichCurveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URichCurveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URichCurveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URichCurveComponent::AddKey(float x, float y) {
	auto handle=curve.AddKey(x, y);
	curve.SetKeyInterpMode(handle, RCIM_Cubic);
}

float URichCurveComponent::GetYByX(float x) {
	return curve.Eval(x);
}

