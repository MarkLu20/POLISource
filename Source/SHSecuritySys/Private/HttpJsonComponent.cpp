// Fill out your copyright notice in the Description page of Project Settings.

#include "HttpJsonComponent.h"
#include "Json.h"

// Sets default values for this component's properties
UHttpJsonComponent::UHttpJsonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	isStart = false;
	tnowTime = 0.0f;
	// ...
}


// Called when the game starts
void UHttpJsonComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UHttpJsonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isStart) {
		tnowTime += DeltaTime;
		if (tnowTime >= dTime) {
			tnowTime = 0.0f;
			FetchURL();
		}
	}
}

void UHttpJsonComponent::Init(FString url, float deltaTime) {
	dTime = deltaTime;
	URL = url;
}

void UHttpJsonComponent::Start() {
	isStart = true;
}

void UHttpJsonComponent::FetchURL() {
	TSharedRef<IHttpRequest> HttpReuest = FHttpModule::Get().CreateRequest();
	HttpReuest->SetURL(URL);
	HttpReuest->SetVerb(TEXT("GET"));
	HttpReuest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpReuest->OnProcessRequestComplete().BindUObject(this, &UHttpJsonComponent::OnRequestComplete);
	//HttpReuest->OnRequestProgress().BindRaw(this, &AXCWebTestActor::OnRequestProgress);
	HttpReuest->ProcessRequest();
}

void UHttpJsonComponent::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{

	if (!HttpRequest.IsValid() || !HttpResponse.IsValid())
	{
		return;
	}
	else if (bSucceeded && EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
	{

		FString Filename = FPaths::GetCleanFilename(HttpRequest->GetURL());
		//write file
		//FFileHelper::SaveArrayToFile(HttpResponse->GetContent(), *FString::Printf(TEXT("D://xcJson.txt")));

		////////////////////
		TSharedPtr<FJsonObject> JsonRoot;
		TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(HttpResponse->GetContentAsString());

		//Deserialize the json data given Reader and the actual object to deserialize
		if (FJsonSerializer::Deserialize(Reader, JsonRoot))
		{
			FSys_TrafficStatus re;
			re.TrafficDataForAll = JsonRoot->GetStringField("TrafficDataForAll");
			re.TrafficAvgSpeed = JsonRoot->GetStringField("TrafficAvgSpeed");
			TArray<FSys_TrafficRoadStatus> reArr;
			auto arr = JsonRoot->GetArrayField("TopsRoads");
			for (auto& iter : arr) {
				auto temobj = iter->AsObject();
				FSys_TrafficRoadStatus tEle;
				tEle.RoadName = temobj->GetStringField("RoadName");
				tEle.TrafficAvgSpeed= temobj->GetStringField("TrafficAvgSpeed");
				tEle.TrafficData=temobj->GetStringField("TrafficData");
				reArr.Add(tEle);
			}
			re.TopsRoads = reArr;
			recievedTrafficStatusJson(re);
		}
	}
}