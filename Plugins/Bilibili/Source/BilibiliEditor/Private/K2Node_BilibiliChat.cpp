// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_BilibiliChat.h"

#include "BilibiliChatRoom.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Bilibili/Public/BlueprintJsonObject.h"

static FName ExitRoomPinName(TEXT("ExitRoom"));

UK2Node_BilibiliChat::UK2Node_BilibiliChat(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UBilibiliChatRoom, EnterBilibiliChatRoom);
	ProxyFactoryClass = UBilibiliChatRoom::StaticClass();
	ProxyClass = UBilibiliChatRoom::StaticClass();
}

void UK2Node_BilibiliChat::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

