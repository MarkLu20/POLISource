// Fill out your copyright notice in the Description page of Project Settings.

#include "GetExtremeValueBPL.h"



void UGetExtremeValueBPL::GetArrayAverage(const TArray<float>& datalist, float &average)
{
	float min = 0;
	float max = 0;

	if (datalist.Num() > 0)
	{
		min = datalist[0];
		max = datalist[0];
		for (int i = 1; i < datalist.Num(); i++)
		{
			if (datalist[i] < min)
			{
				min = datalist[i];
			}

			if (datalist[i] > max)
			{
				max = datalist[i];
			}
		}
		average = (min + max) / 2;
	}
}


