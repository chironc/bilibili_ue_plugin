// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "BilibiliGetUserIcon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBilibiliGetUserIconCB, int32, UserID, UTexture2D*, Texture);

/**
 * 
 */
UCLASS()
class UBilibiliGetUserIcon : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", Category = "Bilibili",  WorldContext = "WorldContextObject"))
	static UBilibiliGetUserIcon* BilibiliGetUserIcon(UObject* WorldContextObject, const int32 UserID);

	UPROPERTY(BlueprintAssignable)
	FOnBilibiliGetUserIconCB OnFailed;

	UPROPERTY(BlueprintAssignable)
	FOnBilibiliGetUserIconCB OnCompleted;

	/** Execute the actual operation */
	virtual void Activate() override;
	virtual void RegisterWithGameInstance(UObject* WorldContextObject) override;

private:
	int UserID = -1;
	int RetryTimes = 0;
	const int MaxRetryGetInfoTimes = 1;
	const int MaxRetryDownloadTimes = 3;
	const float RetryDelayTime = 5;
	TWeakObjectPtr<UWorld> CurrentWorld = nullptr;
	FString FaceURL;
	//const FString GetUserInfoURL = "https://api.bilibili.com/x/space/acc/info?mid={UserID}&jsonp=jsonp";


	const FString GetUserInfoURL = "https://api.live.bilibili.com/live_user/v1/Master/info?uid={UserID}";
	const FString FaceURLJsonPath = "/data/info/face";
	
	const FString FaceConvertURL = "{FaceURL}@{Width}w_{Height}h_{Quality}q.{Format}";
	const int MaxImageWidth = 72;
	const int MaxImageHeight = 72;
	const int ImageQuality = 50;
	const FString ImageFormat = "jpg";
	const FString UserAgent = "LiveRoom Unreal Engine Plugin <chiron.chan@gmail.com>";

	//*.hdslb.com/bfs/*/*.[jpg/png/gif]@{idth}w_{high}h_{quality}q.jpg
	//http://i0.hdslb.com/bfs/face/member/noface.jpg
	
	void OnGetUserInfoResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Succ);
	void OnDownloadIconResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Succ);

	UTexture2D* LoadTexture2D_FromMemory(const TArray<uint8>& ImageData) const;


	bool TryRequestGetUserInfo(bool Retry);
	bool TryDownloadIcon(bool Retry);

	void RequestGetUserInfo();
	void DownloadIcon();
};
