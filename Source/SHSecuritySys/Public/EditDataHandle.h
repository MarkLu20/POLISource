// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EditDataHandle.generated.h"


// 小队成员
USTRUCT(BlueprintType)
struct FExh_VechicleData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString PosX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString PosY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString PosZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString RotX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString RotY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString RotZ;
};


// 小队成员
USTRUCT(BlueprintType)
struct FExh_MemberTeam
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString MemberTeam;
};

USTRUCT(BlueprintType)
struct FExh_MemberTeamArray
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		TArray<FExh_MemberTeam> TeamArray;
};

USTRUCT(BlueprintType)
struct FExh_Editor_MemberInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString MemberName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString MemberCount;
};

// 成员
USTRUCT(BlueprintType)
struct FExh_Editor_PoliceMember
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		TArray<FExh_Editor_MemberInfo> Memebers;
};

// 小队
USTRUCT(BlueprintType)
struct FExh_Editor_SPolice_Part
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString PartName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		int32 PartCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString IconPosX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString IconPosY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString RspArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString PartTeamPhone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString PartTeamLeader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString PartTeamLeaderPhone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString FloorZPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString Floor;
};

// 大队
USTRUCT(BlueprintType)
struct FExh_Editor_SPolice
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString TeamName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		int32 TeamPeopleCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString TeamLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString TeamPhone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString TeamLeader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString TeamLeaderPhone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString IconPosX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString IconPosY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString CheckZoneID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		TArray<FExh_Editor_SPolice_Part> Parts;
};

USTRUCT(BlueprintType)
struct Fxh_Editor_SBox
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString BoxName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString Building;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString Room;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString Company;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString PoliceArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		TArray<FExh_Editor_SPolice> PoliceTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString iconIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString RecordPosX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString RecordPosY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString RecordPosZ;
};


USTRUCT(BlueprintType)
struct FExh_Park
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString ParkName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		FString ParkState;
};

USTRUCT(BlueprintType)
struct FExh_ParkingInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhConfig")
		TArray<FExh_Park> Parking;
};


class AAPrintImpActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHSECURITYSYS_API UEditDataHandle : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEditDataHandle();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FExh_ParkingInfo ReadParkIndo(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FExh_Editor_SPolice ReadPoliceTeamInfo(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WriteParkInfo(FString _path, FExh_ParkingInfo parkInfo);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WritePoliceTeamInfo(FString _path, FExh_Editor_SPolice policeTeamInfo);

	UFUNCTION(BlueprintPure, Category = "ExhConfig")
		FString GetGameDir();

	//BoxDataHandle
	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		Fxh_Editor_SBox ReadBoxData(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WriteBoxInfo(FString _path, Fxh_Editor_SBox box);

	//VehicleDataHandle
	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FExh_VechicleData ReadVehicleData(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WriteVehicleInfo(FString _path, FExh_VechicleData box);

	//MemeberDataHandle
	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FExh_Editor_PoliceMember ReadMemberData(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void WriteMemberInfo(FString _path, FExh_Editor_PoliceMember box);

	//Project Operations
	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void CreateProjectFolder(FString _path, FString _projectPolicePath, FString _projectBlockPath, FString _projectPoliceAreaPath, FString _projectLayerPath, FString _parkPath);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void RemoveProjectFolder(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		void RemoveFile(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		TArray<FString> GetExistedPrjects(FString _path);

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		TArray<FString> GetFolderFiles(FString _path);

	// Export csv begin ///////////////////////////////////////////////// 

	// 岗位联系表 Deprecated!
	UFUNCTION(BlueprintCallable, Category = "ExhSH")
		void PrintPostContactorCSV(FString _title, FString _path, TArray<FExh_Editor_SPolice> policeTeamInfo);

	// 岗位设置表
	UFUNCTION(BlueprintCallable, Category = "ExhSH")
		void PrintPostSettingCSV(FString _path, TArray<FExh_Editor_SPolice> policeTeamInfo, AAPrintImpActor* printImpActor);

	// 警力抽调表
	UFUNCTION(BlueprintCallable, Category = "ExhSH")
		void PrintPoliceTransferCSV(FString _path, TArray<FExh_Editor_PoliceMember> policeMembers);

	// Export csv end  ///////////////////////////////////////////////// 

	UFUNCTION(BlueprintCallable, Category = "ExhConfig")
		FExh_MemberTeamArray ReadMemberTeamArray(FString _path);
	
};
