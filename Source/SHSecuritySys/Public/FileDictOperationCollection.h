// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectJsonData.h"
#include "FileDictOperationCollection.generated.h"


//车辆单条记录
USTRUCT(BlueprintType)
struct FCarSingleStruct
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString CarID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString RecordSN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString RecordTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString picName;

};
//车辆增量更新Item
USTRUCT(BlueprintType)
struct FCarUpdateInfoStructItem
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString FolderName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString AddTime;

};
//车辆增量更新
USTRUCT(BlueprintType)
struct FCarUpdateInfoStruct
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		TArray<FCarUpdateInfoStructItem> Data;
};


//车辆细节记录
USTRUCT(BlueprintType)
struct FCarDetailItem
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString RecordTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString plateId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString	RecordSN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString picName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString url;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString isBK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString FolderName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString Year;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString Month;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString Day;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString Hour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString Minute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString Second;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		FString timeStamp;
};

//车辆细节记录
USTRUCT(BlueprintType)
struct FCarDetail
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Car")
		TArray<FCarDetailItem> data;
};

//SysSettings

USTRUCT(BlueprintType)
struct FSys_HongWaiPeopleData
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString sn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString timeStamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Year;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Month;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Day;
};

//寺庙摄像头配置
USTRUCT(BlueprintType)
struct FSIPSiMiaoCameraItem
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiMiao")
		FString area;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiMiao")
		FString SendUrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiMiao")
		FString ConfigUrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiMiao")
		FString CloseUrl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiMiao")
		FString SIPFile;
};

USTRUCT(BlueprintType)
struct FSIPSiMiaoCamera
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiMiao")
			TArray<FSIPSiMiaoCameraItem> data;
};

//红外设备配置表
USTRUCT(BlueprintType)
struct FHongWaiConfigMapItem
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiMiao")
		FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiMiao")
		FString Name;
};
USTRUCT(BlueprintType)
struct FHongWaiConfigMap
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SiMiao")
		TArray<FHongWaiConfigMapItem> data;
};

USTRUCT(BlueprintType)
struct FSys_SystemSettingConfig
{
	GENERATED_BODY()

		//SystemConfig:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString DebugMode;

	//110:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Info110ConfigURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Update110Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Search110ByPhoneURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Search110ByKeyboardURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Update110AreaInfoURL;

	//Wifi
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString WifiInfoConfigURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString WifiInfoUpdateTime;


	//Warning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString WarningInitURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString WarningNewURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString WarningUpdateTime;

	//webURL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString LinkURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString YanPan110Link;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString SuperSearchLink;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString PeopleInfoLink;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString WorkInfoLink;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString GaoDeLink;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString rtspLink;
};

//Camera SIP
USTRUCT(BlueprintType)
struct FSys_SIPOne
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString IP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Port;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Route;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString URLSend;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString URL;
};

//红外数据
USTRUCT(BlueprintType)
struct FHongWaiData
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_HongWaiPeopleData> inList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_HongWaiPeopleData> outList;
};

USTRUCT(BlueprintType)
struct FSys_SIPArray
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_SIPOne> SIPConfig;
};

//Camera SIP
USTRUCT(BlueprintType)
struct FSys_FaceCamera
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString ip;
};

USTRUCT(BlueprintType)
struct FSys_FaceCameraArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_FaceCamera> res;
};

//Camera SIP
USTRUCT(BlueprintType)
struct FSys_WuLianDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString commRmk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString dsnum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString foreignld;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString mtype;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString projectId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString topicType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString timeStamp;
};

USTRUCT(BlueprintType)
struct FSys_WuLianArray
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_WuLianDetail> res;
};

//PeopleDay
USTRUCT(BlueprintType)
struct FSys_DayData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H11;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H13;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H14;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H16;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H17;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H18;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H19;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H21;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H22;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString H23;*/
};

USTRUCT(BlueprintType)
struct FSys_PeopleDayData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FSys_DayData res;
};

//设备误差校正数据
USTRUCT(BlueprintType)
struct FOldAdjust
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HongWaiAdjust")
		FString SN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HongWaiAdjust")
		int InCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HongWaiAdjust")
		int OutCount;
};

USTRUCT(BlueprintType)
struct FAreaDayAdjust
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HongWaiAdjust")
		int AreaType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HongWaiAdjust")
		FString Area;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HongWaiAdjust")
		int AllIn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HongWaiAdjust")
		int AllOut;
	
};

USTRUCT(BlueprintType)
struct FHongWaiDeviation
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HongWaiAdjust")
		TArray<FOldAdjust> OldAdjust;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HongWaiAdjust")
		TArray<FAreaDayAdjust> AreaDayAdjsut;
};

//Camera Sign
USTRUCT(BlueprintType)
struct FSys_CameraSign
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Long;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Lat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString High1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString High2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString CameraID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Axis;
};

USTRUCT(BlueprintType)
struct FSys_CameraSignArray
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_CameraSign> CameraArray;
};

/**
*这里数据众PoliceArray会与PoliceInfo匹配（PoliceArray的PoliceID对应PoliceInfo中的StandID），能匹配到的只有战区和特警，不能匹配到的姓名和电话号码均为空
*Type 0代表战区 1代表特警 2代表PTU 3代表交警其他代表巡警(这里以-1定义)
*/
//GPS
USTRUCT(BlueprintType)
struct FSys_PoliceGPS
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString PoliceID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString GPS_X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString GPS_Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Timestamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Year;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Month;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Day;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString HH;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString MM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString SS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString StandID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Phone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int Type;
	
};

USTRUCT(BlueprintType)
struct FSys_PoliceGPSArray
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_PoliceGPS> PoliceArray;
};


//WifiList
USTRUCT(BlueprintType)
struct FSys_WifiListSingle
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString lang;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString lat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString owner;
};

USTRUCT(BlueprintType)
struct FSys_WifiListTable
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_WifiListSingle> WifiResult;
};

//KaKouList
USTRUCT(BlueprintType)
struct FSys_KaKouListSingle
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString lang;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString lat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString owner;
};

USTRUCT(BlueprintType)
struct FSys_KaKouListTable
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_KaKouListSingle> KaKouResult;
};

USTRUCT(BlueprintType)
struct FSys_KaKouDataOne
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString SBBH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString SBMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString XSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString pass_or_out;
};

USTRUCT(BlueprintType)
struct FSys_KaKouDataArray
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_KaKouDataOne> JinArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_KaKouDataOne> ChuArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_KaKouDataOne> AllArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_KaKouDataOne> TopArray;
};


//Traffic Status
USTRUCT(BlueprintType)
struct FSys_TrafficRoadStatus
{
	GENERATED_BODY()

		//Traffic Data:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString RoadName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TrafficAvgSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TrafficData;
};

USTRUCT(BlueprintType)
struct FSys_TrafficStatus
{
	GENERATED_BODY()

		//Traffic Data:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TrafficDataForAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString TrafficAvgSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_TrafficRoadStatus> TopsRoads;
};

//PeopleWifi
USTRUCT(BlueprintType)
struct FSys_PeopleWifiDetailData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString wifiID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString count;
};

USTRUCT(BlueprintType)
struct FSys_PeopleWifiData
{
	GENERATED_BODY()


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_PeopleWifiDetailData> peopleList;
};


//Traffic Status
USTRUCT(BlueprintType)
struct FSys_KaKouDetailStatus
{
	GENERATED_BODY()

		//Traffic Data:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString SBMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString SBBH;
};

USTRUCT(BlueprintType)
struct FSys_KaKouStatus
{
	GENERATED_BODY()

		//Traffic Data:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 TotalCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_KaKouDetailStatus> KaKous;
};



//Wearther Status
USTRUCT(BlueprintType)
struct FSys_WeartherData
{
	GENERATED_BODY()

		//Wearther Data:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString airQuality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 bigTemperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 humidity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		int32 smallTemperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString weather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString wind;
};
USTRUCT(BlueprintType)
struct FSys_TrafficWeek
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString todayCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString nearWeekCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString MaxNearWeekCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString MaxTodayCount;
};

USTRUCT(BlueprintType)
struct FSys_TrafficStatlist
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString totalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString color;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString overLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString noDiscardTotalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString unauditCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString auditCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString lightVioCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString discardCount;
};

USTRUCT(BlueprintType)
struct FSys_TrafficDatatravio
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString timeNext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString noDiscardTotalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString unauditCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString auditCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString lightVioCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString discardCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FString totalCount;


};
USTRUCT(BlueprintType)
struct FSys_TrafficData
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_TrafficStatlist> TraffictravioByLocationNew;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_TrafficDatatravio> TravioByTodayEachHour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		TArray<FSys_TrafficDatatravio> TravioByNearOneMonth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SysData")
		FSys_TrafficWeek NearWeekCount;



};
/**
* The specific road crowded state
* jamDist 拥堵距离
* jamDist：拥堵距离 2813米；
* jamSpeed: 时速 10km/h
* longTime：持续时间 90分钟
* pubRunStatus：趋势状态 1：拥堵 2.趋向严重
* pubTime: 发布时间
* roadName： 发生路段 延安高架路
* roadType:高速，非高速，轮渡，桥梁，隧道 7：普通道路 6：高架路
* xy: GPS
*/
USTRUCT(BlueprintType)
struct FTrafficCrowded {
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString address;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString countyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString createTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString endAddr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString eventId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString handleReason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int handleState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString insertTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int jamDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int jamSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int longTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString oStartAddr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int pubRunStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString pubTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString roadName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int roadType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString startAddr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D xy;
};

/**
* 获取所有的拥堵信息
*/
USTRUCT(BlueprintType)
struct FTrafficCrowdedAll
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString isNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTrafficCrowded> rows;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString sysTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int total;
};


/**
* 拥堵的具体路段事件
* endAddr 终点
* startAddr 起点
* effectCounty 影响辖区
* pubTime 发布时间
* jamDist 拥堵距离
* jamSpeed 拥堵速度（公里/小时）
* startCounty 拥堵源头辖区
* longTime 持续时间
*/
USTRUCT(BlueprintType)
struct FTrafficCrowdedRoadEvent {
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString roadName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString endAddr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString effectCounty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString pubTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString startAddr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString startCounty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int jamDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int jamSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int longTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString eventId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D xy;


};
/**
*构建拥堵路段某个网格数据
*/
USTRUCT(BlueprintType)
struct FTrafficCrowdedMeshData {
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D xy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector2D> xysArray;
};
/**
* 拥堵的具体路段
* event 拥堵事件
* 具体拥堵的区域的网格构成
*/
USTRUCT(BlueprintType)
struct FTrafficCrowdedRoad {
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTrafficCrowdedRoadEvent event;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTrafficCrowdedMeshData> rows;
};


/*人脸识别对比库数据*/

USTRUCT(BlueprintType)
struct FHumanItem
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString address;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString birthday;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString cityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString cityName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString credentialsNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int credenttialsType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int	 ethnic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString facePicUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString humanId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString humanName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int listLibId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString	listLibName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString provinceID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString provinceName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int sex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float similarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int smile;
};

USTRUCT(BlueprintType)
struct FFaceLibraryData
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int age;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString alarmId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString alarmTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString bkgPicUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString cameraName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int cascadeAlarmId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int	controlId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int enthnic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString facePicUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int glass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString humanId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString humanName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FHumanItem> humansItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString indexCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int	listLibId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int	sex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float similarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int smile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int status;
};

/*人脸识别数据*/


USTRUCT(BlueprintType)
struct FFaceItemData {
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString alarmTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int timeStamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString cameraName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString alarmId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString humanId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString humanName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString matchHumanList;
};

USTRUCT(BlueprintType)
struct FFaceData {
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FFaceItemData> dataArr;
};
#pragma region the struct for Camera calcute people number
USTRUCT(BlueprintType)
struct FCamPeople
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D vec2d1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D vec2d2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int peoplenum;
};
//USTRUCT(BlueprintType)
//struct CamPeopleJson
//{
//
//};

#pragma endregion
/**
 *
 */
UCLASS()
class SHSECURITYSYS_API UFileDictOperationCollection : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
#pragma region FileDictOperation
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Game Dir", Keywords = "Get Game Dir"), Category = "FileDictCollection")
		static FString GetGameDir();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove File", Keywords = "Remove File"), Category = "FileDictCollection")
		static void RemoveFile(FString _path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove Directory", Keywords = "Remove Directory"), Category = "FileDictCollection")
		static void RemoveFolder(FString _path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Folder", Keywords = "Create Folder"), Category = "FileDictCollection")
		static void CreateFolder(FString _path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Folder Files", Keywords = "Get Folder Files"), Category = "FileDictCollection")
		static TArray<FString> GetFolderFiles(FString _path);

#pragma endregion

#pragma region Time
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentDateAndTime", Keywords = "GetCurrentDateAndTime"), Category = "FileDictCollection")
		static FDateTime GetCurrentDateAndTime();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentUtcDateAndTime", Keywords = "GetCurrentUtcDateAndTime"), Category = "FileDictCollection")
		static FDateTime GetCurrentUtcDateAndTime(int32 timezone);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentTime", Keywords = "GetCurrentTime"), Category = "FileDictCollection")
		static FTimespan GetCurrentTime();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentUtcTime", Keywords = "GetCurrentUtcTime"), Category = "FileDictCollection")
		static FTimespan GetCurrentUtcTime(int32 timezone);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentTimeInTicks", Keywords = "GetCurrentTimeInTicks"), Category = "FileDictCollection")
		static float GetCurrentTimeInTicks();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentTimeInMilliseconds", Keywords = "GetCurrentTimeInMilliseconds"), Category = "FileDictCollection")
		static float GetCurrentTimeInMilliseconds();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentTimeInSeconds", Keywords = "GetCurrentTimeInSeconds"), Category = "FileDictCollection")
		static float GetCurrentTimeInSeconds();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentUtcTimeInSeconds", Keywords = "GetCurrentUtcTimeInSeconds"), Category = "FileDictCollection")
		static float GetCurrentUtcTimeInSeconds(int32 timezone);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentTimeInMinutes", Keywords = "GetCurrentTimeInMinutes"), Category = "FileDictCollection")
		static float GetCurrentTimeInMinutes();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentTimeInHours", Keywords = "GetCurrentTimeInHours"), Category = "FileDictCollection")
		static float GetCurrentTimeInHours();
#pragma endregion


	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_SystemSettingConfig ReadSystemConfig(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static void WriteSystemConfig(FString _path, FSys_SystemSettingConfig info);


	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_TrafficStatus ReadTrafficData(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_WeartherData ReadWeartherData(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_KaKouStatus ReadKaKouData(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_PoliceGPSArray ReadPoliceGPSData(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_CameraSignArray ReadCameraSignData(FString _path);


	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_WifiListTable ReadWifiListTable(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static void WriteWifiListTable(FString _path,FSys_WifiListTable list,TArray<FString> Locat);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_PeopleWifiData ReadWifiDataTable(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_KaKouListTable ReadKaKouListTable(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static void WriteKaKouListTable(FString _path,FSys_KaKouListTable list,TArray<FString> Locat);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_KaKouDataArray ReadKaKouDataArray(FString JsonValue);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_SIPArray ReadSIPConfigArray(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_PeopleDayData ReadDayData(FString JsonValue);
	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static  FSys_TrafficData ReadTraffic(FString JsonValue);
#pragma region TrafficCrowded
	UFUNCTION(BlueprintCallable, Category = "TrafficCrowded")
		static	FTrafficCrowdedAll DealTrafficCrowdedAllJson(FString JsonValue);
	UFUNCTION(BlueprintCallable, Category = "TrafficCrowded")
		static FTrafficCrowdedRoad DealTrafficCrowdedRoadJson(FString JsonValue);
#pragma endregion
	UFUNCTION(BlueprintCallable, Category = "SysData")
		static void ReadHongWarPeopleData(FString json, TArray<FSys_HongWaiPeopleData>& HongWaiArray);

	UFUNCTION(BlueprintCallable, Category = "Tool")
		static FString UseRegex(const FString str,const FString Reg,bool &HasFind);
	

	UFUNCTION(BlueprintCallable, Category = "FaceData")
		static FFaceData ReadFaceData(FString JsonValue);

	UFUNCTION(BlueprintCallable, Category = "FaceData")
		static FFaceLibraryData ReadFaceLibraryData(FString JsonValue);

	UFUNCTION(BlueprintCallable, Category = "FaceData")
		static FSys_FaceCameraArray ReadFaceCameraData(FString _path);

	UFUNCTION(BlueprintCallable, Category = "SiMiao")
		static FHongWaiConfigMap ReadHongWaiConfigMap(FString _path);

	UFUNCTION(BlueprintCallable, Category = "SiMiao")
		static FSIPSiMiaoCamera ReadSiMiaoCameraConfig(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FSys_WuLianArray ReadWuLianDataArray(FString JsonValue);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		static FHongWaiData ReadHongWaiData(FString JsonValue);

	UFUNCTION(BlueprintCallable, Category = "HongWaiAdjust")
		static FHongWaiDeviation ReadHongWaiDeviation(FString _path);
	UFUNCTION(BlueprintCallable, Category = "HongWaiAdjust")
		static void WriteHongWaiDeviation(FHongWaiDeviation data,FString _path);

	//读取文件并将其转换为字符串
	UFUNCTION(BlueprintCallable, Category = "File")
		static void FileToString(FString _path, FString &Value);

	//读CarSingleStruct
	UFUNCTION(BlueprintCallable, Category = "Car")
		static FCarSingleStruct ReadCarSingleStruct(FString jsonValue);
	//将CarSingleStruct转换为String
	UFUNCTION(BlueprintCallable, Category = "Car")
		static FString CarSingleStructToString(FCarSingleStruct data);
	//读CarUpdateInfoStruct
	UFUNCTION(BlueprintCallable, Category = "Car")
		static FCarUpdateInfoStruct ReadCarUpdateInfoStruct(FString jsonValue);
	//将CarUpdateInfoStruct转换为String
	UFUNCTION(BlueprintCallable, Category = "Car")
		static FString CarUpdateInfoStructToString(FCarUpdateInfoStruct data);
	//读CarDetail
	UFUNCTION(BlueprintCallable, Category = "Car")
		static FCarDetail ReadCarDetail(FString jsonValue);
#pragma region the struct for Camera calcute people number
	UFUNCTION(BlueprintCallable, Category = "CamPeopleCount")
		static void DealCamPeoPle(FString JsonValue, TArray<FCamPeople>& camPeoCountArray);
#pragma endregion

	UFUNCTION(BlueprintCallable, Category = "HongWaiAdjust")
		static FSys_Warning110Array Parse110Data(FString data);
};
