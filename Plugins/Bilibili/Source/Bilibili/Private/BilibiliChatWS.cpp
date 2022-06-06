// Fill out your copyright notice in the Description page of Project Settings.


#include "BilibiliChatWS.h"
#include "IWebSocket.h"       // Socket definition
#include "WebSocketsModule.h" // Module definition
#include "Modules/ModuleManager.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "miniz.h"
#include "JsonObjectWrapper.h"
#include <brotli/decode.h>
#include <cassert>


void swapByteOrder(short& us)
{
    us = (us >> 8) |
        (us << 8);
}

void swapByteOrder(int& ui)
{
    ui = (ui >> 24) |
        ((ui << 8) & 0x00FF0000) |
        ((ui >> 8) & 0x0000FF00) |
        (ui << 24);
}

void swapByteOrder(long long& ull)
{
    ull = (ull >> 56) |
        ((ull << 40) & 0x00FF000000000000) |
        ((ull << 24) & 0x0000FF0000000000) |
        ((ull << 8) & 0x000000FF00000000) |
        ((ull >> 8) & 0x00000000FF000000) |
        ((ull >> 24) & 0x0000000000FF0000) |
        ((ull >> 40) & 0x000000000000FF00) |
        (ull << 56);
}

void swapByteOrder(unsigned short& us)
{
    us = (us >> 8) |
        (us << 8);
}

void swapByteOrder(unsigned int& ui)
{
    ui = (ui >> 24) |
        ((ui << 8) & 0x00FF0000) |
        ((ui >> 8) & 0x0000FF00) |
        (ui << 24);
}

void swapByteOrder(unsigned long long& ull)
{
    ull = (ull >> 56) |
        ((ull << 40) & 0x00FF000000000000) |
        ((ull << 24) & 0x0000FF0000000000) |
        ((ull << 8) & 0x000000FF00000000) |
        ((ull >> 8) & 0x00000000FF000000) |
        ((ull >> 24) & 0x0000000000FF0000) |
        ((ull >> 40) & 0x000000000000FF00) |
        (ull << 56);
}

bool IsLittleEndian()
{
    int n = 0x12345678;
    static bool bLittle = *((char*)&n) == 0x78;
    return bLittle;
}
template<typename UINT_TYPE>
void MakeSureBigEndian(UINT_TYPE& us)
{
    if (IsLittleEndian())
        swapByteOrder(us);
}

enum class Protocol {
    JSON = 0,
    ZLIB = 2,
    BROTLI = 3,
};

struct Package {
    uint32_t    PackageLength;
    uint16_t    HeaderLength;
    uint16_t    ProtocolVersion;//0:JSON纯文本, 1: 房间人气值 , 2: zlib， 3:brotli
    uint32_t    Operation;      //BILIBILI_WS_OP
    uint32_t    SequenceID;     //数据包头部长度（固定为 1）
    uint8_t     Data[0];

    void SwapByteOrder() {
        swapByteOrder(this->PackageLength);
        swapByteOrder(this->HeaderLength);
        swapByteOrder(this->ProtocolVersion);
        swapByteOrder(this->Operation);
        swapByteOrder(this->SequenceID);
    }
};


inline uint8_t* FBilibiliChatWS::AllocBuffer(size_t size) {
    if (BufferStack.IsEmpty() || size > DefaultBufferSize) {
        size_t alloc_size = std::max(DefaultBufferSize, size);
        BufferInfo* buffer = (BufferInfo*)malloc(alloc_size + sizeof(BufferInfo));
        buffer->size = alloc_size;
        return buffer->data;
    }
    else {
        BufferInfo* buffer = BufferStack.Pop();
        memset(buffer->data, 0, buffer->size);
        return buffer->data;
    }
}

inline void FBilibiliChatWS::ReleaseBuffer(uint8_t* Buffer) {
    if (!Buffer)
        return;
    BufferInfo* buffer = (BufferInfo*)(Buffer - offsetof(BufferInfo, data));
    if (buffer->size != DefaultBufferSize) {
        free(buffer);
    }
    else {
        BufferStack.Push(buffer);
    }
}

inline void FBilibiliChatWS::ReleaseBufferStack() {
    for (auto buffer : BufferStack) {
        free(buffer);
    }
    BufferStack.Empty();
}

void FBilibiliChatWS::OnConnected()
{
    UE_LOG(LogTemp, Warning, TEXT("Connect Bilibili Web Successful."));
    bConnecting = false;

    EnterRoom();
}

void FBilibiliChatWS::OnConnectionError(const FString& Error)
{
    UE_LOG(LogTemp, Warning, TEXT("Connect Bilibili Web Failed."));
    bConnecting = false;
}

void FBilibiliChatWS::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
    UE_LOG(LogTemp, Warning, TEXT("Close Bilibili Web Link."));
    Close();
}

void FBilibiliChatWS::OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
    if (Size <= 0)
        return;
    PendingMessage.Emplace(Message(this, (const uint8_t*)Data, Size));

    if (BytesRemaining == 0)
        Dipatch();
}


bool FBilibiliChatWS::Connect(UWorld* world, int roomID) {
    if (roomID != this->nRoomID)
    {
        if (ContextWorld != world) {
            hHeartBeatTimer.Invalidate();
            ContextWorld = nullptr;
        }
        Close();
    }

    if (bConnecting)
        return true;

    this->nRoomID = roomID;

    ContextWorld = world;
    

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
    if (!WebSocket.IsValid())
        return false;

    // We bind all available events
    WebSocket->OnConnected().AddRaw(         this, &FBilibiliChatWS::OnConnected);
    WebSocket->OnConnectionError().AddRaw(   this, &FBilibiliChatWS::OnConnectionError);
    WebSocket->OnClosed().AddRaw(            this, &FBilibiliChatWS::OnClosed);
    WebSocket->OnMessage().AddRaw(           this, &FBilibiliChatWS::OnMessage);
    WebSocket->OnRawMessage().AddRaw(        this, &FBilibiliChatWS::OnRawMessage);
    WebSocket->OnMessageSent().AddRaw(       this, &FBilibiliChatWS::OnMessageSent);

    // And we finally connect to the server. 
    WebSocket->Connect();
    if (!WebSocket->IsConnected())
        bConnecting = true;

    return bConnecting;
}
#define CHECK_WEBSOCKET() if (!WebSocket.IsValid() || !WebSocket->IsConnected()) return;

inline bool FBilibiliChatWS::IsConnected() { return WebSocket.IsValid() && WebSocket->IsConnected(); }

FBilibiliChatWS::FBilibiliChatWS()
{
    FModuleManager::Get().LoadModuleChecked("WebSockets");
}

FBilibiliChatWS::~FBilibiliChatWS()
{
    Close();
    ReleaseBufferStack();
}

void FBilibiliChatWS::OnHeartBeat(uint32_t nPop)
{
    //UE_LOG(LogTemp, Warning,  TEXT("OnHertBeat:%d"), nPop);
}

void FBilibiliChatWS::Close()
{
    bConnecting = false;
    if (ContextWorld && hHeartBeatTimer.IsValid() && !IsValidChecked(ContextWorld))
        ContextWorld->GetTimerManager().ClearTimer(hHeartBeatTimer);
    hHeartBeatTimer.Invalidate();

    if (WebSocket.IsValid()) {
        WebSocket->OnConnected().RemoveAll(this);
        WebSocket->OnConnectionError().RemoveAll(this);
        WebSocket->OnClosed().RemoveAll(this);
        WebSocket->OnMessage().RemoveAll(this);
        WebSocket->OnRawMessage().RemoveAll(this);
        WebSocket->OnMessageSent().RemoveAll(this);
        if (WebSocket->IsConnected())
            WebSocket->Close();
        WebSocket.Reset();
    }
}




bool FBilibiliChatWS::TryDeserializingJson(uint8_t* Buffer, size_t Size, TSharedPtr<FJsonObject> JsonObject)
{
    FUTF8ToTCHAR str((const char*)Buffer, Size);

    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<>::Create(FString(str.Length(), str.Get()));

    TSharedPtr<FJsonObject> ptr;
    bool succ =  FJsonSerializer::Deserialize(Reader, ptr);
    if (succ) {
        *JsonObject = *ptr;//必须复制.这个设计有问题.Reader释放时会删除Values
    }
    return succ;
}

FString FBilibiliChatWS::PrettyJson(TSharedPtr<FJsonObject> JsonObject)
{
    FString OutputString;
    typedef TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FPrettyJsonWriter;
    typedef TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FPrettyJsonWriterFactory;
    auto Writer = FPrettyJsonWriterFactory::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    return OutputString;
}

inline uint32_t FBilibiliChatWS::ReadU32FromBigEndian(uint8_t* Data) {
    uint32_t pop = *((uint32_t*)Data);
    MakeSureBigEndian(pop);
    return pop;
}

inline uint16_t FBilibiliChatWS::ReadU16FromBigEndian(uint8_t* Data) {
    uint16_t pop = *((uint16_t*)Data);
    MakeSureBigEndian(pop);
    return pop;
}

inline int32_t FBilibiliChatWS::ReadI32FromBigEndian(uint8_t* Data) {
    int32_t pop = *((int32_t*)Data);
    MakeSureBigEndian(pop);
    return pop;
}

inline int16_t FBilibiliChatWS::ReadI16FromBigEndian(uint8_t* Data) {
    int16_t pop = *((int16_t*)Data);
    MakeSureBigEndian(pop);
    return pop;
}

int FBilibiliChatWS::Pack(BILIBILI_WS_OP Op, const FString& JsonContext, uint8_t* Buffer, size_t BufferSize)
{
    FTCHARToUTF8 data(*JsonContext);
    int packetLen = sizeof(Package) + data.Length();
    assert(BufferSize >= packetLen);
    assert(sizeof(Package) == 16);
    Package* package = (Package*)Buffer;
    MakeSureBigEndian(package->PackageLength = packetLen);
    MakeSureBigEndian(package->HeaderLength = sizeof(Package));
    MakeSureBigEndian(package->ProtocolVersion = 0);
    MakeSureBigEndian(package->Operation = (int)Op);
    memcpy(package->Data, data.Get(), data.Length());
   
    return packetLen;
}

void FBilibiliChatWS::Dipatch()
{
    while (!PendingMessage.IsEmpty()) {
        Message msg;
        PendingMessage.HeapPop(msg);
        Package* pkg = (Package*)msg.Data;
        pkg->SwapByteOrder();
        assert(msg.Size >= pkg->PackageLength);
        if (msg.Size > pkg->PackageLength) {
            PendingMessage.Insert(Message(this, msg.Data + pkg->PackageLength, msg.Size - pkg->PackageLength), 0);
        }
        //UE_LOG(LogTemp, Warning, TEXT("Msg(size:%d, op:%d, protocol:%d"), pkg->PackageLength, pkg->Operation, pkg->ProtocolVersion);
        switch ((BILIBILI_WS_OP)pkg->Operation)
        {
        case BILIBILI_WS_OP::WS_OP_HEARTBEAT_REPLY:
            OnHeartBeat(ReadU32FromBigEndian(pkg->Data));
            break;
        case BILIBILI_WS_OP::WS_OP_CONNECT_SUCCESS:
            OnEnterRoom();
            break;
        case BILIBILI_WS_OP::WS_OP_MESSAGE:
            {
                if (pkg->ProtocolVersion == (uint16_t)Protocol::JSON) {
                    TSharedRef<FJsonObject, ESPMode::ThreadSafe> JsonObject = MakeShareable(new FJsonObject);
                    if (TryDeserializingJson(pkg->Data, pkg->PackageLength - pkg->HeaderLength, JsonObject))
                    {
                        //UE_LOG(LogTemp, Warning, TEXT("Deserializing Json:%d, %s"), JsonObject->Values.Num(), UTF8_TO_TCHAR(pkg->Data));
                        OnMessage(JsonObject);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Deserializing Json Error (size:%d, op:%d, protocol:%d) %s"), pkg->PackageLength, pkg->Operation, pkg->ProtocolVersion, UTF8_TO_TCHAR(pkg->Data));
                    }
                }
                else if (pkg->ProtocolVersion == (uint16_t)Protocol::ZLIB) {
                    mz_ulong InOutLen = DefaultBufferSize;
                    auto buffer = AllocBuffer();
                    int status = uncompress(buffer, &InOutLen, pkg->Data, pkg->PackageLength - pkg->HeaderLength);
                    while (status == MZ_BUF_ERROR) {
                        ReleaseBuffer(buffer);
                        InOutLen *= 2;
                        buffer = AllocBuffer(InOutLen);
                        status = uncompress(buffer, &InOutLen, pkg->Data, pkg->PackageLength - pkg->HeaderLength);
                    }
                    BufferGuard g(this, buffer);
                    if (status == MZ_OK) 
                    {
                        TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
                        if (TryDeserializingJson(buffer, InOutLen, JsonObject))
                        {
                            //UE_LOG(LogTemp, Warning, TEXT("Deserializing Json: %s"), UTF8_TO_TCHAR(buffer));
                            OnMessage(JsonObject);
                        }
                        else
                        {
                            PendingMessage.Insert(Message(this, buffer, InOutLen), 0);
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Unzlib fail:%s, Msg(size:%d, op:%d, protocol:%d"), UTF8_TO_TCHAR(mz_error(status)), pkg->PackageLength, pkg->Operation, pkg->ProtocolVersion);                    
                    }
                }
                else if (pkg->ProtocolVersion == (uint16_t)Protocol::BROTLI) 
                {
                    size_t InOutLen = DefaultBufferSize;
                    auto buffer = AllocBuffer();
                    auto status = BrotliDecoderDecompress(pkg->PackageLength - pkg->HeaderLength, pkg->Data, &InOutLen, buffer);
                    while (status == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT) {
                        ReleaseBuffer(buffer);
                        InOutLen *= 2;
                        buffer = AllocBuffer(InOutLen);
                        status = BrotliDecoderDecompress(pkg->PackageLength - pkg->HeaderLength, pkg->Data, &InOutLen, buffer);
                    }
                    BufferGuard g(this, buffer);
                    if (status == BROTLI_DECODER_RESULT_SUCCESS)
                    {
                        TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
                        if (TryDeserializingJson(buffer, InOutLen, JsonObject))
                        {
                            //UE_LOG(LogTemp, Warning, TEXT("Deserializing Json: %s"), UTF8_TO_TCHAR(buffer));
                            OnMessage(JsonObject);
                        }
                        else
                        {
                            PendingMessage.Insert(Message(this, buffer, InOutLen), 0);
                        }
                    }
                    else
                    {
                        UE_LOG(LogTemp, Error, TEXT("Brotli fail:%d, Msg(size:%d, op:%d, protocol:%d"), status, pkg->PackageLength, pkg->Operation, pkg->ProtocolVersion);
                    }
                }
                else 
                {
                    UE_LOG(LogTemp, Error, TEXT("Unknow Msg(size:%d, op:%d, protocol:%d"), pkg->PackageLength, pkg->Operation, pkg->ProtocolVersion);
                }
            }
            break;
        default:

            break;
        }
       
    }
}

void FBilibiliChatWS::EnterRoom()
{
    if (!WebSocket.IsValid() || !WebSocket->IsConnected())
        return;
    auto buffer = AllocBuffer();
    BufferGuard g(this, buffer);
    int len = Pack(BILIBILI_WS_OP::WS_OP_USER_AUTHENTICATION, FString::Printf(TEXT("{\"roomid\":%d,\"protover\":3,\"uid\":10206127,\"platform\":\"web\",\"type\":2}"), nRoomID), buffer, DefaultBufferSize);
    WebSocket->Send(buffer, len, true);
}

void FBilibiliChatWS::OnEnterRoom()
{
    hHeartBeatTimer.Invalidate();
    ContextWorld->GetTimerManager().SetTimer(hHeartBeatTimer, FTimerDelegate::CreateRaw(this, &FBilibiliChatWS::HeartBeat), HeartBeatInterval, true, HeartBeatInterval);
}

void FBilibiliChatWS::HeartBeat()
{
    CHECK_WEBSOCKET();
    auto buffer = AllocBuffer();
    BufferGuard g(this, buffer);
    int len = Pack(BILIBILI_WS_OP::WS_OP_HEARTBEAT, FString::Printf(TEXT("")), buffer, DefaultBufferSize);
    WebSocket->Send(buffer, len, true);
}


void FBilibiliChatWS::OnMessage(TSharedPtr<FJsonObject> JsonObject)
{
    auto str = JsonObject->GetStringField(TEXT("cmd"));
    static TArray<TCHAR*> blackList = { TEXT("WATCHED_CHANGE"), TEXT("SEND_GIFT"), TEXT("COMBO_SEND"), TEXT("ROOM_REAL_TIME_MESSAGE_UPDATE"), TEXT("DANMU_MSG"), TEXT("DANMU_MSG:4:0:2:2:2:0"), TEXT("NOTICE_MSG"), TEXT("STOP_LIVE_ROOM_LIST"), TEXT("INTERACT_WORD"), TEXT("HOT_RANK_CHANGED_V2"), TEXT("ONLINE_RANK_V2"), TEXT("ONLINE_RANK_COUNT"), TEXT("ENTRY_EFFECT"), TEXT("SUPER_CHAT_MESSAGE"), TEXT("GUARD_BUY"), TEXT("USER_TOAST_MSG")
                                      , TEXT("ONLINE_RANK_TOP3"), TEXT("HOT_RANK_CHANGED"), TEXT("HOT_RANK_SETTLEMENT_V2"),TEXT("LIVE_MULTI_VIEW_CHANGE"),TEXT("PREPARING"),TEXT("HOT_ROOM_NOTIFY"),TEXT("LIVE_INTERACTIVE_GAME"),TEXT("HOT_RANK_SETTLEMENT_V2"),TEXT("HOT_RANK_SETTLEMENT_V2"),TEXT("HOT_RANK_SETTLEMENT_V2"),TEXT("HOT_RANK_SETTLEMENT_V2"),TEXT("HOT_RANK_SETTLEMENT_V2"), };
    if (!blackList.Contains(str))
    {
        //UE_LOG(LogTemp, Warning, TEXT("on message json:%s"), *PrettyJson(JsonObject));
        //OnMsgCallback.ExecuteIfBound(str, JsonObject);
    }
    OnMsgCallback.ExecuteIfBound(str, JsonObject);
}

FBilibiliChatWS::Message::Message(FBilibiliChatWS* mgr, const uint8_t* data, size_t size)
    : Size(size), Mgr(mgr)
{
    Data = Mgr->AllocBuffer(size);
    memcpy(Data, data, size);
}

FBilibiliChatWS::Message::Message(Message& other)
{
    Data = other.Data;
    Size = other.Size;
    Mgr = other.Mgr;
    other.Data = nullptr;
    other.Size = 0;
}

FBilibiliChatWS::Message::Message(Message&& other) {
    Data = other.Data;
    Size = other.Size;
    Mgr = other.Mgr;
    other.Data = nullptr;
    other.Size = 0;
}

FBilibiliChatWS::Message::~Message() { 
    if (Data) 
        Mgr->ReleaseBuffer(Data); 
    Data = nullptr; 
    Size = 0; 
}

FBilibiliChatWS::Message& FBilibiliChatWS::Message::operator = (FBilibiliChatWS::Message&& other) {
    Data = other.Data; 
    Size = other.Size; 
    Mgr = other.Mgr; 
    other.Data = nullptr; 
    other.Size = 0;
    return *this;
}
