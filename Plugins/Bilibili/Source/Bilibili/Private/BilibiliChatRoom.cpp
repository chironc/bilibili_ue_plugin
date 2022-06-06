// Fill out your copyright notice in the Description page of Project Settings.


#include "BilibiliChatRoom.h"
#include "BilibiliChatWS.h"

void UBilibiliChatRoom::ExitChatRoom()
{
	ChatWS.Reset();
}

void UBilibiliChatRoom::EnterChatRoom(int RoomID)
{
	AddToRoot();
	FWorldDelegates::OnWorldCleanup.AddUObject(this, &UBilibiliChatRoom::BeginCleanUp);
	ChatWS = MakeShareable(new FBilibiliChatWS());
	ChatWS->OnMsgCallback.BindUObject(this, &UBilibiliChatRoom::OnMsg);
	ChatWS->Connect(GetWorld(), RoomID);
}

UBilibiliChatRoom::UBilibiliChatRoom()
{
}

UBilibiliChatRoom::~UBilibiliChatRoom()
{
}

void UBilibiliChatRoom::SetMessageCallback(const FString Msg, const FOnBilibiliMessage& OnMsg)
{
	CallbackMap.FindOrAdd(Msg).Add(OnMsg);
}

void UBilibiliChatRoom::BeginCleanUp(UWorld*, bool, bool)
{
	RemoveFromRoot();
	ExitChatRoom();
}


void UBilibiliChatRoom::OnMsg(const FString& Msg, TSharedPtr<FJsonObject> Object)
{
	//UE_LOG(LogTemp, Warning, TEXT("Msg:%s"), *Msg);
	auto Obj = UBlueprintJsonObject::FromSharedPointer(Object);
	if (CallbackMap.Contains(Msg)) {
		auto& cb = CallbackMap.FindChecked(Msg);
		cb.Broadcast(Obj);
	}
	//
	// static TArray<const TCHAR*> MsgTypeList = { TEXT("WATCHED_CHANGE"), TEXT("SEND_GIFT"), TEXT("COMBO_SEND"),
	// TEXT("ROOM_REAL_TIME_MESSAGE_UPDATE"), TEXT("DANMU_MSG"), TEXT("NOTICE_MSG"), TEXT("STOP_LIVE_ROOM_LIST"),
	// TEXT("INTERACT_WORD"), TEXT("HOT_RANK_CHANGED_V2"), TEXT("ONLINE_RANK_V2"), TEXT("ONLINE_RANK_COUNT"),
	// TEXT("ENTRY_EFFECT"), TEXT("SUPER_CHAT_MESSAGE"), TEXT("GUARD_BUY"), TEXT("USER_TOAST_MSG"),
	// TEXT("ONLINE_RANK_TOP3"), TEXT("HOT_RANK_CHANGED"), TEXT("HOT_RANK_SETTLEMENT_V2"),
	// TEXT("LIVE_MULTI_VIEW_CHANGE"),TEXT("PREPARING"),TEXT("HOT_ROOM_NOTIFY"),TEXT("LIVE_INTERACTIVE_GAME")};

	switch (GetMsgTypeByString(Msg))
	{
	case MsgType::WATCHED_CHANGE:				WATCHED_CHANGE.Broadcast(Obj);break;
	case MsgType::SEND_GIFT:					SEND_GIFT.Broadcast(Obj);break;
	case MsgType::DANMU_MSG:					DANMU_MSG.Broadcast(Obj);break;
	case MsgType::INTERACT_WORD:				INTERACT_WORD.Broadcast(Obj);break;
	case MsgType::COMBO_SEND:					COMBO_SEND.Broadcast(Obj);break;
	case MsgType::ENTRY_EFFECT:					ENTRY_EFFECT.Broadcast(Obj);break;
	case MsgType::SUPER_CHAT_MESSAGE:			SUPER_CHAT_MESSAGE.Broadcast(Obj);break;
	case MsgType::NOTICE_MSG:					NOTICE_MSG.Broadcast(Obj);break;
	case MsgType::HOT_RANK_CHANGED:				HOT_RANK_CHANGED.Broadcast(Obj);break;
	case MsgType::HOT_RANK_CHANGED_V2:			HOT_RANK_CHANGED_V2.Broadcast(Obj);break;
	case MsgType::HOT_RANK_SETTLEMENT_V2:		HOT_RANK_SETTLEMENT_V2.Broadcast(Obj);break;
	case MsgType::HOT_ROOM_NOTIFY:				HOT_ROOM_NOTIFY.Broadcast(Obj);break;
	case MsgType::ONLINE_RANK_COUNT:			ONLINE_RANK_COUNT.Broadcast(Obj);break;
	case MsgType::ONLINE_RANK_V2:				ONLINE_RANK_V2.Broadcast(Obj);break;
	case MsgType::ONLINE_RANK_TOP3:				ONLINE_RANK_TOP3.Broadcast(Obj);break;
	case MsgType::GUARD_BUY:					GUARD_BUY.Broadcast(Obj);break;
	case MsgType::USER_TOAST_MSG:				USER_TOAST_MSG.Broadcast(Obj);break;
	case MsgType::LIVE_MULTI_VIEW_CHANGE:		LIVE_MULTI_VIEW_CHANGE.Broadcast(Obj);break;
	case MsgType::PREPARING:					PREPARING.Broadcast(Obj);break;
	case MsgType::LIVE_INTERACTIVE_GAME:		LIVE_INTERACTIVE_GAME.Broadcast(Obj);break;
	case MsgType::ROOM_REAL_TIME_MESSAGE_UPDATE:ROOM_REAL_TIME_MESSAGE_UPDATE.Broadcast(Obj);break;
	case MsgType::STOP_LIVE_ROOM_LIST:			STOP_LIVE_ROOM_LIST.Broadcast(Obj);break;
	default:break;
	}
}
TMap<FString, MsgType> UBilibiliChatRoom::sMsgTypeMap;
bool UBilibiliChatRoom::sInitedMsgConvertMap = false;
MsgType UBilibiliChatRoom::GetMsgTypeByString(const FString& Msg)
{
	if (!sInitedMsgConvertMap)
	{
		sInitedMsgConvertMap = true;
		for (int i = 0; i < (int) MsgType::Total; i++)
			sMsgTypeMap.Add(StaticEnum<MsgType>()->GetNameStringByValue(i), (MsgType)i);
	}
	return sMsgTypeMap.FindRef(Msg);
}

UBilibiliChatRoom* UBilibiliChatRoom::EnterBilibiliChatRoom(UObject* WorldContextObject, int RoomID,  UBilibiliChatRoom*& ChatRoom)
{
	auto chat = NewObject<UBilibiliChatRoom>(WorldContextObject);
	chat->EnterChatRoom(RoomID);
	ChatRoom = chat;
	return chat;
}
