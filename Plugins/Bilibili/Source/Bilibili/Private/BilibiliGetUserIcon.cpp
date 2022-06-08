// Fill out your copyright notice in the Description page of Project Settings.


#include "BilibiliGetUserIcon.h"

#include "BlueprintJsonObject.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

UBilibiliGetUserIcon* UBilibiliGetUserIcon::BilibiliGetUserIcon(UObject* WorldContextObject, const int32 UserID)
{
	UBilibiliGetUserIcon* GetUserIcon = NewObject<UBilibiliGetUserIcon>();
	GetUserIcon->UserID = UserID;
	GetUserIcon->RegisterWithGameInstance(WorldContextObject);
	return GetUserIcon;
}

void UBilibiliGetUserIcon::Activate()
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UBilibiliGetUserIcon::OnGetUserInfoResponse);
	
	TMap<FString, FStringFormatArg> FormatMap;
	FormatMap.Add(TEXT("UserID"), FStringFormatArg(UserID));
	
	Request->SetURL(FString::Format(*GetUserInfoURL, FormatMap));
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
	//SetReadyToDestroy();
}

void UBilibiliGetUserIcon::OnGetUserInfoResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Succ)
{
	if (!Succ)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetUserInfo Http failed. URL:%s"), *Request->GetURL());
		OnFailed.Broadcast(UserID, nullptr);
		SetReadyToDestroy();
		return;
	}
	UBlueprintJsonObject* JsonObject;
	if (!UBlueprintJsonObject::Parse(Response->GetContentAsString(), JsonObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetUserInfo Json Parse failed. str:%s"), *Response->GetContentAsString());
		OnFailed.Broadcast(UserID, nullptr);
		SetReadyToDestroy();
		return;
	}
	FString FaceURL;
	if (!JsonObject->GetStringByPath(TEXT("/data/face"), FaceURL))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetUserInfo Cann't find face info. str:%s"), *Response->GetContentAsString());
		OnFailed.Broadcast(UserID, nullptr);
		SetReadyToDestroy();
		return;
	}
	TSharedRef<IHttpRequest> DownloadIconRequest = FHttpModule::Get().CreateRequest();
	DownloadIconRequest->OnProcessRequestComplete().BindUObject(this, &UBilibiliGetUserIcon::OnDownloadIconResponse);
	
	DownloadIconRequest->SetURL(FaceURL);
	DownloadIconRequest->SetVerb("GET");
	DownloadIconRequest->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	DownloadIconRequest->ProcessRequest();
}

void UBilibiliGetUserIcon::OnDownloadIconResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Succ)
{
	if (!Succ)
	{
		UE_LOG(LogTemp, Warning, TEXT("Download Icon failed. URL：%s"), *Request->GetURL());
		OnFailed.Broadcast(UserID, nullptr);
		SetReadyToDestroy();
		return;
	}
	UTexture2D* Texture = LoadTexture2D_FromMemory(Response->GetContent());
	if (!Texture)
	{
		UE_LOG(LogTemp, Warning, TEXT("Load Icon failed. URL：%s"), *Request->GetURL());
		OnFailed.Broadcast(UserID, nullptr);
		SetReadyToDestroy();
		return;
	}

	OnCompleted.Broadcast(UserID, Texture);
	SetReadyToDestroy();
}

UTexture2D* UBilibiliGetUserIcon::LoadTexture2D_FromMemory(const TArray<uint8>& ImageData) const
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	const auto Format = ImageWrapperModule.DetectImageFormat(ImageData.GetData(), ImageData.Num());
	if (Format == EImageFormat::Invalid)
		return nullptr;

	const TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(Format);
	if (!ImageWrapper.IsValid())
		return nullptr;

	if (!ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
		return nullptr;
	
	TArray<uint8> UncompressedBGRA;
	if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		return nullptr;

	const auto LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

	if(!LoadedT2D)
		return nullptr;
		
	void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
	LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

	LoadedT2D->UpdateResource();
	
	return LoadedT2D;
}
