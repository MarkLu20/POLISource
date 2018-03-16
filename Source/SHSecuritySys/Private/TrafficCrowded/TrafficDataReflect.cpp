// Fill out your copyright notice in the Description page of Project Settings.

#include "TrafficDataReflect.h"
#include "Json.h"

// Sets default values
ATrafficDataReflect::ATrafficDataReflect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrafficDataReflect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrafficDataReflect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATrafficDataReflect::SetData(TArray<FTrafficCrowdedMeshData> data)
{
	trafficData = data;
}

void ATrafficDataReflect::InitData()
{
	
	FString _path =  FPaths::GameContentDir() + TEXT("BluepirntLogic/Sys_Modules/TrafficDataReflect/Data/Data.json");
	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	TArray<TSharedPtr<FJsonValue>> dataArray = JsonObject->GetArrayField("rows");
	if (BFlag)
	{
		//,*(JsonObject->GetArrayField("rows").Num)
		//UE_LOG(LOGTest,Warning, TEXT("Test: %s"),*_path);
		trafficData.Empty();
		for (int i = 0; i < dataArray.Num(); i++)
		{
			TSharedPtr<FJsonObject> objectTemp = dataArray[i]->AsObject();
			FTrafficCrowdedMeshData ftdTemp;
			//获取xy数据
			FString xy = objectTemp->GetStringField("xy");
			TArray<FString> strVec2_xy;
			xy.ParseIntoArray(strVec2_xy, TEXT(","), true);
			FVector2D vect2_xy;
			vect2_xy.X = FCString::Atof(*strVec2_xy[0]);
			vect2_xy.Y = FCString::Atof(*strVec2_xy[1]);

			ftdTemp.xy = vect2_xy;

			//获取xys数据
			FString xys = objectTemp->GetStringField("xys");
			TArray<FString> strArray;
			xys.ParseIntoArray(strArray, TEXT(";"), true);
		
			for (int j = 0; j < strArray.Num(); j++)
			{
				//字符串转成二维向量
				TArray<FString> strVec2;
				strArray[j].ParseIntoArray(strVec2, TEXT(","), true);
				FVector2D vect2;
				vect2.X = FCString::Atof(*strVec2[0]);
				vect2.Y = FCString::Atof(*strVec2[1]);
				ftdTemp.xysArray.Add(vect2);

			}
			trafficData.Add(ftdTemp);

		}
	}
}

