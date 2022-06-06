# bilibili_ue_plugin

### 前言
无言。

### 初衷
    做弹幕互动游戏。最近才发现有这种玩法，真是后知后觉>.< 
    召唤有兴趣的小伙伴一起开发着玩玩，对现行的蹦迪和挤地铁比较无感,缺少目标感和成就感，适合尝鲜。
    后面想搞点有深度一点，或者有趣的一点的玩法。例如朋友在做的《真物理.抓娃娃》。

### ****使用说明****
    第一步：需要正确安装UE及相关编译软件（暂时没有提交编译结果，也没有配置引擎版本设置，我是用UE5.0.2源码编译出来的版本。具体有问题可以联系我。  
    第二步：把Plugins文件夹放到UE工程文件夹内。如果已经有Plugins文件夹，就把Bilibili文件夹复制到Plugins文件夹内就可以了。  
    第三步：正确获取直播间RoomID,在直播间视频区域右键,点击“播放器日志信息/显示日志”,在信息窗口找到"live player load roomid: XXXXXXXX",这个XXXXXXX就是RoomID。  

![roomid获取](https://raw.githubusercontent.com/chironc/bilibili_ue_plugin/main/doc/roomid%E8%8E%B7%E5%8F%96%E6%96%B9%E5%BC%8F.png)

    第四步：蓝图使用，具体有两种方式，一看就懂，上图。   
        使用方式1：  
![使用方式1](https://raw.githubusercontent.com/chironc/bilibili_ue_plugin/main/doc/%E4%BD%BF%E7%94%A8%E6%96%B9%E5%BC%8F1.png)

        使用方式2：  
![使用方式1](https://raw.githubusercontent.com/chironc/bilibili_ue_plugin/main/doc/%E4%BD%BF%E7%94%A8%E6%96%B9%E5%BC%8F2.png)

    第四步：最大疑惑应该是GetStringByPath节点的Path参数。这里解释一下：  
        Json支持简单Path方式访问，例如参考中获取DANMU_MSG的/info/2/1，是指获得Root对象的info为key的子对象的，如果子对象是数组，则取下标，所以/2表示子对象数组的第3个元素，第三个元素作为一个数组，再继续取/1即第2个元素，例如下面的json样例，读到的就是"叫这个吧-".

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
        ]
    ]
}
```

    第五步：定位自己想要的信息的.
        请参考 **doc/JsonProtocol.md** 文件  
        例如用户进入房间Json,完整的弹幕Json,送礼物Json,基本挂了好些直播间，BilibiliChatWS.cpp:OnMessage输入统计而来。
        理论上都够用了，如果b站协议更新了，可能需要自行修正，或联系我修正。

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