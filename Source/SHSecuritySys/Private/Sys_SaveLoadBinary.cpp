// Fill out your copyright notice in the Description page of Project Settings.

#include "Sys_SaveLoadBinary.h"
#include "BufferArchive.h"
#include "FileHelper.h"
#include "MemoryReader.h"


// Sets default values for this component's properties
USys_SaveLoadBinary::USys_SaveLoadBinary()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USys_SaveLoadBinary::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USys_SaveLoadBinary::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USys_SaveLoadBinary::SaveData(float data,FString saveName)
{
	//Save the data to binary
	FBufferArchive ToBinary;
	SaveLoadData<float>(ToBinary,data);

	//No data were saved
	if (ToBinary.Num() <= 0) return false;

	//Save binaries to disk
	bool result = FFileHelper::SaveArrayToFile(ToBinary, *saveName);

	//Empty the buffer's contents
	ToBinary.FlushCache();
	ToBinary.Empty();

	return result;
}

bool USys_SaveLoadBinary::LoadData(float& data, FString saveName)
{
	TArray<uint8> BinaryArray;

	//load disk data to binary array
	if (!FFileHelper::LoadFileToArray(BinaryArray, *saveName)) return false;

	if (BinaryArray.Num() <= 0) return false;

	//Memory reader is the archive that we're going to use in order to read the loaded data
	FMemoryReader FromBinary = FMemoryReader(BinaryArray, true);
	FromBinary.Seek(0);

	SaveLoadData<float>(FromBinary, data);

	//Empty the buffer's contents
	FromBinary.FlushCache();
	BinaryArray.Empty();
	//Close the stream
	FromBinary.Close();

	return true;
}