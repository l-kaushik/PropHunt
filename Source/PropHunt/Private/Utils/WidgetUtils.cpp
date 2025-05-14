// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/WidgetUtils.h"
#include "Structs/ImageData.h"

#include "Components/Button.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

void WidgetUtils::SetImageToButton(UButton* InButton, UTexture2D* InImage)
{
	if (!InButton || !InImage) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(InImage);
	Brush.ImageSize = FVector2D(157, 165);
	Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
	Brush.OutlineSettings.CornerRadii = FVector4(10, 10, 10, 10);
	Brush.OutlineSettings.Color = FLinearColor::Black;
	Brush.OutlineSettings.Width = 2.f;
	Brush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;

	FButtonStyle NewStyle = InButton->GetStyle();;
	NewStyle.Normal = Brush;
	NewStyle.Pressed = Brush;

	Brush.OutlineSettings.Width = 4.f;

	NewStyle.Hovered = Brush;

	InButton->SetStyle(NewStyle);
}

bool WidgetUtils::ExtractRawDataFromTexture(UTexture2D* Texture, FImageData& OutImageData)
{
	if (!Texture || !Texture->GetPlatformData() || Texture->GetPlatformData()->Mips.Num() == 0)
	{
		return false;
	}

	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
	OutImageData.Width = Mip.SizeX;
	OutImageData.Height = Mip.SizeY;

	const int32 PixelCount = OutImageData.Width * OutImageData.Height;
	const int32 ByteCount = PixelCount * 4; // RGBA = 4 bytes per pixel

	void* Data = Mip.BulkData.Lock(LOCK_READ_ONLY);

	if (!Data)
	{
		return false;
	}

	OutImageData.RawBytes.SetNumUninitialized(ByteCount);
	FMemory::Memcpy(OutImageData.RawBytes.GetData(), Data, ByteCount);

	Mip.BulkData.Unlock();

	return true;
}


UTexture2D* WidgetUtils::CreateTextureFromRawData(const FImageData& ImageData)
{
	if (ImageData.RawBytes.Num() == 0 || ImageData.Width <= 0 || ImageData.Height <= 0)
	{
		return nullptr;
	}

	UTexture2D* Texture = UTexture2D::CreateTransient(ImageData.Width, ImageData.Height, PF_R8G8B8A8);
	if (!Texture) return nullptr;

	// Ensure the texture has platform data
	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);

	FMemory::Memcpy(Data, ImageData.RawBytes.GetData(), ImageData.RawBytes.Num());
	Mip.BulkData.Unlock();

	Texture->GetPlatformData()->SetNumSlices(1);
	Texture->NeverStream = true;
	Texture->UpdateResource();

	return Texture;
}

UTexture2D* WidgetUtils::OpenFileDialogueAndLoadImage()
{
	//Here we initialize a pointer to the Desktop Platform Module, so we can open the file dialog. 
	//Dont forget to include "DesktopPlatform" in the Build.cs
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{

		TArray<FString> OutFilenames; //This stores the file path
		bool bOpened = DesktopPlatform->OpenFileDialog // File Dialogue so we can choose a file
		(
			nullptr,
			TEXT("Choose Image"),
			FPaths::ProjectContentDir(),
			TEXT(""),
			TEXT("Image Files (*.png; *.jpg| *.png;*.jpg)"),
			EFileDialogFlags::None,
			OutFilenames
		);

		if (bOpened && OutFilenames.Num() > 0) //if we managed to open the dialogue and we chose at least one file
		{

			FString SelectedImagePath = OutFilenames[0];

			TArray <uint8> ImageData; // this will be our image

			if (FFileHelper::LoadFileToArray(ImageData, *SelectedImagePath)) //here we load the data
			{
				IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));//Get a reference to the wrapper module 
				EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(ImageData.GetData(), ImageData.Num()); // Here i wanted to get the Image format, JPG, PNG and so on so that we could create an appropriate wrapper for it 
				TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat); // here we create an image wrapper for the given format 

				if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
				{

					TArray64<uint8> RawData;
					if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, RawData))// we want to take the raw data from the wrapper and put it in the raw data variable
					{
						TArray64<uint8>* RawDataProcessed = &RawData;
						UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);// we want to make a transient Texture object we can modify in code
						// we also get the widht and height from the wrapper and set the pixel format
						if (Texture)
						{
							FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];// we get platform specific data for the texture and select the firt mipmap
							void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE); //bulk data contains well... the bulk data of the mipmap and we want to write to it, we lock it so nothing accesses it now
							FMemory::Memcpy(Data, RawDataProcessed->GetData(), RawDataProcessed->Num()); // this copies memory data from one place to another, so we copy the image data 
							Mip.BulkData.Unlock(); // we unlock it so it can be accessed again

							Texture->GetPlatformData()->SetNumSlices(1); //we dont really need more but you can set it to more
							Texture->NeverStream = true; //if you want it streamed

							Texture->UpdateResource(); //update 
							return Texture;
						}
					}


				}
			}
		}
	}
	return nullptr;
}


