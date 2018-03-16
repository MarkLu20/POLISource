#include "TileMeshComponent.h"
#include "RenderingThread.h"
#include "RenderResource.h"
#include "PrimitiveViewRelevance.h"
#include "PrimitiveSceneProxy.h"
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

const static int MipLevel = 14;
const static int MaxMipLevel = 17;
const static int MipLevelDelta = 19 - MipLevel;
const static int MaxMipLevelDelta = 19 - MaxMipLevel;
const static float BaseTileSize = 128 * MapUnit;
const static float TileSize = BaseTileSize * (1 << MipLevelDelta);
const static float MaxTileSize = BaseTileSize * (1 << MaxMipLevelDelta);
const static float TileHalfSize = TileSize / 2;
const static float ClipDist = 60000 * MapUnit;
const static float DistanceLod[] = { 6000 * MapUnit, 12000 * MapUnit, 24000 * MapUnit };
const static float BoxHeight = 10 * MapUnit;

static float GetTileSize(int Lod) { return BaseTileSize * (1 << (19 - Lod)); }

struct CheckNode
{
	int X;
	int Y;
	int Z;
	float Distance;
	CheckNode(int x, int y, int z, float dis)
	{
		X = x;
		Y = y;
		Z = z;
		Distance = dis;
	}
};

/** Scene proxy */
class FTileMeshSceneProxy : public FPrimitiveSceneProxy
{
	UTileMeshComponent* mComponent;
	TArray<CheckNode> CheckList;
	UMaterialInterface* BaseMaterial;
	UMaterialInterface* ScanMaterial;
	FMapPtr Map;
	FMapRenderPtr MapRender;

public:
	FTileMeshSceneProxy(UTileMeshComponent* Component)
		: FPrimitiveSceneProxy(Component)
		, mComponent(Component)
		, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
	{
		BaseMaterial = UMaterial::GetDefaultMaterial(MD_Surface);
		ScanMaterial = mComponent->GetMaterialBySlot(FSlot::Slot.Scan);
		Map = FMap::New();
		MapRender = FMapRender::Instance();
	}


	virtual ~FTileMeshSceneProxy()
	{
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_TileMeshSceneProxy_GetDynamicMeshElements);

		//		Map->Sync();
		MapRender->Sync();

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy(IsSelected()) : NULL,
			FLinearColor(0, 0.5f, 1.f)
		);

		FMaterialRenderProxy* MatScanProxy = NULL;
		if (ScanMaterial != NULL) {
			MatScanProxy = ScanMaterial->GetRenderProxy(false);
		}

		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);


		FVector Offset;
		int Delta = MaxMipLevel - MipLevel;
		int BaseX = mComponent->X >> Delta;
		int BaseY = mComponent->Y >> Delta;
		Offset.X = (BaseX << Delta) - mComponent->X;
		Offset.Y = mComponent->Y - (BaseY << Delta);
		Offset.Z = 0;
		Offset *= GetTileSize(MaxMipLevel);

		FMatrix WorldToLocal = FMatrix::Identity;
		WorldToLocal.SetOrigin(Offset);// +FVector(0, -1, 0) * GetTileSize(MaxMipLevel));
		WorldToLocal *= GetLocalToWorld();
		WorldToLocal = WorldToLocal.Inverse();

		float FarDist = ClipDist + TileSize;
		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];

				FVector ViewPosition = WorldToLocal.TransformPosition(View->ViewLocation);
				ViewPosition.Y *= -1;
				FPlane MirrorPlane(ViewPosition, FVector(0, 1, 0));
				FConvexVolume ViewFrustum;
				for (int i = 0; i < View->ViewFrustum.Planes.Num(); ++i) {
					FPlane plane = View->ViewFrustum.Planes[i].TransformBy(WorldToLocal);
					FVector normal = plane;
					FVector position = plane.W * normal;
					normal.Y *= -1;// = normal.MirrorByPlane(MirrorPlane);
					position.Y *= -1;
					//position = position.MirrorByPlane(MirrorPlane);
					ViewFrustum.Planes.Add(FPlane(position, normal));
				}
				//FVector ViewDirection = View->GetViewDirection();
				//FPlane FarClipPlane(View->ViewLocation + ViewDirection * ClipDist, ViewDirection);
				//ViewFrustum.Planes.Add(FarClipPlane.TransformBy(WorldToLocal));
				ViewFrustum.Init();


				const_cast<FTileMeshSceneProxy*>(this)->BuildCheckList(ViewPosition, ViewFrustum);

				for (int i = 0; i < CheckList.Num(); ++i) {

					int X = CheckList[i].X;
					int Y = CheckList[i].Y;
					int Z = CheckList[i].Z;
					if (Z != MaxMipLevel)
						continue;

					int delta = Z - MipLevel;
					int xx = BaseX << delta;
					int yy = BaseY << delta;
					TSharedPtr<FTileBuilding> tile = MapRender->GetTileBuilding(xx + X, yy + Y, Z);

					if (!tile.IsValid())
						continue;

					FMatrix tLocalToWorld;
					tLocalToWorld.SetIdentity();

					int TileOffset = Z - 14;
					X = (((xx + X) >> TileOffset) << TileOffset) - xx;
					Y = (((yy + Y) >> TileOffset) << TileOffset) - yy;
					float Size = GetTileSize(Z);
					tLocalToWorld.SetOrigin(Offset + FVector(X, -Y, 0) * Size);
					FMatrix LocalToWorld = tLocalToWorld*GetLocalToWorld();

					float HalfSize = Size / 2;
					FBoxSphereBounds LocalBounds;
					LocalBounds.Origin = FVector(HalfSize, -HalfSize, 0);
					LocalBounds.BoxExtent = FVector(HalfSize, HalfSize, BoxHeight);
					LocalBounds.SphereRadius = FMath::Sqrt(2.0f) * HalfSize;

					for (int j = 0; j < tile->Primitives.Num(); ++j) {
						FMapPrimitivePtr Primitive = tile->Primitives[j];

						//visiable filter
						if (!Primitive->isVisiable)
							continue;
						
						int32 ShowLod = FSlot::Slot.GetLod(Primitive->Slot);
						if (Z < ShowLod)	continue;

						if (!Primitive->VertexFactory.IsInitialized()) {
							// Init vertex factory
							Primitive->VertexFactory.Init(&Primitive->VertexBuffer);

							// Enqueue initialization of render resource
							BeginInitResource(&(Primitive->VertexBuffer));
							BeginInitResource(&(Primitive->IndexBuffer));
							BeginInitResource(&(Primitive->VertexFactory));
							continue;
						}

						FMaterialRenderProxy* MaterialProxy[2] = { NULL };
						if (bWireframe)
						{
							MaterialProxy[0] = WireframeMaterialInstance;
						}
						else
						{
							UMaterialInterface* material = mComponent->GetMaterialBySlot(Primitive->Slot);
							if (material == NULL)
								continue;
							MaterialProxy[0] = material->GetRenderProxy(IsSelected());
							MaterialProxy[1] = MatScanProxy;
						}

						for (int m = 0; m < 2; ++m) {
							if (MaterialProxy[m] == NULL)
								continue;
							// Draw the mesh.
							FMeshBatch& Mesh = Collector.AllocateMesh();
							FMeshBatchElement& BatchElement = Mesh.Elements[0];
							BatchElement.IndexBuffer = &(Primitive->IndexBuffer);
							Mesh.bWireframe = bWireframe;
							Mesh.VertexFactory = &(Primitive->VertexFactory);

							Mesh.MaterialRenderProxy = MaterialProxy[m];

							BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(LocalToWorld, LocalBounds.TransformBy(LocalToWorld), LocalBounds, true, UseEditorDepthTest());
							BatchElement.FirstIndex = 0;
							BatchElement.NumPrimitives = Primitive->IndexBuffer.Indices.Num() / 3;
							BatchElement.MinVertexIndex = 0;
							BatchElement.MaxVertexIndex = Primitive->VertexBuffer.Vertices.Num() - 1;
							Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
							Mesh.Type = PT_TriangleList;
							Mesh.DepthPriorityGroup = SDPG_World;
							Mesh.bCanApplyViewModeOverrides = false;
							Collector.AddMesh(ViewIndex, Mesh);
						}
					}
				}

				for (int i = 0; i < CheckList.Num(); ++i) {

					int X = CheckList[i].X;
					int Y = CheckList[i].Y;
					int Z = CheckList[i].Z;

					int delta = Z - MipLevel;
					int xx = BaseX << delta;
					int yy = BaseY << delta;
					TSharedPtr<FMapTile> tile = MapRender->GetTile(xx + X, yy + Y, Z);//Map->GetTile(xx + X, yy + Y, Z);

					if (!tile.IsValid())
						continue;

					FMatrix tLocalToWorld;
					tLocalToWorld.SetIdentity();
					//					int TileOffset = MipLevel - 14;
					//					X = (((mComponent->X + X) >> TileOffset) << TileOffset) - mComponent->X;
					//					Y = (((mComponent->Y + Y) >> TileOffset) << TileOffset) - mComponent->Y;
					int TileOffset = Z - 14;
					X = (((xx + X) >> TileOffset) << TileOffset) - xx;
					Y = (((yy + Y) >> TileOffset) << TileOffset) - yy;
					float Size = GetTileSize(Z);
					tLocalToWorld.SetOrigin(Offset + FVector(X, -Y, 0) * Size);
					FMatrix LocalToWorld = tLocalToWorld*GetLocalToWorld();

					float HalfSize = Size / 2;
					FBoxSphereBounds LocalBounds;
					LocalBounds.Origin = FVector(HalfSize, -HalfSize, 0);
					LocalBounds.BoxExtent = FVector(HalfSize, HalfSize, 1);
					LocalBounds.SphereRadius = FMath::Sqrt(2.0f) * HalfSize;

					for (int j = 0; j < tile->TilePrimitives.Num(); ++j) {
						FMapPrimitiveRefPtr PrimRef = tile->TilePrimitives[j];
						FMapPrimitivePtr Primitive = PrimRef->Primitive;
						//FMapPrimitivePtr Primitive = MakeShareable<FMapPrimitive>(new FMapPrimitive);

						int32 ShowLod = FSlot::Slot.GetLod(Primitive->Slot);
						if (Z < ShowLod)	continue;
						
						if (!Primitive->VertexFactory.IsInitialized()) {
							// Init vertex factory
							Primitive->VertexFactory.Init(&Primitive->VertexBuffer);

							// Enqueue initialization of render resource
							BeginInitResource(&(Primitive->VertexBuffer));
							BeginInitResource(&(Primitive->IndexBuffer));
							BeginInitResource(&(Primitive->VertexFactory));
							continue;
						}

						FMaterialRenderProxy* MaterialProxy = NULL;
						if (bWireframe)
						{
							MaterialProxy = WireframeMaterialInstance;
						}
						else
						{
							UMaterialInterface* material = mComponent->GetMaterialBySlot(Primitive->Slot);
							if (material != NULL)
								MaterialProxy = material->GetRenderProxy(IsSelected());
						}

						if (MaterialProxy == NULL)
							continue;

						// Draw the mesh.
						FMeshBatch& Mesh = Collector.AllocateMesh();
						FMeshBatchElement& BatchElement = Mesh.Elements[0];
						BatchElement.IndexBuffer = &(Primitive->IndexBuffer);
						Mesh.bWireframe = bWireframe;
						Mesh.VertexFactory = &(Primitive->VertexFactory);

						Mesh.MaterialRenderProxy = MaterialProxy;

						BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(LocalToWorld, LocalBounds.TransformBy(LocalToWorld), LocalBounds, true, UseEditorDepthTest());
						BatchElement.FirstIndex = PrimRef->StartIndex;
						BatchElement.NumPrimitives = PrimRef->Count / 3;
						BatchElement.MinVertexIndex = 0;
						BatchElement.MaxVertexIndex = Primitive->VertexBuffer.Vertices.Num() - 1;
						Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
						Mesh.Type = PT_TriangleList;
						Mesh.DepthPriorityGroup = SDPG_World;
						Mesh.bCanApplyViewModeOverrides = false;
						Collector.AddMesh(ViewIndex, Mesh);
					}
				}
			}
		}
	}

	void BuildLowerQuad(const int CurLod, const FVector2D& Center, const FVector& ViewLocation, const FConvexVolume& ViewFrustum) {
		float Size = GetTileSize(CurLod);
		float Range = Size / 2;
		FVector Origin(Center, 0);
		FVector Extent(Range, Range, 1);

		bool FullyContain = false;
		FOutcode code = ViewFrustum.GetBoxIntersectionOutcode(Origin, Extent);
		bool inside = code.GetInside();
		bool outside = code.GetOutside();
		if (ViewFrustum.IntersectBox(Origin, Extent, FullyContain)) {
			FVector vec;
			vec.X = FMath::Max(0.f, FMath::Abs(ViewLocation.X - Center.X) - Range);
			vec.Y = FMath::Max(0.f, FMath::Abs(ViewLocation.Y - Center.Y) - Range);
			vec.Z = ViewLocation.Z;
			float distance = vec.Size();
			//float distance = FVector::Distance(ViewLocation, Origin);		

			if (distance < DistanceLod[0]) {
				if (CurLod >= 17) {
					FVector2D v = Center / Size;
					v.X = FMath::FloorToInt(v.X);
					v.Y = FMath::FloorToInt(v.Y);
					CheckList.Add(CheckNode(v.X, v.Y, CurLod, distance));
				}
				else {
					float HalfRange = Range / 2;
					BuildLowerQuad(CurLod + 1, Center + FVector2D(HalfRange, HalfRange), ViewLocation, ViewFrustum);
					BuildLowerQuad(CurLod + 1, Center + FVector2D(HalfRange, -HalfRange), ViewLocation, ViewFrustum);
					BuildLowerQuad(CurLod + 1, Center + FVector2D(-HalfRange, HalfRange), ViewLocation, ViewFrustum);
					BuildLowerQuad(CurLod + 1, Center + FVector2D(-HalfRange, -HalfRange), ViewLocation, ViewFrustum);
				}
			}
			else if (distance < DistanceLod[1]) {
				if (CurLod >= 16) {
					FVector2D v = Center / Size;
					v.X = FMath::FloorToInt(v.X);
					v.Y = FMath::FloorToInt(v.Y);
					CheckList.Add(CheckNode(v.X, v.Y, CurLod, distance));
				}
				else {
					float HalfRange = Range / 2;
					BuildLowerQuad(CurLod + 1, Center + FVector2D(HalfRange, HalfRange), ViewLocation, ViewFrustum);
					BuildLowerQuad(CurLod + 1, Center + FVector2D(HalfRange, -HalfRange), ViewLocation, ViewFrustum);
					BuildLowerQuad(CurLod + 1, Center + FVector2D(-HalfRange, HalfRange), ViewLocation, ViewFrustum);
					BuildLowerQuad(CurLod + 1, Center + FVector2D(-HalfRange, -HalfRange), ViewLocation, ViewFrustum);
				}
			}
			else if (distance < DistanceLod[2]) {
				if (CurLod >= 15) {
					FVector2D v = Center / Size;
					v.X = FMath::FloorToInt(v.X);
					v.Y = FMath::FloorToInt(v.Y);
					CheckList.Add(CheckNode(v.X, v.Y, CurLod, distance));
				}
				else {
					float HalfRange = Range / 2;
					BuildLowerQuad(CurLod + 1, Center + FVector2D(HalfRange, HalfRange), ViewLocation, ViewFrustum);
					BuildLowerQuad(CurLod + 1, Center + FVector2D(HalfRange, -HalfRange), ViewLocation, ViewFrustum);
					BuildLowerQuad(CurLod + 1, Center + FVector2D(-HalfRange, HalfRange), ViewLocation, ViewFrustum);
					BuildLowerQuad(CurLod + 1, Center + FVector2D(-HalfRange, -HalfRange), ViewLocation, ViewFrustum);
				}
			}
			else {
				FVector2D v = Center / Size;
				v.X = FMath::FloorToInt(v.X);
				v.Y = FMath::FloorToInt(v.Y);
				CheckList.Add(CheckNode(v.X, v.Y, CurLod, distance));
			}
		}
		//else{
		//if(true){// (CurLod >= 17) {
		//		FVector2D v = Center / Size;
		//		v.X = (int)v.X;
		//		v.Y = (int)v.Y;
		//		CheckList.Add(CheckNode(v.X, v.Y, CurLod, FVector::Distance(ViewLocation, Origin)));
		//	}
		//	else {
		//		float HalfRange = Range / 2;
		//		BuildLowerQuad(CurLod + 1, Center + FVector2D(HalfRange, HalfRange), ViewLocation, ViewFrustum, HalfRange);
		//		BuildLowerQuad(CurLod + 1, Center + FVector2D(HalfRange, -HalfRange), ViewLocation, ViewFrustum, HalfRange);
		//		BuildLowerQuad(CurLod + 1, Center + FVector2D(-HalfRange, HalfRange), ViewLocation, ViewFrustum, HalfRange);
		//		BuildLowerQuad(CurLod + 1, Center + FVector2D(-HalfRange, -HalfRange), ViewLocation, ViewFrustum, HalfRange);
		//	}		
		//}
	}

	void BuildQuad(const FVector2D& Center, const FVector& ViewLocation, const FConvexVolume& ViewFrustum, int level) {
		if (level == 0) {
			BuildLowerQuad(MipLevel, Center, ViewLocation, ViewFrustum);
			return;
		}

		float Range = FMath::Pow(2, level) * TileHalfSize;
		FVector Origin(Center, 0);
		FVector Extent(Range, Range, 1);
		bool FullyContain = false;
		if (ViewFrustum.IntersectBox(Origin, Extent, FullyContain)) {
			float HalfRange = Range / 2;
			BuildQuad(Center + FVector2D(HalfRange, HalfRange), ViewLocation, ViewFrustum, level - 1);
			BuildQuad(Center + FVector2D(HalfRange, -HalfRange), ViewLocation, ViewFrustum, level - 1);
			BuildQuad(Center + FVector2D(-HalfRange, HalfRange), ViewLocation, ViewFrustum, level - 1);
			BuildQuad(Center + FVector2D(-HalfRange, -HalfRange), ViewLocation, ViewFrustum, level - 1);
		}
	}

	void BuildCheckList(const FVector& ViewLocation, const FConvexVolume& ViewFrustum) {
		FVector2D vCenter(ViewLocation.X / TileSize, ViewLocation.Y / TileSize);//所在Lod14的Tile Origin
		FVector2D center(FMath::RoundToInt(vCenter.X), FMath::RoundToInt(vCenter.Y));
		center *= TileSize;//Camera 的调整后坐标
		CheckList.Empty();
		int32 level = FMath::CeilToInt(FMath::Log2(ClipDist / TileHalfSize));
		BuildQuad(center, ViewLocation, ViewFrustum, level);

		CheckList.Sort([](const CheckNode& A, const CheckNode& B) -> bool {
			return A.Distance < B.Distance;
		});

		/*		UE_LOG(LogTemp, Log, TEXT("START"));
		for (int i = 0; i < CheckList.Num(); ++i)
		{
		UE_LOG(LogTemp, Log, TEXT("%f , %f"), CheckList[i].X , CheckList[i].Y);
		}
		UE_LOG(LogTemp, Log, TEXT("End"));
		*/
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;
		Result.bRenderInMainPass = ShouldRenderInMainPass();
		Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		return Result;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

private:

	FMaterialRelevance MaterialRelevance;
};


UTileMeshComponent::UTileMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	X = Y = -1;
}

void UTileMeshComponent::Initialize(int x, int y) {
	x = x >> MaxMipLevelDelta;
	y = y >> MaxMipLevelDelta;
	if (X == x && Y == y)
		return;
	X = x;
	Y = y;
	BaseX = X << MaxMipLevelDelta;
	BaseY = Y << MaxMipLevelDelta;
	MarkRenderStateDirty();
}

FPrimitiveSceneProxy* UTileMeshComponent::CreateSceneProxy() {
	FPrimitiveSceneProxy* Proxy = NULL;
	if (X != -1 && Y != -1) {
		Proxy = new FTileMeshSceneProxy(this);
	}
	return Proxy;
}

int32 UTileMeshComponent::GetNumMaterials() const {
	return FSlot::Slot.Num();
}

FBoxSphereBounds UTileMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds NewBounds;
	NewBounds.Origin = FVector::ZeroVector;
	NewBounds.BoxExtent = FVector(HALF_WORLD_MAX, HALF_WORLD_MAX, HALF_WORLD_MAX);
	NewBounds.SphereRadius = FMath::Sqrt(3.0f * FMath::Square(HALF_WORLD_MAX));
	return NewBounds;
}

int32 UTileMeshComponent::GetMaterialIndex(FName MaterialSlotName) const {
	return FSlot::Slot.GetIndex(MaterialSlotName);
}

TArray<FName> UTileMeshComponent::GetMaterialSlotNames() const {
	return FSlot::Slot;
}

bool UTileMeshComponent::IsMaterialSlotNameValid(FName MaterialSlotName) const {
	return GetMaterialIndex(MaterialSlotName) != -1;
}

UMaterialInterface* UTileMeshComponent::GetMaterialBySlot(FName SlotName) const {
	int32 idx = GetMaterialIndex(SlotName);
	return GetMaterial(idx);
}

struct BMapPixel {
	int X, Y;

	BMapPixel() {
		X = Y = 0;
	}
	BMapPixel(int x, int y) {
		X = x;
		Y = y;
	}
};

struct BMapPoint {
	double longitude;
	double latitude;

	BMapPoint() {
		longitude = latitude = 0;
	}
	BMapPoint(double x, double y) {
		longitude = x;
		latitude = y;
	}
};


const static double pi = 3.1415926535897932384626433832795;
const static double a = 6378245.0; //卫星椭球坐标投影到平面坐标系的投影因子
const static double ee = 0.00669342162296594323; //椭球的偏心率
class BMapCoordinate {
public:
	static BMapPixel lngLatToPoint(double x, double y)
	{
		BMapPoint point = zb(BMapPoint(x, y));
		return BMapPixel((int)point.longitude, (int)point.latitude);
	}

	static double PixelToTile(int pixel, int Level)
	{
		double n = pow(2, Level - 18);
		return pixel * n / 256;
	}


	/*
	* 地球坐标转火星坐标
	* @param wgLat  地球坐标
	* @param wgLon
	* mglat,mglon 火星坐标
	*/
	static BMapPoint wgs84_To_gcj02(double wgLon, double wgLat)
	{
		double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
		double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
		double radLat = wgLat / 180.0 * pi;
		double magic = sin(radLat);
		magic = 1 - ee * magic * magic;
		double sqrtMagic = sqrt(magic);
		dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
		dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
		double mgLat = wgLat + dLat;
		double mgLon = wgLon + dLon;

		return BMapPoint(mgLon, mgLat);
	}
	/*
	* 火星转地球
	*/
	static BMapPoint gcj02_To_wgs84(double wgLon, double wgLat)
	{
		double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
		double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
		double radLat = wgLat / 180.0 * pi;
		double magic = sin(radLat);
		magic = 1 - ee * magic * magic;
		double sqrtMagic = sqrt(magic);
		dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
		dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
		double mgLat = wgLat + dLat;
		double mgLon = wgLon + dLon;
		return BMapPoint(wgLon * 2 - mgLon, wgLat * 2 - mgLat);
	}


	static double transformLat(double x, double y)
	{
		double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(FMath::Abs(x));
		ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
		ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
		ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
		return ret;
	}

	static double transformLon(double x, double y)
	{
		double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(FMath::Abs(x));
		ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
		ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
		ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
		return ret;
	}
	/*
	** 火星坐标系 (GCJ-02) 与百度坐标系 (BD-09) 的转换算法 将 GCJ-02 坐标转换成 BD-09 坐标
	*/
	static BMapPoint gcj02_To_Bd09(double gg_lon, double gg_lat)
	{
		double x = gg_lon, y = gg_lat;
		double z = sqrt(x * x + y * y) + 0.00002 * sin(y * 3.1415926535897932384626433832795 * 3000.0 / 180.0);
		double theta = atan2(y, x) + 0.000003 * cos(x * 3.1415926535897932384626433832795 * 3000.0 / 180.0);
		double bd_lon = z * cos(theta) + 0.0065;
		double bd_lat = z * sin(theta) + 0.006;
		return BMapPoint(bd_lon, bd_lat);
	}

private:
	static const BMapPoint& zb(BMapPoint point)
	{
		double c[10] = { 0 };
		point.longitude = MD(point.longitude, -180, 180);
		point.latitude = QD(point.latitude, -74, 74);
		BMapPoint b(point.longitude, point.latitude);

		int Bu[] = { 75, 60, 45, 30, 15, 0 };
		double oG[][10] =
		{ { -0.0015702102444, 111320.7020616939, 1704480524535203, -10338987376042340, 26112667856603880, -35149669176653700, 26595700718403920, -10725012454188240, 1800819912950474, 82.5 }
			,{ 8.277824516172526E-4, 111320.7020463578, 6.477955746671607E8, -4.082003173641316E9, 1.077490566351142E10, -1.517187553151559E10, 1.205306533862167E10, -5.124939663577472E9, 9.133119359512032E8, 67.5 }
			,{ 0.00337398766765, 111320.7020202162, 4481351.045890365, -2.339375119931662E7, 7.968221547186455E7, -1.159649932797253E8, 9.723671115602145E7, -4.366194633752821E7, 8477230.501135234, 52.5 }
			,{ 0.00220636496208, 111320.7020209128, 51751.86112841131, 3796837.749470245, 992013.7397791013, -1221952.21711287, 1340652.697009075, -620943.6990984312, 144416.9293806241, 37.5 }
			,{ -3.441963504368392E-4, 111320.7020576856, 278.2353980772752, 2485758.690035394, 6070.750963243378, 54821.18345352118, 9540.606633304236, -2710.55326746645, 1405.483844121726, 22.5 }
		,{ -3.218135878613132E-4, 111320.7020701615, 0.00369383431289, 823725.6402795718, 0.46104986909093, 2351.343141331292, 1.58060784298199, 8.77738589078284, 0.37238884252424, 7.45 } };

		bool processed = false;
		for (int d = 0; d < 6; d++)
		{
			if (b.latitude >= Bu[d])
			{
				for (int m = 0; m < 10; m++)
					c[m] = oG[d][m];
				processed = true;
				break;
			}
		}
		if (!processed)
		{
			for (int d = 0; d < 6; d++)
				if (b.latitude <= -Bu[d])
				{
					for (int m = 0; m < 10; m++)
						c[m] = oG[d][m];
					processed;
					break;
				}
		}
		point = oK(point, c, processed ? 10 : 0);
		return point;
	}

	static double MD(double a, double b, double c)
	{
		for (; a > c;) a -= c - b;
		for (; a < b;) a += c - b;
		return a;
	}

	static double QD(double a, double b, double c)
	{
		if (b != 0)
			a = a > b ? a : b;
		if (c != 0)
			a = a < c ? a : c;
		return a;
	}

	static BMapPoint oK(BMapPoint a, double b[], int length)
	{
		if (length > 0)
		{
			double c = b[0] + b[1] * (a.longitude < 0 ? -a.longitude : a.longitude);
			double d = (a.latitude < 0 ? -a.latitude : a.latitude) / b[9];
			d = b[2] + b[3] * d + b[4] * d * d + b[5] * d * d * d + b[6] * d * d * d * d + b[7] * d * d * d * d * d + b[8] * d * d * d * d * d * d;
			c = c * (0 > a.longitude ? -1 : 1);
			d = d * (0 > a.latitude ? -1 : 1);
			return BMapPoint(c, d);
		}
		return BMapPoint(0, 0);
	}
};

FVector UTileMeshComponent::ComputeLocalPositionImpl(double longitude, double latitude) {
	BMapPixel pixel = BMapCoordinate::lngLatToPoint(longitude, latitude);
	double tileX = BMapCoordinate::PixelToTile(pixel.X, MaxMipLevel) - X;
	double tileY = Y - BMapCoordinate::PixelToTile(pixel.Y, MaxMipLevel);

	return FVector((float)tileX * MaxTileSize, (float)tileY * MaxTileSize, 0);
}

FVector UTileMeshComponent::ComputeLocalPosition(const FString& longitude, const FString& latitude) {
	return ComputeLocalPositionImpl(FCString::Atod(*longitude), FCString::Atod(*latitude));
}

FVector UTileMeshComponent::ComputeWorldPosition(const FString& longitude, const FString& latitude) {
	FVector localPosition = ComputeLocalPosition(longitude, latitude);

	return GetComponentTransform().TransformPosition(localPosition);
}

FVector UTileMeshComponent::ComputeLocalPositionGCJ(const FString& longitude, const FString& latitude) {
	double lng = FCString::Atod(*longitude);
	double lat = FCString::Atod(*latitude);
	BMapPoint pt = BMapCoordinate::gcj02_To_Bd09(lng, lat);
	return ComputeLocalPositionImpl(pt.longitude, pt.latitude);
}

FVector UTileMeshComponent::ComputeWorldPositionGCJ(const FString& longitude, const FString& latitude) {
	FVector localPosition = ComputeLocalPositionGCJ(longitude, latitude);
	return GetComponentTransform().TransformPosition(localPosition);
}

FVector UTileMeshComponent::ComputeLocalPositionMars(const FString& longitude, const FString& latitude) {
	double lng = FCString::Atod(*longitude);
	double lat = FCString::Atod(*latitude);
	BMapPoint ptMars = BMapCoordinate::wgs84_To_gcj02(lng, lat);
	BMapPoint ptBD = BMapCoordinate::gcj02_To_Bd09(ptMars.longitude, ptMars.latitude);
	return ComputeLocalPositionImpl(ptBD.longitude, ptBD.latitude);
}

FVector UTileMeshComponent::ComputeWorldPositionMars(const FString& longitude, const FString& latitude) {
	FVector localPosition = ComputeLocalPositionMars(longitude, latitude);
	return GetComponentTransform().TransformPosition(localPosition);
}

TArray<int> UTileMeshComponent::FindNearRoutePoints(FVector InPoint, float InRange) {
	FRouteMap& RouteMap = mRouteMap;
	FVector vDelta(BaseTileSize * (BaseX - RouteMap.TileX), -BaseTileSize * (BaseY - RouteMap.TileY), 0);
	FVector Point = InPoint + vDelta;

	TArray<int> Result;
	RouteMap.FindNearPoints(Point, InRange, Result);
	return Result;
}

FVector UTileMeshComponent::GetRoutePointWorldPosition(int id) {
	FRouteMap& RouteMap = mRouteMap;
	FVector vDelta(BaseTileSize * (BaseX - RouteMap.TileX), -BaseTileSize * (BaseY - RouteMap.TileY), 0);
	FVector localPosition = mRouteMap.Nodes[id].Position - vDelta;
	return GetComponentTransform().TransformPosition(localPosition);
}

TArray<int> UTileMeshComponent::GetRoutePointFollowPath(int id) {
	return mRouteMap.Nodes[id].FollowNode;
}