// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MKScreenCaptureBPLibrary.h"
#include "MKScreenCapture.h"
#include "IPluginManager.h"

#include "IImageWrapperModule.h"
#include "IImageWrapper.h"

UMKScreenCaptureBPLibrary::UMKScreenCaptureBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

/**
* This fills any pixels of a texture with have an alpha value of zero,
* with an RGB from the nearest neighboring pixel which has non-zero alpha.
*/
template<typename PixelDataType, typename ColorDataType, int32 RIdx, int32 GIdx, int32 BIdx, int32 AIdx> class PNGDataFill
{
public:

	PNGDataFill(const FTextureSource& TextureSource, uint8* SourceTextureData)
		: SourceData(reinterpret_cast<PixelDataType*>(SourceTextureData))
		, TextureWidth(TextureSource.GetSizeX())
		, TextureHeight(TextureSource.GetSizeY())
	{
	}

	void ProcessData()
	{
		ClearZeroAlphaData();
		HorizontalPass(1);
		HorizontalPass(-1);
		VerticalPass(1);
		VerticalPass(-1);
	}

	void ClearZeroAlphaData()
	{
		for (int32 Y = 0; Y < TextureHeight; ++Y)
		{
			for (int32 X = 0; X < TextureWidth; ++X)
			{
				PixelDataType* PixelData = SourceData + (Y * TextureWidth + X) * 4;

				if (PixelData[AIdx] == 0)
				{
					ColorDataType* ColorData = reinterpret_cast<ColorDataType*>(PixelData);
					*ColorData = 0;
				}
			}
		}
	}

	void HorizontalPass(int32 XStep)
	{
		const int32 XStart = XStep > 0 ? 0 : TextureWidth - 1;
		const int32 XEnd = XStep > 0 ? TextureWidth : -1;

		for (int32 Y = 0; Y < TextureHeight; ++Y)
		{
			uint8 FillRed = 0, FillGreen = 0, FillBlue = 0;
			bool bHaveFillColor = false;

			for (int32 X = XStart; X != XEnd; X += XStep)
			{
				PixelDataType* PixelData = SourceData + (Y * TextureWidth + X) * 4;
				const ColorDataType* ColorData = reinterpret_cast<const ColorDataType*>(PixelData);

				if (*ColorData == 0)
				{
					if (bHaveFillColor)
					{
						PixelData[RIdx] = FillRed;
						PixelData[GIdx] = FillGreen;
						PixelData[BIdx] = FillBlue;
					}
				}
				else
				{
					bHaveFillColor = true;
					FillRed = PixelData[RIdx];
					FillGreen = PixelData[GIdx];
					FillBlue = PixelData[BIdx];
				}
			}
		}
	}

	void VerticalPass(int32 YStep)
	{
		const int32 YStart = YStep > 0 ? 0 : TextureHeight - 1;
		const int32 YEnd = YStep > 0 ? TextureHeight : -1;

		for (int32 X = 0; X < TextureWidth; ++X)
		{
			uint8 FillRed = 0, FillGreen = 0, FillBlue = 0;
			bool bHaveFillColor = false;

			for (int32 Y = YStart; Y != YEnd; Y += YStep)
			{
				PixelDataType* PixelData = SourceData + (Y * TextureWidth + X) * 4;
				const ColorDataType* ColorData = reinterpret_cast<const ColorDataType*>(PixelData);

				if (*ColorData == 0)
				{
					if (bHaveFillColor)
					{
						PixelData[RIdx] = FillRed;
						PixelData[GIdx] = FillGreen;
						PixelData[BIdx] = FillBlue;
					}
				}
				else
				{
					bHaveFillColor = true;
					FillRed = PixelData[RIdx];
					FillGreen = PixelData[GIdx];
					FillBlue = PixelData[BIdx];
				}
			}
		}
	}

	PixelDataType* SourceData;
	int32 TextureWidth;
	int32 TextureHeight;
};

static bool IsImportResolutionValid(int32 Width, int32 Height, bool bAllowNonPowerOfTwo/*, FFeedbackContext* Warn*/)
{
	// Calculate the maximum supported resolution utilizing the global max texture mip count
	// (Note, have to subtract 1 because 1x1 is a valid mip-size; this means a GMaxTextureMipCount of 4 means a max resolution of 8x8, not 2^4 = 16x16)
	const int32 MaximumSupportedResolution = 1 << (GMaxTextureMipCount - 1);

	bool bValid = true;

	// Check if the texture is above the supported resolution and prompt the user if they wish to continue if it is
	if (Width > MaximumSupportedResolution || Height > MaximumSupportedResolution)
	{
		if (EAppReturnType::Yes != FMessageDialog::Open(EAppMsgType::YesNo, FText::Format(
			NSLOCTEXT("UnrealEd", "Warning_LargeTextureImport", "Attempting to import {0} x {1} texture, proceed?\nLargest supported texture size: {2} x {3}"),
			FText::AsNumber(Width), FText::AsNumber(Height), FText::AsNumber(MaximumSupportedResolution), FText::AsNumber(MaximumSupportedResolution))))
		{
			bValid = false;
		}
	}

	const bool bIsPowerOfTwo = FMath::IsPowerOfTwo(Width) && FMath::IsPowerOfTwo(Height);
	// Check if the texture dimensions are powers of two
	if (!bAllowNonPowerOfTwo && !bIsPowerOfTwo)
	{
		//Warn->Logf(ELogVerbosity::Error, *NSLOCTEXT("UnrealEd", "Warning_TextureNotAPowerOfTwo", "Cannot import texture with non-power of two dimensions").ToString());
		bValid = false;
	}

	return bValid;
}

UTexture2D* UMKScreenCaptureBPLibrary::RegionScreenCapture() {
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("MKScreenCapture")->GetBaseDir();
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString SaveFolder = FPaths::ScreenShotDir() / "ScreenCapture";
	if (!PlatformFile.DirectoryExists(*SaveFolder)) {
		PlatformFile.CreateDirectoryTree(*SaveFolder);
	}
	FString SavePath = SaveFolder / "Region";
	// Add on the relative location of the third party dll and load it
	FString ScreenCapturePath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/MKScreenCaptureLibrary/Win64/ScreenCapture.exe"));
	SavePath = FPaths::GetBaseFilename(SavePath, false);
	FString Filename = SavePath;
	FFileHelper::GenerateNextBitmapFilename(SavePath, TEXT("png"), Filename);
	FString Params = FString::Printf(TEXT("-region \"%s\""), *Filename);

	FProcHandle Proc = FPlatformProcess::CreateProc(*ScreenCapturePath, *Params, true, false, false, NULL, -1, NULL, NULL);
	bool bCaptureWasSuccessful = true;

	// Failed to start the compressor process?
	if (!Proc.IsValid())
	{
		//UE_LOG(LogTextureFormatPVR, Error, TEXT("Failed to start PVR compressor tool. (Path:%s)"), *CompressorPath);
		bCaptureWasSuccessful = false;
	}

	if (bCaptureWasSuccessful)
	{
		// Wait for the process to complete
		int ReturnCode;
		while (!FPlatformProcess::GetProcReturnCode(Proc, &ReturnCode))
		{
			FPlatformProcess::Sleep(0.01f);
		}
		FPlatformProcess::CloseProc(Proc);

		// Did it fail?
		if (ReturnCode == 0)
		{
			bCaptureWasSuccessful = false;
		}
	}

	if (bCaptureWasSuccessful) {
		return CreateTexture2DFromPNG(Filename);
	}

	return NULL;
}

UTexture2D* UMKScreenCaptureBPLibrary::CreateTexture2DFromPNG(FString InPath) {

	TArray<uint8> Buffer;
	if (FFileHelper::LoadFileToArray(Buffer, *InPath)) {

		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

		IImageWrapperPtr PngImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		if (PngImageWrapper.IsValid() && PngImageWrapper->SetCompressed(Buffer.GetData(), Buffer.Num()))
		{
			bool bAllowNonPowerOfTwo = false;
			GConfig->GetBool(TEXT("TextureImporter"), TEXT("AllowNonPowerOfTwoTextures"), bAllowNonPowerOfTwo, GEditorIni);

			if (!IsImportResolutionValid(PngImageWrapper->GetWidth(), PngImageWrapper->GetHeight(), bAllowNonPowerOfTwo))
			{
				//return nullptr;
			}

			// Select the texture's source format
			ETextureSourceFormat TextureFormat = TSF_Invalid;
			int32 BitDepth = PngImageWrapper->GetBitDepth();
			ERGBFormat::Type Format = PngImageWrapper->GetFormat();
			if (Format == ERGBFormat::Gray)
			{
				if (BitDepth <= 8)
				{
					TextureFormat = TSF_G8;
					Format = ERGBFormat::Gray;
					BitDepth = 8;
				}
				else if (BitDepth == 16)
				{
					// TODO: TSF_G16?
					TextureFormat = TSF_RGBA16;
					Format = ERGBFormat::RGBA;
					BitDepth = 16;
				}
			}
			else if (Format == ERGBFormat::RGBA || Format == ERGBFormat::BGRA)
			{
				if (BitDepth <= 8)
				{
					TextureFormat = TSF_BGRA8;
					Format = ERGBFormat::BGRA;
					BitDepth = 8;
				}
				else if (BitDepth == 16)
				{
					TextureFormat = TSF_RGBA16;
					Format = ERGBFormat::RGBA;
					BitDepth = 16;
				}
			}

			if (TextureFormat == TSF_Invalid)
			{
				//Warn->Logf(ELogVerbosity::Error, TEXT("PNG file contains data in an unsupported format."));
				return nullptr;
			}

			UTexture2D* Texture = UTexture2D::CreateTransient(PngImageWrapper->GetWidth(), PngImageWrapper->GetHeight());
			if (Texture)
			{
				Texture->AddressX = TextureAddress::TA_Clamp;
				Texture->AddressY = TextureAddress::TA_Clamp;
				Texture->Filter = TextureFilter::TF_Nearest;

				Texture->SRGB = BitDepth < 16;
				const TArray<uint8>* RawPNG = nullptr;
				if (PngImageWrapper->GetRaw(Format, BitDepth, RawPNG))
				{
					FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
					uint8* Data = (uint8*)Mip.BulkData.Lock(LOCK_READ_WRITE);
					FMemory::Memcpy(Data, RawPNG->GetData(), RawPNG->Num());
					Mip.BulkData.Unlock();
					Texture->UpdateResource();
				}
			}

			return Texture;
		}
	}

	return NULL;
}
