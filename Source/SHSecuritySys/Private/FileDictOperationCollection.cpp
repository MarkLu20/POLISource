// Fill out your copyright notice in the Description page of Project Settings.



#include "FileDictOperationCollection.h"
#include "Json.h"
#include  "Kismet/KismetStringLibrary.h"
#include "regex"




//获取项目当前目录
FString UFileDictOperationCollection::GetGameDir()
{
	const  FString _gamePath = FPaths::GameDir();

	return _gamePath;
}

//删除文件
void UFileDictOperationCollection::RemoveFile(FString _path)
{
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*_path))
	{
		FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*_path);
	}
}

//删除目录
void UFileDictOperationCollection::RemoveFolder(FString _path)
{
	if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_path))
	{
		FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*_path);
	}
}

//获取某一目录下的所有文件
TArray<FString> UFileDictOperationCollection::GetFolderFiles(FString _path)
{
	TArray<FString> files;

	FPaths::NormalizeDirectoryName(_path);
	IFileManager& FileManager = IFileManager::Get();

	FString FinalPath = _path / TEXT("*");
	FileManager.FindFiles(files, *FinalPath, true, true);

	return files;
}

//创建文件夹
void UFileDictOperationCollection::CreateFolder(FString _path)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_path))
	{
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*_path);
	}
}



FDateTime UFileDictOperationCollection::GetCurrentDateAndTime()
{
	return FDateTime::Now();
}

FDateTime UFileDictOperationCollection::GetCurrentUtcDateAndTime(int32 timezone)
{
	return (FDateTime::UtcNow() + FTimespan(timezone, 0, 0));
}

FTimespan UFileDictOperationCollection::GetCurrentTime()
{
	return GetCurrentDateAndTime().GetTimeOfDay();
}

FTimespan UFileDictOperationCollection::GetCurrentUtcTime(int32 timezone)
{
	return GetCurrentUtcDateAndTime(timezone).GetTimeOfDay();
}

float UFileDictOperationCollection::GetCurrentTimeInTicks()
{
	return GetCurrentTime().GetTicks();
}

float UFileDictOperationCollection::GetCurrentTimeInMilliseconds()
{
	return GetCurrentTime().GetTotalMilliseconds();
}

float UFileDictOperationCollection::GetCurrentTimeInSeconds()
{
	return GetCurrentTime().GetTotalSeconds();
}

float UFileDictOperationCollection::GetCurrentUtcTimeInSeconds(int32 timezone)
{
	return GetCurrentUtcTime(timezone).GetTotalSeconds();
}

float UFileDictOperationCollection::GetCurrentTimeInMinutes()
{
	return GetCurrentTime().GetTotalMinutes();
}

float UFileDictOperationCollection::GetCurrentTimeInHours()
{
	return GetCurrentTime().GetTotalHours();
}



FSys_SystemSettingConfig UFileDictOperationCollection::ReadSystemConfig(FString _path)
{
	FSys_SystemSettingConfig info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		info.DebugMode = JsonObject->GetStringField("DebugMode");

		info.Info110ConfigURL = JsonObject->GetStringField("Info110ConfigURL");
		info.Update110Time = JsonObject->GetStringField("Update110Time");
		info.Search110ByPhoneURL = JsonObject->GetStringField("Search110ByPhoneURL");
		info.Search110ByKeyboardURL = JsonObject->GetStringField("Search110ByKeyboardURL");
		info.Update110AreaInfoURL = JsonObject->GetStringField("Update110AreaInfoURL");

		info.WifiInfoConfigURL = JsonObject->GetStringField("WifiInfoConfigURL");
		info.WifiInfoUpdateTime = JsonObject->GetStringField("WifiInfoUpdateTime");

		info.WarningInitURL = JsonObject->GetStringField("WarningInitURL");
		info.WarningNewURL = JsonObject->GetStringField("WarningNewURL");
		info.WarningUpdateTime = JsonObject->GetStringField("WarningUpdateTime");

		info.LinkURL = JsonObject->GetStringField("LinkURL");
		info.YanPan110Link = JsonObject->GetStringField("YanPan110Link");
		info.SuperSearchLink = JsonObject->GetStringField("SuperSearchLink");
		info.PeopleInfoLink = JsonObject->GetStringField("PeopleInfoLink");
		info.WorkInfoLink = JsonObject->GetStringField("WorkInfoLink");
		info.GaoDeLink = JsonObject->GetStringField("GaoDeLink");
		info.rtspLink = JsonObject->GetStringField("rtspLink");
	}

	return info;
}


void UFileDictOperationCollection::WriteSystemConfig(FString _path, FSys_SystemSettingConfig info)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	JsonWriter->WriteObjectStart();

	JsonWriter->WriteValue(TEXT("DebugMode"), info.DebugMode);
	JsonWriter->WriteValue(TEXT("Info110ConfigURL"), info.Info110ConfigURL);
	JsonWriter->WriteValue(TEXT("Update110Time"), info.Update110Time);
	JsonWriter->WriteValue(TEXT("Search110ByPhoneURL"), info.Search110ByPhoneURL);
	JsonWriter->WriteValue(TEXT("Search110ByKeyboardURL"), info.Search110ByKeyboardURL);
	JsonWriter->WriteValue(TEXT("Update110AreaInfoURL"), info.Update110AreaInfoURL);


	JsonWriter->WriteValue(TEXT("WifiInfoConfigURL"), info.WifiInfoConfigURL);
	JsonWriter->WriteValue(TEXT("WifiInfoUpdateTime"), info.WifiInfoUpdateTime);

	JsonWriter->WriteValue(TEXT("WarningInitURL"), info.WarningInitURL);
	JsonWriter->WriteValue(TEXT("WarningNewURL"), info.WarningNewURL);
	JsonWriter->WriteValue(TEXT("WarningUpdateTime"), info.WarningUpdateTime);

	JsonWriter->WriteValue(TEXT("LinkURL"), info.LinkURL);
	JsonWriter->WriteValue(TEXT("YanPan110Link"), info.YanPan110Link);
	JsonWriter->WriteValue(TEXT("SuperSearchLink"), info.SuperSearchLink);
	JsonWriter->WriteValue(TEXT("PeopleInfoLink"), info.PeopleInfoLink);
	JsonWriter->WriteValue(TEXT("WorkInfoLink"), info.WorkInfoLink);
	JsonWriter->WriteValue(TEXT("GaoDeLink"), info.GaoDeLink);
	JsonWriter->WriteValue(TEXT("rtspLink"), info.rtspLink);

	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);
}

void UFileDictOperationCollection::WriteHongWaiDeviation(FHongWaiDeviation data, FString _path)
{
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteArrayStart("OldAdjust");
	for (int i = 0; i < data.OldAdjust.Num(); ++i)
	{
		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue("SN", data.OldAdjust[i].SN);
		JsonWriter->WriteValue("InCount", data.OldAdjust[i].InCount);
		JsonWriter->WriteValue("OutCount", data.OldAdjust[i].OutCount);
		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteArrayStart("AreaDayAdjust");
	for (int i = 0; i < data.AreaDayAdjsut.Num(); ++i)
	{
		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue("AreaType", data.AreaDayAdjsut[i].AreaType);
		JsonWriter->WriteValue("Area", data.AreaDayAdjsut[i].Area);
		JsonWriter->WriteValue("AllIn", data.AreaDayAdjsut[i].AllIn);
		JsonWriter->WriteValue("AllOut", data.AreaDayAdjsut[i].AllOut);

		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteObjectEnd();

	JsonWriter->Close();

	FFileHelper::SaveStringToFile(JsonStr, *_path);

}

FSys_TrafficStatus UFileDictOperationCollection::ReadTrafficData(FString _path)
{
	FSys_TrafficStatus info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		info.TrafficDataForAll = JsonObject->GetStringField("TrafficDataForAll");
		info.TrafficAvgSpeed = JsonObject->GetStringField("TrafficAvgSpeed");

		//Add Unit Team-->
		TArray<FSys_TrafficRoadStatus> parts;
		TArray<TSharedPtr<FJsonValue>> police_array = JsonObject->GetArrayField("TopsRoads");
		if (police_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> PoliceObject;
			for (int i = 0; i < police_array.Num(); i++)
			{
				PoliceObject = police_array[i]->AsObject();

				FSys_TrafficRoadStatus part;
				part.RoadName = PoliceObject->GetStringField("RoadName");
				part.TrafficAvgSpeed = PoliceObject->GetStringField("TrafficAvgSpeed");
				part.TrafficData = PoliceObject->GetStringField("TrafficData");

				parts.Add(part);
			}
		}
		info.TopsRoads = parts;
	}

	return info;
}



FSys_WeartherData UFileDictOperationCollection::ReadWeartherData(FString _path)
{
	FSys_WeartherData info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		info.airQuality = JsonObject->GetStringField("airQuality");
		info.bigTemperature = JsonObject->GetIntegerField("bigTemperature");
		info.humidity = JsonObject->GetIntegerField("humidity");
		info.smallTemperature = JsonObject->GetIntegerField("smallTemperature");
		info.weather = JsonObject->GetStringField("weather");
		info.wind = JsonObject->GetStringField("wind");

	}

	return info;
}

void UFileDictOperationCollection::FileToString(FString _path, FString &Value)
{
	FFileHelper::LoadFileToString(Value, *_path);
}

FSys_KaKouStatus UFileDictOperationCollection::ReadKaKouData(FString _path)
{
	FSys_KaKouStatus info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		info.TotalCount = JsonObject->GetIntegerField("TotalCount");

		//Add Unit Team-->
		TArray<FSys_KaKouDetailStatus> parts;
		TArray<TSharedPtr<FJsonValue>> police_array = JsonObject->GetArrayField("KaKous");
		if (police_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> PoliceObject;
			for (int i = 0; i < police_array.Num(); i++)
			{
				PoliceObject = police_array[i]->AsObject();

				FSys_KaKouDetailStatus part;
				part.SBMC = PoliceObject->GetStringField("SBMC");
				part.Count = PoliceObject->GetIntegerField("Count");
				part.SBBH = PoliceObject->GetStringField("SBBH");

				parts.Add(part);
			}
		}
		info.KaKous = parts;
	}

	return info;
}


FSys_PoliceGPSArray UFileDictOperationCollection::ReadPoliceGPSData(FString _path)
{
	FSys_PoliceGPSArray info;

	//FString JsonValue;
	//bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(_path);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<FSys_PoliceGPS> parts;
		TArray<TSharedPtr<FJsonValue>> police_array = JsonObject->GetArrayField("PoliceArray");
		TArray<TSharedPtr<FJsonValue>> police_info_array = JsonObject->GetArrayField("PoliceInfo");
		if (police_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> PoliceObject;
			TSharedPtr<FJsonObject> PoliceInfoObject;
			for (int i = 0; i < police_array.Num(); i++)
			{

				PoliceObject = police_array[i]->AsObject();
				FSys_PoliceGPS part;
				part.Type = -1;
				part.Phone = TEXT("XXXX");
				part.Name = TEXT("XXXX");
				part.PoliceID = TEXT("XXXX");

				part.Id = PoliceObject->GetStringField("Id");
				part.GPS_X = PoliceObject->GetStringField("GPS_X");
				part.GPS_Y = PoliceObject->GetStringField("GPS_Y");
				part.Timestamp = PoliceObject->GetStringField("Timestamp");
				part.Year = PoliceObject->GetStringField("Year");
				part.Month = PoliceObject->GetStringField("Month");
				part.Day = PoliceObject->GetStringField("Day");
				part.HH = PoliceObject->GetStringField("HH");
				part.MM = PoliceObject->GetStringField("MM");
				part.SS = PoliceObject->GetStringField("SS");
				part.StandID = PoliceObject->GetStringField("PoliceID");
				for (int j = 0;j < police_info_array.Num();j++)
				{
					PoliceInfoObject = police_info_array[j]->AsObject();

					FString temp_id = part.StandID.RightChop(2);
					FString police_info_stand_id = PoliceInfoObject->GetStringField("StandID");
					if (temp_id.Equals(police_info_stand_id))
					{
						part.Type = PoliceInfoObject->GetIntegerField("type");
						part.Phone = PoliceInfoObject->GetStringField("Phone");
						part.Name = PoliceInfoObject->GetStringField("name");
						part.PoliceID = PoliceInfoObject->GetStringField("PoliceID");
					}
					

				}

				parts.Add(part);
			}
		}
		info.PoliceArray = parts;
	}

	return info;
}


FSys_CameraSignArray UFileDictOperationCollection::ReadCameraSignData(FString _path)
{
	FSys_CameraSignArray info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<FSys_CameraSign> parts;
		TArray<TSharedPtr<FJsonValue>> camera_array = JsonObject->GetArrayField("CameraArray");
		if (camera_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject;
			for (int i = 0; i < camera_array.Num(); i++)
			{
				cameraObject = camera_array[i]->AsObject();

				FSys_CameraSign part;
				part.name = cameraObject->GetStringField("name");
				part.Long = cameraObject->GetStringField("Long");
				part.Lat = cameraObject->GetStringField("Lat");
				part.High1 = cameraObject->GetStringField("High1");
				part.High2 = cameraObject->GetStringField("High2");
				part.Direction = cameraObject->GetStringField("Direction");
				part.CameraID = cameraObject->GetStringField("CameraID");
				part.Axis = cameraObject->GetStringField("Axis");

				parts.Add(part);
			}
		}
		info.CameraArray = parts;
	}

	return info;
}

FSys_WifiListTable UFileDictOperationCollection::ReadWifiListTable(FString JsonValue)
{
	FSys_WifiListTable info;

	//FString JsonValue;
	//bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<FSys_WifiListSingle> parts;
		TArray<TSharedPtr<FJsonValue>> camera_array = JsonObject->GetArrayField("WifiResult");
		if (camera_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject;
			for (int i = 0; i < camera_array.Num(); i++)
			{
				cameraObject = camera_array[i]->AsObject();

				FSys_WifiListSingle part;
				part.id = cameraObject->GetStringField("id");
				part.name = cameraObject->GetStringField("name");
				part.lang = cameraObject->GetStringField("lang");
				part.lat = cameraObject->GetStringField("lat");
				part.owner = cameraObject->GetStringField("owner");

				parts.Add(part);
			}
		}
		info.WifiResult = parts;
	}

	return info;
}

void UFileDictOperationCollection::WriteWifiListTable(FString _path, FSys_WifiListTable list, TArray<FString> Locat)
{
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteArrayStart();

	for (int i = 0; i < Locat.Num(); i++)
	{
		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue(TEXT("id"), list.WifiResult[i].id);
		JsonWriter->WriteValue(TEXT("name"), list.WifiResult[i].name);
		JsonWriter->WriteValue(TEXT("lang"), list.WifiResult[i].lang);
		JsonWriter->WriteValue(TEXT("lat"), list.WifiResult[i].lat);
		JsonWriter->WriteValue(TEXT("owner"), list.WifiResult[i].owner);
		JsonWriter->WriteValue(TEXT("Locate"), Locat[i]);
		JsonWriter->WriteObjectEnd();
	}

	JsonWriter->WriteArrayEnd();
	JsonWriter->Close();
	
	FFileHelper::SaveStringToFile(JsonStr, *_path);
}

void UFileDictOperationCollection::WriteKaKouListTable(FString _path, FSys_KaKouListTable list, TArray<FString> Locat)
{
	FString JsonStr;

	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteArrayStart();

	for (int i = 0; i < Locat.Num(); i++)
	{
		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue(TEXT("id"), list.KaKouResult[i].id);
		JsonWriter->WriteValue(TEXT("name"), list.KaKouResult[i].name);
		JsonWriter->WriteValue(TEXT("lang"), list.KaKouResult[i].lang);
		JsonWriter->WriteValue(TEXT("lat"), list.KaKouResult[i].lat);
		JsonWriter->WriteValue(TEXT("owner"), list.KaKouResult[i].owner);
		JsonWriter->WriteValue(TEXT("locate"), Locat[i]);
		JsonWriter->WriteObjectEnd();
	}

	JsonWriter->WriteArrayEnd();
	JsonWriter->Close();
	FFileHelper::SaveStringToFile(JsonStr, *_path);
}

FSys_PeopleWifiData UFileDictOperationCollection::ReadWifiDataTable(FString JsonValue)
{
	FSys_PeopleWifiData info;

	//FString JsonValue;
	//bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<FSys_PeopleWifiDetailData> parts;
		TArray<TSharedPtr<FJsonValue>> camera_array = JsonObject->GetArrayField("peopleList");
		if (camera_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject;
			for (int i = 0; i < camera_array.Num(); i++)
			{
				cameraObject = camera_array[i]->AsObject();

				FSys_PeopleWifiDetailData part;
				part.wifiID = cameraObject->GetStringField("wifiID");
				part.count = cameraObject->GetStringField("count");

				parts.Add(part);
			}
		}
		info.peopleList = parts;
	}

	return info;
}

FSys_KaKouListTable UFileDictOperationCollection::ReadKaKouListTable(FString JsonValue)
{
	FSys_KaKouListTable info;

	//FString JsonValue;
	//bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<FSys_KaKouListSingle> parts;
		TArray<TSharedPtr<FJsonValue>> camera_array = JsonObject->GetArrayField("KaKouResult");
		if (camera_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject;
			for (int i = 0; i < camera_array.Num(); i++)
			{
				cameraObject = camera_array[i]->AsObject();

				FSys_KaKouListSingle part;
				part.id = cameraObject->GetStringField("id");
				part.name = cameraObject->GetStringField("name");
				part.lang = cameraObject->GetStringField("lang");
				part.lat = cameraObject->GetStringField("lat");
				part.owner = cameraObject->GetStringField("owner");

				parts.Add(part);
			}
		}
		info.KaKouResult = parts;
	}

	return info;
}

void UFileDictOperationCollection::ReadHongWarPeopleData(FString json,TArray<FSys_HongWaiPeopleData>& HongWaiArray)
{
	FSys_HongWaiPeopleData data;

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(json);

	/*TSharedPtr<FJsonObject>  JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<TSharedPtr<FJsonValue>> list = JsonObject->GetArrayField("res");*/
	TSharedPtr<FJsonObject> JsonObject;
	TArray<TSharedPtr<FJsonValue>> JsonParesed;	

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{		
			JsonParesed = JsonObject->GetArrayField("res");
			for (int i = 0; i < JsonParesed.Num(); i++)
			{
				data.key = FString::FromInt(JsonParesed[i]->AsObject()->GetIntegerField("key"));
				data.sn = JsonParesed[i]->AsObject()->GetStringField("sn");
				data.type = JsonParesed[i]->AsObject()->GetStringField("type");
				data.count = JsonParesed[i]->AsObject()->GetStringField("count");
				data.timeStamp = FString::FromInt(JsonParesed[i]->AsObject()->GetIntegerField("timeStamp"));
				data.Year = JsonParesed[i]->AsObject()->GetStringField("Year");
				data.Month = JsonParesed[i]->AsObject()->GetStringField("Month");
				data.Day = JsonParesed[i]->AsObject()->GetStringField("Day");
				HongWaiArray.Add(data);
			}
		
	}
}

FString UFileDictOperationCollection::UseRegex(const FString str, const FString Reg, bool &HasFind)
{
	FRegexPattern Pattern(Reg);
	FRegexMatcher regMatcher(Pattern, str);
	regMatcher.SetLimits(0, str.Len());
	HasFind = regMatcher.FindNext();
	FString result;
	for (int i = 0; i < regMatcher.GetMatchEnding(); i++)
	{
		result += regMatcher.GetCaptureGroup(i);
	}
	
	return result;
}

FSys_KaKouDataArray UFileDictOperationCollection::ReadKaKouDataArray(FString JsonValue)
{
	FSys_KaKouDataArray info;

	//FString JsonValue;
	//bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<FSys_KaKouDataOne> parts;
		TArray<FSys_KaKouDataOne> chuparts;
		TArray<FSys_KaKouDataOne> allparts;
		TArray<FSys_KaKouDataOne> topparts;

		TArray<TSharedPtr<FJsonValue>> camera_array = JsonObject->GetArrayField("JinArray");
		if (camera_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject;
			for (int i = 0; i < camera_array.Num(); i++)
			{
				cameraObject = camera_array[i]->AsObject();

				FSys_KaKouDataOne part;
				part.SBBH = cameraObject->GetStringField("SBBH");
				part.SBMC = cameraObject->GetStringField("SBMC");
				part.XSFX = cameraObject->GetStringField("XSFX");
				part.Count = FString::FromInt(cameraObject->GetNumberField("Count"));
				part.pass_or_out = FString::FromInt(cameraObject->GetIntegerField("pass_or_out"));

				parts.Add(part);
			}
		}
		info.JinArray = parts;

		TArray<TSharedPtr<FJsonValue>> camera_array2 = JsonObject->GetArrayField("ChuArray");
		if (camera_array2.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject2;
			for (int i = 0; i < camera_array2.Num(); i++)
			{
				cameraObject2 = camera_array2[i]->AsObject();

				FSys_KaKouDataOne part;
				part.SBBH = cameraObject2->GetStringField("SBBH");
				part.SBMC = cameraObject2->GetStringField("SBMC");
				part.XSFX = cameraObject2->GetStringField("XSFX");
				part.Count = FString::FromInt(cameraObject2->GetNumberField("Count"));
				part.pass_or_out = FString::FromInt(cameraObject2->GetIntegerField("pass_or_out"));

				chuparts.Add(part);
			}
		}
		info.ChuArray = chuparts;

		TArray<TSharedPtr<FJsonValue>> camera_array3 = JsonObject->GetArrayField("AllArray");
		if (camera_array3.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject3;
			for (int i = 0; i < camera_array3.Num(); i++)
			{
				cameraObject3 = camera_array3[i]->AsObject();

				FSys_KaKouDataOne part;
				part.SBBH = cameraObject3->GetStringField("SBBH");
				part.SBMC = cameraObject3->GetStringField("SBMC");
				part.XSFX = cameraObject3->GetStringField("XSFX");
				part.Count = FString::FromInt(cameraObject3->GetNumberField("Count"));
				part.pass_or_out = FString::FromInt(cameraObject3->GetIntegerField("pass_or_out"));

				allparts.Add(part);
			}
		}
		info.AllArray = allparts;

		TArray<TSharedPtr<FJsonValue>> camera_array4 = JsonObject->GetArrayField("TopArray");
		if (camera_array4.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject4;
			for (int i = 0; i < camera_array4.Num(); i++)
			{
				cameraObject4 = camera_array4[i]->AsObject();

				FSys_KaKouDataOne part;
				part.SBBH = cameraObject4->GetStringField("SBBH");
				part.SBMC = cameraObject4->GetStringField("SBMC");
				part.XSFX = cameraObject4->GetStringField("XSFX");
				part.Count = FString::FromInt(cameraObject4->GetNumberField("Count"));
				part.pass_or_out = FString::FromInt(cameraObject4->GetIntegerField("pass_or_out"));

				topparts.Add(part);
			}
		}
		info.TopArray = topparts;
	}

	return info;
}

FSys_SIPArray UFileDictOperationCollection::ReadSIPConfigArray(FString _path)
{
	FSys_SIPArray info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<FSys_SIPOne> parts;
		TArray<TSharedPtr<FJsonValue>> camera_array = JsonObject->GetArrayField("SIPConfig");
		if (camera_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject;
			for (int i = 0; i < camera_array.Num(); i++)
			{
				cameraObject = camera_array[i]->AsObject();

				FSys_SIPOne part;
				part.IP = cameraObject->GetStringField("IP");
				part.Port = cameraObject->GetStringField("Port");
				part.Route = cameraObject->GetStringField("Route");
				part.URLSend = cameraObject->GetStringField("URLSend");
				part.URL = cameraObject->GetStringField("URL");

				parts.Add(part);
			}
		}
		info.SIPConfig = parts;
	}

	return info;
}

FSys_PeopleDayData UFileDictOperationCollection::ReadDayData(FString JsonValue)
{
	FSys_PeopleDayData info;

	//FString JsonValue;
	//bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TSharedPtr<FJsonObject> camera_array = JsonObject->GetObjectField("res");
		FSys_DayData part;
		camera_array->TryGetStringField("H0", part.H0);
		camera_array->TryGetStringField("H2", part.H2);
		camera_array->TryGetStringField("H3", part.H3);
		camera_array->TryGetStringField("H4", part.H4);
		camera_array->TryGetStringField("H5", part.H5);
		camera_array->TryGetStringField("H6", part.H6);
		camera_array->TryGetStringField("H7", part.H7);
		camera_array->TryGetStringField("H8", part.H8);
		camera_array->TryGetStringField("H9", part.H9);
		camera_array->TryGetStringField("H10", part.H10);
		camera_array->TryGetStringField("H11", part.H11);
		camera_array->TryGetStringField("H12", part.H12);
		camera_array->TryGetStringField("H13", part.H13);
		camera_array->TryGetStringField("H14", part.H14);
		camera_array->TryGetStringField("H15", part.H15);
		camera_array->TryGetStringField("H1", part.H1);
		camera_array->TryGetStringField("H16", part.H16);
		/*part.H17 = camera_array->GetStringField("H17");
		part.H18 = camera_array->GetStringField("H18");
		part.H19 = camera_array->GetStringField("H19");
		part.H20 = camera_array->GetStringField("H20");
		part.H21 = camera_array->GetStringField("H21");
		part.H22 = camera_array->GetStringField("H22");
		part.H23 = camera_array->GetStringField("H23");
*/
		info.res = part;
	}

	return info;
}

FSys_TrafficData UFileDictOperationCollection::ReadTraffic(FString JsonValue)
{
	FSys_TrafficData trafficData;
	FSys_TrafficStatlist trafficstatlist;
	FSys_TrafficDatatravio TravioByTodayEachHour;
	FSys_TrafficDatatravio TravioByNearOneMonth;
	FSys_TrafficWeek NearWeekCount;
	//FSys_TrafficWeek MaxTodayCount;
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TSharedPtr<FJsonObject>  JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	
	if (BFlag)
	{
		TSharedPtr<FJsonObject> travioByLocationNew = JsonObject->GetObjectField("travioByLocationNew");
		TArray<TSharedPtr<FJsonValue>> statListt = travioByLocationNew->GetArrayField("statList");
		for (int i = 0; i < statListt.Num(); i++)
		{ 
			trafficstatlist.name = statListt[i]->AsObject()->GetStringField("name");
			trafficstatlist.totalCount = FString::FromInt(statListt[i]->AsObject()->GetNumberField("totalCount"));
			trafficstatlist.color = statListt[i]->AsObject()->GetStringField("color");
			trafficstatlist.overLimit= UKismetStringLibrary::Conv_BoolToString(statListt[i]->AsObject()->GetBoolField("overLimit"));
			trafficstatlist.noDiscardTotalCount= FString::FromInt(statListt[i]->AsObject()->GetNumberField("noDiscardTotalCount"));
			trafficstatlist.unauditCount = FString::FromInt(statListt[i]->AsObject()->GetNumberField("unauditCount"));
			trafficstatlist.auditCount = FString::FromInt(statListt[i]->AsObject()->GetNumberField("auditCount"));
			trafficstatlist.lightVioCount = FString::FromInt(statListt[i]->AsObject()->GetNumberField("lightVioCount"));
			trafficstatlist.discardCount = FString::FromInt(statListt[i]->AsObject()->GetNumberField("discardCount"));
			trafficData.TraffictravioByLocationNew.Add(trafficstatlist);
			
		}
		TSharedPtr<FJsonObject> travioByTodayEachHour = JsonObject->GetObjectField("TravioByTodayEachHour");
		TArray<TSharedPtr<FJsonValue>> list = travioByTodayEachHour->GetArrayField("list");
		for (int i = 0; i < list.Num(); i++)
		{
			TravioByTodayEachHour.time= list[i]->AsObject()->GetStringField("time");
			TravioByTodayEachHour.timeNext = list[i]->AsObject()->GetStringField("timeText");
			TravioByTodayEachHour.noDiscardTotalCount = list[i]->AsObject()->GetStringField("noDiscardTotalCount");
			TravioByTodayEachHour.unauditCount = list[i]->AsObject()->GetStringField("unauditCount");
			TravioByTodayEachHour.auditCount = list[i]->AsObject()->GetStringField("auditCount");
			TravioByTodayEachHour.lightVioCount = list[i]->AsObject()->GetStringField("lightVioCount");
			TravioByTodayEachHour.discardCount = list[i]->AsObject()->GetStringField("discardCount");
			TravioByTodayEachHour.totalCount = list[i]->AsObject()->GetStringField("totalCount");
			trafficData.TravioByTodayEachHour.Add(TravioByTodayEachHour);
			trafficData.TravioByTodayEachHour.Add(TravioByTodayEachHour);
		}
		TSharedPtr<FJsonObject> travioByNearOneMonth = JsonObject->GetObjectField("TravioByNearOneMonth");
		TArray<TSharedPtr<FJsonValue>> listm = travioByNearOneMonth->GetArrayField("list");
		for (int i = 0; i < listm.Num(); i++)
		{
			TravioByNearOneMonth.time = listm[i]->AsObject()->GetStringField("time");
			TravioByNearOneMonth.timeNext = listm[i]->AsObject()->GetStringField("timeText");
			TravioByNearOneMonth.noDiscardTotalCount = listm[i]->AsObject()->GetStringField("noDiscardTotalCount");
			TravioByNearOneMonth.unauditCount = listm[i]->AsObject()->GetStringField("unauditCount");
			TravioByNearOneMonth.auditCount = listm[i]->AsObject()->GetStringField("auditCount");
			TravioByNearOneMonth.lightVioCount = listm[i]->AsObject()->GetStringField("lightVioCount");
			TravioByNearOneMonth.discardCount = listm[i]->AsObject()->GetStringField("discardCount");
			TravioByNearOneMonth.totalCount = listm[i]->AsObject()->GetStringField("totalCount");
			trafficData.TravioByNearOneMonth.Add(TravioByNearOneMonth);
		}
		TSharedPtr<FJsonObject> nearWeekCount = JsonObject->GetObjectField("NearWeekCount");
		NearWeekCount.todayCount= nearWeekCount->GetStringField("todayCount");
		NearWeekCount.nearWeekCount = nearWeekCount->GetStringField("nearWeekCount");
		NearWeekCount.MaxNearWeekCount =  FString::FromInt(JsonObject->GetIntegerField("MaxNearWeekCount"));
		NearWeekCount.MaxTodayCount = FString::FromInt(JsonObject->GetIntegerField("MaxTodayCount"));
		trafficData.NearWeekCount = NearWeekCount;
		

	}
	return trafficData;
}

FFaceData UFileDictOperationCollection::ReadFaceData(FString JsonValue)
{
	FFaceData data;
	FFaceItemData item;
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TSharedPtr<FJsonObject>  JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<TSharedPtr<FJsonValue>> list = JsonObject->GetArrayField("res");
		for (int i = 0; i < list.Num(); i++)
		{
			TSharedPtr<FJsonObject> objtemp = list[i]->AsObject();
			item.Id = objtemp->GetIntegerField("Id");
			item.alarmTime = objtemp->GetStringField("alarmTime");
			item.timeStamp = objtemp->GetNumberField("timeStamp");
			item.cameraName = objtemp->GetStringField("cameraName");
			item.position = objtemp->GetStringField("position");
			item.alarmId = objtemp->GetStringField("alarmId");
			item.humanId = objtemp->GetStringField("humanId");
			item.humanName = objtemp->GetStringField("humanName");
			item.matchHumanList = objtemp->GetStringField("matchHumanList");
			data.dataArr.Add(item);
		}
	}
	return data;
}

FFaceLibraryData UFileDictOperationCollection::ReadFaceLibraryData(FString JsonValue)
{
	FFaceLibraryData result;
	TArray<FHumanItem> itemArr;
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TSharedPtr<FJsonObject>  JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag)
	{
		result.age = JsonObject->GetIntegerField("age");
		result.alarmId = JsonObject->GetStringField("alarmId");
		result.alarmTime = JsonObject->GetStringField("alarmTime");
		result.bkgPicUrl = JsonObject->GetStringField("bkgPicUrl");
		result.cameraName = JsonObject->GetStringField("cameraName");
		result.cascadeAlarmId = JsonObject->GetIntegerField("cascadeAlarmId");
		result.controlId = JsonObject->GetIntegerField("controlId");
		result.enthnic = JsonObject->GetIntegerField("ethnic");
		result.facePicUrl = JsonObject->GetStringField("facePicUrl");
		result.glass = JsonObject->GetIntegerField("glass");
		result.humanId = JsonObject->GetStringField("humanId");
		result.humanName = JsonObject->GetStringField("humanName");
		TArray<TSharedPtr<FJsonValue>>list = JsonObject->GetArrayField("humans");
		for (int i = 0; i < list.Num(); i++)
		{
			FHumanItem temp;
			temp.address = list[i]->AsObject()->GetStringField("address");
			temp.birthday = list[i]->AsObject()->GetStringField("birthday");
			temp.cityId = list[i]->AsObject()->GetStringField("cityId");
			temp.cityName = list[i]->AsObject()->GetStringField("cityName");
			temp.credentialsNum = list[i]->AsObject()->GetStringField("credentialsNum");
			temp.credenttialsType = list[i]->AsObject()->GetIntegerField("credentialsType");
			temp.ethnic = list[i]->AsObject()->GetIntegerField("ethnic");
			temp.facePicUrl = list[i]->AsObject()->GetStringField("facePicUrl");
			temp.humanId = list[i]->AsObject()->GetStringField("humanId");
			temp.humanName = list[i]->AsObject()->GetStringField("humanName");
			temp.listLibId = list[i]->AsObject()->GetIntegerField("listLibId");
			temp.listLibName = list[i]->AsObject()->GetStringField("listLibName");
			temp.provinceID = list[i]->AsObject()->GetStringField("provinceId");
			temp.provinceName = list[i]->AsObject()->GetStringField("provinceName");
			temp.sex = list[i]->AsObject()->GetIntegerField("sex");
			temp.similarity = list[i]->AsObject()->GetNumberField("similarity");
			temp.smile = list[i]->AsObject()->GetIntegerField("smile");
			itemArr.Add(temp);
		}
		result.humansItems = itemArr;
		result.indexCode = JsonObject->GetStringField("indexCode");
		result.listLibId = JsonObject->GetIntegerField("listLibId");
		result.sex = JsonObject->GetIntegerField("sex");
		result.similarity = JsonObject->GetIntegerField("similarity");
		result.smile = JsonObject->GetIntegerField("smile");
		result.status = JsonObject->GetIntegerField("status");
	}
	return result;

}


FTrafficCrowdedAll UFileDictOperationCollection::DealTrafficCrowdedAllJson(FString JsonValue)
{

	FTrafficCrowdedAll all_data;

	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TSharedPtr<FJsonObject>  JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		all_data.isNormal = JsonObject->GetStringField("isNormal");
		all_data.sysTime = JsonObject->GetStringField("sysTime");
		all_data.total = JsonObject->GetIntegerField("total");

		TArray<TSharedPtr<FJsonValue>> list = JsonObject->GetArrayField("rows");
		for (int i = 0; i < list.Num(); i++)
		{
			TSharedPtr<FJsonObject> objtemp = list[i]->AsObject();
			FTrafficCrowded temp;
			temp.address = objtemp->GetStringField("address");
			temp.countyName = objtemp->GetStringField("countyName");
			temp.createTime = objtemp->GetStringField("createTime");
			temp.endAddr = objtemp->GetStringField("endAddr");
			temp.eventId = objtemp->GetStringField("eventId");
			temp.handleReason = objtemp->GetStringField("handleReason");
			temp.handleState = objtemp->GetIntegerField("handleState");
			temp.insertTime = objtemp->GetStringField("insertTime");
			temp.jamDist = objtemp->GetIntegerField("jamDist");
			temp.jamSpeed = objtemp->GetIntegerField("jamSpeed");
			temp.longTime = objtemp->GetIntegerField("longTime");
			temp.oStartAddr = objtemp->GetStringField("oStartAddr");
			temp.pubRunStatus = objtemp->GetIntegerField("pubRunStatus");
			temp.pubTime = objtemp->GetStringField("pubTime");
			temp.roadName = objtemp->GetStringField("roadName");
			temp.roadType = objtemp->GetIntegerField("roadType");
			temp.startAddr = objtemp->GetStringField("startAddr");
			FString xy_str = objtemp->GetStringField("xy");
			TArray<FString> strVec2_xy;
			xy_str.ParseIntoArray(strVec2_xy, TEXT(","), true);
			temp.xy.X = FCString::Atof(*strVec2_xy[0]);
			temp.xy.Y = FCString::Atof(*strVec2_xy[1]);
			all_data.rows.Add(temp);
		}
	}
	return all_data;
}

FTrafficCrowdedRoad UFileDictOperationCollection::DealTrafficCrowdedRoadJson(FString JsonValue)
{
	FTrafficCrowdedRoad road;

	FTrafficCrowdedRoadEvent road_event;
	TArray<FTrafficCrowdedMeshData> rows;
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TSharedPtr<FJsonObject>  JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		//event
		TSharedPtr<FJsonObject> event_temp = JsonObject->GetObjectField("event");
		road_event.roadName = event_temp->GetStringField("roadName");
		road_event.startAddr = event_temp->GetStringField("startAddr");
		road_event.startCounty = event_temp->GetStringField("startCounty");
		road_event.endAddr = event_temp->GetStringField("endAddr");
		road_event.effectCounty = event_temp->GetStringField("effectCounty");
		road_event.pubTime = event_temp->GetStringField("pubTime");
		road_event.jamDist = event_temp->GetIntegerField("jamDist");
		road_event.jamSpeed = event_temp->GetIntegerField("jamSpeed");
		road_event.longTime = event_temp->GetIntegerField("longTime");
		road_event.eventId = event_temp->GetStringField("eventId");


		//获取xy数据
		FString xy = event_temp->GetStringField("xy");
		TArray<FString> str_xy;
		xy.ParseIntoArray(str_xy, TEXT(","), true);
		FVector2D vect2_xy_temp;
		vect2_xy_temp.X = FCString::Atof(*str_xy[0]);
		vect2_xy_temp.Y = FCString::Atof(*str_xy[1]);
		road_event.xy = vect2_xy_temp;


		//rows
		TArray<TSharedPtr<FJsonValue>> list = JsonObject->GetArrayField("rows");

		for (int i = 0; i < list.Num(); i++)
		{
			TSharedPtr<FJsonObject> objectTemp = list[i]->AsObject();
			FTrafficCrowdedMeshData row_data;


			//获取xy数据
			FString xy = objectTemp->GetStringField("xy");
			TArray<FString> strVec2_xy;
			xy.ParseIntoArray(strVec2_xy, TEXT(","), true);
			FVector2D vect2_xy;
			vect2_xy.X = FCString::Atof(*strVec2_xy[0]);
			vect2_xy.Y = FCString::Atof(*strVec2_xy[1]);

			row_data.xy = vect2_xy;

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
				row_data.xysArray.Add(vect2);

			}
			rows.Add(row_data);
		}

		road.event = road_event;
		road.rows = rows;

	}
	return road;
}

FSys_WuLianArray UFileDictOperationCollection::ReadWuLianDataArray(FString JsonValue)
{
	FSys_WuLianArray info;

	//FString JsonValue;
	//bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<FSys_WuLianDetail> parts;
		TArray<TSharedPtr<FJsonValue>> camera_array = JsonObject->GetArrayField("res");
		if (camera_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject;
			for (int i = 0; i < camera_array.Num(); i++)
			{
				cameraObject = camera_array[i]->AsObject();

				FSys_WuLianDetail part;
				part.key = FString::FromInt(cameraObject->GetIntegerField("key"));
				part.commRmk = cameraObject->GetStringField("commRmk");
				part.dsnum = cameraObject->GetStringField("dsnum");
				//part.foreignld = cameraObject->GetStringField("foreignld");
				part.foreignld = "";
				part.mtype = cameraObject->GetStringField("mtype");
				part.projectId = cameraObject->GetStringField("projectId");
				part.time = cameraObject->GetStringField("time");
				part.userId = cameraObject->GetStringField("userId");
				part.topicType = cameraObject->GetStringField("topicType");
				part.timeStamp = FString::FromInt(cameraObject->GetNumberField("timeStamp"));

				parts.Add(part);
			}
		}
		info.res = parts;
	}

	return info;
}
void UFileDictOperationCollection::DealCamPeoPle(FString JsonValue, TArray<FCamPeople>& camPeoCountArray)
{
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<TSharedPtr<FJsonValue>> array = JsonObject->GetArrayField("data");
		TSharedPtr<FJsonObject> obj;
		for (int i = 0; i < array.Num(); i++)
		{
			obj = array[i]->AsObject();
			FCamPeople cam_people;
			cam_people.vec2d1.X = FCString::Atof(*(obj->GetStringField("x1")));
			cam_people.vec2d1.Y = FCString::Atof(*(obj->GetStringField("y1")));
			cam_people.vec2d2.X = FCString::Atof(*(obj->GetStringField("x2")));
			cam_people.vec2d2.Y = FCString::Atof(*(obj->GetStringField("y2")));
			cam_people.peoplenum = FCString::Atof(*(obj->GetStringField("peopleNum")));
			camPeoCountArray.Add(cam_people);
		}
	}
}



FSys_FaceCameraArray UFileDictOperationCollection::ReadFaceCameraData(FString _path)
{
	FSys_FaceCameraArray info;

	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);

	if (BFlag)
	{
		TArray<FSys_FaceCamera> parts;
		TArray<TSharedPtr<FJsonValue>> camera_array = JsonObject->GetArrayField("res");
		if (camera_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> cameraObject;
			for (int i = 0; i < camera_array.Num(); i++)
			{
				cameraObject = camera_array[i]->AsObject();

				FSys_FaceCamera part;
				part.id = cameraObject->GetStringField("id");
				part.location = cameraObject->GetStringField("location");
				part.ip = cameraObject->GetStringField("ip");

				parts.Add(part);
			}
		}
		info.res = parts;
	}

	return info;
}

FSIPSiMiaoCamera UFileDictOperationCollection::ReadSiMiaoCameraConfig(FString _path)
{
	FSIPSiMiaoCamera result;
	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag)
	{
		TArray<TSharedPtr<FJsonValue>> JsonArr = JsonObject->GetArrayField("res");
		for (int i = 0; i < JsonArr.Num(); i++)
		{
			FSIPSiMiaoCameraItem item;
			item.area = JsonArr[i]->AsObject()->GetStringField("area");
			item.SendUrl = JsonArr[i]->AsObject()->GetStringField("SendUrl");
			item.ConfigUrl = JsonArr[i]->AsObject()->GetStringField("ConfigUrl");
			item.CloseUrl = JsonArr[i]->AsObject()->GetStringField("CloseURL");
			item.SIPFile = JsonArr[i]->AsObject()->GetStringField("SIPFile");
			result.data.Add(item);
		}
	}
	return result;
}

FHongWaiConfigMap UFileDictOperationCollection::ReadHongWaiConfigMap(FString _path)
{
	FHongWaiConfigMap result;
	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TArray < TSharedPtr<FJsonValue>> JsonArr;

	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonArr);
	if (BFlag)
	{
		for (int i = 0; i < JsonArr.Num(); i++)
		{
			FHongWaiConfigMapItem item;
			item.ID = JsonArr[i]->AsObject()->GetStringField("ID");
			item.Name = JsonArr[i]->AsObject()->GetStringField("Name");
			result.data.Add(item);
		}
	}
	return result;
}



FHongWaiData UFileDictOperationCollection::ReadHongWaiData(FString JsonValue)
{
	FHongWaiData result;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag)
	{
		TArray<TSharedPtr<FJsonValue>> arr = JsonObject->GetArrayField("inList");
		for (int i = 0; i < arr.Num(); i++)
		{
			FSys_HongWaiPeopleData temp;
			temp.key = FString::FromInt(arr[i]->AsObject()->GetIntegerField("key"));
			temp.sn = arr[i]->AsObject()->GetStringField("sn");
			temp.type = arr[i]->AsObject()->GetStringField("type");
			temp.count = arr[i]->AsObject()->GetStringField("count");
			temp.timeStamp = FString::FromInt(arr[i]->AsObject()->GetNumberField("timeStamp"));
			temp.Year = arr[i]->AsObject()->GetStringField("Year");
			temp.Month = arr[i]->AsObject()->GetStringField("Month");
			temp.Day = arr[i]->AsObject()->GetStringField("Day");
			result.inList.Add(temp);
		}
		arr = JsonObject->GetArrayField("outList");
		for (int i = 0; i < arr.Num(); i++)
		{
			FSys_HongWaiPeopleData temp;
			temp.key = FString::FromInt(arr[i]->AsObject()->GetIntegerField("key"));
			temp.sn = arr[i]->AsObject()->GetStringField("sn");
			temp.type = arr[i]->AsObject()->GetStringField("type");
			temp.count = arr[i]->AsObject()->GetStringField("count");
			temp.timeStamp =FString::FromInt(arr[i]->AsObject()->GetNumberField("timeStamp"));
			temp.Year = arr[i]->AsObject()->GetStringField("Year");
			temp.Month = arr[i]->AsObject()->GetStringField("Month");
			temp.Day = arr[i]->AsObject()->GetStringField("Day");

			result.outList.Add(temp);
		}
	}
	return result;
}


FCarSingleStruct UFileDictOperationCollection::ReadCarSingleStruct(FString jsonValue)
{
	FCarSingleStruct result;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonValue);
	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag)
	{
		TSharedPtr<FJsonObject> JObject = JsonObject->GetObjectField("res");
		result.CarID = JObject->GetStringField("CarID");
		result.RecordSN = JObject->GetStringField("RecordSN");
		result.RecordTime = JObject->GetStringField("RecordTime");
		result.picName = JObject->GetStringField("picName");
	}
	return result;
}

FString UFileDictOperationCollection::CarSingleStructToString(FCarSingleStruct data)
{
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("CarID"), data.CarID);
	JsonWriter->WriteValue(TEXT("RecordSN"), data.RecordSN);
	JsonWriter->WriteValue(TEXT("RecordTime"), data.RecordTime);
	JsonWriter->WriteValue(TEXT("picName"), data.picName);
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
	return JsonStr;


}
FCarUpdateInfoStruct UFileDictOperationCollection::ReadCarUpdateInfoStruct(FString jsonValue)
{
	FCarUpdateInfoStruct result;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonValue);
	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag)
	{
		result.Time = JsonObject->GetStringField("Time");
		TArray<TSharedPtr<FJsonValue>> data = JsonObject->GetArrayField("Data");
		for (int i = 0; i < data.Num(); i++)
		{
			FCarUpdateInfoStructItem item;
			item.FolderName = data[i]->AsObject()->GetStringField("FolderName");
			item.AddTime = data[i]->AsObject()->GetStringField("AddTime");
			result.Data.Add(item);
		}
	}
	return result;
}
FString UFileDictOperationCollection::CarUpdateInfoStructToString(FCarUpdateInfoStruct data)
{
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("Time"), data.Time);
	JsonWriter->WriteArrayStart("Data");
	for (int i = 0; i < data.Data.Num(); i++)
	{
		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue(TEXT("FolderName"), data.Data[i].FolderName);
		JsonWriter->WriteValue(TEXT("AddTime"), data.Data[i].AddTime);
		JsonWriter->WriteObjectEnd();

	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
	return JsonStr;
}

FCarDetail UFileDictOperationCollection::ReadCarDetail(FString jsonValue)
{
	FCarDetail Result;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonValue);
	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag)
	{
		TArray<TSharedPtr<FJsonValue>> dataArr = JsonObject->GetArrayField("res");
		for (int i = 0; i < dataArr.Num(); i++)
		{
			FCarDetailItem temp;
			temp.id = dataArr[i]->AsObject()->GetStringField("Id");
			temp.RecordTime = dataArr[i]->AsObject()->GetStringField("RecordTime");
			temp.plateId = dataArr[i]->AsObject()->GetStringField("plateId");
			temp.RecordSN = dataArr[i]->AsObject()->GetStringField("RecordSN");
			temp.picName = dataArr[i]->AsObject()->GetStringField("picName");
			temp.url = dataArr[i]->AsObject()->GetStringField("url");
			temp.FolderName = dataArr[i]->AsObject()->GetStringField("FolderName");
			temp.Year = dataArr[i]->AsObject()->GetStringField("Year");
			temp.Month = dataArr[i]->AsObject()->GetStringField("Month");
			temp.Day = dataArr[i]->AsObject()->GetStringField("Day");
			temp.Hour = dataArr[i]->AsObject()->GetStringField("Hour");
			temp.Minute = dataArr[i]->AsObject()->GetStringField("Minute");
			temp.Second = dataArr[i]->AsObject()->GetStringField("Second");
			temp.timeStamp = dataArr[i]->AsObject()->GetStringField("timeStamp");



			temp.isBK = dataArr[i]->AsObject()->GetStringField("isBK");

			Result.data.Add(temp);



		}

	}
	return Result;
}


FHongWaiDeviation UFileDictOperationCollection::ReadHongWaiDeviation(FString _path)
{
	FHongWaiDeviation result;
	FString JsonValue;
	bool flag = FFileHelper::LoadFileToString(JsonValue, *_path);
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag && flag)
	{
		TArray < TSharedPtr<FJsonValue>> item = JsonObject->GetArrayField("OldAdjust");
		for (int i = 0; i < item.Num(); ++i)
		{
			FOldAdjust sevice;
			sevice.SN = item[i]->AsObject()->GetStringField("SN");
			sevice.InCount = item[i]->AsObject()->GetIntegerField("InCount");
			sevice.OutCount = item[i]->AsObject()->GetIntegerField("OutCount");
			result.OldAdjust.Add(sevice);
		}
		item = JsonObject->GetArrayField("AreaDayAdjust");
		for (int i = 0; i < item.Num(); ++i)
		{
			FAreaDayAdjust area;
			area.Area = item[i]->AsObject()->GetStringField("Area");
			area.AllIn = item[i]->AsObject()->GetIntegerField("AllIn");
			area.AllOut = item[i]->AsObject()->GetIntegerField("AllOut");
			area.AreaType = item[i]->AsObject()->GetIntegerField("AreaType");
			result.AreaDayAdjsut.Add(area);
		}
	}
	return result;
}



FSys_Warning110Array UFileDictOperationCollection::Parse110Data(FString data)
{
	FSys_Warning110Array arrayData;
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(data);

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		TArray<FSys_Warning110Data> parts;
		TArray<TSharedPtr<FJsonValue>> warningArray = JsonObject->GetArrayField("array");
		if (warningArray.Num() > 0)
		{
			TSharedPtr<FJsonObject> warnObject;
			for (int i = 0; i < warningArray.Num(); i++)
			{
				warnObject = warningArray[i]->AsObject();

				FSys_Warning110Data warnData;
				warnData.JJD_ID = warnObject->GetStringField("JJD_ID");
				warnData.AF_ADDR = warnObject->GetStringField("AF_ADDR");
				warnData.YEAR = warnObject->GetStringField("YEAR");
				warnData.MONTH = warnObject->GetStringField("MONTH");
				warnData.DAY = warnObject->GetStringField("DAY");
				warnData.HH = warnObject->GetStringField("HH");
				warnData.MM = warnObject->GetStringField("MM");
				warnData.SS = warnObject->GetStringField("SS");
				warnData.BJAY1 = warnObject->GetStringField("BJAY1");
				warnData.BJAY2 = warnObject->GetStringField("BJAY2");
				warnData.BJAY3 = warnObject->GetStringField("BJAY3");
				warnData.BJAY4 = warnObject->GetStringField("BJAY4");
				warnData.BJAY5 = warnObject->GetStringField("BJAY5");
				warnData.FKAY1 = warnObject->GetStringField("FKAY1");
				warnData.FKAY2 = warnObject->GetStringField("FKAY2");
				warnData.FKAY3 = warnObject->GetStringField("FKAY3");
				warnData.FKAY4 = warnObject->GetStringField("FKAY4");
				warnData.FKAY5 = warnObject->GetStringField("FKAY5");
				warnData.QY = warnObject->GetStringField("QY");
				warnData.KYE_AREAS = warnObject->GetStringField("KYE_AREAS");
				warnData.ROAD = warnObject->GetStringField("ROAD");
				warnData.JJY_NAME = warnObject->GetStringField("JJY_NAME");
				warnData.JJY_ID = warnObject->GetStringField("JJY_ID");
				warnData.CJDW = warnObject->GetStringField("CJDW");
				warnData.CJY_NAME = warnObject->GetStringField("CJY_NAME");
				warnData.CJY_ID = warnObject->GetStringField("CJY_ID");
				warnData.COMMET = warnObject->GetStringField("COMMET");
				warnData.AMAP_GPS_X = warnObject->GetStringField("AMAP_GPS_X");
				warnData.AMAP_GPS_Y = warnObject->GetStringField("AMAP_GPS_Y");

				parts.Add(warnData);

				//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, warnData.JJD_ID);
			}
		}

		arrayData.W110Arrays = parts;
	}

	return arrayData;
}