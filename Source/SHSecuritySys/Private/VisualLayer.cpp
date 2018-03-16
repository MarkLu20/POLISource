// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualLayer.h"
#include "Engine/Texture2D.h"
#include "UnrealMathUtility.h"

// Sets default values for this component's properties
UVisualLayer::UVisualLayer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVisualLayer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



// Called every frame
void UVisualLayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

struct PixelData {
	UINT8 B;
	UINT8 G;
	UINT8 R;
	UINT8 A;
};
void UVisualLayer::Initialize(int _width, int _height) {
	Texture = UTexture2D::CreateTransient(_width, _height);
	Texture->AddressX = TextureAddress::TA_Clamp;
	Texture->AddressY = TextureAddress::TA_Clamp;
	Texture->Filter = TextureFilter::TF_Nearest;
	FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
	PixelData* Data = (PixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);
	for (int i = 0; i < _height; ++i) {
		int start_idx = i * _width;
		for (int j = 0; j < _width; ++j) {
			Data[start_idx + j].B = 0;
			Data[start_idx + j].G = 0;// 127 + j % 127;
			Data[start_idx + j].R = 0;
			Data[start_idx + j].A = 0;
		}
	}
	Mip.BulkData.Unlock();
	Texture->UpdateResource();
}

void UVisualLayer::SetColor(int x, int y, FLinearColor color) {
	FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
	if (x < 0 || x >= Mip.SizeX)
		return;

	if (y < 0 || y >= Mip.SizeY)
		return;

	PixelData* Data = (PixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);
	int idx = y * Mip.SizeX + x;
	Data[idx].B = (int)(FMath::Clamp(color.B, 0.f, 1.f) * 255);
	Data[idx].G = (int)(FMath::Clamp(color.G, 0.f, 1.f) * 255);
	Data[idx].R = (int)(FMath::Clamp(color.R, 0.f, 1.f) * 255);
	Data[idx].A = (int)(FMath::Clamp(color.A, 0.f, 1.f) * 255);
	Mip.BulkData.Unlock();
	Texture->UpdateResource();
}

void UVisualLayer::PaintRect(int x, int y, int brush_size, FLinearColor color) {
	FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
	PixelData* Data = (PixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);

	for (int i = 0; i < brush_size; ++i) {
		int _y = y + i;
		if (_y < 0 || _y >= Mip.SizeY)
			continue;
		for (int j = 0; j < brush_size; ++j) {
			int _x = x + j;
			if (_x < 0 || _x >= Mip.SizeX)
				continue;
			int idx = _y * Mip.SizeX + _x;
			Data[idx].B = (int)(FMath::Clamp(color.B, 0.f, 1.f) * 255);
			Data[idx].G = (int)(FMath::Clamp(color.G, 0.f, 1.f) * 255);
			Data[idx].R = (int)(FMath::Clamp(color.R, 0.f, 1.f) * 255);
			Data[idx].A = (int)(FMath::Clamp(color.A, 0.f, 1.f) * 255);
		}
	}
	Mip.BulkData.Unlock();
	Texture->UpdateResource();
}

void UVisualLayer::PaintCircle(int x, int y, int brush_size, FLinearColor color) {
	FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
	PixelData* Data = (PixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);

	float brush_radius = brush_size * 0.5f;
	int radius = FMath::FloorToInt(brush_radius);
	float brush_radius2 = radius * radius;
	for (int i = 0; i < brush_size; ++i) {
		int _y = y + i;
		if (_y < 0 || _y >= Mip.SizeY)
			continue;
		float _i = i - brush_radius + 0.5;
		_i *= _i;
		for (int j = 0; j < brush_size; ++j) {
			int _x = x + j;
			if (_x < 0 || _x >= Mip.SizeX)
				continue;
			float _j = j - brush_radius + 0.5;
			_j *= _j;

			if ((_i + _j) >= (brush_radius2 + 0.01f))
				continue;
			int idx = _y * Mip.SizeX + _x;
			Data[idx].B = (int)(FMath::Clamp(color.B, 0.f, 1.f) * 255);
			Data[idx].G = (int)(FMath::Clamp(color.G, 0.f, 1.f) * 255);
			Data[idx].R = (int)(FMath::Clamp(color.R, 0.f, 1.f) * 255);
			Data[idx].A = (int)(FMath::Clamp(color.A, 0.f, 1.f) * 255);
		}
	}
	Mip.BulkData.Unlock();
	Texture->UpdateResource();
}

