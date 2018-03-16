// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ProjectJsonData.generated.h"


//110派出所信息
USTRUCT(BlueprintType)
struct FSys_110AreaData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString AreaName;	//派出所名称

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString AllDayCount;	//全天报警案件数

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Present; //警报百分比
};


USTRUCT(BlueprintType)
struct FSys_110AreaDataArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_110AreaData> area110Array;
};



//重点对象信息
USTRUCT(BlueprintType)
struct FSys_WarningPeopleData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString ID;	//乘客姓名

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString PassageName;	//乘客姓名

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString PassageID; //身份证

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString PassageType; //乘客类型：吸毒，涉毒...

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString PassageState; //乘客状态

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString GoTime;	//出发时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString GoDate;	//出发时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString GoLocation;	//出发地点

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString ToLocation;	//目的地

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString SeatNo;	//座位号

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TicketTime;	//售票时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TicketDate;	//售票时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString CheckTime;	//比中时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString CheckInName;	//比中时间
};

//重点对象信息
USTRUCT(BlueprintType)
struct FSys_WarningPeopleArray
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_WarningPeopleData> warningPeopleArray;
};


#pragma region wifi

//wifi嗅探数据包
USTRUCT(BlueprintType)
struct FSys_WifiBoxData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString COLLECTION_EQUIPMENT_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString AllDeviceCount;
};
USTRUCT(BlueprintType)
//URL
struct FSys_URLData
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString URL;

};
USTRUCT(BlueprintType)
struct FSys_WifiBoxArray
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_WifiBoxData> wifiBoxArray;
};


#pragma endregion


#pragma region Warning110
//单条110警情信息
USTRUCT(BlueprintType)
struct FSys_Warning110Data
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString JJD_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString AF_ADDR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString YEAR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString MONTH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString DAY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString HH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString MM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString SS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString BJAY1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString BJAY2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString BJAY3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString BJAY4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString BJAY5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString FKAY1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString FKAY2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString FKAY3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString FKAY4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString FKAY5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString QY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString KYE_AREAS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString ROAD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString JJY_NAME;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString JJY_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString CJDW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString CJY_NAME;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString CJY_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString COMMET;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString AMAP_GPS_X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString AMAP_GPS_Y;
};

USTRUCT(BlueprintType)
struct FSys_Warning110Array
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_Warning110Data> W110Arrays;
};
#pragma endregion


#pragma region AreaData
//功能点信息
USTRUCT(BlueprintType)
struct FSys_FeatureBoxData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 FBoxType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString FBoxOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float Widgth;
};

//隔断，人流信息
USTRUCT(BlueprintType)
struct FSys_BlockerStreamData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 BlockType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FVector Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float Length;
};

//警车信息
USTRUCT(BlueprintType)
struct FSys_PoliceVehicleData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 VechicleType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float LocationX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float LocationY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float LocationZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float RotationX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float RotationY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float RotationZ;
};

//构成
USTRUCT(BlueprintType)
struct FSys_UnitMember
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString MemberName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString MemberCount;
};

// 小队
USTRUCT(BlueprintType)
struct FSys_UnitPolice
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 UnitCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString UnitLeader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString UnitLeaderPhone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString UnitCall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString UnitColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 Floor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float UnitFlagPosX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float UnitFlagPosY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float UnitFlagPosZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		TArray<FSys_UnitMember> MembersArray;
};

//大队
USTRUCT(BlueprintType)
struct FSys_PoliceTeam
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TeamName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 TeamCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TeamLeader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TeamLeaderPhone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TeamCall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TeamColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float TeamFlagPosX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float TeamFlagPosY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		float TeamFlagPosZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		TArray<FSys_UnitPolice> UnitsArray;
};

USTRUCT(BlueprintType)
struct FSys_Traffic
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString indexM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString displayName;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
	//TArray<FSys_Traffic> trafficDeltailInfo;

};

USTRUCT(BlueprintType)
struct Fsys_CameraData
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString Label;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString Longitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString Latitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString High1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString High2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString CameraID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString Notice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraConfig")
		FString Axis;
};


#pragma endregion



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHSECURITYSYS_API UProjectJsonData : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProjectJsonData();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "CameraConfig")
		void ReadCameraConfig(FString _path, TArray<Fsys_CameraData> &CameraDataArray);
	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FSys_PoliceTeam ReadPoliceTeamInfo(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WritePoliceTeamInfo(FString _path, FSys_PoliceTeam policeTeamInfo);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FSys_PoliceVehicleData ReadPoliceVehicleInfo(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WritePoliceVehicleInfo(FString _path, FSys_PoliceVehicleData vehicleInfo);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FSys_BlockerStreamData ReadBlockerStreamInfo(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WriteBlockerStreamInfo(FString _path, FSys_BlockerStreamData info);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FSys_FeatureBoxData ReadFeatureBoxInfo(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WriteFeatureBoxInfo(FString _path, FSys_FeatureBoxData info);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FSys_URLData ReadURLData(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WriteURLData(FString _path, FSys_URLData info);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WriteTraffic(FString _path, TArray<FSys_Traffic> TrafficInfoArray);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void ReadTrafficInfo(FString _path, TArray<FSys_Traffic> &trafficinfoArray, bool &flag);
};
