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
	TryRequestGetUserInfo(false);
}

void UBilibiliGetUserIcon::RegisterWithGameInstance(UObject* WorldContextObject)
{
	if (WorldContextObject) {
		CurrentWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}
	Super::RegisterWithGameInstance(WorldContextObject);
}

bool UBilibiliGetUserIcon::TryRequestGetUserInfo(bool Retry)
{
	if (RetryTimes++ >= MaxRetryGetInfoTimes)
		return false;

	if (Retry)
	{
		if (!CurrentWorld.IsValid())
			return false;
		FTimerHandle Handle;
		CurrentWorld->GetTimerManager().SetTimer(Handle, this, &UBilibiliGetUserIcon::RequestGetUserInfo, RetryDelayTime);
		return true;
	}

	RequestGetUserInfo();
	return true;
}

bool UBilibiliGetUserIcon::TryDownloadIcon(bool Retry)
{
	if (RetryTimes++ >= MaxRetryDownloadTimes)
		return false;

	if (Retry) 
	{
		if (!CurrentWorld.IsValid())
			return false;
		FTimerHandle Handle;
		CurrentWorld->GetTimerManager().SetTimer(Handle, this, &UBilibiliGetUserIcon::DownloadIcon, RetryDelayTime);
		return true;
	}
	
	DownloadIcon();
	return true;
}

void UBilibiliGetUserIcon::RequestGetUserInfo()
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UBilibiliGetUserIcon::OnGetUserInfoResponse);

	TMap<FString, FStringFormatArg> FormatMap;
	FormatMap.Add(TEXT("UserID"), FStringFormatArg(UserID));

	Request->SetURL(FString::Format(*GetUserInfoURL, FormatMap));
	Request->SetVerb("GET");

	Request->SetHeader("User-Agent", UserAgent);
	Request->SetHeader("Accept", TEXT("text/html"));
	Request->ProcessRequest();
}

void UBilibiliGetUserIcon::DownloadIcon()
{
	TSharedRef<IHttpRequest> DownloadIconRequest = FHttpModule::Get().CreateRequest();
	DownloadIconRequest->OnProcessRequestComplete().BindUObject(this, &UBilibiliGetUserIcon::OnDownloadIconResponse);

	TMap<FString, FStringFormatArg> FormatMap;
	FormatMap.Add(TEXT("FaceURL"), FStringFormatArg(FaceURL));
	FormatMap.Add(TEXT("Width"), FStringFormatArg(MaxImageWidth));
	FormatMap.Add(TEXT("Height"), FStringFormatArg(MaxImageHeight));
	FormatMap.Add(TEXT("Quality"), FStringFormatArg(ImageQuality));
	FormatMap.Add(TEXT("Format"), FStringFormatArg("jpg"));

	DownloadIconRequest->SetURL(FString::Format(*FaceConvertURL, FormatMap));
	DownloadIconRequest->SetVerb("GET");
	DownloadIconRequest->SetHeader("User-Agent", UserAgent);
	DownloadIconRequest->SetHeader("Accept", "image/jpeg");
	DownloadIconRequest->ProcessRequest();
}

void UBilibiliGetUserIcon::OnGetUserInfoResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Succ)
{
	if (!Succ)
	{
		if (!TryRequestGetUserInfo(true))
		{
			UE_LOG(LogTemp, Warning, TEXT("GetUserInfo Http failed. URL:%s"), *Request->GetURL());
			OnFailed.Broadcast(UserID, nullptr);
			SetReadyToDestroy();
		}
		return;
	}
	UBlueprintJsonObject* JsonObject;
	if (!UBlueprintJsonObject::Parse(Response->GetContentAsString(), JsonObject))
	{
		if (!TryRequestGetUserInfo(true))
		{
			UE_LOG(LogTemp, Warning, TEXT("GetUserInfo Json Parse failed. url:%s, str:%s"), *Request->GetURL(), *Response->GetContentAsString());
			OnFailed.Broadcast(UserID, nullptr);
			SetReadyToDestroy();
		}
		return;
	}
	if (!JsonObject->GetStringByPath(FaceURLJsonPath, FaceURL))
	{
		if (!TryRequestGetUserInfo(true))
		{
			UE_LOG(LogTemp, Warning, TEXT("GetUserInfo Cann't find face info. url:%s, str:%s"), *Request->GetURL(), *Response->GetContentAsString());
			OnFailed.Broadcast(UserID, nullptr);
			SetReadyToDestroy();
		}
		return;
	}
	
	RetryTimes = 0;//重置
	TryDownloadIcon(false);
}

void UBilibiliGetUserIcon::OnDownloadIconResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Succ)
{
	if (!Succ)
	{
		if (!TryDownloadIcon(true))
		{
			UE_LOG(LogTemp, Warning, TEXT("Download Icon failed. URL：%s"), *Request->GetURL());
			OnFailed.Broadcast(UserID, nullptr);
			SetReadyToDestroy();
		}
		return;
	}
	UTexture2D* Texture = LoadTexture2D_FromMemory(Response->GetContent());
	if (!Texture)
	{
		if (!TryDownloadIcon(true))
		{
			UE_LOG(LogTemp, Warning, TEXT("Load Icon failed. URL：%s"), *Request->GetURL());
			OnFailed.Broadcast(UserID, nullptr);
			SetReadyToDestroy();
		}
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
