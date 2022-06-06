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
	/*  Sub Message */
//https://github.com/SocialSisterYi/bilibili-API-collect/blob/master/live/message_stream.md
//https://github.com/lovelyyoshino/Bilibili-Live-API/blob/master/API.WebSocket.md
	void WATCHED_CHANGE(TSharedPtr<FJsonObject> JsonObject) {
		/*{
			"cmd": "WATCHED_CHANGE",
				"data" :
			{
				"num": 1286571,
					"text_small" : "128.6万",
					"text_large" : "128.6万人看过"
			}
		}*/
	}
	void SEND_GIFT(TSharedPtr<FJsonObject> JsonObject) {
		/*{
			"cmd": "SEND_GIFT",
				"data" :
			{
				"action": "投喂",
					"batch_combo_id" : "batch:gift:combo_id:22649028:456664753:31036:1654420788.3229",
					"batch_combo_send" : null,
					"beatId" : "",
					"biz_source" : "Live",
					"blind_gift" : null,
					"broadcast_id" : 0,
					"coin_type" : "gold",
					"combo_resources_id" : 1,
					"combo_send" : null,
					"combo_stay_time" : 3,
					"combo_total_coin" : 300,
					"crit_prob" : 0,
					"demarcation" : 1,
					"discount_price" : 100,
					"dmscore" : 24,
					"draw" : 0,
					"effect" : 0,
					"effect_block" : 0,
					"face" : "http://i0.hdslb.com/bfs/face/4e7c1488112020ce81d111fff7920d80c26b1932.jpg",
					"float_sc_resource_id" : 0,
					"giftId" : 31036,
					"giftName" : "小花花",
					"giftType" : 0,
					"gold" : 0,
					"guard_level" : 0,
					"is_first" : false,
					"is_special_batch" : 0,
					"magnification" : 1,
					"medal_info" :
				{
					"anchor_roomid": 0,
						"anchor_uname" : "",
						"guard_level" : 0,
						"icon_id" : 0,
						"is_lighted" : 0,
						"medal_color" : 6126494,
						"medal_color_border" : 12632256,
						"medal_color_end" : 12632256,
						"medal_color_start" : 12632256,
						"medal_level" : 6,
						"medal_name" : "宠兮兮",
						"special" : "",
						"target_id" : 1855954322
				},
					"name_color": "",
						"num" : 1,
						"original_gift_name" : "",
						"price" : 100,
						"rcost" : 14705368,
						"remain" : 0,
						"rnd" : "1654420800121800005",
						"send_master" : null,
						"silver" : 0,
						"super" : 0,
						"super_batch_gift_num" : 3,
						"super_gift_num" : 3,
						"svga_block" : 0,
						"switch" : true,
						"tag_image" : "",
						"tid" : "1654420800121800005",
						"timestamp" : 1654420800,
						"top_list" : null,
						"total_coin" : 100,
						"uid" : 22649028,
						"uname" : "蜗蜗奶糖V"
			}
		}*/
	}

	void DANMU_MSG(TSharedPtr<FJsonObject> JsonObject) {
		/*{
			"cmd": "DANMU_MSG",
				"info" : [
					[ 0, 1, 25, 16777215, 1654436842357, 1654423063, 0,
						"b019d170",
						0, 0, 0,
						"",
						0,
						"{}",
						"{}",
					{
						"mode": 0,
						"show_player_type" : 0,
						"extra" : "{\"send_from_me\":false,\"mode\":0,\"color\":16777215,\"dm_type\":0,\"font_size\":25,\"player_mode\":1,\"show_player_type\":0,\"content\":\"龙和虎和其它队伍有质的差距了感觉\",\"user_hash\":\"2954482032\",\"emoticon_unique\":\"\",\"bulge_display\":0,\"recommend_score\":0,\"main_state_dm_color\":\"\",\"objective_state_dm_color\":\"\",\"direction\":0,\"pk_direction\":0,\"quartet_direction\":0,\"yeah_space_type\":\"\",\"yeah_space_url\":\"\",\"jump_to_url\":\"\",\"space_type\":\"\",\"space_url\":\"\"}"
					}
					],
						"龙和虎和其它队伍有质的差距了感觉",
						[1742876760,
						"叫这个吧-",
						0, 0, 0, 10000, 1,
						""
						],
						[],
						[0, 0, 9868950,
						">50000",
						0],
						[
							"",
							""
						],
						0, 0, null,
					{
						"ts": 1654436842,
						"ct" : "35D8E046"
					},
						0, 0, null, null, 0, 7 ]
		}*/
		/*{
			"cmd": "DANMU_MSG:4:0:2:2:2:0",
				"info" : [
					[ 0, 1, 25, 16777215, 1654420801084, 207055124, 0,
						"19dffc0d",
						0, 0, 0,
						"",
						0,
						"{}",
						"{}",
					{
						"mode": 0,
						"show_player_type" : 0,
						"extra" : "{\"send_from_me\":false,\"mode\":0,\"color\":16777215,\"dm_type\":0,\"font_size\":25,\"player_mode\":1,\"show_player_type\":0,\"content\":\"哦哦哦\",\"user_hash\":\"434109453\",\"emoticon_unique\":\"\",\"bulge_display\":0,\"recommend_score\":5,\"main_state_dm_color\":\"\",\"objective_state_dm_color\":\"\",\"direction\":0,\"pk_direction\":0,\"quartet_direction\":0,\"yeah_space_type\":\"\",\"yeah_space_url\":\"\",\"jump_to_url\":\"\",\"space_type\":\"\",\"space_url\":\"\"}"
					}
					],
						"哦哦哦",
						[22150900,
						"yijq23",
						0, 0, 0, 10000, 1,
						""
						],
						[6,
						"氿氿吖",
						"氿氿ayumiko",
						943565, 6126494,
						"",
						0, 6126494, 6126494, 6126494, 0, 1, 2482644],
						[20, 0, 6406234,
						">50000",
						0],
						[
							"",
							""
						],
						0, 0, null,
					{
						"ts": 1654420801,
						"ct" : "84F98100"
					},
						0, 0, null, null, 0, 21 ]
		}*/
	}
	void COMBO_SEND() {
		/*{
			"cmd": "COMBO_SEND",
				"data" :
			{
				"action": "投喂",
					"batch_combo_id" : "batch:gift:combo_id:22649028:456664753:31036:1654420788.3229",
					"batch_combo_num" : 3,
					"combo_id" : "gift:combo_id:22649028:456664753:31036:1654420788.3217",
					"combo_num" : 3,
					"combo_total_coin" : 300,
					"dmscore" : 24,
					"gift_id" : 31036,
					"gift_name" : "小花花",
					"gift_num" : 0,
					"is_show" : 1,
					"medal_info" :
				{
					"anchor_roomid": 0,
						"anchor_uname" : "",
						"guard_level" : 0,
						"icon_id" : 0,
						"is_lighted" : 0,
						"medal_color" : 6126494,
						"medal_color_border" : 12632256,
						"medal_color_end" : 12632256,
						"medal_color_start" : 12632256,
						"medal_level" : 6,
						"medal_name" : "宠兮兮",
						"special" : "",
						"target_id" : 1855954322
				},
					"name_color": "",
						"r_uname" : "央视新闻",
						"ruid" : 456664753,
						"send_master" : null,
						"total_num" : 3,
						"uid" : 22649028,
						"uname" : "蜗蜗奶糖V"
			}
		}*/
	}
	void ROOM_REAL_TIME_MESSAGE_UPDATE() {
		/*{
			"cmd": "ROOM_REAL_TIME_MESSAGE_UPDATE",
				"data" :
			{
				"roomid": 21686237,
					"fans" : 9320372,
					"red_notice" : -1,
					"fans_club" : 1606
			}
		}*/
	}
	void STOP_LIVE_ROOM_LIST() {
		/*{
			"cmd": "STOP_LIVE_ROOM_LIST",
				"data" :
			{
				"room_id_list": [24733021, 24744593, 25136474, 25137090, 25181122, 8537979, 22889626, 23664398, 24733098, 448754, 5258493, 25120050, 25172172, 6966258, 10157925, 25154308, 25180673, 275352, 4303193, 24149259, 2031985, 22084345, 23468368, 24868377, 5583742, 11158815, 24304437, 24564740, 25181152, 6018689, 11767789, 22297175, 23300997, 25154369, 25178696, 6435599, 2319440, 24461888, 1563452, 23639469, 24488018, 1003341, 1834615, 24932171, 3398825, 6260457, 7318270, 12662195, 23122690, 23236741, 24886413, 25007965, 9886092, 22528148, 24551910, 24651217, 25181085, 40106, 11405792, 2253731
					9, 230748, 24956542, 25180598, 4533923, 6247146, 12564065, 21764215, 21941505, 25019202, 486135, 4302683, 14407639, 23945658, 9893976, 22610048, 24431032, 2471826, 25046497, 9496659, 22588184, 25181160, 21531582, 22433905, 24827233, 6536619, 1152799, 22246183, 22769209, 24732834, 25179743, 25180455, 13556430, 23686277, 23888059, 4525269, 53816, 23878495, 23878821, 24681229, 24857511, 25174979, 281892, 3914152, 1470931, 23879908, 25174114, 25180752, 25181156, 7146791, 11106011, 1292852, 1375172, 22399084, 24390442, 24977224, 39948, 176837, 24083698, 25181108, 24319180, 25116775, 8215019, 133873
					9, 21432732, 23945792, 24589239, 5245738]
			}
		}*/
	}

	void NOTICE_MSG() {
		/*{
			"cmd": "NOTICE_MSG",
				"id" : 272,
				"name" : "超级战舰-BLS小分",
				"full" :
			{
				"head_icon": "https://i0.hdslb.com/bfs/live/caf93f24cb84ee33b97e9a92398197fe93553c08.webp",
					"tail_icon" : "http://i0.hdslb.com/bfs/live/822da481fdaba986d738db5d8fd469ffa95a8fa1.webp",
					"head_icon_fa" : "https://i0.hdslb.com/bfs/live/724896ec186250c0a530b22a416665a862f0ef77.png",
					"tail_icon_fa" : "https://i0.hdslb.com/bfs/live/7d7af6107ef808438e6c3b4db0c7868bf78e5172.png",
					"head_icon_fan" : 23,
					"tail_icon_fan" : 4,
					"background" : "#66A74EFF",
					"color" : "#FFFFFFFF",
					"highlight" : "#FDFF2FFF",
					"time" : 20
			},
				"half":
				{
					"head_icon": "https://i0.hdslb.com/bfs/live/ec9b374caec5bd84898f3780a10189be96b86d4e.png",
						"tail_icon" : "",
						"background" : "#85B971FF",
						"color" : "#FFFFFFFF",
						"highlight" : "#FDFF2FFF",
						"time" : 15
				},
					"side":
					{
						"head_icon": "",
							"background" : "",
							"color" : "",
							"highlight" : "",
							"border" : ""
					},
						"roomid": 23358451,
							"real_roomid" : 23358451,
							"msg_common" : "<%北凉丶齊當國%>投喂<%吴尽意%>1个超级战舰，欧皇登场，快来围观吧！",
							"msg_self" : "<%北凉丶齊當國%>投喂<%吴尽意%>1个超级战舰，欧皇登场，快来围观吧！",
							"link_url" : "https://live.bilibili.com/23358451?accept_quality=%5B10000%2C150%5D&broadcast_type=0&current_qn=150&current_quality=150&is_room_feed=1&live_play_network=other&p2p_type=0&playurl_h264=http%3A%2F%2Fd1--cn-gotcha09.bilivideo.com%2Flive-bvc%2F713517%2Flive_1210635994_33081795_1500.flv%3Fexpires%3D1654440362%26len%3D0%26oi%3D0%26pt%3D%26qn%3D150%26trid%3D100062c7117adcdd4577aa773f0b06e6a66d%26sigparams%3Dcdn%2Cexpires%2Clen%2Coi%2Cpt%2Cqn%2Ctrid%26cdn%3Dcn-gotcha09%26sign%3D9d3e934b29606c1f38f92d70702b0682%26sk%3D0b857f4697b25ef030efaf17b001921b%26p2p_type%3D0%26src%3D57345%26sl%3D
							1 % 26flowtype % 3D0 % 26source % 3Dbatch % 26order % 3D1 % 26machinezone % 3Dylf % 26pp % 3Dsrt % 26site % 3D56ee8fcebf10667cbaca4d4ccc2f3237 & playurl_h265 = &quality_description = % 5B % 7B % 22qn % 22 % 3A10000 % 2C % 22desc % 22 % 3A % 22 % E5 % 8E % 9F % E7 % 94 % BB % 22 % 7D % 2C % 7B % 22qn % 22 % 3A150 % 2C % 22desc % 22 % 3A % 22 % E9 % AB % 98 % E6 % B8 % 85 % 22 % 7D % 5D & from = 28003 & extra_jump_from = 28003 & live_lottery_type = 1",
							"msg_type" : 2,
							"shield_uid" : -1,
							"business_id" : "20009",
							"scatter" :
						{
							"min": 0,
								"max" : 0
						},
							"marquee_id" : "",
								"notice_type" : 0
		}*/
	}

	void INTERACT_WORD() {
		/*{
			"cmd": "INTERACT_WORD",
				"data" :
			{
				"contribution":
				{
					"grade": 0
				},
					"dmscore" : 4,
						"fans_medal" :
					{
						"anchor_roomid": 22093072,
							"guard_level" : 0,
							"icon_id" : 0,
							"is_lighted" : 0,
							"medal_color" : 6067854,
							"medal_color_border" : 12632256,
							"medal_color_end" : 12632256,
							"medal_color_start" : 12632256,
							"medal_level" : 2,
							"medal_name" : "魔栗喵",
							"score" : 279,
							"special" : "",
							"target_id" : 21512451
					},
						"identities": [1] ,
							"is_spread" : 0,
							"msg_type" : 1,
							"roomid" : 719085,
							"score" : 1654436842929,
							"spread_desc" : "",
							"spread_info" : "",
							"tail_icon" : 0,
							"timestamp" : 1654436842,
							"trigger_time" : 1.6544368418200123e+18,
							"uid" : 187909379,
							"uname" : "光脚丫的学者",
							"uname_color" : ""
			}
		}*/
	}
	void HOT_RANK_CHANGED_V2() {
		/*{
			"cmd": "HOT_RANK_CHANGED_V2",
				"data" :
			{
				"rank": 5,
					"trend" : 0,
					"countdown" : 745,
					"timestamp" : 1654436855,
					"web_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=2&area_id=2&parent_area_id=2&second_area_id=87",
					"live_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=1&area_id=2&parent_area_id=2&second_area_id=87&is_live_half_webview=1&hybrid_rotate_d=1&hybrid_half_ui=1,3,100p,70p,ffffff,0,30,100,12,0;2,2,375,100p,ffffff,0,30,100,0,0;3,3,100p,70p,ffffff,0,30,100,12,0;4,2,375,100p,ffffff,0,30,100,0,0;5,3,100p,70p,ffffff,0,30,100,0,0;6,3,100p,70p,ffffff,0,30,100,0,0;7,3,100p,70p,ffffff,0,30,100,0,0;8,3,100p,70p,ffffff,0,30,100,0,0",
					"blink_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=3&area_id=2&parent_area_id=2&second_area_id=87&is_live_half_webview=1&hybrid_rotate_d=1&is_cling_player=1&hybrid_half_ui=1,3,100p,70p,ffffff,0,30,100,0,0;2,2,375,100p,ffffff,0,30,100,0,0;3,3,100p,70p,ffffff,0,30,100,0,0;4,2,375,100p,ffffff,0,30,100,0,0;5,3,100p,70p,ffffff,0,30,100,0,0;6,3,100p,70p,ffffff,0,30,100,0,0;7,3,100p,70p,ffffff,0,30,100,0,0;8,3,100p,70p,ffffff,0,30,100,0,0",
					"live_link_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=5&area_id=2&parent_area_id=2&second_area_id=87&is_live_half_webview=1&hybrid_rotate_d=1&is_cling_player=1&hybrid_half_ui=1,3,100p,70p,f4eefa,0,30,100,0,0;2,2,375,100p,f4eefa,0,30,100,0,0;3,3,100p,70p,f4eefa,0,30,100,0,0;4,2,375,100p,f4eefa,0,30,100,0,0;5,3,100p,70p,f4eefa,0,30,100,0,0;6,3,100p,70p,f4eefa,0,30,100,0,0;7,3,100p,70p,f4eefa,0,30,100,0,0;8,3,100p,70p,f4eefa,0,30,100,0,0",
					"pc_link_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=4&is_live_half_webview=1&area_id=2&parent_area_id=2&second_area_id=87&pc_ui=338,465,f4eefa,0",
					"icon" : "https://i0.hdslb.com/bfs/live/cb2e160ac4f562b347bb5ae6e635688ebc69580f.png",
					"area_name" : "守望先锋",
					"rank_desc" : "守望先锋top50"
			}
		}*/
	}
	void ONLINE_RANK_V2() {
		/*{
			"cmd": "ONLINE_RANK_V2",
				"data" :
			{
				"list": [
				{
					"uid": 296829884,
						"face" : "http://i2.hdslb.com/bfs/face/c2b6e0bda4f99b490ee38ba3046b628ad26d2318.jpg",
						"score" : "2481",
						"uname" : "gun1powder",
						"rank" : 1,
						"guard_level" : 3
				},
			{
				"uid": 20644196,
				"face" : "http://i2.hdslb.com/bfs/face/3c6deaa829bb63363435f160451975e03cfdc52f.jpg",
				"score" : "1383",
				"uname" : "鹽巴NaCl",
				"rank" : 2,
				"guard_level" : 3
			},
			{
				"uid": 2946772,
				"face" : "http://i1.hdslb.com/bfs/face/f20eb476f3ebdfc5379603464169ff895efcc1ad.jpg",
				"score" : "1001",
				"uname" : "弓长张兖州牧",
				"rank" : 3,
				"guard_level" : 2
			},
			{
				"uid": 45996933,
				"face" : "http://i0.hdslb.com/bfs/face/c7398f65f0a9f699f1fe3290bfebfacc007fb044.jpg",
				"score" : "302",
				"uname" : "想玩车的老鱼",
				"rank" : 4,
				"guard_level" : 0
			},
			{
				"uid": 385049295,
				"face" : "http://i0.hdslb.com/bfs/face/81e688f2d6bed25db96efc2be77c4bb0351cbcda.jpg",
				"score" : "300",
				"uname" : "好饿的胖纸",
				"rank" : 5,
				"guard_level" : 3
			},
			{
				"uid": 7302523,
				"face" : "http://i1.hdslb.com/bfs/face/47b18d8a0e4217bec1a65735212abd17099cf3a0.jpg",
				"score" : "195",
				"uname" : "おおかみさん",
				"rank" : 6,
				"guard_level" : 3
			},
			{
				"uid": 241804320,
				"face" : "http://i0.hdslb.com/bfs/face/f73d7607c51b96716a3209a5fa94533cac7cff67.jpg",
				"score" : "119",
				"uname" : "九秋蓬莱",
				"rank" : 7,
				"guard_level" : 0
			}
				],
					"rank_type": "gold-rank"
			}
		}*/
	}
	void ONLINE_RANK_COUNT() {
		/*{
			"cmd": "ONLINE_RANK_COUNT",
				"data" :
			{
				"count": 1233
			}
		}*/
	}

	void ENTRY_EFFECT() {
		/*{
			"cmd": "ENTRY_EFFECT",
				"data" :
			{
				"id": 4,
					"uid" : 5811211,
					"target_id" : 3595470,
					"mock_effect" : 0,
					"face" : "https://i1.hdslb.com/bfs/face/8e44250c25414b0efbbe810b31f80b37547e2642.jpg",
					"privilege_type" : 3,
					"copy_writing" : "欢迎舰长 <%梦凝锁心%> 进入直播间",
					"copy_color" : "#ffffff",
					"highlight_color" : "#E6FF00",
					"priority" : 1,
					"basemap_url" : "https://i0.hdslb.com/bfs/live/mlive/11a6e8eb061c3e715d0a6a2ac0ddea2faa15c15e.png",
					"show_avatar" : 1,
					"effective_time" : 2,
					"web_basemap_url" : "https://i0.hdslb.com/bfs/live/mlive/11a6e8eb061c3e715d0a6a2ac0ddea2faa15c15e.png",
					"web_effective_time" : 2,
					"web_effect_close" : 0,
					"web_close_time" : 0,
					"business" : 1,
					"copy_writing_v2" : "欢迎舰长 <%梦凝锁心%> 进入直播间",
					"icon_list" : [] ,
					"max_delay_time" : 7,
					"trigger_time" : 1.6544368459467692e+18,
					"identities" : 6,
					"effect_silent_time" : 300,
					"effective_time_new" : 0,
					"web_dynamic_url_webp" : "",
					"web_dynamic_url_apng" : "",
					"mobile_dynamic_url_webp" : ""
			}
		}*/
	}

	void SUPER_CHAT_MESSAGE() {
		/*{
			"cmd": "SUPER_CHAT_MESSAGE",
				"data" :
			{
				"background_bottom_color": "#2A60B2",
					"background_color" : "#EDF5FF",
					"background_color_end" : "#405D85",
					"background_color_start" : "#3171D2",
					"background_icon" : "",
					"background_image" : "https://i0.hdslb.com/bfs/live/a712efa5c6ebc67bafbe8352d3e74b820a00c13e.png",
					"background_price_color" : "#7497CD",
					"color_point" : 0.69999999999999996,
					"dmscore" : 120,
					"end_time" : 1654436912,
					"gift" :
				{
					"gift_id": 12000,
						"gift_name" : "醒目留言",
						"num" : 1
				},
					"id": 4203437,
						"is_ranked" : 1,
						"is_send_audit" : 1,
						"medal_info" :
					{
						"anchor_roomid": 719085,
							"anchor_uname" : "萌萌哒老陈",
							"guard_level" : 3,
							"icon_id" : 0,
							"is_lighted" : 1,
							"medal_color" : "#6154c",
							"medal_color_border" : 6809855,
							"medal_color_end" : 6850801,
							"medal_color_start" : 398668,
							"medal_level" : 25,
							"medal_name" : "老陈皮",
							"special" : "",
							"target_id" : 3595470
					},
						"message": "今晚最大的输家，上海龙",
							"message_font_color" : "#A3F6FF",
							"message_trans" : "",
							"price" : 30,
							"rate" : 1000,
							"start_time" : 1654436852,
							"time" : 60,
							"token" : "43EED317",
							"trans_mark" : 0,
							"ts" : 1654436852,
							"uid" : 385049295,
							"user_info" :
						{
							"face": "http://i0.hdslb.com/bfs/face/81e688f2d6bed25db96efc2be77c4bb0351cbcda.jpg",
								"face_frame" : "https://i0.hdslb.com/bfs/live/80f732943cc3367029df65e267960d56736a82ee.png",
								"guard_level" : 3,
								"is_main_vip" : 1,
								"is_svip" : 0,
								"is_vip" : 0,
								"level_color" : "#61c05a",
								"manager" : 0,
								"name_color" : "#00D1F1",
								"title" : "0",
								"uname" : "好饿的胖纸",
								"user_level" : 12
						}
			},
				"roomid": 719085
		}*/
	}
	
	void GUARD_BUY()
	{
		/*{
			"cmd": "GUARD_BUY",
				"data" :
			{
				"uid": 10865396,
					"username" : "AdamBayern27",
					"guard_level" : 3,
					"num" : 1,
					"price" : 198000,
					"gift_id" : 10003,
					"gift_name" : "舰长",
					"start_time" : 1654437545,
					"end_time" : 1654437545
			}
		}*/
	}

	void USER_TOAST_MSG() {
		/*{
			"cmd": "USER_TOAST_MSG",
				"data" :
			{
				"anchor_show": true,
					"color" : "#00D1F1",
					"dmscore" : 90,
					"effect_id" : 397,
					"end_time" : 1654437545,
					"guard_level" : 3,
					"is_show" : 0,
					"num" : 1,
					"op_type" : 3,
					"payflow_id" : "2206052158497432153960528",
					"price" : 138000,
					"role_name" : "舰长",
					"start_time" : 1654437545,
					"svga_block" : 0,
					"target_guard_count" : 147,
					"toast_msg" : "<%AdamBayern27%> 自动续费了舰长",
					"uid" : 10865396,
					"unit" : "月",
					"user_show" : true,
					"username" : "AdamBayern27"
			}
		}*/
	}

	void ONLINE_RANK_TOP3() {
		/*{
			"cmd": "ONLINE_RANK_TOP3",
				"data" :
			{
				"dmscore": 112,
					"list" : [
				{
					"msg": "恭喜 <%AdamBayern27%> 成为高能榜",
						"rank" : 2
				}
					]
			}
		}*/
	}

	void HOT_RANK_CHANGED() {
		/*{
			"cmd": "HOT_RANK_CHANGED",
				"data" :
			{
				"rank": 36,
					"trend" : 1,
					"countdown" : 50,
					"timestamp" : 1654437550,
					"web_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=2&area_id=2&parent_area_id=2&second_area_id=0",
					"live_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=1&area_id=2&parent_area_id=2&second_area_id=0&is_live_half_webview=1&hybrid_rotate_d=1&hybrid_half_ui=1,3,100p,70p,ffffff,0,30,100,12,0;2,2,375,100p,ffffff,0,30,100,0,0;3,3,100p,70p,ffffff,0,30,100,12,0;4,2,375,100p,ffffff,0,30,100,0,0;5,3,100p,70p,ffffff,0,30,100,0,0;6,3,100p,70p,ffffff,0,30,100,0,0;7,3,100p,70p,ffffff,0,30,100,0,0;8,3,100p,70p,ffffff,0,30,100,0,0",
					"blink_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=3&area_id=2&parent_area_id=2&second_area_id=0&is_live_half_webview=1&hybrid_rotate_d=1&is_cling_player=1&hybrid_half_ui=1,3,100p,70p,ffffff,0,30,100,0,0;2,2,375,100p,ffffff,0,30,100,0,0;3,3,100p,70p,ffffff,0,30,100,0,0;4,2,375,100p,ffffff,0,30,100,0,0;5,3,100p,70p,ffffff,0,30,100,0,0;6,3,100p,70p,ffffff,0,30,100,0,0;7,3,100p,70p,ffffff,0,30,100,0,0;8,3,100p,70p,ffffff,0,30,100,0,0",
					"live_link_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=5&area_id=2&parent_area_id=2&second_area_id=0&is_live_half_webview=1&hybrid_rotate_d=1&is_cling_player=1&hybrid_half_ui=1,3,100p,70p,f4eefa,0,30,100,0,0;2,2,375,100p,f4eefa,0,30,100,0,0;3,3,100p,70p,f4eefa,0,30,100,0,0;4,2,375,100p,f4eefa,0,30,100,0,0;5,3,100p,70p,f4eefa,0,30,100,0,0;6,3,100p,70p,f4eefa,0,30,100,0,0;7,3,100p,70p,f4eefa,0,30,100,0,0;8,3,100p,70p,f4eefa,0,30,100,0,0",
					"pc_link_url" : "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=4&is_live_half_webview=1&area_id=2&parent_area_id=2&second_area_id=0&pc_ui=338,465,f4eefa,0",
					"icon" : "https://i0.hdslb.com/bfs/live/65dbe013f7379c78fc50dfb2fd38d67f5e4895f9.png",
					"area_name" : "网游",
					"rank_desc" : ""
			}
		}*/
	}

	void HOT_RANK_SETTLEMENT_V2() {
		/*{
			"cmd": "HOT_RANK_SETTLEMENT_V2",
				"data" :
			{
				"rank": 4,
					"uname" : "萌萌哒老陈",
					"face" : "http://i0.hdslb.com/bfs/face/ee8b46e70a7d333204aebfd56779d25692928a38.jpg",
					"timestamp" : 1654437300,
					"icon" : "https://i0.hdslb.com/bfs/live/cb2e160ac4f562b347bb5ae6e635688ebc69580f.png",
					"area_name" : "守望先锋",
					"url" : "https://live.bilibili.com/p/html/live-app-hotrank/result.html?is_live_half_webview=1&hybrid_half_ui=1,5,250,200,f4eefa,0,30,0,0,0;2,5,250,200,f4eefa,0,30,0,0,0;3,5,250,200,f4eefa,0,30,0,0,0;4,5,250,200,f4eefa,0,30,0,0,0;5,5,250,200,f4eefa,0,30,0,0,0;6,5,250,200,f4eefa,0,30,0,0,0;7,5,250,200,f4eefa,0,30,0,0,0;8,5,250,200,f4eefa,0,30,0,0,0&areaId=87&cache_key=0a0c939a43deac325955eb76d313a469",
					"cache_key" : "0a0c939a43deac325955eb76d313a469",
					"dm_msg" : "恭喜主播 <% 萌萌哒老陈 %> 荣登限时热门榜守望先锋榜top4! 即将获得热门流量推荐哦！"
			}
		}*/
	}

	void LIVE_MULTI_VIEW_CHANGE() {
		/*{
			"cmd": "LIVE_MULTI_VIEW_CHANGE",
				"data" :
			{
				"scatter":
				{
					"max": 120,
						"min" : 5
				}
			}
		}*/
	}

	void PREPARING() {
		/*{
			"cmd": "PREPARING",
				"roomid" : "719085"
		}*/
	}

	void  HOT_ROOM_NOTIFY() {
		/*{
			"cmd": "HOT_ROOM_NOTIFY",
				"data" :
			{
				"threshold": 10000,
					"ttl" : 300,
					"exit_no_refresh" : 1,
					"random_delay_req_v2" : [
				{
					"path": "/live/getRoundPlayVideo",
						"delay" : 10
				},
			{
				"path": "/xlive/web-room/v1/index/getOffLiveList",
				"delay" : 120000
			}
					]
			}
		}*/
	}

	void LIVE_INTERACTIVE_GAME() {
		/*{
			"cmd": "LIVE_INTERACTIVE_GAME",
				"data" :
			{
				"type": 2,
					"uid" : 398827747,
					"uname" : "时愈悲",
					"uface" : "",
					"gift_id" : 0,
					"gift_name" : "",
					"gift_num" : 0,
					"price" : 0,
					"paid" : false,
					"msg" : "ts",
					"fans_medal_level" : 15,
					"guard_level" : 0,
					"timestamp" : 1654440092,
					"anchor_lottery" : null,
					"pk_info" : null,
					"anchor_info" : null
			}
		}*/
	}

	int nRoomID = -1;
	FTimerHandle hHeartBeatTimer;

public:
	bool Connect(UWorld* world, int nRoomID);
	bool IsConnected();
	FBilibiliChatWS();
	~FBilibiliChatWS();

	FOnMessage OnMsgCallback;
	
};
