// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TimerManager.h"

enum class BILIBILI_WS_OP {
	WS_OP_HEARTBEAT = 2, //心跳
	WS_OP_HEARTBEAT_REPLY = 3, //心跳回应 
	WS_OP_MESSAGE = 5, //弹幕,消息等
	WS_OP_USER_AUTHENTICATION = 7,//用户进入房间
	WS_OP_CONNECT_SUCCESS = 8, //进房回应
};

DECLARE_DELEGATE_TwoParams(FOnMessage,const FString&, TSharedPtr<FJsonObject>);

class IWebSocket;
class UWorld;

class FBilibiliChatWS 
{
	/* Config Begin */
	const FString ServerURL = TEXT("ws://broadcastlv.chat.bilibili.com:2244/sub"); // Your server URL. You can use ws, wss or wss+insecure.
	const FString ServerProtocol = TEXT("ws");
	const float HeartBeatInterval = 30;
	/* Config End */




	/* Buffer Helper Begin */
#pragma pack(push) //保存对齐状态
#pragma pack(4)//设定为4字节对齐
	struct BufferInfo {
		uint32_t size;
		uint8_t data[0];
	};
#pragma pack(pop)//恢复对齐状态
	class BufferGuard {
		uint8_t* _buffer;
		FBilibiliChatWS* _mgr;
	public:
		BufferGuard() = delete;
		BufferGuard(const BufferGuard&) = delete;
		BufferGuard(FBilibiliChatWS* mgr, uint8_t* buffer) : _buffer(buffer), _mgr(mgr) {};
		~BufferGuard() { _mgr->ReleaseBuffer(_buffer); _buffer = nullptr; };
	};
	static constexpr size_t DefaultBufferSize = 16 * 1024 - sizeof(BufferInfo); //16K,保留一个header存大小
	TArray<BufferInfo*> BufferStack;
	uint8_t* AllocBuffer(size_t size = DefaultBufferSize);
	void ReleaseBuffer(uint8_t* Buffer);
	void ReleaseBufferStack();
	/* Buffer Helper End */





	/* WebSocket Begin */
	TSharedPtr<IWebSocket> WebSocket;
	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnMessage(const FString& Message) {};
	void OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
	void OnMessageSent(const FString& MessageString) {};
	void Close();


	UWorld* ContextWorld = nullptr;
	bool bConnecting = false;
	/* WebSocket End */






	/* Json Helper Begin */
	bool TryDeserializingJson(uint8_t* Buffer, size_t Size, TSharedPtr<FJsonObject> JsonObject);
	FString PrettyJson(TSharedPtr<FJsonObject> JsonObject);
	/* Json Helper End */


	/* Byte Helper Begin */
	uint32_t ReadU32FromBigEndian(uint8_t* data);
	uint16_t ReadU16FromBigEndian(uint8_t* data);
	int32_t ReadI32FromBigEndian(uint8_t* data);
	int16_t ReadI16FromBigEndian(uint8_t* data);
	/* Byte Helper End */


	/*BiliBili 数据层*/
	struct Message {
		uint8_t* Data = nullptr;
		size_t Size = 0;
		FBilibiliChatWS* Mgr = nullptr;
		Message(FBilibiliChatWS* mgr, const uint8_t* data, size_t size);
		Message() {};
		Message(Message& other);
		Message(const Message& other) = delete;
		Message(Message&& other);
		~Message();
		Message& operator = (FBilibiliChatWS::Message&& other);
		bool operator < (const Message& other) const { return Data < other.Data; }
	};
	TArray<Message> PendingMessage;
	int Pack(BILIBILI_WS_OP Op, const FString& JsonContext, uint8_t* Buffer, size_t BufferSize);

	void Dipatch();

	/*BiliBili 业务层*/
	
	void EnterRoom();
	void OnEnterRoom();

	void HeartBeat();
	void OnHeartBeat(uint32_t nPop);

	void OnMessage(TSharedPtr<FJsonObject> JsonObject);

	int nRoomID = -1;
	FTimerHandle hHeartBeatTimer;

public:
	bool Connect(UWorld* world, int nRoomID);
	bool IsConnected();
	FBilibiliChatWS();
	~FBilibiliChatWS();

	FOnMessage OnMsgCallback;
	
};
