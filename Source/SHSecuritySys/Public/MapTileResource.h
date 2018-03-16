// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "MKDatabase.h"

const static float MapUnit = 100;//1 meter | 100centimeter

struct FMapRender;
typedef TSharedPtr<FMapRender, ESPMode::ThreadSafe> FMapRenderPtr;
typedef TWeakPtr<FMapRender, ESPMode::ThreadSafe> FMapRenderWeakPtr;

struct FMap;
typedef TSharedPtr<FMap, ESPMode::ThreadSafe> FMapPtr;
typedef TWeakPtr<FMap, ESPMode::ThreadSafe> FMapWeakPtr;

struct FMapPrimitive;
typedef TSharedPtr<FMapPrimitive, ESPMode::ThreadSafe> FMapPrimitivePtr;
typedef TWeakPtr<FMapPrimitive, ESPMode::ThreadSafe> FMapPrimitiveWeakPtr;

struct FMapPrimitiveRef;
typedef TSharedPtr<FMapPrimitiveRef, ESPMode::ThreadSafe> FMapPrimitiveRefPtr;
typedef TWeakPtr<FMapPrimitiveRef, ESPMode::ThreadSafe> FMapPrimitiveRefWeakPtr;

class FSlot : public TArray<FName> {
public:
#define ADD_SLOT(SLOT, LOD) FName SLOT
#include "DefSlots.h"
#undef ADD_SLOT

	FSlot();

	int32 GetIndex(const FName& key);
	int32 GetLod(const FName& key);
public:
	static FSlot Slot;

private:
	TMap<FName, int32> IndexMap;
	TMap<FName, int32> LodMap;
};



struct FCacheNode {
	FCacheNode* Prev;
	FCacheNode* Next;

	FCacheNode() {
		Prev = Next = NULL;
	}
};

struct FCacheList {
	FCacheNode First;
	FCacheNode Last;

public:
	FCacheList() {
		First.Next = &Last;
		Last.Prev = &First;
	}

	bool Empty() {
		return First.Next == &Last;
	}

	void Push(FCacheNode* node) {
		AddBetween(node, Last.Prev, &Last);
	}

	void Merge(FCacheList& list) {
		if (list.Empty())
			return;

		Last.Prev->Next = list.First.Next;
		list.First.Next->Prev = Last.Prev;
		Last.Prev = list.Last.Prev;
		list.Last.Prev->Next = &Last;

		list.First.Next = &list.Last;
		list.Last.Prev = &list.First;
	}

	FCacheNode* Pop() {
		if (Empty())
			return NULL;

		return Remove(First.Next);
	}

	FCacheNode* PopLast() {
		if (Empty())
			return NULL;

		return Remove(Last.Prev);
	}

	FCacheNode* PopAll() {
		if (Empty())
			return NULL;

		FCacheNode* node = First.Next;
		First.Next->Prev = NULL;
		First.Next = &Last;
		Last.Prev->Next = NULL;
		Last.Prev = &First;

		return node;
	}

	void AddFirst(FCacheNode* node) {
		AddBetween(node, &First, First.Next);
	}

	FCacheNode* Remove(FCacheNode* node) {
		node->Prev->Next = node->Next;
		node->Next->Prev = node->Prev;
		node->Next = node->Prev = NULL;

		return node;
	}

private:
	void AddBetween(FCacheNode* node, FCacheNode* before, FCacheNode* after) {
		before->Next = node;
		after->Prev = node;
		node->Prev = before;
		node->Next = after;
	}
};

/** Vertex Buffer */
class FMapVertexBuffer : public FVertexBuffer
{
public:
	TArray<FDynamicMeshVertex> Vertices;

	virtual void InitRHI() override;
};

/** Index Buffer */
class FMapIndexBuffer : public FIndexBuffer
{
public:
	TArray<int32> Indices;
	virtual void InitRHI() override;
};

/** Vertex Factory */
class FMapVertexFactory : public FLocalVertexFactory
{
public:
	/** Init function that should only be called on render thread. */
	void Init_RenderThread(const FMapVertexBuffer* VertexBuffer);

	/** Initialization */
	void Init(const FMapVertexBuffer* VertexBuffer);
};

struct FMapPrimitive {
	FMapVertexBuffer VertexBuffer;
	FMapIndexBuffer IndexBuffer;
	FMapVertexFactory VertexFactory;
	FName Slot;
	int SlotID;
	bool isVisiable;

	~FMapPrimitive();
};
struct FMapPrimitiveRef
{
	FMapPrimitivePtr Primitive;
	int StartIndex;
	int Count;
};

struct FMapTileDataSource
{
	//TArray<FMapPrimitivePtr> AllPrimitives;
	TMap<int32, FMapPrimitivePtr> AllPrimitives;
};

struct FMapTileDB {
	int X, Y, Z;

	// db connection
	TSharedPtr<IMKDatabaseConnection> mDB;

	// select Data from BuildingTable where GUID==@GUID
	TSharedPtr<IMKDatabaseCommand> mBuildingCmd;
	TSharedPtr<IMKDatabaseParameter> mBuildingParam_GUID;

	// select Data from TileDataTable where X==@X and Y==@Y and Z==@Z
	TSharedPtr<IMKDatabaseCommand> mTileCmd;
	TSharedPtr<IMKDatabaseParameter> mTileParam_X;
	TSharedPtr<IMKDatabaseParameter> mTileParam_Y;
	TSharedPtr<IMKDatabaseParameter> mTileParam_Z;

	// select Data from DataSourceTable where X==@X and Y==@Y
	TSharedPtr<IMKDatabaseCommand> mTileDataSourceCmd;
	TSharedPtr<IMKDatabaseParameter> mTileDataSourceCmd_X;
	TSharedPtr<IMKDatabaseParameter> mTileDataSourceCmd_Y;
	TSharedPtr<IMKDatabaseParameter> mTileDataSourceCmd_Z;

	// select Data from BuildingRefTable where X == @X and Y == @Y and Z==@Z
	TSharedPtr<IMKDatabaseCommand> mBuildingRefCmd;
	TSharedPtr<IMKDatabaseParameter> mBuildingParam_X;
	TSharedPtr<IMKDatabaseParameter> mBuildingParam_Y;
	TSharedPtr<IMKDatabaseParameter> mBuildingParam_Z;

	TSharedPtr<FMapTileDataSource> mDataSource;

public:
	FMapTileDB(TSharedPtr<IMKDatabaseConnection> InDB, int InX = 0, int InY = 0, int InZ = 0);

	static TSharedPtr<FMapTileDB> Empty() {
		static TSharedPtr<FMapTileDB> empty = MakeShareable(new FMapTileDB(NULL));
		return empty;
	}
};

struct FTileBuilding {
	int X, Y, Z;
	FTileBuilding(int X, int Y, int Z);
	TSharedPtr<FMapTileDB> DB;
	TArray<FMapPrimitivePtr> Primitives;
	void Load();

	void LoadBuilding(FMapPrimitivePtr Element, ANSICHAR* BuildingName, FVector Position, FQuat Rotation, FVector Scale);
};

struct FMapTile
{
	int X, Y, Z;
	TArray<FMapPrimitiveRefPtr> TilePrimitives;
	TSharedPtr<FMapTileDB> DB;

	FMapTile(int X, int Y, int Z);
	~FMapTile() {}
	void Load();
};


struct FRouteNode {
	FRouteNode* Next;
	int Index;
	FVector Position;
	TArray<int> FollowNode;

	FRouteNode();
};

struct FRouteMap {
	typedef int32 FNodeRef;

	int TileX;
	int TileY;

	TArray<FRouteNode> Nodes;
	TMap<uint64, FRouteNode*> Map;

	FRouteMap();

	void AddNode(FRouteNode& node);
	FRouteNode* GetNodeList(int32 X, int32 Y);

	void FindNearPoints(const FVector& InPoint, float InRange, TArray<int32>& OutPoints);

	int32 GetNeighbourCount(FNodeRef NodeRef) const { return Nodes[NodeRef].FollowNode.Num(); }
	bool IsValidRef(FNodeRef NodeRef) const { return NodeRef >= 0 && NodeRef < Nodes.Num(); }
	FNodeRef GetNeighbour(const FNodeRef NodeRef, const int32 NeiIndex) const { return Nodes[NodeRef].FollowNode[NeiIndex]; }
};

template <typename TCacheNode>
struct FMapLoader : FRunnable {

public:
	FMapLoader(const TCHAR* Name) {
		static const int32 ThreadStackSize = 4 * 1024 * 1024;
		Terminate = false;
		WorkerThread = FRunnableThread::Create(this, Name, ThreadStackSize, TPri_Normal);
	}

	~FMapLoader() {
		Terminate = true;
		WorkerThread->WaitForCompletion();
		delete WorkerThread;
		WorkerThread = nullptr;
	}

	void Sync() {
		FCacheNode* listCached = NULL;
		{
			FScopeLock Lock(&Mutex);
			listCached = CachedList.PopAll();
		}

		while (listCached != NULL) {
			TCacheNode* node = (TCacheNode*)listCached;
			listCached = listCached->Next;

			node->Sync();

			delete node;
		}

		{
			FScopeLock Lock(&Mutex);
			FreeList.Merge(CacheList);
		}
	}

	void CacheTile(TCacheNode* Info) {
		if (Info == NULL)
			return;

		FScopeLock Lock(&Mutex);
		if (Info->IsValid())
			return;

		if (Info->Next != Info->Prev)
			FreeList.Remove(Info);
		CacheList.Push(Info);
	}

private:
	virtual bool Init() override {
		return true;
	}

	virtual uint32 Run() override {
		while (!Terminate) {
			TCacheNode* Info = NULL;
			{
				FScopeLock Lock(&Mutex);
				Info = (TCacheNode*)CacheList.Pop();
				if (Info != NULL)
					Info->Init();
			}

			if (Info == NULL) {
				//FPlatformProcess::Sleep(0);
			}
			else {
				Info->Load();
				//Info->Cache->Load();

				FScopeLock Lock(&Mutex);
				CachedList.Push(Info);
			}
		}

		return 0;
	}

private:
	FRunnableThread* WorkerThread;
	FCriticalSection Mutex;

	struct FCacheInfo : FCacheNode {
		int X, Y, Z;
		TSharedPtr<FMapTile> Cache;
	};

	FCacheList CacheList;
	FCacheList FreeList;
	FCacheList CachedList;
	bool Terminate;
};

struct FMapRender {
	FMapRender();
	~FMapRender();

	static FMapRenderWeakPtr sInstance;
	static FMapRenderPtr Instance() {
		if (sInstance.IsValid())
			return sInstance.Pin();
		FMapRenderPtr newMap = MakeShareable(new FMapRender());
		sInstance = newMap;
		return newMap;
		return NULL;
	}
public:
	void Sync();
	TSharedPtr<FMapTile> GetTile(int X, int Y, int Z);
	TSharedPtr<FTileBuilding> GetTileBuilding(int X, int Y, int Z);
private:
	//	void CacheTile(int X, int Y, int Z);

	static const int MipMapCount = 20;

	//	TMap<int64, FMap::FCacheInfo*> CachingTiles[MipMapCount];

	struct TileElement : FCacheNode {
		TSharedPtr<FMapTile> Tile;
	};
	TArray<TileElement> TileElements;
	FCacheList TileElementList;
	TMap<int64, TileElement*> AllTiles[MipMapCount];

	struct TileBuildingElement : FCacheNode {
		TSharedPtr<FTileBuilding> Tile;
	};
	TArray<TileBuildingElement> TileBuildingElements;
	FCacheList TileBuildingElementList;
	TMap<int64, TileBuildingElement*> TileBuilding;

public:
	void AddTileBuilding(int X, int Y, int Z, TSharedPtr<FTileBuilding> Tile);
	void AddMapTile(int X, int Y, int Z, TSharedPtr<FMapTile> Tile);
};

struct FMap {
	~FMap();
private:
	FMap();

	friend struct FMapRender;

	static FMapWeakPtr sInstance;
public:
	static FMapPtr New() {
		if (sInstance.IsValid())
			return sInstance.Pin();

		FMapPtr newMap = MakeShareable(new FMap());
		sInstance = newMap;
		return newMap;
	}

	static FMapPtr Instance() {
		if (sInstance.IsValid())
			return sInstance.Pin();
		return NULL;
	}

	//	TSharedPtr<FMapTile> GetTile(int X, int Y, int Z);
	//	TSharedPtr<FTileBuilding> GetTileBuilding(int X, int Y, int Z);

	void Sync();

public:
	static int64 Key(int X, int Y, int Z);

private:
	TMap<FName, FMapPrimitiveWeakPtr> Buildings;

private:
	friend struct FMapTile;
	friend struct FMapTileDataSource;
	friend struct FTileBuilding;

	// Cache Tile
	struct FCacheInfo : FCacheNode {
		int X, Y, Z;
		TSharedPtr<FMapTile> Cache;

		void Init() {
			Cache = MakeShareable(new FMapTile(X, Y, Z));
		}

		void Load() {
			Cache->Load();
		}

		bool IsValid() {
			return Cache.IsValid();
		}

		// called in render thread
		void Sync() {
			FMapRenderPtr mapRender = FMapRender::Instance();
			if (mapRender.IsValid())
				mapRender->AddMapTile(X, Y, Z, Cache);
		}
	};

	void CacheTile(int X, int Y, int Z);

	static const int MipMapCount = 20;
	TMap<int64, FCacheInfo*> CachingTiles[MipMapCount];
	FMapLoader<FCacheInfo> TileLoader;

	// Cache Tile Building
	struct FBuildingCacheInfo : FCacheNode {
		int X, Y, Z;
		TSharedPtr<FTileBuilding> Cache;

		void Init() {
			Cache = MakeShareable(new FTileBuilding(X, Y, Z));
		}

		void Load() {
			Cache->Load();
		}

		bool IsValid() {
			return Cache.IsValid();
		}

		// called in render thread
		void Sync() {
			FMapRenderPtr map = FMapRender::Instance();
			//	if (map.IsValid())

			map->AddTileBuilding(X, Y, Z, Cache);
		}
	};

	void CacheTileBuilding(int X, int Y, int Z);

	TMap<int64, FBuildingCacheInfo*> CachingBuildings;
	FMapLoader<FBuildingCacheInfo> TileBuildingLoader;

	FCriticalSection TileDBMapMutex;
	TMap<int64, TWeakPtr<FMapTileDB>> TileDBs;
	IMKDatabaseModule& DBModule;
	TSharedPtr<FMapTileDB> GetTileDB(int X, int Y, int Z);

	/*	struct TileElement : FCacheNode{
	TSharedPtr<FMapTile> Tile;
	};
	TArray<TileElement> TileElements;
	FCacheList TileElementList;
	TMap<int64, TileElement*> AllTiles[MipMapCount];
	void AddMapTile(int X, int Y, int Z, TSharedPtr<FMapTile> Tile);

	struct TileBuildingElement : FCacheNode{
	TSharedPtr<FTileBuilding> Tile;
	};
	TArray<TileBuildingElement> TileBuildingElements;
	FCacheList TileBuildingElementList;
	TMap<int64, TileBuildingElement*> TileBuilding;
	void AddTileBuilding(int X, int Y, int Z, TSharedPtr<FTileBuilding> Tile);
	*/
};