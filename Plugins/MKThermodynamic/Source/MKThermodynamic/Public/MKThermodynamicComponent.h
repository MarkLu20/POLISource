// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "DynamicMeshBuilder.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneComponent.h"
#include "MKThermodynamicComponent.generated.h"

class FPrimitiveSceneProxy;

// MKThermodynamic Quad
class FMKThermodynamicQuadVertexBuffer : public FVertexBuffer
{
public:
	virtual void InitRHI() override;
};

/** MKThermodynamic Instance */
USTRUCT(BlueprintType)
struct FMKThermodynamicInstance
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MKThermodynamicItem)
		FVector WorldPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MKThermodynamicItem)
		float Heat;
};

class FMKThermodynamicInstanceVertexBuffer : public FVertexBuffer
{
public:
	int InstanceCount;
	float Radius;
	void Rebuild(const TArray<FMKThermodynamicInstance>& InInstances, float InRadius);
};

/** Vertex declaration for just one FVector4 position. */
class FMKThermodynamicVertexDeclaration : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;
	virtual void InitRHI() override;
	virtual void ReleaseRHI() override;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MKTHERMODYNAMIC_API UMKThermodynamicComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MKThermodynamic")
	UTextureRenderTarget2D* RenderTarget;

public:	
	// Sets default values for this component's properties
	UMKThermodynamicComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginDestroy() override;

	/** Set the geometry to use on this triangle mesh */
	UFUNCTION(BlueprintCallable, Category = "Components|MKThermodynamic")
	void SetThermodynamicInfo(const TArray<FMKThermodynamicInstance>& InItems, float InRadius);

protected:
	virtual void RenderThread_UpdateThermodynamic(FRHICommandList& RHICmdList, FTextureRenderTarget2DResource* TextureRenderTarget, FMatrix& InViewProj, FIntRect& InViewRect, ERHIFeatureLevel::Type FeatureLevel);

	virtual void RenderThread_SetThermodynamicInfo(const TArray<FMKThermodynamicInstance>& InItems, float InRadius);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FMKThermodynamicQuadVertexBuffer QuadVB;
	FMKThermodynamicInstanceVertexBuffer InstanceVB;
	FMKThermodynamicVertexDeclaration VertexDecl;
};
