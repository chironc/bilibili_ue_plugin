// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BilibiliBlueprintLibrary.generated.h"

class UBilibiliChatRoom;
/**
 * 
 */
UCLASS(Blueprintable)
class BILIBILI_API UBilibiliBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UBilibiliChatRoom* EnterChatRoom(UObject* WorldContextObject, int nRoomID);

};
