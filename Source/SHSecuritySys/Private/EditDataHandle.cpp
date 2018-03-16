// Fill out your copyright notice in the Description page of Project Settings.

#include "EditDataHandle.h"
#include "Json.h"
#include "APrintImpActor.h"

//// Sets default values for this component's properties
//UEditDataHandle::UEditDataHandle()
//{
//	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
//	// off to improve performance if you don't need them.
//	PrimaryComponentTick.bCanEverTick = true;
//
//	// ...
//}


// Called when the game starts
void UEditDataHandle::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UEditDataHandle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//获取项目当前目录
FString UEditDataHandle::GetGameDir()
{
	const  FString _gamePath = FPaths::GameDir();

	return _gamePath;
}

//创建方案数据存放目录
void UEditDataHandle::CreateProjectFolder(FString _path, FString _projectPolicePath, FString _projectBlockPath, FString _projectPoliceAreaPath, FString _projectLayerPath, FString _parkPath)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_path))
	{
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*_path);

		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*_projectPolicePath);
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*_projectBlockPath);
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*_projectPoliceAreaPath);
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*_projectLayerPath);
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*_parkPath);
	}
}

//删除方案数据
void UEditDataHandle::RemoveProjectFolder(FString _path)
{
	if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_path))
	{
		FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*_path);
	}
}

//查找指定目录下的文件夹
TArray<FString> UEditDataHandle::GetExistedPrjects(FString _path)
{
	TArray<FString> projects;

	FPaths::NormalizeDirectoryName(_path);
	IFileManager& FileManager = IFileManager::Get();

	FString FinalPath = _path / TEXT("*");
	FileManager.FindFiles(projects, *FinalPath, true, true);

	return projects;
}

//删除文件
void UEditDataHandle::RemoveFile(FString _path)
{
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*_path))
	{
		FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*_path);
	}
}

//获取某一目录下的所有文件
TArray<FString> UEditDataHandle::GetFolderFiles(FString _path)
{
	TArray<FString> files;

	FPaths::NormalizeDirectoryName(_path);
	IFileManager& FileManager = IFileManager::Get();

	FString FinalPath = _path / TEXT("*");
	FileManager.FindFiles(files, *FinalPath, true, false);

	return files;
}

FExh_Editor_SPolice UEditDataHandle::ReadPoliceTeamInfo(const FString _path)
{
	FExh_Editor_SPolice polices;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		polices.TeamName = JsonObject->GetStringField("TeamName");
		polices.TeamPeopleCount = FCString::Atoi(*(JsonObject->GetStringField("TeamPeopleCount")));
		polices.TeamLevel = JsonObject->GetStringField("TeamLevel");
		polices.Parent = JsonObject->GetStringField("Parent");
		polices.TeamPhone = JsonObject->GetStringField("TeamPhone");
		polices.TeamLeader = JsonObject->GetStringField("TeamLeader");
		polices.TeamLeaderPhone = JsonObject->GetStringField("TeamLeaderPhone");
		polices.IconPosX = JsonObject->GetStringField("IconPosX");
		polices.IconPosY = JsonObject->GetStringField("IconPosY");
		polices.CheckZoneID = JsonObject->GetStringField("CheckZoneID");

		TArray<FExh_Editor_SPolice_Part> parts;
		TArray<TSharedPtr<FJsonValue>> police_array = JsonObject->GetArrayField("Parts");

		if (police_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> PoliceObject;
			for (int i = 0; i<police_array.Num(); i++)
			{
				PoliceObject = police_array[i]->AsObject();

				FExh_Editor_SPolice_Part part;
				part.PartName = PoliceObject->GetStringField("PartName");
				part.PartCount = FCString::Atoi(*(PoliceObject->GetStringField("PartCount")));
				part.IconPosX = PoliceObject->GetStringField("IconPosX");
				part.IconPosY = PoliceObject->GetStringField("IconPosY");
				part.RspArea = PoliceObject->GetStringField("RspArea");
				part.color = PoliceObject->GetStringField("color");
				part.PartTeamLeader = PoliceObject->GetStringField("PartTeamLeader");
				part.PartTeamPhone = PoliceObject->GetStringField("PartTeamPhone");
				part.PartTeamLeaderPhone = PoliceObject->GetStringField("PartTeamLeaderPhone");
				part.FloorZPos = PoliceObject->GetStringField("FloorZPos");
				part.Floor = PoliceObject->GetStringField("Floor");

				parts.Add(part);
			}
		}
		polices.Parts = parts;
	}


	return polices;
}

void UEditDataHandle::WritePoliceTeamInfo(FString _path, FExh_Editor_SPolice policeTeamInfo)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteValue(TEXT("TeamName"), policeTeamInfo.TeamName);
	JsonWriter->WriteValue(TEXT("TeamPeopleCount"), policeTeamInfo.TeamPeopleCount);
	JsonWriter->WriteValue(TEXT("TeamLevel"), policeTeamInfo.TeamLevel);
	JsonWriter->WriteValue(TEXT("Parent"), policeTeamInfo.Parent);
	JsonWriter->WriteValue(TEXT("TeamPhone"), policeTeamInfo.TeamPhone);
	JsonWriter->WriteValue(TEXT("TeamLeader"), policeTeamInfo.TeamLeader);
	JsonWriter->WriteValue(TEXT("TeamLeaderPhone"), policeTeamInfo.TeamLeaderPhone);
	JsonWriter->WriteValue(TEXT("IconPosX"), policeTeamInfo.IconPosX);
	JsonWriter->WriteValue(TEXT("IconPosY"), policeTeamInfo.IconPosY);
	JsonWriter->WriteValue(TEXT("CheckZoneID"), policeTeamInfo.CheckZoneID);

	JsonWriter->WriteArrayStart("Parts");
	for (int i = 0; i < policeTeamInfo.Parts.Num(); i++)
	{
		JsonWriter->WriteObjectStart();

		JsonWriter->WriteValue(TEXT("PartName"), policeTeamInfo.Parts[i].PartName);
		JsonWriter->WriteValue(TEXT("PartCount"), policeTeamInfo.Parts[i].PartCount);
		JsonWriter->WriteValue(TEXT("IconPosX"), policeTeamInfo.Parts[i].IconPosX);
		JsonWriter->WriteValue(TEXT("IconPosY"), policeTeamInfo.Parts[i].IconPosY);
		JsonWriter->WriteValue(TEXT("RspArea"), policeTeamInfo.Parts[i].RspArea);
		JsonWriter->WriteValue(TEXT("color"), policeTeamInfo.Parts[i].color);
		JsonWriter->WriteValue(TEXT("PartTeamLeader"), policeTeamInfo.Parts[i].PartTeamLeader);
		JsonWriter->WriteValue(TEXT("PartTeamPhone"), policeTeamInfo.Parts[i].PartTeamPhone);
		JsonWriter->WriteValue(TEXT("PartTeamLeaderPhone"), policeTeamInfo.Parts[i].PartTeamLeaderPhone);
		JsonWriter->WriteValue(TEXT("FloorZPos"), policeTeamInfo.Parts[i].FloorZPos);
		JsonWriter->WriteValue(TEXT("Floor"), policeTeamInfo.Parts[i].Floor);

		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);
}

Fxh_Editor_SBox UEditDataHandle::ReadBoxData(const FString _path)
{
	Fxh_Editor_SBox box;
	FString JsonValue;
	if (FFileHelper::LoadFileToString(JsonValue, *_path))
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
		TSharedPtr<FJsonObject> OutValue;
		if (FJsonSerializer::Deserialize(JsonReader, OutValue))
		{
			box.BoxName = OutValue->GetStringField("BoxName");
			box.Building = OutValue->GetStringField("Building");
			box.Company = OutValue->GetStringField("Company");
			box.PoliceArea = OutValue->GetStringField("PoliceArea");
			box.Room = OutValue->GetStringField("Room");

			TArray<TSharedPtr<FJsonValue>> teamArr = OutValue->GetArrayField("PoliceTeam");
			TArray<FExh_Editor_SPolice> teams;
			for (int i = 0; i < teamArr.Num(); i++)
			{
				TSharedPtr<FJsonObject> data = teamArr[i]->AsObject();

				FExh_Editor_SPolice team;
				team.TeamName = data->GetStringField("TeamName");

				teams.Add(team);
			}
			box.PoliceTeam = teams;

			box.Length = OutValue->GetStringField("Length");
			box.Width = OutValue->GetStringField("Width");
			box.iconIndex = OutValue->GetStringField("iconIndex");

			box.RecordPosX = OutValue->GetStringField("RecordPosX");
			box.RecordPosY = OutValue->GetStringField("RecordPosY");
			box.RecordPosZ = OutValue->GetStringField("RecordPosZ");
		}
	}

	return box;
}

void UEditDataHandle::WriteBoxInfo(FString _path, Fxh_Editor_SBox box)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteValue(TEXT("BoxName"), box.BoxName);
	JsonWriter->WriteValue(TEXT("Building"), box.Building);
	JsonWriter->WriteValue(TEXT("Company"), box.Company);
	JsonWriter->WriteValue(TEXT("PoliceArea"), box.PoliceArea);
	JsonWriter->WriteValue(TEXT("Room"), box.Room);

	JsonWriter->WriteArrayStart("PoliceTeam");
	for (int i = 0; i < box.PoliceTeam.Num(); i++)
	{
		JsonWriter->WriteObjectStart();

		JsonWriter->WriteValue(TEXT("TeamName"), box.PoliceTeam[i].TeamName);


		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();

	JsonWriter->WriteValue(TEXT("Length"), box.Length);
	JsonWriter->WriteValue(TEXT("Width"), box.Width);
	JsonWriter->WriteValue(TEXT("iconIndex"), box.iconIndex);
	JsonWriter->WriteValue(TEXT("RecordPosX"), box.RecordPosX);
	JsonWriter->WriteValue(TEXT("RecordPosY"), box.RecordPosY);
	JsonWriter->WriteValue(TEXT("RecordPosZ"), box.RecordPosZ);

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);

}

FExh_Editor_PoliceMember UEditDataHandle::ReadMemberData(FString _path)
{
	FExh_Editor_PoliceMember member;
	FString JsonValue;
	if (FFileHelper::LoadFileToString(JsonValue, *_path))
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
		TSharedPtr<FJsonObject> OutValue;
		if (FJsonSerializer::Deserialize(JsonReader, OutValue))
		{

			TArray<TSharedPtr<FJsonValue>> teamArr = OutValue->GetArrayField("Memebers");
			TArray<FExh_Editor_MemberInfo> members;
			for (int i = 0; i < teamArr.Num(); i++)
			{
				TSharedPtr<FJsonObject> data = teamArr[i]->AsObject();

				FExh_Editor_MemberInfo team;

				team.MemberName = data->GetStringField("MemberName");
				team.MemberCount = data->GetStringField("MemberCount");

				members.Add(team);
			}

			member.Memebers = members;
		}
	}

	return member;

}

void UEditDataHandle::WriteMemberInfo(FString _path, FExh_Editor_PoliceMember box)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteArrayStart("Memebers");
	for (int i = 0; i < box.Memebers.Num(); i++)
	{
		JsonWriter->WriteObjectStart();

		JsonWriter->WriteValue(TEXT("MemberName"), box.Memebers[i].MemberName);
		JsonWriter->WriteValue(TEXT("MemberCount"), box.Memebers[i].MemberCount);

		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);
}

FExh_ParkingInfo UEditDataHandle::ReadParkIndo(FString _path)
{
	FExh_ParkingInfo result;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{

		TArray<FExh_Park> parts;
		TArray<TSharedPtr<FJsonValue>> police_array = JsonObject->GetArrayField("Parking");

		if (police_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> PoliceObject;
			for (int i = 0; i<police_array.Num(); i++)
			{
				PoliceObject = police_array[i]->AsObject();

				FExh_Park part;
				part.ParkName = PoliceObject->GetStringField("ParkName");
				part.ParkState = PoliceObject->GetStringField("ParkState");

				parts.Add(part);
			}
		}
		result.Parking = parts;
	}
	return result;
}

void UEditDataHandle::WriteParkInfo(FString _path, FExh_ParkingInfo parkInfo)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteArrayStart("Parking");
	for (int i = 0; i < parkInfo.Parking.Num(); i++)
	{
		JsonWriter->WriteObjectStart();

		JsonWriter->WriteValue(TEXT("ParkName"), parkInfo.Parking[i].ParkName);
		JsonWriter->WriteValue(TEXT("ParkState"), parkInfo.Parking[i].ParkState);
		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);
}

// 岗位联系表
void UEditDataHandle::PrintPostContactorCSV(FString _title,FString _path, TArray<FExh_Editor_SPolice> policeTeamInfo/*, TMap<FString, FExh_Editor_PoliceMember> policeMember*/)
{
	FString strContent;
	//UE_LOG(LogStreaming, Warning, TEXT("StartPage 错误."));
	//
	strContent += _title;
	/*strContent += (L"负责岗位,");
	strContent += (L"部门,");
	strContent += (L"联系人,");
	strContent += (L"联系电话");*/
	strContent += TEXT("\n\r");

	for (int i = 0; i < policeTeamInfo.Num(); ++i)
	{
		for (int j = 0; j < policeTeamInfo[i].Parts.Num(); ++j)
		{
			FString partName = policeTeamInfo[i].Parts[j].PartName;
			//FExh_Editor_PoliceMember policeMember = policeMember.Find(partName);

			//if (policeMemeber == NULL)	continue;

			//for(int m =0; m<policeMember.Num(); ++m )
			{
				strContent += policeTeamInfo[i].TeamName;
				strContent += ",";

				strContent += partName;
				strContent += ",";

				strContent += FString::Printf(TEXT("%d"), policeTeamInfo[i].Parts[j].PartCount);
				strContent += ",";

				strContent += policeTeamInfo[i].Parts[j].PartTeamLeader;
				strContent += ",";

				strContent += policeTeamInfo[i].Parts[j].PartTeamLeaderPhone;
				strContent += "\n\r";

				//stringContent += policeMember[m].MemberName;
			}
		}
	}

	FFileHelper::SaveStringToFile(strContent, *_path, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
}

// 岗位设置表
void UEditDataHandle::PrintPostSettingCSV(FString _path, TArray<FExh_Editor_SPolice> policeTeamInfo, AAPrintImpActor* printImpActor)
{
	FString strContent;

	// header
	strContent += TEXT("岗位名称,");
	strContent += TEXT("负责岗位,");
	strContent += TEXT("部门,");
	strContent += TEXT("人数");
	strContent += "\n\r";

	for (int i = 0; i < policeTeamInfo.Num(); ++i)
	{
		if (policeTeamInfo[i].Parts.Num() == 0)
		{
			FString teamName = policeTeamInfo[i].TeamName;

			TArray<FExh_Editor_MemberInfo> memInfo = printImpActor->GetMemberInfoByTeamName(0, teamName);
			for (int j = 0; j<memInfo.Num(); ++j)
			{
				strContent += teamName;
				strContent += ",";

				strContent += teamName;
				strContent += ",";

				strContent += memInfo[j].MemberName;
				strContent += ",";

				strContent += memInfo[j].MemberCount;
				strContent += "\n\r";
			}
		}
		else
		{
			for (int j = 0; j < policeTeamInfo[i].Parts.Num(); ++j)
			{
				FString partName = policeTeamInfo[i].Parts[j].PartName;
				TArray<FExh_Editor_MemberInfo> memInfo = printImpActor->GetMemberInfoByTeamName(1, partName);
				for (int m = 0; m<memInfo.Num(); ++m)
				{
					strContent += policeTeamInfo[i].TeamName;
					strContent += ",";

					strContent += partName;
					strContent += ",";

					strContent += memInfo[m].MemberName;
					strContent += ",";

					strContent += memInfo[m].MemberCount;
					strContent += "\n\r";
				}
			}
		}
	}

	FFileHelper::SaveStringToFile(strContent, *_path, FFileHelper::EEncodingOptions::ForceUTF8);
}

// 警力抽调表
void UEditDataHandle::PrintPoliceTransferCSV(FString _path, TArray<FExh_Editor_PoliceMember> policeMembers)
{
	FString strContent;

	strContent.Append(TEXT("部门,"));
	strContent.Append(TEXT("警力数"));
	strContent.Append(TEXT("\n\r"));

	for (int i = 0; i<policeMembers.Num(); ++i)
	{
		for (int j = 0; j < policeMembers[i].Memebers.Num(); j++)
		{
			strContent.Append(policeMembers[i].Memebers[j].MemberName);
			strContent.Append(",");
			//FString::Printf(TEXT("%d"), policeMember.Memebers[i].MemberCount);
			strContent.Append(policeMembers[i].Memebers[j].MemberCount);
			strContent.Append("\n\r");
		}
	}

	FFileHelper::SaveStringToFile(strContent, *_path, FFileHelper::EEncodingOptions::ForceUTF8);
}


FExh_MemberTeamArray UEditDataHandle::ReadMemberTeamArray(const FString _path)
{
	FExh_MemberTeamArray mArray;
	FString JsonValue;
	if (FFileHelper::LoadFileToString(JsonValue, *_path))
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
		TSharedPtr<FJsonObject> OutValue;
		if (FJsonSerializer::Deserialize(JsonReader, OutValue))
		{
			TArray<TSharedPtr<FJsonValue>> teamArr = OutValue->GetArrayField("TeamArray");
			TArray<FExh_MemberTeam> teams;
			for (int i = 0; i < teamArr.Num(); i++)
			{
				TSharedPtr<FJsonObject> data = teamArr[i]->AsObject();

				FExh_MemberTeam team;
				team.MemberTeam = data->GetStringField("MemberTeam");

				teams.Add(team);
			}
			mArray.TeamArray = teams;
		}
	}

	return mArray;
}


FExh_VechicleData UEditDataHandle::ReadVehicleData(const FString _path)
{
	FExh_VechicleData car;
	FString JsonValue;
	if (FFileHelper::LoadFileToString(JsonValue, *_path))
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
		TSharedPtr<FJsonObject> OutValue;
		if (FJsonSerializer::Deserialize(JsonReader, OutValue))
		{
			car.Type = OutValue->GetStringField("Type");
			car.PosX = OutValue->GetStringField("PosX");
			car.PosY = OutValue->GetStringField("PosY");
			car.PosZ = OutValue->GetStringField("PosZ");
			car.RotX = OutValue->GetStringField("RotX");
			car.RotY = OutValue->GetStringField("RotY");
			car.RotZ = OutValue->GetStringField("RotZ");
		}
	}

	return car;
}

void UEditDataHandle::WriteVehicleInfo(FString _path, FExh_VechicleData box)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteValue(TEXT("Type"), box.Type);
	JsonWriter->WriteValue(TEXT("PosX"), box.PosX);
	JsonWriter->WriteValue(TEXT("PosY"), box.PosY);
	JsonWriter->WriteValue(TEXT("PosZ"), box.PosZ);
	JsonWriter->WriteValue(TEXT("RotX"), box.RotX);
	JsonWriter->WriteValue(TEXT("RotY"), box.RotY);
	JsonWriter->WriteValue(TEXT("RotZ"), box.RotZ);

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);

}

// Sets default values for this component's properties
UEditDataHandle::UEditDataHandle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}






