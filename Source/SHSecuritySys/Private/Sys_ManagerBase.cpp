// Fill out your copyright notice in the Description page of Project Settings.

#include "Sys_ManagerBase.h"


// Sets default values
ASys_ManagerBase::ASys_ManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASys_ManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASys_ManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

