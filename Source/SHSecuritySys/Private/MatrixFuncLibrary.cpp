// Fill out your copyright notice in the Description page of Project Settings.

#include "MatrixFuncLibrary.h"

FMatrix UMatrixFuncLibrary::ProjectionMatrix(float InWidth, float InHeight, float InFOV, float InMinZ) {
	return FPerspectiveMatrix(InFOV / 2, InWidth, InHeight, InMinZ);
}

FMatrix UMatrixFuncLibrary::ViewMatrix(const FVector& InViewLocation, const FRotator& InViewRotation) {
	FMatrix lViewPlanesMatrix = FMatrix(
		FPlane(0, 0, 1, 0),
		FPlane(1, 0, 0, 0),
		FPlane(0, 1, 0, 0),
		FPlane(0, 0, 0, 1));

	FMatrix lViewMatrix = FTranslationMatrix(-InViewLocation);
	lViewMatrix = lViewMatrix * FInverseRotationMatrix(InViewRotation);
	lViewMatrix = lViewMatrix * lViewPlanesMatrix;

	return lViewMatrix;
}

FMatrix UMatrixFuncLibrary::Multiply(const FMatrix& InLeft, const FMatrix& InRight) {
	return InLeft * InRight;
}

FMatrix UMatrixFuncLibrary::Transpose(const FMatrix& InMatrix, bool InTranspose) {
	if (InTranspose)
		return InMatrix.GetTransposed();
	return InMatrix;
}
