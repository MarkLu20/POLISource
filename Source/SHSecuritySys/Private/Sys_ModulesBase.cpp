// Fill out your copyright notice in the Description page of Project Settings.

#include "Sys_ModulesBase.h"


// Sets default values
ASys_ModulesBase::ASys_ModulesBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASys_ModulesBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASys_ModulesBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

