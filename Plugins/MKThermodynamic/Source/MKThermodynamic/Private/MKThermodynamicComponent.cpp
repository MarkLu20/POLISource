// Fill out your copyright notice in the Description page of Project Settings.

#include "MKThermodynamicComponent.h"

#include "RenderingThread.h"
#include "RenderResource.h"
#include "VertexFactory.h"
#include "LocalVertexFactory.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GlobalShader.h"
#include "PipelineStateCache.h"
#include "ShaderParameterUtils.h"
#include "Engine/LocalPlayer.h"
#include "SceneView.h"

/** Vertex Buffer */
void FMKThermodynamicQuadVertexBuffer::InitRHI()
{
	FRHIResourceCreateInfo CreateInfo;
	void* VertexBufferData = nullptr;
	VertexBufferRHI = RHICreateAndLockVertexBuffer(4 * sizeof(FVector), BUF_Static, CreateInfo, VertexBufferData);

	FVector* Data = (FVector*)VertexBufferData;
	Data[0] = FVector(-1, -1, 0);
	Data[1] = FVector(-1, 1, 0);
	Data[2] = FVector(1, -1, 0);
	Data[3] = FVector(1, 1, 0);

	RHIUnlockVertexBuffer(VertexBufferRHI);
}

void FMKThermodynamicInstanceVertexBuffer::Rebuild(const TArray<FMKThermodynamicInstance>& InInstances, float InRadius) {
	FRHIResourceCreateInfo CreateInfo;
	void* VertexBufferData = nullptr;
	InstanceCount = InInstances.Num();
	Radius = InRadius;

	VertexBufferRHI = nullptr;
	if (InstanceCount > 0) {
		VertexBufferRHI = RHICreateAndLockVertexBuffer(InstanceCount * sizeof(FMKThermodynamicInstance), BUF_Static, CreateInfo, VertexBufferData);

		// Copy the vertex data into the vertex buffer.		
		FMemory::Memcpy(VertexBufferData, InInstances.GetData(), InstanceCount * sizeof(FMKThermodynamicInstance));

		RHIUnlockVertexBuffer(VertexBufferRHI);
	}
}

/** Vertex Declaration */
void FMKThermodynamicVertexDeclaration::InitRHI()
{
	FVertexDeclarationElementList Elements;

	// vertex
	Elements.Add(FVertexElement(0, 0, VET_Float3, 0, sizeof(FVector)));

	uint16 Stride = sizeof(FMKThermodynamicInstance);
	Elements.Add(FVertexElement(1, STRUCT_OFFSET(FMKThermodynamicInstance, WorldPosition), VET_Float3, 1, Stride, true));
	Elements.Add(FVertexElement(1, STRUCT_OFFSET(FMKThermodynamicInstance, Heat), VET_Float1, 2, Stride, true));
	VertexDeclarationRHI = RHICreateVertexDeclaration(Elements);
}

void FMKThermodynamicVertexDeclaration::ReleaseRHI()
{
	VertexDeclarationRHI.SafeRelease();
}

//////////////////////////////////////////////////////////////////////////
// shaders

class FMKThermodynamicVS : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FMKThermodynamicVS, Global);

public:
	FMKThermodynamicVS() {}
	FMKThermodynamicVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) :
		FGlobalShader(Initializer)
	{
		ThermodynamicRadius.Bind(Initializer.ParameterMap, TEXT("ThermodynamicRadius"));
		ThermodynamicViewProjection.Bind(Initializer.ParameterMap, TEXT("ThermodynamicViewProjection"));
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}

	void SetParameters(FRHICommandList& RHICmdList, FMatrix& matViewProj, float Radius)
	{
		//FGlobalShader::SetParameters<FViewUniformShaderParameters>(RHICmdList, GetVertexShader(), View.ViewUniformBuffer);
		SetShaderValue(RHICmdList, GetVertexShader(), ThermodynamicViewProjection, matViewProj);
		SetShaderValue(RHICmdList, GetVertexShader(), ThermodynamicRadius, Radius);
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ThermodynamicRadius;
		Ar << ThermodynamicViewProjection;
		return bShaderHasOutdatedParameters;
	}

	FShaderParameter ThermodynamicRadius;
	FShaderParameter ThermodynamicViewProjection;
};

/** Pixel shader used for filtering a mip. */
class FMKThermodynamicPS : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FMKThermodynamicPS , Global);

public:
	FMKThermodynamicPS() {}
	FMKThermodynamicPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer) :
		FGlobalShader(Initializer)
	{
		ThermodynamicRadius.Bind(Initializer.ParameterMap, TEXT("ThermodynamicRadius"));
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}

	void SetParameters(FRHICommandList& RHICmdList, float Radius)
	{
		SetShaderValue(RHICmdList, GetPixelShader(), ThermodynamicRadius, Radius);
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ThermodynamicRadius;
		return bShaderHasOutdatedParameters;
	}

	FShaderParameter ThermodynamicRadius;
};

IMPLEMENT_SHADER_TYPE(,FMKThermodynamicVS,TEXT("/Plugin/MKThermodynamic/ThermodynamicShaders.usf"),TEXT("ThermodynamicVS"),SF_Vertex);
IMPLEMENT_SHADER_TYPE(,FMKThermodynamicPS,TEXT("/Plugin/MKThermodynamic/ThermodynamicShaders.usf"),TEXT("ThermodynamicPS"),SF_Pixel);
//////////////////////////////////////////////////////////////////////////
// UMKThermodynamic Component


// Sets default values for this component's properties
UMKThermodynamicComponent::UMKThermodynamicComponent()
{
	RenderTarget = NULL;
	PrimaryComponentTick.bCanEverTick = true;

	InstanceVB.InstanceCount = 0;
}

// Called when the game starts
void UMKThermodynamicComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMKThermodynamicComponent::RenderThread_SetThermodynamicInfo(const TArray<FMKThermodynamicInstance>& Items, float Radius) {

	InstanceVB.Rebuild(Items, Radius);

	if (!VertexDecl.IsInitialized()) {
		VertexDecl.InitResource();
		QuadVB.InitResource();
	}
}

void UMKThermodynamicComponent::SetThermodynamicInfo(const TArray<FMKThermodynamicInstance>& Instances, float Radius)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_THREEPARAMETER(
		SetThermodynamicInfo,
		UMKThermodynamicComponent*, Component, this,
		TArray<FMKThermodynamicInstance>, Instances, Instances,
		float, Radius, Radius,
		{
			Component->RenderThread_SetThermodynamicInfo(Instances, Radius);
		});
}

// Called every frame
void UMKThermodynamicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (RenderTarget == NULL)
		return;

	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer || !LocalPlayer->ViewportClient || !LocalPlayer->ViewportClient->Viewport)
	{
		return;
	}

	FSceneViewFamily ViewFamily(FSceneViewFamily::ConstructionValues(
		LocalPlayer->ViewportClient->Viewport,
		GetWorld()->Scene,
		LocalPlayer->ViewportClient->EngineShowFlags)
		.SetRealtimeUpdate(true));

	FVector ViewLocation;
	FRotator ViewRotation;

	FSceneView* View = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);
	if (!View) { return; }

	FMatrix matViewProj = View->ViewMatrices.GetViewProjectionMatrix();

	ENQUEUE_UNIQUE_RENDER_COMMAND_FIVEPARAMETER(
		RenderThermodynamic,
		UMKThermodynamicComponent*, Component, this,
		FTextureRenderTarget2DResource*, TextureRenderTarget, (FTextureRenderTarget2DResource*)(RenderTarget->GameThread_GetRenderTargetResource()),
		FMatrix, matViewProj, matViewProj,
		FIntRect, rcViewRect, View->ViewRect,
		ERHIFeatureLevel::Type, FeatureLevel, GWorld->Scene->GetFeatureLevel(),
		{
			Component->RenderThread_UpdateThermodynamic(RHICmdList, TextureRenderTarget, matViewProj, rcViewRect, FeatureLevel);
		});

	delete View;
}

void UMKThermodynamicComponent::BeginDestroy()
{
	USceneComponent::BeginDestroy();
	
	BeginReleaseResource(&InstanceVB);
	BeginReleaseResource(&QuadVB);
	BeginReleaseResource(&VertexDecl);
	FlushRenderingCommands();
}

void UMKThermodynamicComponent::RenderThread_UpdateThermodynamic(FRHICommandList& RHICmdList, FTextureRenderTarget2DResource* TextureRenderTarget, FMatrix& InViewProj, FIntRect& InViewRect, ERHIFeatureLevel::Type FeatureLevel)
{
	if (!VertexDecl.IsInitialized())
		return;

	if (InstanceVB.InstanceCount == 0)
		return;

	// 设置渲染目标
	//SetRenderTarget(RHICmdList, TextureRenderTarget->GetRenderTargetTexture(), FTexture2DRHIRef(), ESimpleRenderTargetMode::EClearColorAndDepth, FExclusiveDepthStencil::DepthNop_StencilNop, true);
	FRHIRenderTargetView View = FRHIRenderTargetView(TextureRenderTarget->GetRenderTargetTexture(), ERenderTargetLoadAction::EClear);
	FRHISetRenderTargetsInfo Info(1, &View, FRHIDepthRenderTargetView());
	RHICmdList.SetRenderTargetsAndClear(Info);
	RHICmdList.SetViewport(0, 0, 0.0f, InViewRect.Width(), InViewRect.Height(), 1.f);// TextureRenderTarget->GetSizeXY().X, TextureRenderTarget->GetSizeXY().Y, 1.0f);

	FGraphicsPipelineStateInitializer GraphicsPSOInit;
	RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
	GraphicsPSOInit.RasterizerState = TStaticRasterizerState<FM_Solid, CM_None>::GetRHI();
	GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
	GraphicsPSOInit.BlendState = TStaticBlendState<CW_RGBA, BO_Max, BF_One, BF_One>::GetRHI();

	TShaderMapRef<FMKThermodynamicVS> VertexShader(GetGlobalShaderMap(FeatureLevel));
	TShaderMapRef<FMKThermodynamicPS> PixelShader(GetGlobalShaderMap(FeatureLevel));

	GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = VertexDecl.VertexDeclarationRHI;
	GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
	GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
	GraphicsPSOInit.PrimitiveType = PT_TriangleStrip;

	SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

	VertexShader->SetParameters(RHICmdList, InViewProj, InstanceVB.Radius);
	PixelShader->SetParameters(RHICmdList, InstanceVB.Radius);

	// Quad
	RHICmdList.SetStreamSource(0, QuadVB.VertexBufferRHI, sizeof(FVector), 0);
	RHICmdList.SetStreamSource(1, InstanceVB.VertexBufferRHI, sizeof(FMKThermodynamicInstance), 0);

	RHICmdList.DrawPrimitive(
		PT_TriangleStrip,
		0,
		2,
		InstanceVB.InstanceCount
	);

	//RHICmdList.Flush();

	// 更新贴图
	RHICmdList.CopyToResolveTarget(TextureRenderTarget->GetRenderTargetTexture(), TextureRenderTarget->TextureRHI, true, FResolveParams());
}
