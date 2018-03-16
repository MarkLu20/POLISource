// Fill out your copyright notice in the Description page of Project Settings.

#include "GridLayer.h"
#include "GridManager.h"



void UGridLayer::Initialize() {
	UGridManager* mgr = (UGridManager*)GetOuter();
	Bits.Init(false, mgr->Width * mgr->Height);
	ValidRect_Dirty = false;
	ValidRect_MinX = mgr->Width;
	ValidRect_MaxX = 0;
	ValidRect_MinY = mgr->Height;
	ValidRect_MaxY = 0;

	ValidNode_Dirty = false;
}

void UGridLayer::GetSize(int& width, int& height) {
	UGridManager* mgr = (UGridManager*)GetOuter();
	width = mgr->Width;
	height = mgr->Height;
}

bool UGridLayer::GetValidRect(int& min_x, int& min_y, int& max_x, int& max_y) {
	if (ValidRect_Dirty) {
		UGridManager* mgr = (UGridManager*)GetOuter();
		ValidRect_MinX = mgr->Width;
		ValidRect_MaxX = 0;
		ValidRect_MinY = mgr->Height;
		ValidRect_MaxY = 0;

		for (int i = 0; i < mgr->Height; ++i) {
			int _idx = i * mgr->Width;
			int _ValidRect_MinY = FMath::Min(ValidRect_MinY, i);
			int _ValidRect_MaxY = FMath::Max(ValidRect_MaxY, i);
			for (int j = 0; j < mgr->Width; ++j) {
				if (Bits[_idx + j]) {
					ValidRect_MinX = FMath::Min(ValidRect_MinX, j);
					ValidRect_MinY = _ValidRect_MinY;
					ValidRect_MaxX = FMath::Max(ValidRect_MaxX, j);
					ValidRect_MaxY = _ValidRect_MaxY;
				}
			}
		}
		ValidRect_Dirty = false;
	}

	min_x = ValidRect_MinX;
	min_y = ValidRect_MinY;
	max_x = ValidRect_MaxX;
	max_y = ValidRect_MaxY;

	return (ValidRect_MinX <= ValidRect_MaxX) || (ValidRect_MinY <= ValidRect_MaxY);
}

void UGridLayer::PaintRect(int x, int y, int brush_size, bool value) {
	if (brush_size <= 0)
		return;

	UGridManager* mgr = (UGridManager*)GetOuter();

	int min_x = x + brush_size;
	int min_y = y + brush_size;
	int max_x = x;
	int max_y = y;

	for (int i = 0; i < brush_size; ++i) {
		int _y = y + i;
		if (_y < 0 || _y >= mgr->Height)
			continue;
		int _min_y = FMath::Min(min_y, _y);
		int _max_y = FMath::Max(max_y, _y);
		for (int j = 0; j < brush_size; ++j) {
			int _x = x + j;
			if (_x < 0 || _x >= mgr->Width)
				continue;

			int idx = _y * mgr->Width + _x;

			if (Bits[idx] != value) {
				min_x = FMath::Min(min_x, _x);
				min_y = _min_y;
				max_x = FMath::Max(max_x, _x);
				max_y = _max_y;
				Bits[idx] = value;
				ValidNode_Dirty = true;
			}
		}
	}

	if (min_x <= max_x && min_y <= max_y) {
		ValidRect_Dirty = true;
		OnDirty.Broadcast(this, min_x, min_y, max_x, max_y);
	}
}

void UGridLayer::PaintCircle(int x, int y, int brush_size, bool value) {
	if (brush_size <= 0)
		return;

	UGridManager* mgr = (UGridManager*)GetOuter();
	int min_x = x + brush_size;
	int min_y = y + brush_size;
	int max_x = x;
	int max_y = y;

	float brush_radius = brush_size * 0.5f;
	int radius = FMath::FloorToInt(brush_radius);
	float brush_radius2 = radius * radius;
	for (int i = 0; i < brush_size; ++i) {
		int _y = y + i;
		if (_y < 0 || _y >= mgr->Height)
			continue;
		int _min_y = FMath::Min(min_y, _y);
		int _max_y = FMath::Max(max_y, _y);

		float _i = i - brush_radius + 0.5;
		_i *= _i;
		for (int j = 0; j < brush_size; ++j) {
			int _x = x + j;
			if (_x < 0 || _x >= mgr->Width)
				continue;
			float _j = j - brush_radius + 0.5;
			_j *= _j;

			if ((_i + _j) >= (brush_radius2 + 0.01f))
				continue;
			int idx = _y * mgr->Width + _x;

			if (Bits[idx] != value) {
				min_x = FMath::Min(min_x, _x);
				min_y = _min_y;
				max_x = FMath::Max(max_x, _x);
				max_y = _max_y;
				Bits[idx] = value;
				ValidNode_Dirty = true;
			}
		}
	}

	if (min_x <= max_x && min_y <= max_y) {
		ValidRect_Dirty = true;
		OnDirty.Broadcast(this, min_x, min_y, max_x, max_y);
	}
}

void UGridLayer::Save(FString path) {
	FArchive* Ar = IFileManager::Get().CreateFileWriter(*path, 0);
	if (!Ar)
		return;

	*Ar << ValidRect_MinX << ValidRect_MinY << ValidRect_MaxX << ValidRect_MaxY << ValidRect_Dirty << Bits;
	delete Ar;
}

void UGridLayer::Load(FString path) {
	FArchive* Ar = IFileManager::Get().CreateFileReader(*path, 0);
	if (!Ar)
		return;

	*Ar << ValidRect_MinX << ValidRect_MinY << ValidRect_MaxX << ValidRect_MaxY << ValidRect_Dirty << Bits;
	delete Ar;
	ValidNode_Dirty = true;
}

const TArray<int32>& UGridLayer::GetValidNodes() {

	if (ValidNode_Dirty) {
		ValidNodes.Empty();
		int min_x, max_x, min_y, max_y;
		if (GetValidRect(min_x, min_y, max_x, max_y)) {
			UGridManager* mgr = (UGridManager*)GetOuter();
			for (int y = min_y; y <= max_y; ++y) {
				int x_start = y * mgr->Width;
				for (int x = min_x; x <= max_x; ++x) {
					int idx = x_start + x;
					if (Bits[idx])
						ValidNodes.Add(idx);
				}
			}
		}
		ValidNode_Dirty = false;
	}

	return ValidNodes;
}

FIntPoint UGridLayer::IndexToPoint(int32 index) {
	UGridManager* mgr = (UGridManager*)GetOuter();
	FIntPoint pt;
	pt.X = index % mgr->Width;
	pt.Y = index / mgr->Width;
	return pt;
}
