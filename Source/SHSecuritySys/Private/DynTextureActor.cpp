// Fill out your copyright notice in the Description page of Project Settings.

#include "DynTextureActor.h"
#include "Engine/Texture2D.h"
#include "Engine/CurveTable.h"

// Sets default values
ADynTextureActor::ADynTextureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADynTextureActor::BeginPlay()
{
	Super::BeginPlay();

	pRichCurve = new FRichCurve();
}

// Called every frame
void ADynTextureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UTexture2D*	ADynTextureActor::CreateDynTexture(int32 width, int32 height)
{	
	pTexture = UTexture2D::CreateTransient(width, height);
	pTexture->AddressX = TextureAddress::TA_Clamp;
	pTexture->AddressY = TextureAddress::TA_Clamp; 
	pTexture->Filter = TextureFilter::TF_Bilinear;
	FTexture2DMipMap& Mip = pTexture->PlatformData->Mips[0];
	FPixelData* Data = (FPixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);
	for (int y = 0; y < height; ++y) 
	{
		int start_idx = y * width;
		for (int x = 0; x < width; ++x) 
		{
			int idx = start_idx + x;
			Data[idx].B = 0;
			Data[idx].G = 0;
			Data[idx].R = 0;
			Data[idx].A = 0;
		}
	}
	Mip.BulkData.Unlock();
	pTexture->UpdateResource();

	return pTexture;
}

void	ADynTextureActor::UpdateDyntexture(UTexture2D* pDynTexture, TArray<float> values, float totalTime) 
{
	FTexture2DMipMap& Mip = pDynTexture->PlatformData->Mips[0];
	FPixelData* Data = (FPixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);	
	
	for (int x = 0; x < pDynTexture->GetSizeX(); ++x)
	{
		float vx = totalTime + x / (float)pDynTexture->GetSizeX() * 2 * PI;
		float vy = (sin(vx) + 1) * 0.5f * pDynTexture->GetSizeY();

		vx = x / (float)pDynTexture->GetSizeX();
		vy = pRichCurve->Eval(vx);

		for (int y = 0; y < pDynTexture->GetSizeY(); ++y)
		{
			int start_idx = y * pDynTexture->GetSizeX();
			int idx = start_idx + x;
			
			int32 v = vy*pDynTexture->GetSizeY();
			if (y >= v && y <= v + LineWidth)
			{
				Data[idx].R = CurveColor.R;
				Data[idx].G = CurveColor.G;
				Data[idx].B = CurveColor.B;
				Data[idx].A = CurveColor.A;				
			}
			else if (y > v + LineWidth)
			{
				Data[idx].R = CurveAreaColor.R;
				Data[idx].G = CurveAreaColor.G;
				Data[idx].B = CurveAreaColor.B;
				Data[idx].A = CurveAreaColor.A;
			}
			else
			{
				//Data[idx].B = 0;
				Data[idx].A = 0;
			}

			//Data[idx].G = 0;
			//Data[idx].R = 0;			
		}
	}
	Mip.BulkData.Unlock();
	pDynTexture->UpdateResource();
}

void	ADynTextureActor::SetCurvesValue(TArray<float> timeAry, TArray<float> valueAry)
{
	if (pRichCurve == NULL)
		pRichCurve = new FRichCurve();

	//void SetKeys(const TArray<float>& InTimes, const TArray<float>& InValues);
	pRichCurve->SetKeys(timeAry, valueAry);

	//FKeyHandle FindKey(float KeyTime, float KeyTimeTolerance = KINDA_SMALL_NUMBER) const;
	FKeyHandle keyHandle = pRichCurve->FindKey(timeAry[0]);
	while (true)
	{
		if(pRichCurve->IsKeyHandleValid(keyHandle))
		{
			pRichCurve->SetKeyInterpMode(keyHandle, CurveInterpMode);
			pRichCurve->SetKeyTangentMode(keyHandle, RCTM_Auto);

			keyHandle = pRichCurve->GetNextKey(keyHandle);
		}
		else
		{
			break;
		}
	}	
}

void	ADynTextureActor::SetCurveMode(int32 lineWidth, FColor curveClr, float fillWeight, ERichCurveInterpMode curveMode)
{
	LineWidth = lineWidth;
	CurveColor = curveClr;
	
	CurveAreaColor.R = curveClr.R * fillWeight;
	CurveAreaColor.G = curveClr.G * fillWeight;
	CurveAreaColor.B = curveClr.B * fillWeight;
	CurveAreaColor.A = curveClr.A * fillWeight;

	CurveInterpMode = curveMode;
}

