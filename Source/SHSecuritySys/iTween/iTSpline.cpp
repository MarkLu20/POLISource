// Fill out your copyright notice in the Description page of Project Settings.

#include "iTSpline.h"
#include "SHSecuritySys.h"
#include "iTweenPCH.h"


AiTSpline::AiTSpline(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	spline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("Spline"));

	RootComponent = spline;
}


