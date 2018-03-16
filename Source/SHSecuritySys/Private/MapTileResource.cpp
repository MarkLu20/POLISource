// Fill out your copyright notice in the Description page of Project Settings.

#include "MapTileResource.h"
#include "RenderingThread.h"
#include "RenderResource.h"
#include "PrimitiveViewRelevance.h"
#include "VertexFactory.h"
#include "MaterialShared.h"
#include "Engine/CollisionProfile.h"
#include "Materials/Material.h"
#include "LocalVertexFactory.h"
#include "SceneManagement.h"
#include "DynamicMeshBuilder.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "FileManager.h"
#include "GraphAStar.h"
#include "Serialization/MemoryReader.h"

static FString CacheDir() {
	return FPaths::GameDir() / L"/MapDBCache";
}

FSlot::FSlot()
{
#define ADD_SLOT(SLOT, LOD) \
	SLOT = #SLOT;\
	IndexMap.Add(SLOT, Add(SLOT));\
	LodMap.Add(SLOT, LOD)
#include "DefSlots.h"

#undef ADD_SLOT
}

int32 FSlot::GetIndex(const FName& key) {
	int32* value = IndexMap.Find(key);
	if (value == NULL)
		return -1;
	return *value;
}

int32 FSlot::GetLod(const FName& key) {
	int32* value = LodMap.Find(key);
	if (value == NULL)
		return 14;
	return *value;
}

const FName& FindSlot(int32 type) {
	switch (type) {
	case 3192: return FSlot::Slot.Ground;
	case 3185:
	case 5636:
	case 5637:
	case 5638:
	case 5639: // 泳池
		return FSlot::Slot.River;
	case 3174:
	case 5643:
	case 5640:
	case 3177: // 高尔夫球场
	case 6080: //公园
	case 5648: // 排球场
		return FSlot::Slot.Grass;
	case 3197:
		return FSlot::Slot.Business;
	case 3193:
		return FSlot::Slot.Ground;
	case 4356:
		return FSlot::Slot.MetroNo1;
	case 4357:
		return FSlot::Slot.MetroNo2;
	case 4358:
		return FSlot::Slot.MetroNo3;
	case 4359:
		return FSlot::Slot.MetroNo4;
	case 4360:
		return FSlot::Slot.MetroNo5;
	case 4361:
		return FSlot::Slot.MetroNo6;
	case 4362:
		return FSlot::Slot.MetroNo7;
	case 4363:
		return FSlot::Slot.MetroNo8;
	case 4364:
		return FSlot::Slot.MetroNo9;
	case 4365:
		return FSlot::Slot.MetroNo10;
	case 4366:
		return FSlot::Slot.MetroNo11;
	case 5133:
		return FSlot::Slot.MetroNo12;
	case 4367:
		return FSlot::Slot.MetroNo13;
	case 5009:
		return FSlot::Slot.MetroNo16;
	case 4368: // 磁悬浮
		return FSlot::Slot.Maglev;
	case 4063:
		return FSlot::Slot.Building;
	case 3194:
		return FSlot::Slot.Hospital;
	case 3195:
		return FSlot::Slot.School;
	case 3198://机场
		return FSlot::Slot.Airport;
	case 3199://机场公共设施
		return FSlot::Slot.AirportUtility;
	case 3200://公共设施
		return FSlot::Slot.Public;
	case 5645:
		return FSlot::Slot.Runway;
	case 5646:
	case 5647:
	case 5649:
		return FSlot::Slot.Playground;
	case 5635:// 教学楼
		return FSlot::Slot.TeachingBuilding;
	case 5644://停车场
	case 4125:
		return FSlot::Slot.Parking;

	case 31924: // 小路
		return FSlot::Slot.SmallRoad;
	case 31927: // 马路
		return FSlot::Slot.Road;
	case 31932:// 马路尽头
		return FSlot::Slot.Road;
	case 31929://公路
		return FSlot::Slot.HighWay;
	case 31931://高架桥            
		return FSlot::Slot.Viaduct;
	case 31935://小区等建筑设施出入口
		return FSlot::Slot.Aisle;
	case 31939://高架桥入口/出口 ?
	case 31971://隧道出口
		return FSlot::Slot.Road;
	case 31937://辅路
		return FSlot::Slot.Road;
	case 31963://地道
	case 31961: //外滩隧道
		return FSlot::Slot.Subway;
	case 31967: //隧道
		return FSlot::Slot.Tunnel;

	default:
		return FSlot::Slot.Invalid;
	}
}


FSlot FSlot::Slot;

const uint32 kMeshElem_Position = 0x1 << 0;
const uint32 kMeshElem_Normal = 0x1 << 1;
const uint32 kMeshElem_UV0 = 0x1 << 2;

/** Vertex Buffer */
void FMapVertexBuffer::InitRHI()
{
	FRHIResourceCreateInfo CreateInfo;
	void* VertexBufferData = nullptr;
	VertexBufferRHI = RHICreateAndLockVertexBuffer(Vertices.Num() * sizeof(FDynamicMeshVertex), BUF_Static, CreateInfo, VertexBufferData);

	// Copy the vertex data into the vertex buffer.		
	FMemory::Memcpy(VertexBufferData, Vertices.GetData(), Vertices.Num() * sizeof(FDynamicMeshVertex));
	RHIUnlockVertexBuffer(VertexBufferRHI);
}

/** Index Buffer */
void FMapIndexBuffer::FMapIndexBuffer::InitRHI()
{
	FRHIResourceCreateInfo CreateInfo;
	void* Buffer = nullptr;
	IndexBufferRHI = RHICreateAndLockIndexBuffer(sizeof(int32), Indices.Num() * sizeof(int32), BUF_Static, CreateInfo, Buffer);

	// Write the indices to the index buffer.		
	FMemory::Memcpy(Buffer, Indices.GetData(), Indices.Num() * sizeof(int32));
	RHIUnlockIndexBuffer(IndexBufferRHI);
}

/** Vertex Factory */
/** Init function that should only be called on render thread. */
void FMapVertexFactory::Init_RenderThread(const FMapVertexBuffer* VertexBuffer)
{
	check(IsInRenderingThread());

	FDataType NewData;
	NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Position, VET_Float3);
	NewData.TextureCoordinates.Add(
		FVertexStreamComponent(VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate), sizeof(FDynamicMeshVertex), VET_Float2)
	);
	NewData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal);
	NewData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal);
	NewData.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Color, VET_Color);

	SetData(NewData);
}

/** Initialization */
void FMapVertexFactory::Init(const FMapVertexBuffer* VertexBuffer)
{
	if (IsInRenderingThread())
	{
		Init_RenderThread(VertexBuffer);
	}
	else
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			InitMapVertexFactory,
			FMapVertexFactory*, VertexFactory, this,
			const FMapVertexBuffer*, VertexBuffer, VertexBuffer,
			{
				VertexFactory->Init_RenderThread(VertexBuffer);
			});
	}
}

FMapPrimitive::~FMapPrimitive() {
	if (VertexFactory.IsInitialized()) {
		VertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}
}

FMapTile::FMapTile(int X, int Y, int Z)
	: X(X), Y(Y), Z(Z)
{}

FVector ReadVector(FArchive* file) {
	FVector v;
	*file << v.X;
	*file << v.Z;
	*file << v.Y;
	v.Y *= -1;
	return v;
}

FVector ReadPosition(FArchive* file) {
	return ReadVector(file) * 0.01f * MapUnit;
}

FQuat ReadQuat(FArchive* file) {
	FQuat quat;
	*file << quat.X;
	*file << quat.Z;
	*file << quat.Y;
	*file << quat.W;

	return quat;
}

static FMapPrimitivePtr FindPrimitive(TArray<FMapPrimitivePtr>& Primitives, TMap<FName, FMapPrimitivePtr>& map, const FName& key) {
	FMapPrimitivePtr* ptr = map.Find(key);
	if (ptr != NULL)
		return *ptr;

	FMapPrimitivePtr primitive = MakeShareable<FMapPrimitive>(new FMapPrimitive);
	map.Add(key, primitive);
	primitive->Slot = key;
	Primitives.Add(primitive);
	return primitive;
}

FTileBuilding::FTileBuilding(int X, int Y, int Z)
	: X(X)
	, Y(Y)
	, Z(Z)
{
}

void FTileBuilding::Load()
{
	FMapPtr map = FMap::Instance();
	if (!map.IsValid())
		return;

	DB = map->GetTileDB(X, Y, Z);
	if (!DB->mDB.IsValid())
		return;

	//Building的加载；
	DB->mBuildingParam_X->SetInt(X);
	DB->mBuildingParam_Y->SetInt(Y);
	DB->mBuildingParam_Z->SetInt(Z);
	TSharedPtr<IMKDatabaseReader> DBReader = DB->mBuildingRefCmd->ExecuteReader();
	TArray<BYTE> BuildingRefData;
	if (DBReader.IsValid() && DBReader->MoveNext()) {
		DBReader->GetBlob(0, BuildingRefData);
	}
	else
		return;

	FMemoryReader readerBuilding(BuildingRefData);
	FArchive* fileBuilding = &readerBuilding;
	int32 BuildingCount;
	int32 BuildingType;
	ANSICHAR BuildingName[256] = { 0 };
	FVector BuildingPosition;
	FQuat BuildingRotation;
	FVector BuildingScale;
	*fileBuilding << BuildingCount;

	TMap<FName, FMapPrimitivePtr> PrimMap;

	for (int i = 0; i < BuildingCount; ++i)
	{
		*fileBuilding << BuildingType;
		FMapPrimitivePtr Primitive = FindPrimitive(Primitives, PrimMap, FindSlot(BuildingType));

		BYTE nameCount;
		*fileBuilding << nameCount;
		for (int j = 0; j < nameCount; ++j)
		{
			*fileBuilding << BuildingName[j];
		}
		BuildingName[nameCount] = 0;
		BuildingPosition = ReadPosition(fileBuilding);
		BuildingRotation = ReadQuat(fileBuilding);
		BuildingScale = ReadVector(fileBuilding);
		LoadBuilding(Primitive, BuildingName, BuildingPosition, BuildingRotation, BuildingScale);
	}

	for (int i = 0; i < Primitives.Num(); ) {
		if (Primitives[i]->VertexBuffer.Vertices.Num() == 0) {
			Primitives.RemoveAtSwap(i);
			continue;
		}
		++i;
	}
}

void FTileBuilding::LoadBuilding(FMapPrimitivePtr Primitive, ANSICHAR* BuildingName, FVector Position, FQuat Rotation, FVector Scale) {
	FMapPtr map = FMap::Instance();
	if (!map.IsValid())
		return;

	TCHAR* BuildingNameTC = ANSI_TO_TCHAR(BuildingName);
	FName Name(BuildingNameTC);
	if (map->Buildings.Contains(Name)) {
		FMapPrimitiveWeakPtr primitive = map->Buildings[Name];
		if (primitive.IsValid())
			return;
	}


	TArray<BYTE> DBData;
	int IsVisiable;

	//if (Map->mDB.Open() != MKSQLITE_OK)
	//	return;

	DB->mBuildingParam_GUID->SetString(BuildingNameTC);
	TSharedPtr<IMKDatabaseReader> DBReader = DB->mBuildingCmd->ExecuteReader();

	if (DBReader.IsValid() && DBReader->MoveNext()) {
		DBReader->GetBlob(0, DBData);
		IsVisiable = DBReader->GetInt(0);
	}
	else
		return;

	if (DBData.Num() <= 0)
		return;

	if (DBReader->GetInt(1) == 0)	return;

	Primitive->isVisiable = IsVisiable == 0 ? true : false;

	FMemoryReader reader(DBData);
	FArchive* file = &reader;

	uint32 mesh_elem;
	*file << mesh_elem;
	FMatrix trans;
	trans.SetIdentity();
	trans.SetAxis(0, Rotation.GetAxisX());
	trans.SetAxis(1, Rotation.GetAxisY());
	trans.SetAxis(2, Rotation.GetAxisZ());
	trans.SetOrigin(Position);

	if ((mesh_elem & kMeshElem_Position) == kMeshElem_Position)
	{
		int vertexCount = 0;
		*file << vertexCount;
		if (vertexCount > 0) {
			int VertexOffset = Primitive->VertexBuffer.Vertices.Num();
			int IndexOffset = Primitive->IndexBuffer.Indices.Num();

			Primitive->VertexBuffer.Vertices.AddUninitialized(vertexCount);

			for (int i = 0; i < vertexCount; ++i) {
				Primitive->VertexBuffer.Vertices[VertexOffset + i].Position = trans.TransformPosition(ReadPosition(file));
			}

			if ((mesh_elem & kMeshElem_Normal) == kMeshElem_Normal)
			{
				int normalCount = 0;
				*file << normalCount;

				for (int i = 0; i < normalCount; ++i) {
					FVector normal = trans.TransformVector(ReadVector(file));
					Primitive->VertexBuffer.Vertices[VertexOffset + i].SetTangents(FVector(1, 0, 0), FVector(0, 1, 0), normal);
				}
			}

			if ((mesh_elem & kMeshElem_UV0) == kMeshElem_UV0)
			{
				int uvCount = 0;
				*file << uvCount;
				for (int i = 0; i < uvCount; ++i) {
					*file << Primitive->VertexBuffer.Vertices[VertexOffset + i].TextureCoordinate;
				}
				int vCount = uvCount / 6;
				for (int i = 0; i < vCount; ++i) {
					Primitive->VertexBuffer.Vertices[VertexOffset + i].TextureCoordinate = FVector2D(0, 0);
					Primitive->VertexBuffer.Vertices[VertexOffset + vCount + i].TextureCoordinate = FVector2D(0, 1);
				}
				for (int i = vCount * 2; i < uvCount; i += 4) {
					Primitive->VertexBuffer.Vertices[VertexOffset + i + 0].TextureCoordinate = FVector2D(0, 0);
					Primitive->VertexBuffer.Vertices[VertexOffset + i + 1].TextureCoordinate = FVector2D(1, 0);
					Primitive->VertexBuffer.Vertices[VertexOffset + i + 2].TextureCoordinate = FVector2D(1, 1);
					Primitive->VertexBuffer.Vertices[VertexOffset + i + 3].TextureCoordinate = FVector2D(0, 1);
				}
			}

			int indexCount;
			*file << indexCount;
			if (indexCount > 0) {
				Primitive->IndexBuffer.Indices.AddUninitialized(indexCount);
				for (int i = 0; i < indexCount / 3; ++i) {
					int32 i0, i1, i2;
					*file << i0;
					*file << i2;
					*file << i1;
					Primitive->IndexBuffer.Indices[IndexOffset + 3 * i + 0] = i0 + VertexOffset;
					Primitive->IndexBuffer.Indices[IndexOffset + 3 * i + 1] = i1 + VertexOffset;
					Primitive->IndexBuffer.Indices[IndexOffset + 3 * i + 2] = i2 + VertexOffset;
				}

				//Primitive->ReferenceCount++;
				map->Buildings.Add(Name, Primitive);
			}
		}
	}

	file->Close();
}

void FMapTile::Load()
{
	FMapPtr map = FMap::Instance();
	if (!map.IsValid())
		return;

	DB = map->GetTileDB(X, Y, Z);
	if (!DB->mDB.IsValid())
		return;

	////TMap<FName, FMapPrimitivePtr> PrimMap;
	////所对应的14级的tile的数据加载；
	//const int TileDataZ = 14;
	//int lodDelta = Z - TileDataZ;
	//int64 dataTileX = X >> lodDelta;
	//int64 dataTileY = Y >> lodDelta;
	//int64 key = (dataTileX << 32) + dataTileY;

	//TSharedPtr<FMapTileDataSource> lDataSource;
	//if (map->TileDataSource.Contains(key))
	//	lDataSource = map->TileDataSource[key].Pin();

	if (!DB->mDataSource.IsValid())
	{
		DB->mDataSource = MakeShareable(new FMapTileDataSource());

		DB->mTileDataSourceCmd_X->SetInt(DB->X);
		DB->mTileDataSourceCmd_Y->SetInt(DB->Y);
		TSharedPtr<IMKDatabaseReader> DBReader = DB->mTileDataSourceCmd->ExecuteReader();
		TArray <BYTE> TileDataSource;
		if (DBReader.IsValid() && DBReader->MoveNext()) {
			DBReader->GetBlob(0, TileDataSource);
		}
		else
			return;

		//反序列化14级的data；
		FMemoryReader readerSourceData(TileDataSource);
		FArchive* fileDataSource = &readerSourceData;

		int32 GroundCount;
		*fileDataSource << GroundCount;
		for (int i = 0; i < GroundCount; ++i)
		{
			FMapPrimitivePtr primitive = MakeShareable(new FMapPrimitive);
			*fileDataSource << primitive->SlotID;
			primitive->Slot = FindSlot(primitive->SlotID);

			int32 VBCount;
			*fileDataSource << VBCount;
			primitive->VertexBuffer.Vertices.AddUninitialized(VBCount);
			for (int j = 0; j < VBCount; ++j)
			{
				FDynamicMeshVertex Vert;

				Vert.Color.DWColor() = primitive->SlotID;// = FColor::White;
				Vert.SetTangents(FVector(1, 0, 0), FVector(0, 1, 0), FVector(0, 0, 1));

				Vert.Position = ReadPosition(fileDataSource);
				primitive->VertexBuffer.Vertices[j] = Vert;
			}

			int32 IBCount;
			*fileDataSource << IBCount;
			primitive->IndexBuffer.Indices.AddUninitialized(IBCount);
			int TriangleCount = IBCount / 3;
			for (int j = 0; j < TriangleCount; ++j) {
				int32 i0, i1, i2;
				*fileDataSource << i0;
				*fileDataSource << i2;
				*fileDataSource << i1;
				primitive->IndexBuffer.Indices[3 * j + 0] = i0;
				primitive->IndexBuffer.Indices[3 * j + 1] = i1;
				primitive->IndexBuffer.Indices[3 * j + 2] = i2;
			}

			if (VBCount > 0 && IBCount > 0)
				DB->mDataSource->AllPrimitives.Add(primitive->SlotID, primitive);
		}
	}

	//tile的加载；
	TArray<BYTE> DBData;
	DB->mTileParam_X->SetInt(X);
	DB->mTileParam_Y->SetInt(Y);
	DB->mTileParam_Z->SetInt(Z);
	TSharedPtr<IMKDatabaseReader> DBReader = DB->mTileCmd->ExecuteReader();
	if (DBReader.IsValid() && DBReader->MoveNext()) {
		DBReader->GetBlob(0, DBData);
	}
	else
		return;

	FMemoryReader reader(DBData);
	FArchive* file = &reader;

	int32 SlotCount;
	*file << SlotCount;
	for (int i = 0; i < SlotCount; ++i)
	{
		FMapPrimitiveRefPtr tempRef = MakeShareable(new FMapPrimitiveRef);
		//需要加根据slot遍历一下primitive数组返回一个primitive赋值
		int SlotID, StartIndex, Count;
		*file << SlotID;
		*file << StartIndex;
		*file << Count;

		//第一次加载tile的时候把所在的primitive引用赋值给tile；

		if (DB->mDataSource->AllPrimitives.Contains(SlotID)) {
			tempRef->Primitive = DB->mDataSource->AllPrimitives[SlotID];
			tempRef->StartIndex = StartIndex;
			tempRef->Count = Count;

			TilePrimitives.Add(tempRef);
		}
	}
}

FMapWeakPtr FMap::sInstance;
FMapRenderWeakPtr FMapRender::sInstance;

FMapRender::FMapRender()
{
	TileElements.AddDefaulted(10000);
	TileBuildingElements.AddDefaulted(10000);

	for (int i = 0; i < TileElements.Num(); ++i) {
		TileElementList.Push(&TileElements[i]);
	}
	for (int i = 0; i < TileBuildingElements.Num(); ++i) {
		TileBuildingElementList.Push(&TileBuildingElements[i]);
	}
}
FMapRender::~FMapRender()
{
//	GEngine->AddOnScreenDebugMessage(-1,10.f, FColor::Red, TEXT("MapRender Disposed!"));
}

FMap::FMap()
	: TileLoader(L"FMapTileLoader")
	, TileBuildingLoader(L"FTileBuildingLoader")
	, DBModule(FModuleManager::Get().LoadModuleChecked<IMKDatabaseModule>(TEXT("MKSQLite")))
{
	/*	TileElements.AddDefaulted(10000);
	TileBuildingElements.AddDefaulted(10000);

	for (int i = 0; i < TileElements.Num(); ++i) {
	TileElementList.Push(&TileElements[i]);

	for (int i = 0; i < TileBuildingElements.Num(); ++i) {
	TileBuildingElementList.Push(&TileBuildingElements[i]);
	}
	*/
}

FMap::~FMap() {
}

int64 FMap::Key(int X, int Y, int Z) {
	return ((int64)X << 32) | (int64)Y;
}

TSharedPtr<FMapTile> FMapRender::GetTile(int X, int Y, int Z) {
	if (Z >= MipMapCount || Z < 0)
		return NULL;

	int64 key = FMap::Key(X, Y, Z);

	TileElement** value = AllTiles[Z].Find(key);

	if (value == NULL) {
		FMap::Instance()->CacheTile(X, Y, Z);
		return NULL;
	}
	TileElementList.Remove(*value);
	TileElementList.AddFirst(*value);
	return (*value)->Tile;
}

TSharedPtr<FTileBuilding> FMapRender::GetTileBuilding(int X, int Y, int Z) {
	if (Z >= MipMapCount || Z < 0)
		return NULL;

	int64 key = FMap::Key(X, Y, Z);

	TileBuildingElement** value = TileBuilding.Find(key);
	if (value == NULL) {
		FMap::Instance()->CacheTileBuilding(X, Y, Z);
		return NULL;
	}

	TileBuildingElementList.Remove(*value);
	TileBuildingElementList.AddFirst(*value);
	return (*value)->Tile;
}

void FMapRender::Sync()
{
	FMap::Instance()->TileLoader.Sync();
	FMap::Instance()->TileBuildingLoader.Sync();
}

void FMap::Sync() {
	TileLoader.Sync();
	TileBuildingLoader.Sync();
}

void FMap::CacheTile(int X, int Y, int Z) {
	int64 key = FMap::Key(X, Y, Z);

	FCacheInfo** InfoPtr = CachingTiles[Z].Find(key);
	FCacheInfo* Info = NULL;
	if (InfoPtr == NULL) {
		Info = new FMap::FCacheInfo();
		Info->X = X;
		Info->Y = Y;
		Info->Z = Z;
		CachingTiles[Z].Add(key, Info);
	}
	else {
		Info = *InfoPtr;
	}
	TileLoader.CacheTile(Info);
}

void FMap::CacheTileBuilding(int X, int Y, int Z) {

	int64 key = Key(X, Y, Z);

	FBuildingCacheInfo** InfoPtr = CachingBuildings.Find(key);
	FBuildingCacheInfo* Info = NULL;
	if (InfoPtr == NULL) {
		Info = new FBuildingCacheInfo();
		Info->X = X;
		Info->Y = Y;
		Info->Z = Z;
		CachingBuildings.Add(key, Info);
	}
	else {
		Info = *InfoPtr;
	}
	TileBuildingLoader.CacheTile(Info);
}

TSharedPtr<FMapTileDB> FMap::GetTileDB(int X, int Y, int Z) {
	int delta = Z - 14;
	X = X >> delta;
	Y = Y >> delta;
	int64 key = Key(X, Y, 14);

	FScopeLock Lock(&TileDBMapMutex);
	TWeakPtr<FMapTileDB> TileDB;
	if (TileDBs.Contains(key))
		TileDB = TileDBs[key];

	if (TileDB.IsValid()) {
		return TileDB.Pin();
	}

	FString connectString = FString::Printf(L"%s/Map/%d_%d.db", *CacheDir(), X, Y);
	TSharedPtr<FMapTileDB> spTileDB;
	if (FPaths::FileExists(connectString))
		spTileDB = MakeShareable(new FMapTileDB(DBModule.CreateConnection(connectString), X, Y, 14));
	else
		spTileDB = FMapTileDB::Empty();

	TileDBs.Add(key, spTileDB);
	return spTileDB;
}

void FMapRender::AddMapTile(int X, int Y, int Z, TSharedPtr<FMapTile> Tile) {
	int64 key = FMap::Key(X, Y, Z);
	TileElement** elem = AllTiles[Z].Find(key);
	TileElement* element = NULL;
	if (elem == NULL) {
		element = (TileElement*)TileElementList.Last.Prev;
		if (element->Tile.IsValid()) {
			int64 delKey = FMap::Key(element->Tile->X, element->Tile->Y, element->Tile->Z);
			AllTiles[element->Tile->Z].Remove(delKey);
		}
	}
	else
		element = *elem;

	TileElementList.Remove(element);
	TileElementList.AddFirst(element);

	element->Tile = Tile;

	AllTiles[Z].Add(key, element);
	FMap::Instance()->CachingTiles[Z].Remove(key);
}

void FMapRender::AddTileBuilding(int X, int Y, int Z, TSharedPtr<FTileBuilding> Tile) {
	int64 key = FMap::Key(X, Y, Z);
	TileBuildingElement** elem = TileBuilding.Find(key);
	TileBuildingElement* element = NULL;
	if (elem == NULL) {
		element = (TileBuildingElement*)TileBuildingElementList.Last.Prev;

		if (element->Tile.IsValid()) {
			int64 delKey = FMap::Key(element->Tile->X, element->Tile->Y, element->Tile->Z);
			TileBuilding.Remove(delKey);
		}
	}
	else
		element = *elem;

	TileBuildingElementList.Remove(element);
	TileBuildingElementList.AddFirst(element);
	element->Tile = Tile;

	TileBuilding.Add(key, element);
	FMap::Instance()->CachingBuildings.Remove(key);
}

FMapTileDB::FMapTileDB(TSharedPtr<IMKDatabaseConnection> InDB, int InX, int InY, int InZ) {
	mDB = InDB;
	X = InX;
	Y = InY;
	Z = InZ;

	if (!mDB.IsValid())
		return;

	TArray<TSharedPtr<IMKDatabaseParameter>> Parameters;
	Parameters.Reserve(3);

	{ // TileDataTable
		mTileParam_X = mDB->CreateParameter(TEXT("@X"));
		mTileParam_Y = mDB->CreateParameter(TEXT("@Y"));
		mTileParam_Z = mDB->CreateParameter(TEXT("@Z"));
		Parameters.Empty();
		Parameters.Add(mTileParam_X);
		Parameters.Add(mTileParam_Y);
		Parameters.Add(mTileParam_Z);
		mTileCmd = mDB->CreateCommand(TEXT("select Data from TileDataTable where X==@X and Y==@Y and Z==@Z"), Parameters);
	}

	{ // DataSourceTable
		mTileDataSourceCmd_X = mDB->CreateParameter(TEXT("@X"));
		mTileDataSourceCmd_Y = mDB->CreateParameter(TEXT("@Y"));
		Parameters.Empty();
		Parameters.Add(mTileDataSourceCmd_X);
		Parameters.Add(mTileDataSourceCmd_Y);
		mTileDataSourceCmd = mDB->CreateCommand(TEXT("select Data from DataSourceTable where X==@X and Y==@Y"), Parameters);
	}

	{ // BuildingTable
		mBuildingParam_GUID = mDB->CreateParameter(TEXT("@GUID"));
		Parameters.Empty();
		Parameters.Add(mBuildingParam_GUID);
		mBuildingCmd = mDB->CreateCommand(TEXT("select Data,IsVisible from BuildingTable where GUID==@GUID"), Parameters);
	}

	{ // BuildingRefTable
		mBuildingParam_X = mDB->CreateParameter(TEXT("@X"));
		mBuildingParam_Y = mDB->CreateParameter(TEXT("@Y"));
		mBuildingParam_Z = mDB->CreateParameter(TEXT("@Z"));
		Parameters.Empty();
		Parameters.Add(mBuildingParam_X);
		Parameters.Add(mBuildingParam_Y);
		Parameters.Add(mBuildingParam_Z);
		mBuildingRefCmd = mDB->CreateCommand(TEXT("select Data from BuildingRefTable where X == @X and Y == @Y and Z==@Z"), Parameters);
	}

	mDB->Open();
}

FRouteNode::FRouteNode() {
	Next = NULL;
	Index = -1;
}

FRouteMap::FRouteMap() {
	FString path = CacheDir() / L"Road\\RoadData";
	FArchive* file = IFileManager::Get().CreateFileReader(*path);

	*file << TileX;
	*file << TileY;

	int NodeCount;
	*file << NodeCount;
	Nodes.AddZeroed(NodeCount);
	for (int i = 0; i < NodeCount; ++i) {
		FRouteNode& node = Nodes[i];
		node.Index = i;
		*file << node.Position.X;
		*file << node.Position.Y;
		node.Position.Y *= -1;
		node.Position.Z = 0;

		int FollowCount;
		*file << FollowCount;
		node.FollowNode.Reserve(FollowCount);// AddZeroed(FollowCount);
		for (int j = 0; j < FollowCount; ++j) {
			int id;
			*file << id;
			if (id == i)
				continue;
			node.FollowNode.Add(id);
		}

		AddNode(node);
	}
}

static const int NodeSize = 128 * MapUnit;
void FRouteMap::AddNode(FRouteNode& node) {
	int32 x = FMath::RoundToInt(node.Position.X / NodeSize);
	int32 y = FMath::RoundToInt(node.Position.Y / NodeSize);
	uint32 uX = *(uint32*)&x;
	uint32 uY = *(uint32*)&y;
	uint64 key = ((uint64)uX << 32) | (uint64)uY;
	FRouteNode** pNode = Map.Find(key);
	if (pNode != NULL)
		node.Next = *pNode;
	Map.Add(key, &node);
}

FRouteNode* FRouteMap::GetNodeList(int32 X, int32 Y) {
	uint32 uX = *(uint32*)&X;
	uint32 uY = *(uint32*)&Y;
	uint64 key = ((uint64)uX << 32) | (uint64)uY;

	FRouteNode** pNode = Map.Find(key);
	if (pNode == NULL)
		return NULL;

	return *pNode;
}

void FRouteMap::FindNearPoints(const FVector& InPoint, float InRange, TArray<int32>& OutPoints) {
	struct FindPair {
		int Index;
		float Distance;

		FindPair(int InIndex, float InDistance)
			: Index(InIndex)
			, Distance(InDistance)
		{}

		bool operator < (const FindPair& other) const {
			return Distance < other.Distance;
		}
	};
	int32 Count = FMath::CeilToInt(InRange / NodeSize);
	int32 X = FMath::RoundToInt(InPoint.X / NodeSize);
	int32 Y = FMath::RoundToInt(InPoint.Y / NodeSize);

	TArray<FindPair> Nodes;
	for (int32 y = -Count; y <= Count; ++y) {
		for (int32 x = -Count; x <= Count; ++x) {
			FRouteNode* node = GetNodeList(X + x, Y + y);
			while (node) {
				float distance = FVector::Dist2D(InPoint, node->Position);
				if (distance < InRange) {
					Nodes.Add(FindPair(node->Index, distance));
				}

				node = node->Next;
			}
		}
	}
	Nodes.Sort();

	OutPoints.AddUninitialized(Nodes.Num());
	for (int i = 0; i < Nodes.Num(); ++i) {
		OutPoints[i] = Nodes[i].Index;
	}
}