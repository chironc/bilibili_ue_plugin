# bilibili_ue_plugin

### 前言
无言。

### 初衷
做弹幕互动游戏。最近才发现有这种玩法，真是后知后觉>.< 
召唤有兴趣的小伙伴一起开发着玩玩，对现行的蹦迪和挤地铁比较无感,缺少目标感和成就感，适合尝鲜。
后面想搞点有深度一点，或者有趣的一点的玩法。例如朋友在做的《真物理.抓娃娃》。

### 使用说明
参考doc文件夹的图片。
下面是各个消息的json完整内容。需要分析的自行研究。

```
Json支持简单Path方式访问，例如参考中获取DANMU_MSG的/info/2/1，是指获得Root对象的info为key的子对象的，如果子对象是数组，则取下标，所以/2表示子对象数组的第3个元素，第三个元素作为一个数组，再继续取/1即第2个元素，例如下面的json样例，读到的就是"叫这个吧-".
```

```json
DANMU_MSG
{
    "cmd": "DANMU_MSG",
    "info": [
        [
            0,
            1,
            25,
            16777215,
            1654436842357,
            1654423063,
            0,
            "b019d170",
            0,
            0,
            0,
            "",
            0,
            "{}",
            "{}",
            {
                "mode": 0,
                "show_player_type": 0,
                "extra": "{\"send_from_me\":false,\"mode\":0,\"color\":16777215,\"dm_type\":0,\"font_size\":25,\"player_mode\":1,\"show_player_type\":0,\"content\":\"龙和虎和其它队伍有质的差距了感觉\",\"user_hash\":\"2954482032\",\"emoticon_unique\":\"\",\"bulge_display\":0,\"recommend_score\":0,\"main_state_dm_color\":\"\",\"objective_state_dm_color\":\"\",\"direction\":0,\"pk_direction\":0,\"quartet_direction\":0,\"yeah_space_type\":\"\",\"yeah_space_url\":\"\",\"jump_to_url\":\"\",\"space_type\":\"\",\"space_url\":\"\"}"
            }
        ],
        "龙和虎和其它队伍有质的差距了感觉",
        [
            1742876760,
            "叫这个吧-",
            0,
            0,
            0,
            10000,
            1,
            ""
        ],
        [],
        [
            0,
            0,
            9868950,
            ">50000",
            0
        ],
        [
            "",
            ""
        ],
        0,
        0,
        null,
        {
            "ts": 1654436842,
            "ct": "35D8E046"
        },
        0,
        0,
        null,
        null,
        0,
        7
    ]
}
```

```json
INTERACT_WORD
{
    "cmd": "INTERACT_WORD",
    "data": {
        "contribution": {
            "grade": 0
        },
        "dmscore": 4,
        "fans_medal": {
            "anchor_roomid": 22093072,
            "guard_level": 0,
            "icon_id": 0,
            "is_lighted": 0,
            "medal_color": 6067854,
            "medal_color_border": 12632256,
            "medal_color_end": 12632256,
            "medal_color_start": 12632256,
            "medal_level": 2,
            "medal_name": "魔栗喵",
            "score": 279,
            "special": "",
            "target_id": 21512451
        },
        "identities": [
            1
        ],
        "is_spread": 0,
        "msg_type": 1,
        "roomid": 719085,
        "score": 1654436842929,
        "spread_desc": "",
        "spread_info": "",
        "tail_icon": 0,
        "timestamp": 1654436842,
        "trigger_time": 1654436841820012300,
        "uid": 187909379,
        "uname": "光脚丫的学者",
        "uname_color": ""
    }
}
```

```json
SEND_GIFT
{
    "cmd": "SEND_GIFT",
    "data": {
        "action": "投喂",
        "batch_combo_id": "batch:gift:combo_id:22649028:456664753:31036:1654420788.3229",
        "batch_combo_send": null,
        "beatId": "",
        "biz_source": "Live",
        "blind_gift": null,
        "broadcast_id": 0,
        "coin_type": "gold",
        "combo_resources_id": 1,
        "combo_send": null,
        "combo_stay_time": 3,
        "combo_total_coin": 300,
        "crit_prob": 0,
        "demarcation": 1,
        "discount_price": 100,
        "dmscore": 24,
        "draw": 0,
        "effect": 0,
        "effect_block": 0,
        "face": "http://i0.hdslb.com/bfs/face/4e7c1488112020ce81d111fff7920d80c26b1932.jpg",
        "float_sc_resource_id": 0,
        "giftId": 31036,
        "giftName": "小花花",
        "giftType": 0,
        "gold": 0,
        "guard_level": 0,
        "is_first": false,
        "is_special_batch": 0,
        "magnification": 1,
        "medal_info": {
            "anchor_roomid": 0,
            "anchor_uname": "",
            "guard_level": 0,
            "icon_id": 0,
            "is_lighted": 0,
            "medal_color": 6126494,
            "medal_color_border": 12632256,
            "medal_color_end": 12632256,
            "medal_color_start": 12632256,
            "medal_level": 6,
            "medal_name": "宠兮兮",
            "special": "",
            "target_id": 1855954322
        },
        "name_color": "",
        "num": 1,
        "original_gift_name": "",
        "price": 100,
        "rcost": 14705368,
        "remain": 0,
        "rnd": "1654420800121800005",
        "send_master": null,
        "silver": 0,
        "super": 0,
        "super_batch_gift_num": 3,
        "super_gift_num": 3,
        "svga_block": 0,
        "switch": true,
        "tag_image": "",
        "tid": "1654420800121800005",
        "timestamp": 1654420800,
        "top_list": null,
        "total_coin": 100,
        "uid": 22649028,
        "uname": "蜗蜗奶糖V"
    }
}
```

```json
COMBO_SEND
{
    "cmd": "COMBO_SEND",
    "data": {
        "action": "投喂",
        "batch_combo_id": "batch:gift:combo_id:22649028:456664753:31036:1654420788.3229",
        "batch_combo_num": 3,
        "combo_id": "gift:combo_id:22649028:456664753:31036:1654420788.3217",
        "combo_num": 3,
        "combo_total_coin": 300,
        "dmscore": 24,
        "gift_id": 31036,
        "gift_name": "小花花",
        "gift_num": 0,
        "is_show": 1,
        "medal_info": {
            "anchor_roomid": 0,
            "anchor_uname": "",
            "guard_level": 0,
            "icon_id": 0,
            "is_lighted": 0,
            "medal_color": 6126494,
            "medal_color_border": 12632256,
            "medal_color_end": 12632256,
            "medal_color_start": 12632256,
            "medal_level": 6,
            "medal_name": "宠兮兮",
            "special": "",
            "target_id": 1855954322
        },
        "name_color": "",
        "r_uname": "央视新闻",
        "ruid": 456664753,
        "send_master": null,
        "total_num": 3,
        "uid": 22649028,
        "uname": "蜗蜗奶糖V"
    }
}
```
```json
ROOM_REAL_TIME_MESSAGE_UPDATE
{
    "cmd": "ROOM_REAL_TIME_MESSAGE_UPDATE",
    "data": {
        "roomid": 21686237,
        "fans": 9320372,
        "red_notice": -1,
        "fans_club": 1606
    }
}
```
```json
STOP_LIVE_ROOM_LIST
{
    "cmd": "STOP_LIVE_ROOM_LIST",
    "data": {
        "room_id_list": [
            24733021,
            24744593,
            25136474,
            25137090,
            25181122,
            8537979,
            22889626
        ]
    }
}
```

```json
WATCHED_CHANGE
{
	"cmd": "WATCHED_CHANGE",
	"data" : {
		"num": 1286571,
		"text_small" : "128.6万",
		"text_large" : "128.6万人看过"
	}
}
```

```json
NOTICE_MSG
{
    "cmd": "NOTICE_MSG",
    "id": 272,
    "name": "超级战舰-BLS小分",
    "full": {
        "head_icon": "https://i0.hdslb.com/bfs/live/caf93f24cb84ee33b97e9a92398197fe93553c08.webp",
        "tail_icon": "http://i0.hdslb.com/bfs/live/822da481fdaba986d738db5d8fd469ffa95a8fa1.webp",
        "head_icon_fa": "https://i0.hdslb.com/bfs/live/724896ec186250c0a530b22a416665a862f0ef77.png",
        "tail_icon_fa": "https://i0.hdslb.com/bfs/live/7d7af6107ef808438e6c3b4db0c7868bf78e5172.png",
        "head_icon_fan": 23,
        "tail_icon_fan": 4,
        "background": "#66A74EFF",
        "color": "#FFFFFFFF",
        "highlight": "#FDFF2FFF",
        "time": 20
    },
    "half": {
        "head_icon": "https://i0.hdslb.com/bfs/live/ec9b374caec5bd84898f3780a10189be96b86d4e.png",
        "tail_icon": "",
        "background": "#85B971FF",
        "color": "#FFFFFFFF",
        "highlight": "#FDFF2FFF",
        "time": 15
    },
    "side": {
        "head_icon": "",
        "background": "",
        "color": "",
        "highlight": "",
        "border": ""
    },
    "roomid": 23358451,
    "real_roomid": 23358451,
    "msg_common": "<%北凉丶齊當國%>投喂<%吴尽意%>1个超级战舰，欧皇登场，快来围观吧！",
    "msg_self": "<%北凉丶齊當國%>投喂<%吴尽意%>1个超级战舰，欧皇登场，快来围观吧！",
    "link_url": "https://live.bilibili.com/23358451?accept_quality=%5B10000%2C150%5D&broadcast_type=0&current_qn=150&current_quality=150&is_room_feed=1&live_play_network=other&p2p_type=0&playurl_h264=http%3A%2F%2Fd1--cn-gotcha09.bilivideo.com%2Flive-bvc%2F713517%2Flive_1210635994_33081795_1500.flv%3Fexpires%3D1654440362%26len%3D0%26oi%3D0%26pt%3D%26qn%3D150%26trid%3D100062c7117adcdd4577aa773f0b06e6a66d%26sigparams%3Dcdn%2Cexpires%2Clen%2Coi%2Cpt%2Cqn%2Ctrid%26cdn%3Dcn-gotcha09%26sign%3D9d3e934b29606c1f38f92d70702b0682%26sk%3D0b857f4697b25ef030efaf17b001921b%26p2p_type%3D0%26src%3D57345%26sl%3D1 % 26flowtype % 3D0 % 26source % 3Dbatch % 26order % 3D1 % 26machinezone % 3Dylf % 26pp % 3Dsrt % 26site % 3D56ee8fcebf10667cbaca4d4ccc2f3237 & playurl_h265 = &quality_description = % 5B % 7B % 22qn % 22 % 3A10000 % 2C % 22desc % 22 % 3A % 22 % E5 % 8E % 9F % E7 % 94 % BB % 22 % 7D % 2C % 7B % 22qn % 22 % 3A150 % 2C % 22desc % 22 % 3A % 22 % E9 % AB % 98 % E6 % B8 % 85 % 22 % 7D % 5D & from = 28003 & extra_jump_from = 28003 & live_lottery_type = 1",
    "msg_type": 2,
    "shield_uid": -1,
    "business_id": "20009",
    "scatter": {
        "min": 0,
        "max": 0
    },
    "marquee_id": "",
    "notice_type": 0
}
```


```json
HOT_RANK_CHANGED_V2
{
    "cmd": "HOT_RANK_CHANGED_V2",
    "data": {
        "rank": 5,
        "trend": 0,
        "countdown": 745,
        "timestamp": 1654436855,
        "web_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=2&area_id=2&parent_area_id=2&second_area_id=87",
        "live_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=1&area_id=2&parent_area_id=2&second_area_id=87&is_live_half_webview=1&hybrid_rotate_d=1&hybrid_half_ui=1,3,100p,70p,ffffff,0,30,100,12,0;2,2,375,100p,ffffff,0,30,100,0,0;3,3,100p,70p,ffffff,0,30,100,12,0;4,2,375,100p,ffffff,0,30,100,0,0;5,3,100p,70p,ffffff,0,30,100,0,0;6,3,100p,70p,ffffff,0,30,100,0,0;7,3,100p,70p,ffffff,0,30,100,0,0;8,3,100p,70p,ffffff,0,30,100,0,0",
        "blink_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=3&area_id=2&parent_area_id=2&second_area_id=87&is_live_half_webview=1&hybrid_rotate_d=1&is_cling_player=1&hybrid_half_ui=1,3,100p,70p,ffffff,0,30,100,0,0;2,2,375,100p,ffffff,0,30,100,0,0;3,3,100p,70p,ffffff,0,30,100,0,0;4,2,375,100p,ffffff,0,30,100,0,0;5,3,100p,70p,ffffff,0,30,100,0,0;6,3,100p,70p,ffffff,0,30,100,0,0;7,3,100p,70p,ffffff,0,30,100,0,0;8,3,100p,70p,ffffff,0,30,100,0,0",
        "live_link_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=5&area_id=2&parent_area_id=2&second_area_id=87&is_live_half_webview=1&hybrid_rotate_d=1&is_cling_player=1&hybrid_half_ui=1,3,100p,70p,f4eefa,0,30,100,0,0;2,2,375,100p,f4eefa,0,30,100,0,0;3,3,100p,70p,f4eefa,0,30,100,0,0;4,2,375,100p,f4eefa,0,30,100,0,0;5,3,100p,70p,f4eefa,0,30,100,0,0;6,3,100p,70p,f4eefa,0,30,100,0,0;7,3,100p,70p,f4eefa,0,30,100,0,0;8,3,100p,70p,f4eefa,0,30,100,0,0",
        "pc_link_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=4&is_live_half_webview=1&area_id=2&parent_area_id=2&second_area_id=87&pc_ui=338,465,f4eefa,0",
        "icon": "https://i0.hdslb.com/bfs/live/cb2e160ac4f562b347bb5ae6e635688ebc69580f.png",
        "area_name": "守望先锋",
        "rank_desc": "守望先锋top50"
    }
}
```
```json
ONLINE_RANK_V2
{
    "cmd": "ONLINE_RANK_V2",
    "data": {
        "list": [
            {
                "uid": 296829884,
                "face": "http://i2.hdslb.com/bfs/face/c2b6e0bda4f99b490ee38ba3046b628ad26d2318.jpg",
                "score": "2481",
                "uname": "gun1powder",
                "rank": 1,
                "guard_level": 3
            },
            {
                "uid": 20644196,
                "face": "http://i2.hdslb.com/bfs/face/3c6deaa829bb63363435f160451975e03cfdc52f.jpg",
                "score": "1383",
                "uname": "鹽巴NaCl",
                "rank": 2,
                "guard_level": 3
            },
            {
                "uid": 2946772,
                "face": "http://i1.hdslb.com/bfs/face/f20eb476f3ebdfc5379603464169ff895efcc1ad.jpg",
                "score": "1001",
                "uname": "弓长张兖州牧",
                "rank": 3,
                "guard_level": 2
            },
            {
                "uid": 45996933,
                "face": "http://i0.hdslb.com/bfs/face/c7398f65f0a9f699f1fe3290bfebfacc007fb044.jpg",
                "score": "302",
                "uname": "想玩车的老鱼",
                "rank": 4,
                "guard_level": 0
            },
            {
                "uid": 385049295,
                "face": "http://i0.hdslb.com/bfs/face/81e688f2d6bed25db96efc2be77c4bb0351cbcda.jpg",
                "score": "300",
                "uname": "好饿的胖纸",
                "rank": 5,
                "guard_level": 3
            },
            {
                "uid": 7302523,
                "face": "http://i1.hdslb.com/bfs/face/47b18d8a0e4217bec1a65735212abd17099cf3a0.jpg",
                "score": "195",
                "uname": "おおかみさん",
                "rank": 6,
                "guard_level": 3
            },
            {
                "uid": 241804320,
                "face": "http://i0.hdslb.com/bfs/face/f73d7607c51b96716a3209a5fa94533cac7cff67.jpg",
                "score": "119",
                "uname": "九秋蓬莱",
                "rank": 7,
                "guard_level": 0
            }
        ],
        "rank_type": "gold-rank"
    }
}
```

```json
ONLINE_RANK_COUNT
{
    "cmd": "ONLINE_RANK_COUNT",
    "data": {
        "count": 1233
    }
}
```

```json
ENTRY_EFFECT
{
    "cmd": "ENTRY_EFFECT",
    "data": {
        "id": 4,
        "uid": 5811211,
        "target_id": 3595470,
        "mock_effect": 0,
        "face": "https://i1.hdslb.com/bfs/face/8e44250c25414b0efbbe810b31f80b37547e2642.jpg",
        "privilege_type": 3,
        "copy_writing": "欢迎舰长 <%梦凝锁心%> 进入直播间",
        "copy_color": "#ffffff",
        "highlight_color": "#E6FF00",
        "priority": 1,
        "basemap_url": "https://i0.hdslb.com/bfs/live/mlive/11a6e8eb061c3e715d0a6a2ac0ddea2faa15c15e.png",
        "show_avatar": 1,
        "effective_time": 2,
        "web_basemap_url": "https://i0.hdslb.com/bfs/live/mlive/11a6e8eb061c3e715d0a6a2ac0ddea2faa15c15e.png",
        "web_effective_time": 2,
        "web_effect_close": 0,
        "web_close_time": 0,
        "business": 1,
        "copy_writing_v2": "欢迎舰长 <%梦凝锁心%> 进入直播间",
        "icon_list": [],
        "max_delay_time": 7,
        "trigger_time": 1654436845946769200,
        "identities": 6,
        "effect_silent_time": 300,
        "effective_time_new": 0,
        "web_dynamic_url_webp": "",
        "web_dynamic_url_apng": "",
        "mobile_dynamic_url_webp": ""
    }
}
```
```json
SUPER_CHAT_MESSAGE
{
    "cmd": "SUPER_CHAT_MESSAGE",
    "data": {
        "background_bottom_color": "#2A60B2",
        "background_color": "#EDF5FF",
        "background_color_end": "#405D85",
        "background_color_start": "#3171D2",
        "background_icon": "",
        "background_image": "https://i0.hdslb.com/bfs/live/a712efa5c6ebc67bafbe8352d3e74b820a00c13e.png",
        "background_price_color": "#7497CD",
        "color_point": 0.7,
        "dmscore": 120,
        "end_time": 1654436912,
        "gift": {
            "gift_id": 12000,
            "gift_name": "醒目留言",
            "num": 1
        },
        "id": 4203437,
        "is_ranked": 1,
        "is_send_audit": 1,
        "medal_info": {
            "anchor_roomid": 719085,
            "anchor_uname": "萌萌哒老陈",
            "guard_level": 3,
            "icon_id": 0,
            "is_lighted": 1,
            "medal_color": "#6154c",
            "medal_color_border": 6809855,
            "medal_color_end": 6850801,
            "medal_color_start": 398668,
            "medal_level": 25,
            "medal_name": "老陈皮",
            "special": "",
            "target_id": 3595470
        },
        "message": "今晚最大的输家，上海龙",
        "message_font_color": "#A3F6FF",
        "message_trans": "",
        "price": 30,
        "rate": 1000,
        "start_time": 1654436852,
        "time": 60,
        "token": "43EED317",
        "trans_mark": 0,
        "ts": 1654436852,
        "uid": 385049295,
        "user_info": {
            "face": "http://i0.hdslb.com/bfs/face/81e688f2d6bed25db96efc2be77c4bb0351cbcda.jpg",
            "face_frame": "https://i0.hdslb.com/bfs/live/80f732943cc3367029df65e267960d56736a82ee.png",
            "guard_level": 3,
            "is_main_vip": 1,
            "is_svip": 0,
            "is_vip": 0,
            "level_color": "#61c05a",
            "manager": 0,
            "name_color": "#00D1F1",
            "title": "0",
            "uname": "好饿的胖纸",
            "user_level": 12
        }
    },
    "roomid": 719085
}
```
```json
GUARD_BUY
{
    "cmd": "GUARD_BUY",
    "data": {
        "uid": 10865396,
        "username": "AdamBayern27",
        "guard_level": 3,
        "num": 1,
        "price": 198000,
        "gift_id": 10003,
        "gift_name": "舰长",
        "start_time": 1654437545,
        "end_time": 1654437545
    }
}
```
```json
USER_TOAST_MSG
{
    "cmd": "USER_TOAST_MSG",
    "data": {
        "anchor_show": true,
        "color": "#00D1F1",
        "dmscore": 90,
        "effect_id": 397,
        "end_time": 1654437545,
        "guard_level": 3,
        "is_show": 0,
        "num": 1,
        "op_type": 3,
        "payflow_id": "2206052158497432153960528",
        "price": 138000,
        "role_name": "舰长",
        "start_time": 1654437545,
        "svga_block": 0,
        "target_guard_count": 147,
        "toast_msg": "<%AdamBayern27%> 自动续费了舰长",
        "uid": 10865396,
        "unit": "月",
        "user_show": true,
        "username": "AdamBayern27"
    }
}
```
```json
ONLINE_RANK_TOP3
		{
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
		}
	
```
```json
HOT_RANK_CHANGED
{
    "cmd": "HOT_RANK_CHANGED",
    "data": {
        "rank": 36,
        "trend": 1,
        "countdown": 50,
        "timestamp": 1654437550,
        "web_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=2&area_id=2&parent_area_id=2&second_area_id=0",
        "live_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=1&area_id=2&parent_area_id=2&second_area_id=0&is_live_half_webview=1&hybrid_rotate_d=1&hybrid_half_ui=1,3,100p,70p,ffffff,0,30,100,12,0;2,2,375,100p,ffffff,0,30,100,0,0;3,3,100p,70p,ffffff,0,30,100,12,0;4,2,375,100p,ffffff,0,30,100,0,0;5,3,100p,70p,ffffff,0,30,100,0,0;6,3,100p,70p,ffffff,0,30,100,0,0;7,3,100p,70p,ffffff,0,30,100,0,0;8,3,100p,70p,ffffff,0,30,100,0,0",
        "blink_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=3&area_id=2&parent_area_id=2&second_area_id=0&is_live_half_webview=1&hybrid_rotate_d=1&is_cling_player=1&hybrid_half_ui=1,3,100p,70p,ffffff,0,30,100,0,0;2,2,375,100p,ffffff,0,30,100,0,0;3,3,100p,70p,ffffff,0,30,100,0,0;4,2,375,100p,ffffff,0,30,100,0,0;5,3,100p,70p,ffffff,0,30,100,0,0;6,3,100p,70p,ffffff,0,30,100,0,0;7,3,100p,70p,ffffff,0,30,100,0,0;8,3,100p,70p,ffffff,0,30,100,0,0",
        "live_link_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=5&area_id=2&parent_area_id=2&second_area_id=0&is_live_half_webview=1&hybrid_rotate_d=1&is_cling_player=1&hybrid_half_ui=1,3,100p,70p,f4eefa,0,30,100,0,0;2,2,375,100p,f4eefa,0,30,100,0,0;3,3,100p,70p,f4eefa,0,30,100,0,0;4,2,375,100p,f4eefa,0,30,100,0,0;5,3,100p,70p,f4eefa,0,30,100,0,0;6,3,100p,70p,f4eefa,0,30,100,0,0;7,3,100p,70p,f4eefa,0,30,100,0,0;8,3,100p,70p,f4eefa,0,30,100,0,0",
        "pc_link_url": "https://live.bilibili.com/p/html/live-app-hotrank/index.html?clientType=4&is_live_half_webview=1&area_id=2&parent_area_id=2&second_area_id=0&pc_ui=338,465,f4eefa,0",
        "icon": "https://i0.hdslb.com/bfs/live/65dbe013f7379c78fc50dfb2fd38d67f5e4895f9.png",
        "area_name": "网游",
        "rank_desc": ""
    }
}
```
```json
HOT_RANK_SETTLEMENT_V2
{
    "cmd": "HOT_RANK_SETTLEMENT_V2",
    "data": {
        "rank": 4,
        "uname": "萌萌哒老陈",
        "face": "http://i0.hdslb.com/bfs/face/ee8b46e70a7d333204aebfd56779d25692928a38.jpg",
        "timestamp": 1654437300,
        "icon": "https://i0.hdslb.com/bfs/live/cb2e160ac4f562b347bb5ae6e635688ebc69580f.png",
        "area_name": "守望先锋",
        "url": "https://live.bilibili.com/p/html/live-app-hotrank/result.html?is_live_half_webview=1&hybrid_half_ui=1,5,250,200,f4eefa,0,30,0,0,0;2,5,250,200,f4eefa,0,30,0,0,0;3,5,250,200,f4eefa,0,30,0,0,0;4,5,250,200,f4eefa,0,30,0,0,0;5,5,250,200,f4eefa,0,30,0,0,0;6,5,250,200,f4eefa,0,30,0,0,0;7,5,250,200,f4eefa,0,30,0,0,0;8,5,250,200,f4eefa,0,30,0,0,0&areaId=87&cache_key=0a0c939a43deac325955eb76d313a469",
        "cache_key": "0a0c939a43deac325955eb76d313a469",
        "dm_msg": "恭喜主播 <% 萌萌哒老陈 %> 荣登限时热门榜守望先锋榜top4! 即将获得热门流量推荐哦！"
    }
}
```
```json
LIVE_MULTI_VIEW_CHANGE
{
    "cmd": "LIVE_MULTI_VIEW_CHANGE",
    "data": {
        "scatter": {
            "max": 120,
            "min": 5
        }
    }
}
```
```json
PREPARING
{
    "cmd": "PREPARING",
    "roomid": "719085"
}
```
```json
HOT_ROOM_NOTIFY
{
    "cmd": "HOT_ROOM_NOTIFY",
    "data": {
        "threshold": 10000,
        "ttl": 300,
        "exit_no_refresh": 1,
        "random_delay_req_v2": [
            {
                "path": "/live/getRoundPlayVideo",
                "delay": 10
            },
            {
                "path": "/xlive/web-room/v1/index/getOffLiveList",
                "delay": 120000
            }
        ]
    }
}
```
```json
LIVE_INTERACTIVE_GAME
{
    "cmd": "LIVE_INTERACTIVE_GAME",
    "data": {
        "type": 2,
        "uid": 398827747,
        "uname": "时愈悲",
        "uface": "",
        "gift_id": 0,
        "gift_name": "",
        "gift_num": 0,
        "price": 0,
        "paid": false,
        "msg": "ts",
        "fans_medal_level": 15,
        "guard_level": 0,
        "timestamp": 1654440092,
        "anchor_lottery": null,
        "pk_info": null,
        "anchor_info": null
    }
}
```


### 参考
https://github.com/lovelyyoshino/Bilibili-Live-API
https://github.com/SocialSisterYi/bilibili-API-collect

### 第三方库  
  
**Brotli** is a generic-purpose lossless compression algorithm.  
MIT License.  
https://github.com/google/brotli


**Miniz** is zlib library.  
MIT licensed.  
https://github.com/richgel999/miniz

**JsonBlueprint** is JsonUtility for Blueprints.  
MIT licensed.  
https://github.com/DaClemens/JsonBlueprint