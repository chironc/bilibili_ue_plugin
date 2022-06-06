// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BlueprintJsonObject.h"
#include "BilibiliChatRoom.generated.h"

class FBilibiliChatWS;


UENUM()
enum class MsgType
{
	WATCHED_CHANGE = 0,
	SEND_GIFT,
	DANMU_MSG,
	INTERACT_WORD,
	COMBO_SEND,
	ENTRY_EFFECT,
	SUPER_CHAT_MESSAGE,
	NOTICE_MSG,
	HOT_RANK_CHANGED,
	HOT_RANK_CHANGED_V2,
	HOT_RANK_SETTLEMENT_V2,
	HOT_ROOM_NOTIFY,
	ONLINE_RANK_COUNT,
	ONLINE_RANK_V2,
	ONLINE_RANK_TOP3,
	GUARD_BUY,
	USER_TOAST_MSG,
	LIVE_MULTI_VIEW_CHANGE,
	PREPARING,
	LIVE_INTERACTIVE_GAME,
	ROOM_REAL_TIME_MESSAGE_UPDATE,
	STOP_LIVE_ROOM_LIST,

	Total,
};

//DECLARE_DYNAMIC_DELEGATE_OneParam(FCommonActionCommited2, bool&, bPassThrough);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBilibiliMessage, const UBlueprintJsonObject*, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBilibiliEvent, const UBlueprintJsonObject*, Data);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class BILIBILI_API UBilibiliChatRoom : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ExitChatRoom();

	UFUNCTION(BlueprintCallable)
	void EnterChatRoom(int RoomID);

	UBilibiliChatRoom();
	~UBilibiliChatRoom();

	UFUNCTION(BlueprintCallable)
	void SetMessageCallback(const FString Msg, const FOnBilibiliMessage& OnMsg);

	void BeginCleanUp(UWorld* /*World*/, bool /*bSessionEnded*/, bool /*bCleanupResources*/);
private:
	TSharedPtr<FBilibiliChatWS> ChatWS;
	TMap<FString, FOnBilibiliEvent> CallbackMap;

	void OnMsg(const FString& Msg, TSharedPtr<FJsonObject> JsonObject);

	static TMap<FString, MsgType> sMsgTypeMap;
	static bool sInitedMsgConvertMap;
	static MsgType GetMsgTypeByString(const FString& Msg);
public:
	// static TArray<const TCHAR*> MsgTypeList = { TEXT("WATCHED_CHANGE"), TEXT("SEND_GIFT"), TEXT("COMBO_SEND"),
	// TEXT("ROOM_REAL_TIME_MESSAGE_UPDATE"), TEXT("DANMU_MSG"), TEXT("NOTICE_MSG"), TEXT("STOP_LIVE_ROOM_LIST"),
	// TEXT("INTERACT_WORD"), TEXT("HOT_RANK_CHANGED_V2"), TEXT("ONLINE_RANK_V2"), TEXT("ONLINE_RANK_COUNT"),
	// TEXT("ENTRY_EFFECT"), TEXT("SUPER_CHAT_MESSAGE"), TEXT("GUARD_BUY"), TEXT("USER_TOAST_MSG"),
	// TEXT("ONLINE_RANK_TOP3"), TEXT("HOT_RANK_CHANGED"), TEXT("HOT_RANK_SETTLEMENT_V2"),
	// TEXT("LIVE_MULTI_VIEW_CHANGE"),TEXT("PREPARING"),TEXT("HOT_ROOM_NOTIFY"),TEXT("LIVE_INTERACTIVE_GAME")};

	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent WATCHED_CHANGE;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent SEND_GIFT;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent DANMU_MSG;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent INTERACT_WORD;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent COMBO_SEND;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent ENTRY_EFFECT;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent SUPER_CHAT_MESSAGE;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent NOTICE_MSG;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent HOT_RANK_CHANGED;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent HOT_RANK_CHANGED_V2;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent HOT_RANK_SETTLEMENT_V2;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent HOT_ROOM_NOTIFY;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent ONLINE_RANK_COUNT;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent ONLINE_RANK_V2;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent ONLINE_RANK_TOP3;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent GUARD_BUY;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent USER_TOAST_MSG;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent LIVE_MULTI_VIEW_CHANGE;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent PREPARING;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent LIVE_INTERACTIVE_GAME;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent ROOM_REAL_TIME_MESSAGE_UPDATE;
	UPROPERTY(BlueprintAssignable)
	FOnBilibiliEvent STOP_LIVE_ROOM_LIST;
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static UBilibiliChatRoom* EnterBilibiliChatRoom(UObject* WorldContextObject, int RoomID, UBilibiliChatRoom*& ChatRoom);
};
