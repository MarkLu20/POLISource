// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectJsonData.h"
#include "Json.h"

// Sets default values for this component's properties
UProjectJsonData::UProjectJsonData()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProjectJsonData::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UProjectJsonData::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FSys_PoliceTeam UProjectJsonData::ReadPoliceTeamInfo(const FString _path)
{
	FSys_PoliceTeam polices;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		polices.TeamName = JsonObject->GetStringField("TeamName");
		polices.TeamCount = FCString::Atoi(*(JsonObject->GetStringField("TeamCount")));
		polices.TeamLeader = JsonObject->GetStringField("TeamLeader");
		polices.TeamLeaderPhone = JsonObject->GetStringField("TeamLeaderPhone");
		polices.TeamCall = JsonObject->GetStringField("TeamCall");
		polices.TeamColor = JsonObject->GetStringField("TeamColor");
		polices.TeamLeaderPhone = JsonObject->GetStringField("TeamLeaderPhone");
		polices.TeamFlagPosX = FCString::Atof(*(JsonObject->GetStringField("TeamFlagPosX")));
		polices.TeamFlagPosY = FCString::Atof(*(JsonObject->GetStringField("TeamFlagPosY")));
		polices.TeamFlagPosZ = FCString::Atof(*(JsonObject->GetStringField("TeamFlagPosZ")));

		//Add Unit Team-->
		TArray<FSys_UnitPolice> parts;
		TArray<TSharedPtr<FJsonValue>> police_array = JsonObject->GetArrayField("UnitsArray");
		if (police_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> PoliceObject;
			for (int i = 0; i < police_array.Num(); i++)
			{
				PoliceObject = police_array[i]->AsObject();

				FSys_UnitPolice part;
				part.UnitName = PoliceObject->GetStringField("UnitName");
				part.UnitCount = FCString::Atoi(*(PoliceObject->GetStringField("UnitCount")));
				part.UnitLeader = PoliceObject->GetStringField("UnitLeader");
				part.UnitLeaderPhone = PoliceObject->GetStringField("UnitLeaderPhone");
				part.UnitCall = PoliceObject->GetStringField("UnitCall");
				part.UnitColor = PoliceObject->GetStringField("UnitColor");
				part.Floor = FCString::Atoi(*(PoliceObject->GetStringField("Floor")));
				part.UnitFlagPosX = FCString::Atof(*(PoliceObject->GetStringField("UnitFlagPosX")));
				part.UnitFlagPosY = FCString::Atof(*(PoliceObject->GetStringField("UnitFlagPosY")));
				part.UnitFlagPosZ = FCString::Atof(*(PoliceObject->GetStringField("UnitFlagPosZ")));

				//Add Member£º--->
				TArray<FSys_UnitMember> members;
				TArray<TSharedPtr<FJsonValue>> memeber_array = PoliceObject->GetArrayField("MembersArray");
				if (memeber_array.Num() > 0)
				{
					TSharedPtr<FJsonObject> memeberObject;
					for (int i = 0; i < memeber_array.Num(); i++)
					{
						memeberObject = memeber_array[i]->AsObject();

						FSys_UnitMember member;

						member.MemberName = memeberObject->GetStringField("MemberName");
						member.MemberCount = memeberObject->GetStringField("MemberCount");

						members.Add(member);
					}
				}
				part.MembersArray = members;

				parts.Add(part);
			}
		}
		polices.UnitsArray = parts;
	}

	return polices;
}

void UProjectJsonData::WritePoliceTeamInfo(FString _path, FSys_PoliceTeam policeTeamInfo)
{

	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteValue(TEXT("TeamName"), policeTeamInfo.TeamName);
	JsonWriter->WriteValue(TEXT("TeamCount"), policeTeamInfo.TeamCount);
	JsonWriter->WriteValue(TEXT("TeamLeader"), policeTeamInfo.TeamLeader);
	JsonWriter->WriteValue(TEXT("TeamLeaderPhone"), policeTeamInfo.TeamLeaderPhone);
	JsonWriter->WriteValue(TEXT("TeamCall"), policeTeamInfo.TeamCall);
	JsonWriter->WriteValue(TEXT("TeamColor"), policeTeamInfo.TeamColor);
	JsonWriter->WriteValue(TEXT("TeamFlagPosX"), policeTeamInfo.TeamFlagPosX);
	JsonWriter->WriteValue(TEXT("TeamFlagPosY"), policeTeamInfo.TeamFlagPosY);
	JsonWriter->WriteValue(TEXT("TeamFlagPosZ"), policeTeamInfo.TeamFlagPosZ);

	JsonWriter->WriteArrayStart("UnitsArray");
	for (int i = 0; i < policeTeamInfo.UnitsArray.Num(); i++)
	{
		JsonWriter->WriteObjectStart();

		JsonWriter->WriteValue(TEXT("UnitName"), policeTeamInfo.UnitsArray[i].UnitName);
		JsonWriter->WriteValue(TEXT("UnitCount"), policeTeamInfo.UnitsArray[i].UnitCount);
		JsonWriter->WriteValue(TEXT("UnitLeader"), policeTeamInfo.UnitsArray[i].UnitLeader);
		JsonWriter->WriteValue(TEXT("UnitLeaderPhone"), policeTeamInfo.UnitsArray[i].UnitLeaderPhone);
		JsonWriter->WriteValue(TEXT("UnitCall"), policeTeamInfo.UnitsArray[i].UnitCall);
		JsonWriter->WriteValue(TEXT("UnitColor"), policeTeamInfo.UnitsArray[i].UnitColor);
		JsonWriter->WriteValue(TEXT("Floor"), policeTeamInfo.UnitsArray[i].Floor);
		JsonWriter->WriteValue(TEXT("UnitFlagPosX"), policeTeamInfo.UnitsArray[i].UnitFlagPosX);
		JsonWriter->WriteValue(TEXT("UnitFlagPosY"), policeTeamInfo.UnitsArray[i].UnitFlagPosY);
		JsonWriter->WriteValue(TEXT("UnitFlagPosZ"), policeTeamInfo.UnitsArray[i].UnitFlagPosZ);

		JsonWriter->WriteArrayStart("MembersArray");
		for (int j = 0; j < policeTeamInfo.UnitsArray[i].MembersArray.Num(); j++)
		{
			JsonWriter->WriteObjectStart();

			JsonWriter->WriteValue(TEXT("MemberName"), policeTeamInfo.UnitsArray[i].MembersArray[j].MemberName);
			JsonWriter->WriteValue(TEXT("MemberCount"), policeTeamInfo.UnitsArray[i].MembersArray[j].MemberCount);

			JsonWriter->WriteObjectEnd();
		}
		JsonWriter->WriteArrayEnd();

		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);

}



FSys_PoliceVehicleData UProjectJsonData::ReadPoliceVehicleInfo(FString _path)
{
	FSys_PoliceVehicleData vehicle;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		vehicle.VechicleType = FCString::Atoi(*(JsonObject->GetStringField("VechicleType")));

		vehicle.LocationX = FCString::Atof(*(JsonObject->GetStringField("LocationX")));
		vehicle.LocationY = FCString::Atof(*(JsonObject->GetStringField("LocationY")));
		vehicle.LocationZ = FCString::Atof(*(JsonObject->GetStringField("LocationZ")));

		vehicle.RotationX = FCString::Atof(*(JsonObject->GetStringField("RotationX")));
		vehicle.RotationY = FCString::Atof(*(JsonObject->GetStringField("RotationY")));
		vehicle.RotationZ = FCString::Atof(*(JsonObject->GetStringField("RotationZ")));
	}

	return vehicle;
}

void UProjectJsonData::WritePoliceVehicleInfo(FString _path, FSys_PoliceVehicleData vehicleInfo)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteValue(TEXT("VechicleType"), vehicleInfo.VechicleType);
	JsonWriter->WriteValue(TEXT("LocationX"), vehicleInfo.LocationX);
	JsonWriter->WriteValue(TEXT("LocationY"), vehicleInfo.LocationY);
	JsonWriter->WriteValue(TEXT("LocationZ"), vehicleInfo.LocationZ);
	JsonWriter->WriteValue(TEXT("RotationX"), vehicleInfo.RotationX);
	JsonWriter->WriteValue(TEXT("RotationY"), vehicleInfo.RotationY);
	JsonWriter->WriteValue(TEXT("RotationZ"), vehicleInfo.RotationZ);

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);
}

FSys_BlockerStreamData UProjectJsonData::ReadBlockerStreamInfo(FString _path)
{
	FSys_BlockerStreamData info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		info.BlockType = FCString::Atoi(*(JsonObject->GetStringField("BlockType")));

		info.Location.X = FCString::Atof(*(JsonObject->GetStringField("Pos_x")));
		info.Location.Y = FCString::Atof(*(JsonObject->GetStringField("Pos_y")));
		info.Location.Z = FCString::Atof(*(JsonObject->GetStringField("Pos_z")));

		info.Rotation.X = FCString::Atof(*(JsonObject->GetStringField("Rot_x")));
		info.Rotation.Y = FCString::Atof(*(JsonObject->GetStringField("Rot_y")));
		info.Rotation.Z = FCString::Atof(*(JsonObject->GetStringField("Rot_z")));

		info.Length = FCString::Atof(*(JsonObject->GetStringField("length")));
	}

	return info;
}

void UProjectJsonData::WriteBlockerStreamInfo(FString _path, FSys_BlockerStreamData info)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteValue(TEXT("BlockType"), info.BlockType);
	JsonWriter->WriteValue(TEXT("Pos_x"), info.Location.X);
	JsonWriter->WriteValue(TEXT("Pos_y"), info.Location.Y);
	JsonWriter->WriteValue(TEXT("Pos_z"), info.Location.Z);
	JsonWriter->WriteValue(TEXT("Rot_x"), info.Rotation.X);
	JsonWriter->WriteValue(TEXT("Rot_y"), info.Rotation.Y);
	JsonWriter->WriteValue(TEXT("Rot_z"), info.Rotation.Z);
	JsonWriter->WriteValue(TEXT("length"), info.Length);

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);
}


FSys_FeatureBoxData UProjectJsonData::ReadFeatureBoxInfo(FString _path)
{
	FSys_FeatureBoxData info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		info.FBoxType = FCString::Atoi(*(JsonObject->GetStringField("FBoxType")));

		info.Location.X = FCString::Atof(*(JsonObject->GetStringField("Pos_x")));
		info.Location.Y = FCString::Atof(*(JsonObject->GetStringField("Pos_y")));
		info.Location.Z = FCString::Atof(*(JsonObject->GetStringField("Pos_z")));

		info.FBoxOwner = JsonObject->GetStringField("FBoxOwner");
		info.Length = FCString::Atof(*(JsonObject->GetStringField("length")));
		info.Widgth = FCString::Atof(*(JsonObject->GetStringField("Widgth")));
	}

	return info;
}


void UProjectJsonData::WriteFeatureBoxInfo(FString _path, FSys_FeatureBoxData info)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteValue(TEXT("FBoxType"), info.FBoxType);
	JsonWriter->WriteValue(TEXT("Pos_x"), info.Location.X);
	JsonWriter->WriteValue(TEXT("Pos_y"), info.Location.Y);
	JsonWriter->WriteValue(TEXT("Pos_z"), info.Location.Z);
	JsonWriter->WriteValue(TEXT("FBoxOwner"), info.FBoxOwner);
	JsonWriter->WriteValue(TEXT("Length"), info.Length);
	JsonWriter->WriteValue(TEXT("Widgth"), info.Widgth);

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);
}
FSys_URLData UProjectJsonData::ReadURLData(FString _path)
{
	FSys_URLData info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag)
	{
		info.URL = JsonObject->GetStringField("URL");
	}

	return info;

}
void UProjectJsonData::WriteURLData(FString _path, FSys_URLData info)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("URL"), info.URL);
	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);
}

void UProjectJsonData::WriteTraffic(FString _path, TArray<FSys_Traffic> TrafficInfoArray)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteArrayStart();

	for (int i = 0; i < TrafficInfoArray.Num(); i++)
	{


		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue(TEXT("DisplayName"), TrafficInfoArray[i].displayName);
		JsonWriter->WriteValue(TEXT("indexM"), TrafficInfoArray[i].indexM);
		JsonWriter->WriteObjectEnd();


	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);
}

void UProjectJsonData::ReadTrafficInfo(FString _path, TArray<FSys_Traffic>& trafficinfoArray, bool &flag)
{
	FString JsonValue;
	FSys_Traffic trafficinfo;
	flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TArray< TSharedPtr<FJsonValue> > JsonParsed;
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		if (flag)
		{
			for (int i = 0; i < JsonParsed.Num(); i++)
			{
				trafficinfo.indexM = JsonParsed[i]->AsObject()->GetStringField("indexM");
				trafficinfo.displayName = JsonParsed[i]->AsObject()->GetStringField("DisplayName");
				trafficinfoArray.Add(trafficinfo);
			}

		}
	}
}

void UProjectJsonData::ReadCameraConfig(FString _path, TArray<Fsys_CameraData>& CameraDataArray)
{
	FString JsonValue;
	Fsys_CameraData cameraInfo;
	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		if (flag)
		{
			for (int i = 0; i < JsonParsed.Num(); i++)
			{
				JsonParsed[i]->AsObject()->TryGetStringField("Name", cameraInfo.Name);
				JsonParsed[i]->AsObject()->TryGetStringField("Label", cameraInfo.Label);
				JsonParsed[i]->AsObject()->TryGetStringField("Longitude", cameraInfo.Longitude);
				JsonParsed[i]->AsObject()->TryGetStringField("Latitude", cameraInfo.Latitude);
				JsonParsed[i]->AsObject()->TryGetStringField("High1", cameraInfo.High1);
				JsonParsed[i]->AsObject()->TryGetStringField("High2", cameraInfo.High2);
				JsonParsed[i]->AsObject()->TryGetStringField("Direction", cameraInfo.Direction);
				JsonParsed[i]->AsObject()->TryGetStringField("CameraID", cameraInfo.CameraID);
				JsonParsed[i]->AsObject()->TryGetStringField("Notice", cameraInfo.Notice);
				JsonParsed[i]->AsObject()->TryGetStringField("Axis", cameraInfo.Axis);
				CameraDataArray.Add(cameraInfo);
			}
		}

	}

}

