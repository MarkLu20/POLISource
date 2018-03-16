// Fill out your copyright notice in the Description page of Project Settings.

#include "GridPathFinder.h"
#include "GridLayer.h"




bool FindValidPoint(UGridLayer* layer, FIntPoint& point) {
	if (layer == NULL)
		return false;

	FIntPoint min;
	FIntPoint max;
	if (!layer->GetValidRect(min.X, min.Y, max.X, max.Y))
		return false;

	return true;
}

TArray<FIntPoint> UGridPathFinder::FindPath(UGridLayer* layer, FIntPoint startPoint, float range) {
	bool bValidStartPoint = FindValidPoint(layer, startPoint);
	TArray<FIntPoint> result;
	result.Add(startPoint);
	if (!bValidStartPoint)
		return result;

	int min_x, min_y, max_x, max_y;
	layer->GetValidRect(min_x, min_y, max_x, max_y);
	int32 x = FMath::RandRange(min_x, max_x);
	int32 y = FMath::RandRange(min_y, max_y);
	result.Add(FIntPoint(x, y));
	return result;
}

bool UGridPathFinder::RandomTarget(class UGridLayer* layer, FIntPoint& target)
{
	if (layer == NULL)
		return false;

	const TArray<int32>& nodes = layer->GetValidNodes();
	if (nodes.Num() <= 0)
		return false;

	int32 index = FMath::Rand() % nodes.Num();
	target = layer->IndexToPoint(nodes[index]);
	return true;
}



