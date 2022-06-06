// Fill out your copyright notice in the Description page of Project Settings.


#include "BilibiliBlueprintLibrary.h"

#include "Bilibili.h"
#include "BilibiliChatWS.h"
#include "BilibiliChatRoom.h"


UBilibiliChatRoom* UBilibiliBlueprintLibrary::EnterChatRoom(UObject* WorldContextObject, int RoomID)
{
	auto ChatRoom = NewObject<UBilibiliChatRoom>(WorldContextObject);
	ChatRoom->EnterChatRoom(RoomID);
	return ChatRoom;
}
