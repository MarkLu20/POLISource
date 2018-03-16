// Fill out your copyright notice in the Description page of Project Settings.

#include "GridPalette.h"
#include "Engine/Texture2D.h"
#include "GridManager.h"


void UGridPalette::Initialize() {
	UGridManager* mgr = (UGridManager*)GetOuter();
	DirtyRect_MinX = 0;
	DirtyRect_MinY = 0;
	DirtyRect_MaxX = mgr->Width - 1;
	DirtyRect_MaxY = mgr->Height - 1;

	Texture = UTexture2D::CreateTransient(mgr->Width, mgr->Height);
	Texture->AddressX = TextureAddress::TA_Clamp;
	Texture->AddressY = TextureAddress::TA_Clamp;
	Texture->Filter = TextureFilter::TF_Nearest;
	FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
	FPixelData* Data = (FPixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);
	for (int y = DirtyRect_MinY; y <= DirtyRect_MaxY; ++y) {
		int start_idx = y * mgr->Width;
		for (int x = 0; x < mgr->Width; ++x) {
			int idx = start_idx + x;
			Data[idx].B = 0;
			Data[idx].G = 0;
			Data[idx].R = 0;
			Data[idx].A = 0;
		}
	}
	Mip.BulkData.Unlock();
	Texture->UpdateResource();
}

bool UGridPalette::AddLayer(UGridLayer* layer, FLinearColor color) {
	if (layer == NULL || layer->GetOuter() != GetOuter())
		return false;

	if (!Layers.Contains(layer)) {
		layer->OnDirty.AddUniqueDynamic(this, &UGridPalette::OnDirtyLayer);
	}
	else {
		if (color == Layers[layer].Color)
			return true;
	}

	FGridLayerInfo info;
	info.Color = color;
	//info.Color.R = (UINT8)(color.R * 255);
	//info.Color.G = (UINT8)(color.G * 255);
	//info.Color.B = (UINT8)(color.B * 255);
	//info.Color.A = (UINT8)(color.A * 255);

	Layers.Add(layer, info);

	int min_x = 0;
	int min_y = 0;
	int max_x = 0;
	int max_y = 0;
	layer->GetValidRect(min_x, min_y, max_x, max_y);
	OnDirtyLayer(layer, min_x, min_y, max_x, max_y);

	return true;
}

bool UGridPalette::RemoveLayer(UGridLayer* layer) {
	if (!Layers.Contains(layer))
		return false;

	layer->OnDirty.RemoveDynamic(this, &UGridPalette::OnDirtyLayer);
	Layers.Remove(layer);
	int min_x = 0;
	int min_y = 0;
	int max_x = 0;
	int max_y = 0;
	layer->GetValidRect(min_x, min_y, max_x, max_y);
	OnDirtyLayer(layer, min_x, min_y, max_x, max_y);

	return true;
}

void UGridPalette::RemoveAllLayers() {
	Layers.Empty();
	UGridManager* mgr = (UGridManager*)GetOuter();
	OnDirtyLayer(NULL, 0, 0, mgr->Width - 1, mgr->Height - 1);
}


void UGridPalette::OnDirtyLayer(UGridLayer* layer, int min_x, int min_y, int max_x, int max_y) {
	UGridManager* mgr = (UGridManager*)GetOuter();
	DirtyRect_MinX = FMath::Clamp(DirtyRect_MinX, 0, min_x);
	DirtyRect_MinY = FMath::Clamp(DirtyRect_MinY, 0, min_y);
	DirtyRect_MaxX = FMath::Clamp(DirtyRect_MaxX, max_x, mgr->Width - 1);
	DirtyRect_MaxY = FMath::Clamp(DirtyRect_MaxY, max_y, mgr->Height - 1);
}

void UGridPalette::UpdateTexture() {
	UGridManager* mgr = (UGridManager*)GetOuter();

	if (DirtyRect_MinX <= DirtyRect_MaxX && DirtyRect_MinY <= DirtyRect_MaxY) {
		FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
		FPixelData* Data = (FPixelData*)Mip.BulkData.Lock(LOCK_READ_WRITE);
		for (int y = DirtyRect_MinY; y <= DirtyRect_MaxY; ++y) {
			int start_idx = y * mgr->Width;
			for (int x = DirtyRect_MinX; x <= DirtyRect_MaxX; ++x) {
				int idx = start_idx + x;
				Data[idx].B = 0;
				Data[idx].G = 0;
				Data[idx].R = 0;
				Data[idx].A = 0;

				FLinearColor color(0, 0, 0, 0);
				for (TMap<UGridLayer*, FGridLayerInfo>::TIterator iter = Layers.CreateIterator(); iter; ++iter) {
					UGridLayer* layer = iter->Key;
					if (layer->Bits[idx]) {
						//color = FMath::Lerp(color, iter->Value.Color, iter->Value.Color.A);
						color += iter->Value.Color;
						//Data[idx].B = FMath::Max(iter->Value.Color.B, Data[idx].B);
						//Data[idx].G = FMath::Max(iter->Value.Color.G, Data[idx].G);
						//Data[idx].R = FMath::Max(iter->Value.Color.R, Data[idx].R);
						//Data[idx].A = FMath::Max(iter->Value.Color.A, Data[idx].A);
					}
				}

				Data[idx].R = FMath::Min((int)(color.R * 255), 255);
				Data[idx].G = FMath::Min((int)(color.G * 255), 255);
				Data[idx].B = FMath::Min((int)(color.B * 255), 255);
				Data[idx].A = FMath::Min((int)(color.A * 255), 255);
			}
		}
		Mip.BulkData.Unlock();
		Texture->UpdateResource();

		DirtyRect_MinX = mgr->Width;
		DirtyRect_MinY = mgr->Height;
		DirtyRect_MaxX = 0;
		DirtyRect_MaxY = 0;
	}
}