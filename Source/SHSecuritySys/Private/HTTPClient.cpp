// Fill out your copyright notice in the Description page of Project Settings.

#include "HTTPClient.h"
#include "ProjectJsonData.h"
#include "Sys_PC.h"
#include "Json.h"
#include "FileDictOperationCollection.h"

// Sets default values for this component's properties
UHTTPClient::UHTTPClient()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//When the object is constructed, Get the HTTP module
	Http = &FHttpModule::Get();
}

/*Http call*/
void UHTTPClient::MyHttpCall(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponseReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

/*Assigned function on successfull http call*/
void UHTTPClient::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FSys_Warning110Array arrayData;
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}


	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

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
				warnObject->TryGetStringField("JJD_ID",warnData.JJD_ID);
				warnObject->TryGetStringField("AF_ADDR", warnData.AF_ADDR);
				warnObject->TryGetStringField("YEAR", warnData.YEAR);
				warnObject->TryGetStringField("MONTH", warnData.MONTH);
				warnObject->TryGetStringField("DAY", warnData.DAY);
				warnObject->TryGetStringField("HH", warnData.HH);
				warnObject->TryGetStringField("MM", warnData.MM);
				warnObject->TryGetStringField("SS", warnData.SS);
				warnObject->TryGetStringField ("BJAY1", warnData.BJAY1);
				warnObject->TryGetStringField("BJAY2",warnData.BJAY2);
				warnObject->TryGetStringField("BJAY3", warnData.BJAY3);
				warnObject->TryGetStringField("BJAY4", warnData.BJAY4);
				warnObject->TryGetStringField("BJAY5", warnData.BJAY5);
				warnObject->TryGetStringField("FKAY1", warnData.FKAY1);
				warnObject->TryGetStringField("FKAY2", warnData.FKAY2);
				warnObject->TryGetStringField("FKAY3", warnData.FKAY3);
				warnObject->TryGetStringField("FKAY4", warnData.FKAY4);
				warnObject->TryGetStringField("FKAY5", warnData.FKAY5);
				warnObject->TryGetStringField("QY", warnData.QY);
				warnObject->TryGetStringField("KYE_AREAS", warnData.KYE_AREAS);
				warnObject->TryGetStringField("ROAD", warnData.ROAD);
				warnObject->TryGetStringField("JJY_NAME", warnData.JJY_NAME);
				warnObject->TryGetStringField("JJY_ID", warnData.JJY_ID);
				warnObject->TryGetStringField("CJDW", warnData.CJDW);
				warnObject->TryGetStringField("CJY_NAME", warnData.CJY_NAME);
				warnObject->TryGetStringField("CJY_ID", warnData.CJY_ID);
				warnObject->TryGetStringField("COMMET", warnData.COMMET);
				warnObject->TryGetStringField("AMAP_GPS_X", warnData.AMAP_GPS_X);
				warnObject->TryGetStringField("AMAP_GPS_Y", warnData.AMAP_GPS_Y);

				parts.Add(warnData);

				//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, warnData.JJD_ID);
			}
		}

		arrayData.W110Arrays = parts;

		if (pc != nullptr)
		{
			pc->recieved110warningMessage(arrayData);
		}
	}

	//(new FAutoDeleteAsyncTask<Parse110DataAsyncTask>(Response->GetContentAsString(),pc))->StartBackgroundTask();
}


void UHTTPClient::MyHttpWifiCall(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponseWifiReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

/*Assigned function on successfull http call*/
void UHTTPClient::OnResponseWifiReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FSys_WifiBoxArray arrayData;
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		TArray<FSys_WifiBoxData> parts;
		TArray<TSharedPtr<FJsonValue>> warningArray = JsonObject->GetArrayField("array");
		if (warningArray.Num() > 0)
		{
			TSharedPtr<FJsonObject> warnObject;
			for (int i = 0; i < warningArray.Num(); i++)
			{
				warnObject = warningArray[i]->AsObject();

				FSys_WifiBoxData wifiData;
				wifiData.COLLECTION_EQUIPMENT_ID = warnObject->GetStringField("COLLECTION_EQUIPMENT_ID");
				wifiData.AllDeviceCount = warnObject->GetStringField("AllDeviceCount");

				parts.Add(wifiData);
			}
		}

		arrayData.wifiBoxArray = parts;

		if (pc != nullptr)
		{
			pc->recievedWifiBoxMessage(arrayData);
		}
	}
}


void UHTTPClient::Http110PhoneSearchCall(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponse110PhoneSearchReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

/*Assigned function on successfull http call*/
void UHTTPClient::OnResponse110PhoneSearchReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FSys_Warning110Array arrayData;
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

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

		if (pc != nullptr)
		{
			pc->recieved110PhoneSearchResultMessage(arrayData);
		}
	}
}

void UHTTPClient::Http110KeywordSearchCall(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponse110KeywordSearchReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

/*Assigned function on successfull http call*/
void UHTTPClient::OnResponse110KeywordSearchReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FSys_Warning110Array arrayData;
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}


	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

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

		if (pc != nullptr)
		{
			pc->recieved110KeywordResultMessage(arrayData);
		}
	}
}




void UHTTPClient::HttpWarningPeopleInit(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponseWarningPeopleInitReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

/*Assigned function on successfull http call*/
void UHTTPClient::OnResponseWarningPeopleInitReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FSys_WarningPeopleArray arrayData;
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		TArray<FSys_WarningPeopleData> parts;
		TArray<TSharedPtr<FJsonValue>> warningArray = JsonObject->GetArrayField("array");
		if (warningArray.Num() > 0)
		{
			TSharedPtr<FJsonObject> warnObject;
			for (int i = 0; i < warningArray.Num(); i++)
			{
				warnObject = warningArray[i]->AsObject();

				FSys_WarningPeopleData warnData;
				warnData.PassageName = warnObject->GetStringField("PassageName");
				warnData.PassageID = warnObject->GetStringField("PassageID");
				warnData.PassageType = warnObject->GetStringField("PassageType");
				warnData.PassageState = warnObject->GetStringField("PassageState");
				warnData.GoTime = warnObject->GetStringField("GoTime");
				warnData.GoLocation = warnObject->GetStringField("GoLocation");
				warnData.ToLocation = warnObject->GetStringField("ToLocation");
				warnData.SeatNo = warnObject->GetStringField("SeatNo");
				warnData.TicketTime = warnObject->GetStringField("TicketTime");
				warnData.CheckTime = warnObject->GetStringField("CheckTime");
				warnData.ID = warnObject->GetStringField("ID");
				warnData.GoDate = warnObject->GetStringField("GoDate");
				warnData.TicketDate = warnObject->GetStringField("TicketDate");
				warnData.CheckInName = warnObject->GetStringField("CheckInName");

				parts.Add(warnData);
			}
		}

		arrayData.warningPeopleArray = parts;

		if (pc != nullptr)
		{
			pc->recievedWarningPeopleInit(arrayData);
		}
	}
}



void UHTTPClient::HttpWarningPeopleNew(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponseWarningPeopleNewReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

/*Assigned function on successfull http call*/
void UHTTPClient::OnResponseWarningPeopleNewReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FSys_WarningPeopleArray arrayData;
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		TArray<FSys_WarningPeopleData> parts;
		TArray<TSharedPtr<FJsonValue>> warningArray = JsonObject->GetArrayField("array");
		if (warningArray.Num() > 0)
		{
			TSharedPtr<FJsonObject> warnObject;
			for (int i = 0; i < warningArray.Num(); i++)
			{
				warnObject = warningArray[i]->AsObject();

				FSys_WarningPeopleData warnData;
				warnData.PassageName = warnObject->GetStringField("PassageName");
				warnData.PassageID = warnObject->GetStringField("PassageID");
				warnData.PassageType = warnObject->GetStringField("PassageType");
				warnData.PassageState = warnObject->GetStringField("PassageState");
				warnData.GoTime = warnObject->GetStringField("GoTime");
				warnData.GoLocation = warnObject->GetStringField("GoLocation");
				warnData.ToLocation = warnObject->GetStringField("ToLocation");
				warnData.SeatNo = warnObject->GetStringField("SeatNo");
				warnData.TicketTime = warnObject->GetStringField("TicketTime");
				warnData.CheckTime = warnObject->GetStringField("CheckTime");
				warnData.ID = warnObject->GetStringField("ID");
				warnData.GoDate = warnObject->GetStringField("GoDate");
				warnData.TicketDate = warnObject->GetStringField("TicketDate");
				warnData.CheckInName = warnObject->GetStringField("CheckInName");

				parts.Add(warnData);
			}
		}

		arrayData.warningPeopleArray = parts;

		if (pc != nullptr)
		{
			pc->recievedWarningPeopleNew(arrayData);
		}
	}
}



void UHTTPClient::HttpArea110Info(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponse110AreaInfoReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}



/*Assigned function on successfull http call*/
void UHTTPClient::OnResponse110AreaInfoReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FSys_110AreaDataArray arrayData;
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		TArray<FSys_110AreaData> parts;

		const TArray<TSharedPtr<FJsonValue>> *warningArray;
		
		if (JsonObject->TryGetArrayField("array", warningArray))
		{
			for (auto warning : *warningArray)
			{
				if (!warning.IsValid())
				{
					continue;
				}

				auto warningObject = warning->AsObject();
				if (!warningObject.IsValid())
				{
					continue;
				}

				FSys_110AreaData warnData;
				warnData.AreaName = warningObject->GetStringField("AreaName");
				warnData.AllDayCount = warningObject->GetStringField("AllDayCount");
				warnData.Present = warningObject->GetStringField("Present");

				parts.Add(warnData);
			}
		}


		/*
		if (warningArray->Num() > 0)
		{
			const TArray<TSharedPtr<FJsonValue>> warningTamp = *warningArray;

			TSharedPtr<FJsonObject> warnObject;
			for (int i = 0; i < warningTamp.Num(); i++)
			{
				warnObject = warningTamp[i]->AsObject();

				FSys_110AreaData warnData;
				warnData.AreaName = warnObject->GetStringField("AreaName");
				warnData.AllDayCount = warnObject->GetStringField("AllDayCount");
				warnData.Present = warnObject->GetStringField("Present");

				parts.Add(warnData);
			}
		}
		*/

		arrayData.area110Array = parts;

		if (pc != nullptr)
		{
			pc->recieved110AreaInfoMsg(arrayData);
		}
	}
}



void UHTTPClient::HttpTrafficInfo(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponseTrafficInfoReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}



/*Assigned function on successfull http call*/
void UHTTPClient::OnResponseTrafficInfoReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());


	FSys_TrafficStatus info;
	bool BFlag = FJsonSerializer::Deserialize(Reader, JsonObject);

	if (BFlag)
	{
		info.TrafficDataForAll = JsonObject->GetStringField("TrafficDataForAll");
		info.TrafficAvgSpeed = JsonObject->GetStringField("TrafficAvgSpeed");

		//Add Unit Team-->
		TArray<FSys_TrafficRoadStatus> parts;
		const TArray<TSharedPtr<FJsonValue>> * police_array;	
		if (JsonObject->TryGetArrayField("TopsRoads", police_array))
		{
			TSharedPtr<FJsonObject> PoliceObject;
			for (auto item : *police_array)
			{
				PoliceObject = item->AsObject();
				if (!PoliceObject.IsValid())
				{
					continue;
				}
				FSys_TrafficRoadStatus part;
				PoliceObject->TryGetStringField("RoadName", part.RoadName);
				PoliceObject->TryGetStringField("TrafficAvgSpeed", part.TrafficAvgSpeed);
				PoliceObject->TryGetStringField("TrafficData", part.TrafficData);

				parts.Add(part);
			}
		/*	for (int i = 0; i < police_array.Num(); i++)
			{
				PoliceObject = police_array[i]->AsObject();

				FSys_TrafficRoadStatus part;
				part.RoadName = PoliceObject->GetStringField("RoadName");
				part.TrafficAvgSpeed = PoliceObject->GetStringField("TrafficAvgSpeed");
				part.TrafficData = PoliceObject->GetStringField("TrafficData");

				parts.Add(part);
			}*/
		}
		info.TopsRoads = parts;

		if (pc != nullptr)
		{
			pc->recievedTrafficData(info);
		}
	}



}


void UHTTPClient::HttpTWeatherInfo(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponseWeatherInfoReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}



/*Assigned function on successfull http call*/
void UHTTPClient::OnResponseWeatherInfoReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

	bool BFlag = FJsonSerializer::Deserialize(Reader, JsonObject);

	FSys_WeartherData info;

	if (BFlag)
	{
		info.airQuality = JsonObject->GetStringField("airQuality");
		info.bigTemperature = JsonObject->GetIntegerField("bigTemperature");
		info.humidity = JsonObject->GetIntegerField("humidity");
		info.smallTemperature = JsonObject->GetIntegerField("smallTemperature");
		info.weather = JsonObject->GetStringField("weather");
		info.wind = JsonObject->GetStringField("wind");


		if (pc != nullptr)
		{
			pc->recievedWeatherData(info);
		}
	}
}

void UHTTPClient::HttpKaKouInfo(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponseKaKouInfoReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}



/*Assigned function on successfull http call*/
void UHTTPClient::OnResponseKaKouInfoReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

	bool BFlag = FJsonSerializer::Deserialize(Reader, JsonObject);
	FSys_KaKouStatus info;
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

		if (pc != nullptr)
		{
			pc->recievedKaKouData(info);
		}
	}
}


void UHTTPClient::HttpPeopleWifiInfo(FString url)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPClient::OnResponsePeopleWifiReceived);
	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}



/*Assigned function on successfull http call*/
void UHTTPClient::OnResponsePeopleWifiReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	if (!Response.IsValid() || !bWasSuccessful)
	{
		return;
	}

	//Create a reader pointer to read the json data
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

	bool BFlag = FJsonSerializer::Deserialize(Reader, JsonObject);
	FSys_PeopleWifiData info;
	if (BFlag)
	{
		//Add Unit Team-->
		TArray<FSys_PeopleWifiDetailData> parts;
		TArray<TSharedPtr<FJsonValue>> police_array = JsonObject->GetArrayField("peopleList");
		if (police_array.Num() > 0)
		{
			TSharedPtr<FJsonObject> PoliceObject;
			for (int i = 0; i < police_array.Num(); i++)
			{
				PoliceObject = police_array[i]->AsObject();

				FSys_PeopleWifiDetailData part;
				part.wifiID = PoliceObject->GetStringField("wifiID");
				part.count = PoliceObject->GetStringField("count");

				parts.Add(part);
			}
		}
		info.peopleList = parts;

		if (pc != nullptr)
		{
			pc->recievedPeopleWifiData(info);
		}
	}
}

void UHTTPClient::ParseSiMiaoData(FString JsonData)
{
	(new FAutoDeleteAsyncTask<ParseSiMiaoDataAsyncTask>(JsonData, pc))->StartBackgroundTask();
}

void UHTTPClient::Parse110Data(FString JsonData)
{
	(new FAutoDeleteAsyncTask<Parse110DataAsyncTask>(JsonData, pc))->StartBackgroundTask();
}