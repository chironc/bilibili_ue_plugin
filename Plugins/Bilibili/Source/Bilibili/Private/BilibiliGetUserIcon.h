// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	int UserID = -1;
	const FString GetUserInfoURL = "https://api.bilibili.com/x/space/acc/info?mid={UserID}";
	void OnGetUserInfoResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Succ);
	void OnDownloadIconResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Succ);

	UTexture2D* LoadTexture2D_FromMemory(const TArray<uint8>& ImageData) const;
};
