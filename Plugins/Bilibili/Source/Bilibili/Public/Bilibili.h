// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

class FBilibiliChatWS;
class FBilibiliModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	//void*	ExampleLibraryHandle;
public:
	//FBilibiliChatWS* Chat = nullptr;
};
