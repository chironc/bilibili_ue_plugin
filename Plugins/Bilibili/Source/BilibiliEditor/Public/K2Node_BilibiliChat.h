// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_BilibiliChat.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BILIBILIEDITOR_API UK2Node_BilibiliChat : public UK2Node_BaseAsyncTask
{
	GENERATED_UCLASS_BODY()

	// UEdGraphNode interface
	virtual FText GetTooltipText() const override {
		return FText::FromString(TEXT("B站直播间")); }
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override {
		return FText::FromString(TEXT("B站直播间")); }
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual FText GetMenuCategory() const override{
		return FText::FromString(TEXT("Bilibili")); }
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// End of UK2Node interface
};
