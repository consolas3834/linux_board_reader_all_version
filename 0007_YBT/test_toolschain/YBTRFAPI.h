#ifndef com_YBTRFAPI_H_
#define com_YBTRFAPI_H_

#include "Global.h"
#define RETURN_OK            1  // 操作成功返回
#define ERR_NODATA           0  // 没有数据返回，接收数据长度不足
#define ERR_HANDLE          -1  // 错误的端口句柄
#define ERR_CMD             -2  //F0  指令字错误
#define ERR_CMD_RUN         -3  //F1 表示指令执行失败
#define ERR_CRC             -4  //F2 表示 CRC 错误
#define ERR_FORMAT          -5  //F3 表示数据帧格式、帧参数错误
#define ERR_FLASH_HIGH      -6  //F4 表示存储区超限
#define ERR_FLASH_LOCK      -7  //F5 表示存储区被锁
#define ERR_LOW_POWER       -8  //F6 表示功率不够
#define ERR_TAG             -9  //F7 表示操作标签不支持
#define ERR_FIND_TAG       -10 //F8 表示未检测到标签或标签无回应
#define ERR_OPERATE_TAG    -11 //F9 表示操作标签失败
#define ERR_ANTENNA        -12 //0x12 天线错误
#define ERR_WRITE_FLASH    -13 //0x14 写 flash 错误
#define ERROR_OTHER_FAIL   -14 //其他错误

#ifdef __cplusplus
extern "C" {
#endif
/**
 *Fun:MsgOpen  打开通讯通道
 *Para:hCom 通讯句柄
 *Para:lsCfgFile 配置文件路径 如"sysit.cfg"
 *Para:lsCfgItem  配置文件中段 如"Reader"
 *Return int  1表示成功  其他失败
 */
 int  MsgOpen(HANDLE &hCom, char* lsCfgFile, char* lsCfgItem);
/**
 *Fun:MsgAcceptSocket  监听客户端(如果要做服务端)
 *Para:hServer 服务端句柄
 *Para:hCom 客户端通讯句柄
 *Para:port 客户端端口
 *Para:strIp  客户端ip地址
 *Return int  1表示成功  其他失败
 */
 bool MsgAcceptSocket(HANDLE hServer, HANDLE &hCom, int &port, string &strIp);
/**
  *Fun:MsgGetNetState  网络是否断开
  *Para:hCom 通讯句柄
  *Return int  0表示断开  1已连网
 */
 int  MsgGetNetState(HANDLE hCom);
/**
  *Fun:MsgClose  关闭通讯通道
  *Para:hCom 通讯句柄
  *Return int  1表示成功  其他失败
 */

 int  MsgClose(HANDLE &hCom);
/**
  *Fun:MsgPowerOff  关功放
  *Para:hCom 通讯句柄
  *Return int  1表示成功  其他失败
 */
 int MsgPowerOff(HANDLE hCom);
/**
  *Fun:MsgPowerOn  开功放
  *Para:hCom 通讯句柄
  *Para:buf 天线号 1--64号天线，第 1 个字节：天线 57-64；第 2 个字节：天线 49~56；以此类推 第 8 个字节：天线 1~8；
  *Return int  1表示成功  其他失败
 */
 int MsgPowerOn(HANDLE hCom, unsigned char buf[8]);
/**
  *Fun:Msg6CTagFieldSet  设置读 6C 标签数据帧包含字段 ucAntenna 1包含天线，0不包天线  ucRssi 1.包含RSSi,0不包RSSI
  *Para:hCom 通讯句柄
  *Para:ucAntenna 0：表示数据帧不含接收标签的天线端口； 1：表示数据帧含接收标签的天线端口
  *Para:ucRssi  0：表示数据帧不含接收标签的 RSSI 值； 1：表示数据帧含接收标签的 RSSI 值；
  *Return int  1表示成功  其他失败
 */
 int Msg6CTagFieldSet(HANDLE hCom, unsigned char ucAntenna, unsigned char ucRssi);
/**
  *Fun:Msg6CTagFieldQuery  查询品读 6C 标签数据帧包含字段 ucAntenna 1包含天线，0不包天线  ucRssi 1.包含RSSi,0不包RSSI
  *Para:hCom 通讯句柄
  *Para:*ucAntenna(返回值 1个字节) 0：表示数据帧不含接收标签的天线端口； 1：表示数据帧含接收标签的天线端口；
  *Para:*ucRssi(返回值 1个字节)  0：表示数据帧不含接收标签的 RSSI 值； 1：表示数据帧含接收标签的 RSSI 值；
  *Return int  1表示成功  其他失败
 */
 int Msg6CTagFieldQuery(HANDLE hCom, unsigned char* ucAntenna, unsigned char *ucRssi);
/**
  *Fun:MsgTagInventory  读6C标签EPC区指令
  *Para:hCom 通讯句柄
  *Para:iReadNum 设置需要读取的次数，达到设定次数后停止读卡。该值为 0 表示不限次数
  *Para:iReadTime 设置需要读取的时间，单位 10ms，达到设定时间后停止读卡。该值为 0表示不限时间
  *Return int  1表示成功  其他失败
 */
 int MsgTagInventory(HANDLE hCom, unsigned short iReadNum, unsigned short iReadTime);
/**
  *Fun:MsgTagInventoryReport  获取epc数据
  *Para:hCom 通讯句柄
  *Para:pEpc epc数据
  *Para:pEpcLen epc数据长度（字节）
  *Para:ucAntenna 天线号 数据帧含接收标签的天线端口才会返回值
  *para:ucRssi RSSI数值  数据帧含接收标签的RSSI才会返回值
  *Return int  1表示成功  其他失败
 */
 int MsgTagInventoryReport(HANDLE hCom, unsigned char* pEpc, unsigned char *pEpcLen, unsigned char *ucAntenna, unsigned char *ucRssi);
/**
  *Fun:MsgTagRead  通用读6C标签
  *Para:hCom 通讯句柄
  *Para:ucReadType  0， 单次读取模式 1表示连续读取模式，读写器一直进行读卡操作直到读写器收到停止指令后结束读卡。
  *Para:pPwd  标签的访问密码 4 字节。默认是全 0
  *Para:ucUseEpc  是指是否需要返回 EPC 数据。 0 表示不需要返回， 1 表示需要返回
  *Para:ucTidStartPtr  TID 起始地址
  *Para:ucTidLen   TID长度(字) 1 个字表示 2 字节
  *Para:ucTidType 0 表示固定长度，表示只读取固定长度的标签区域；1 表示自适应长度，表示可自动适应长度的标签区域
  *Para:ucUserStartPtr  USER 起始地址
  *Para:ucUserLen   USER 长度(字)1 个字表示 2 字节
  *Para:ucReservedStartPtr  保留区起始地址
  *Para:ucReservedLen  保留区长度(字) 1 个字表示 2 字节 最大是 4 个字， 0 代表不返回保留区数据 销毁密码 2 个字+访问密码 2 个字
  *Para:iReadNum 设置需要读取的次数，达到设定次数后停止读卡。该值为 0 表示不限次数
  *Para:iReadTime 设置需要读取的时间，单位 10ms，达到设定时间后停止读卡。该值为 0表示不限时间
  *Return int  1表示成功  其他失败
 */
 int	MsgTagRead(HANDLE hCom, 
	unsigned char ucReadType,
	unsigned char *pPwd,
	unsigned char ucUseEpc,
	unsigned char ucTidStartPtr,
	unsigned char ucTidLen,
	unsigned char ucTidType,
	unsigned char ucUserStartPtr,
	unsigned char ucUserLen,
	unsigned char ucReservedStartPtr,
	unsigned char ucReservedLen,
	unsigned short iReadNum,
	unsigned short iReadTime
);
/**
  *Fun:MsgTagInventoryAllReport  获取标签数据
  *Para:hCom 通讯句柄
  *Para:pEpc epc数据
  *Para:pEpcLen epc数据长度（字节）
  *Para:pTid Tid数据
  *Para:pTidLen Tid数据长度（字节）
  *Para:pUser User数据
  *Para:pUserLen User数据长度（字节）
  *Para:pReverse 保留数据区数据
  *Para:ucReverseLen 保留数据区数据长度（字节）
  *Para:ucAntenna 天线号 数据帧含接收标签的天线端口才会返回值
  *para:ucRssi RSSI数值  数据帧含接收标签的RSSI才会返回值
  *Return int  1表示成功  其他失败
 */
 int MsgTagInventoryAllReport(HANDLE hCom,
	unsigned char* pEpc,
	unsigned char *pEpcLen,
	unsigned char* pTid,
	unsigned char *pTidLen,
	unsigned char* pUser,
	unsigned char *pUserLen,
	unsigned char *pReverse,
	unsigned char *ucReverseLen,
	unsigned char *ucAntenna,
	unsigned char *ucRssi);
/**
  *Fun:MsgTagWrite  写标签数据
  *Para:hCom 通讯句柄
  *Para:ucWriteType 0 表示单次写入模式，只针对指定的标签进行写操作 1 表示连续写入模式，针对符合匹配规则的多个标签连续进行写操作直到关功放停止
  *Para:pPwd 标签的访问密码 4 字节
  *Para:ucType 匹配的数据区 01H EPC 数据区 02H TID 数据区 03H USER 数据区
  *Para:ucStart 匹配数据起始地址
  *Para:ucLen  匹配数据长度字节（偶数）
  *Para:pData  匹配数据内容
  *Para:ucEpcStartPtr  epc数据起始地址
  *Para:ucEpcLen   epc数据长度字节（偶数）
  *Para:pEpcData   epc数据内容
  *Para:ucUserStartPtr  用户数据起始地址
  *Para:ucUserLen  用户区数据长度字节（偶数）
  *Para:pUserData  用户区数据内容
  *Para:ucReservedStartPtr  保留区数据起始地址
  *Para:ucReservedLen  保留区数据长度字节（偶数）
  *Para:pReservedData  保留区数据内容
  *Return int  1表示成功  其他失败
 */
 int MsgTagWrite(HANDLE hCom,
	unsigned char ucWriteType,
	unsigned char *pPwd,
	unsigned char ucType,
	unsigned char ucStart,
	unsigned char ucLen,
	unsigned char *pData,
	unsigned char ucEpcStartPtr,
	unsigned char ucEpcLen,
	unsigned char *pEpcData,
	unsigned char ucUserStartPtr,
	unsigned char ucUserLen,
	unsigned char *pUserData,
	unsigned char ucReservedStartPtr,
	unsigned char ucReservedLen,
	unsigned char *pReservedData
);
/**
  *Fun:MsgTagLock  6C标签锁操 
  *Para:hCom 通讯句柄
  *Para:ucType 匹配的数据区 1 EPC 数据区  2 TID 数据区 3 USER 数据区
  *Para:ucStart 数据起始地址
  *Para:ucLen  数据长度
  *para:pData  数据内容
  *para:pwd  标签的访问密码 4 字节
  *para:ucLockType 0 锁定 1解锁
  *para:ucBankType  0 所有数据区  1 epc 数据区 2 TID 数据区 3 USER 数据区 4访问密码区 5销毁密码区
  *Return int  1表示成功  其他失败
 */
 int MsgTagLock(HANDLE hCom, 
	unsigned char ucType,
	unsigned char ucStart,
	unsigned char ucLen,
	unsigned char *pData,
	unsigned char *pwd, 
	unsigned char ucLockType, 
	unsigned char ucBankType
);

/**
  *Fun:MsgTagKill  销毁 6C 标签
  *Para:hCom 通讯句柄
  *Para:ucType 匹配的数据区 1 EPC 数据区  2 TID 数据区 3 USER 数据区
  *Para:ucStart 数据起始地址
  *Para:ucLen  数据长度
  *para:pData  数据内容
  *para:pwd  标签的访问密码 4 字节
  *Return int  1表示成功  其他失败
 */
 int MsgTagKill(HANDLE hCom, 
	unsigned char ucType,
	unsigned char ucStart,
	unsigned char ucLen,
	unsigned char *pData,
	unsigned char *pwd);
/**
  *Fun:MsgTagSelect  选择标签
  *Para:hCom 通讯句柄
  *Para:ucType 匹配的数据区 1 EPC 数据区  2 TID 数据区 3 USER 数据区
  *Para:ucStart 数据起始地址
  *Para:ucLen  数据长度
  *para:pData  数据内容
  *para:ucFlag  0禁止截断 1使能截断
  *Return int  1表示成功  其他失败
 */
 int MsgTagSelect(HANDLE hCom, 
	unsigned char ucType,
	unsigned char ucStart,
	unsigned char ucLen,
	unsigned char *pData,
	unsigned char ucFlag);

/**
  *Fun:MsgAntennaQuery  查询天线端口
  *Para:hCom 通讯句柄
  *Para:uAntenna 天线数据内容 天线信息 8 个字节分别代表 1~64 号天线的状态，比特位为 0 代表天线关闭，比特位为 1 代表天线开启
  *Para:ucLen 天线数据长度
 */
 int MsgAntennaQuery(HANDLE hCom, unsigned char *uAntenna, unsigned char *ucLen);
/**
  *Fun:MsgAntennaSet  设置天线端口
  *Para:hCom 通讯句柄
  *Para:uAntenna 天线数据内容 天线信息 8 个字节分别代表 1~64 号天线的状态，比特位为 0 代表天线关闭，比特位为 1 代表天线开启
  *Para:ucLen 天线数据长度（8字节）
 */
 int MsgAntennaSet(HANDLE hCom, unsigned char *uAntenna, unsigned char ucLen);
/**
  *Fun:MsgAntennaConnectState  查询天线端口连接状态
  *Para:hCom 通讯句柄
  *Para:bflag 天线连接状态 false 连接异常 true 连接正常  64个字节
 */
 int MsgAntennaConnectState(HANDLE hCom, bool *bflag);
/**
  *Fun:MsgRfidStatusQuery  查询RFID工作状态状态(0217H)
  *Para:hCom 通讯句柄
  *Para:ucState 读写器状态 00 表示读写器空闲 01 表示读写器正在对标签操作
  *Para:ucAirProtocol 协议 仅当状态为读卡时，此部分才有意义 00 表示正在读取 6C 协议标签 01 表示正在读取 6B 协议标签  02 表示正在读取 6B+6C 协议标签
  *Para:ucFrequencyArea 工作频段  00 表示 CN---(920MHz~925MHz) 01 表示 FCC---(902MHz~928MHz) 02 表示 EU---(865 MHz ~ 868 MHz)
  *Para:ucAntennaNO  天线号64字节 天线1到天线64
  *Para:bEnable       天线1到天线64使能情况 0不使能， 1使能
  *Para:ucPowerValue  天线功率值内容
  *Para:ucPowerLen    天线功率值内容长度
 */
 int MsgRfidStatusQuery(HANDLE hCom, 
	unsigned char *ucState,
	unsigned char * ucAirProtocol,
	unsigned char *ucFrequencyArea,
	unsigned char *ucAntennaNO,
	bool *bEnable,
	unsigned char  *ucPowerValue,
	unsigned char *ucPowerLen);

/**
  *Fun:MsgGpioDefinitionQuery  查询gpio 设置gpio 0x00 输入  0x01输出  0xFF 禁止
  *Para:hCom 通讯句柄
  *Para:ucGpio1  0x00 输入  0x01输出  0xFF 禁止
  *Para:ucGpio2  0x00 输入  0x01输出  0xFF 禁止
  *Para:ucGpio3  0x00 输入  0x01输出  0xFF 禁止
  *Para:ucGpio4  0x00 输入  0x01输出  0xFF 禁止
 */
 int MsgGpioDefinitionQuery(HANDLE hCom, unsigned char *ucGpio1, unsigned char *ucGpio2, unsigned char *ucGpio3, unsigned char *ucGpio4);

/**
  *Fun:MsgGpioOutSet  设置输出电平 0低电平，1高电平
  *Para:hCom 通讯句柄
  *Para:ucGpio 指定需要配置的 GPIO 端口号 ， 3 表示 GPIO3， 4 表示 GPIO4；
  *Para:ucGpioValue 0低电平，1高电平
 */
 int MsgGpioOutSet(HANDLE hCom, unsigned char ucGpio, unsigned char ucGpioValue);
/**
  *Fun:MsgGpioOutQuery  查询输出电平 0低电平，1高电平
  *Para:hCom 通讯句柄
  *Para:ucGpio 指定需要配置的 GPIO 端口号 ， 3 表示 GPIO3， 4 表示 GPIO4；
  *Para:ucGpioValue 0低电平，1高电平
 */
 int MsgGpioOutQuery(HANDLE hCom, unsigned char ucGpio, unsigned char *ucGpioValue);
/**
  *Fun:MsgGpioInQuery  查询输入电平 0低电平，1高电平
  *Para:hCom 通讯句柄
  *Para:ucGpio 指定需要配置的 GPIO 端口号 ， 1 表示 GPIO1， 2 表示 GPIO2；
  *Para:ucGpioValue 0低电平，1高电平
 */
 int MsgGpioInQuery(HANDLE hCom, unsigned char ucGpio, unsigned char *ucGpioValue);
/**
  *Fun:MsgGpiTriggerQuery  查询触发条件
  *Para:hCom 通讯句柄
  *Para:ucGpio 指定需要配置的 GPIO 端口号， 1 表示 GPIO1， 2 表示 GPIO2；
  *Para:ucTriggerCondition 触发条件0，表示关闭当前端口的 GPIO 触发功能；1， 表示 GPIO 高电平触发；2， 表示 GPIO 低电平触发；
  *Para:ucStopCondition 停止条件 0， 采用 GPIO 状态反转作为停止条件 1， 采用延时作为停止条件；
  *Para:shDelayTime 延时时间
  *Para:ucTriggerMsg  触发条件绑定的指令数据
  *Para:ucTriggerMsgLen  指令数据长度
 */
 int MsgGpiTriggerQuery(HANDLE hCom, 
	unsigned char ucGpio,
	unsigned char *ucTriggerCondition,
	unsigned char *ucStopCondition,
	unsigned short *shDelayTime,
	unsigned char *ucTriggerMsg,
	unsigned char *ucTriggerMsgLen);
/**
  *Fun:MsgGpiTriggerSet  设置触发条件
  *Para:hCom 通讯句柄
  *Para:ucGpio 指定需要配置的 GPIO 端口号， 1 表示 GPIO1， 2 表示 GPIO2；
  *Para:ucTriggerCondition 触发条件0，表示关闭当前端口的 GPIO 触发功能；1， 表示 GPIO 高电平触发；2， 表示 GPIO 低电平触发；
  *Para:ucStopCondition 停止条件 0， 采用 GPIO 状态反转作为停止条件 1， 采用延时作为停止条件；
  *Para:shDelayTime 延时时间
  *Para:ucTriggerMsg  触发条件绑定的指令数据
  *Para:ucTriggerMsgLen  指令数据长度
 */
 int MsgGpiTriggerSet(HANDLE hCom,
	unsigned char ucGpio,
	unsigned char ucTriggerCondition,
	unsigned char ucStopCondition,
	unsigned short shDelayTime,
	unsigned char *ucTriggerMsg,
	unsigned char ucTriggerMsgLen);
/**
  *Fun:MsgFilteringTimeQuery  查询标签过滤时间
  *Para:hCom 通讯句柄
  *Para:shTime 标签过滤时间
 */
 int MsgFilteringTimeQuery(HANDLE hCom, unsigned short *shTime);
/**
  *Fun:MsgFilteringTimeSet  设置标签过滤时间
  *Para:hCom 通讯句柄
  *Para:shTime 标签过滤时间
 */
 int MsgFilteringTimeSet(HANDLE hCom, unsigned short shTime);
/**
  *Fun:MsgIntervalTimeQuery  查询间隔读标签时间
  *Para:hCom 通讯句柄
  *Para:shReadTime 间歇读标签时间长度
  *Para:shStopTime 间歇停止读标签时间长度
 */
 int MsgIntervalTimeQuery(HANDLE hCom, unsigned short *shReadTime, unsigned short *shStopTime);
/**
  *Fun:MsgIntervalTimeSet  设置间隔读标签时间
  *Para:hCom 通讯句柄
  *Para:shReadTime 间歇读标签时间长度
  *Para:shStopTime 间歇停止读标签时间长度
 */
 int MsgIntervalTimeSet(HANDLE hCom, unsigned short shReadTime, unsigned short shStopTime);
/**
  *Fun:MsgUhfBandQuery查询工作频段 
  *Para:hCom 通讯句柄
  *Para:ucUhf 工作频段 00 表示 CN---(920MHz~925MHz) 01 表示 FCC---(902MHz~928MHz) 02 表示 EU---(865 MHz ~ 868 MHz)
 */
 int  MsgUhfBandQuery(HANDLE hCom,unsigned char *ucUhf);
/**
  *Fun:MsgUhfBandSet设置工作频段
  *Para:hCom 通讯句柄
  *Para:ucUhf 工作频段 00 表示 CN---(920MHz~925MHz) 01 表示 FCC---(902MHz~928MHz) 02 表示 EU---(865 MHz ~ 868 MHz)
 */
 int  MsgUhfBandSet(HANDLE hCom,unsigned char ucUhf);
/**
  *Fun:MsgPowerQuery查询功率
  *Para:hCom 通讯句柄
  *Para:power 天线1到天线n功率
  *Para:powerLen 天线功率内容长度
 */
	int MsgPowerQuery(HANDLE hCom,unsigned char *power,unsigned char *powerLen);
/**
     *Fun:MsgPowerSet设置功率
     *Para:hCom 通讯句柄
     *Para:power 天线1到天线n功率
     *Para:powerLen 天线功率内容长度
 */
	int MsgPowerSet(HANDLE hCom,unsigned char *power,unsigned char powerLen);
/**
     *Fun:MsgAirProtocolQuery查询空口协议
     *Para:hCom 通讯句柄
     *Para:AirProtocol 00 表示支持 6C 协议标签 01 表示支持 6B 协议标签 02 表示支持 6B+6C 协议标签
 */
	int MsgAirProtocolQuery(HANDLE hCom,unsigned char *AirProtocol);
/**
     *Fun:MsgAirProtocolQuery设置空口协议
     *Para:hCom 通讯句柄
     *Para:AirProtocol 00 表示支持 6C 协议标签 01 表示支持 6B 协议标签 02 表示支持 6B+6C 协议标签
 */
	int MsgAirProtocolSet(HANDLE hCom,unsigned char AirProtocol);
/**
     *Fun:MsgFrequencyQuery查询工作频率 不包含自动字段
     *Para:hCom 通讯句柄
     *Para:ucAuto 频率自动设置 0读写器不自动选择频点，只能根据指定的频率列表使用频点 1 读写器在RF频段内自动选择频点
     *Para:frequency 频率数据内容
     *Para:len 频率数据内容长度
 */
	int MsgFrequencyQuery(HANDLE hCom,unsigned char *ucAuto,unsigned char *frequency,unsigned char *len);
/**
     *Fun:MsgFrequencySet设置工作频率 包含自动
     *Para:hCom 通讯句柄
     *Para:ucAuto 频率自动设置 0读写器不自动选择频点，只能根据指定的频率列表使用频点 1 读写器在RF频段内自动选择频点
     *Para:frequency 频率数据内容
     *Para:len 频率数据内容长度
 */
	int MsgFrequencySet(HANDLE hCom,unsigned char ucAuto,unsigned char *frequency,unsigned char len);
/**
     *Fun:MsgResetToFactory恢复出厂设置
     *Para:hCom 通讯句柄
 */
	int MsgResetToFactory(HANDLE hCom);
/**
     *Fun:MsgIdleTimeQuery查询空闲等待时间
     *Para:hCom 通讯句柄
     *Para:time 空闲等待时间
 */
	int MsgIdleTimeQuery(HANDLE hCom,unsigned short *time);
/**
     *Fun:MsgIdleTimeSet设置空闲等待时间
     *Para:hCom 通讯句柄
     *Para:time 空闲等待时间
 */
	int MsgIdleTimeSet(HANDLE hCom,unsigned short time);
/**
     *Fun:MsgIpAddressQuery查询ip地址 12字节
     *Para:hCom 通讯句柄
     *Para:ip IP数据（12字节） 读写器 IP：4 字节 读写器子网掩码：4 字节，读写器默认网关：4 字节
 */
	int MsgIpAddressQuery(HANDLE hCom,unsigned char *ip);
/**
     *Fun:MsgIpAddressSet设置ip地址 12字节
     *Para:hCom 通讯句柄
     *Para:ip IP数据（12字节）
 */
	int MsgIpAddressSet(HANDLE hCom,unsigned char *ip);
/**
     *Fun:MsgTcpModeQuery查询TCP模式 3字节
     *Para:hCom 通讯句柄
     *Para:mode IP数据（3字节）TCP 连接模式： 1 字节0x00 代表服务器模式 0x01 代表客户端模式 服务器模式下 TCP 端口号： 2 字节（1024~65535），默认为 9090
 */
	int MsgTcpModeQuery(HANDLE hCom,unsigned char *mode);
/**
     *Fun:MsgTcpModeSet设置TCP模式 3字节
     *Para:hCom 通讯句柄
     *Para:mode IP数据（3字节）TCP 连接模式： 1 字节0x00 代表服务器模式 0x01 代表客户端模式 服务器模式下 TCP 端口号： 2 字节（1024~65535），默认为 9090
 */
	int MsgTcpModeSet(HANDLE hCom,unsigned char *mode);
/**
     *Fun:MsgMacQuery查询mac地址 6字节
     *Para:hCom 通讯句柄
     *Para:mac mac地址 6字节
 */
	int MsgMacQuery(HANDLE hCom,unsigned char *mac);
/**
     *Fun:MsgMacSet设置mac地址 6字节
     *Para:hCom 通讯句柄
     *Para:mac mac地址 6字节
 */
int MsgMacSet(HANDLE hCom, unsigned char *mac);

//查询 RS-232 或 RS-485 通信接口(确认)
int MsgRs232And485ConnectQuery(HANDLE hCom,unsigned char *type);
//设置 RS-232 或 RS-485 通信接口(确认)
int MsgRs232And485ConnectSet(HANDLE hCom,unsigned char type);
/**
 *Fun:MsgRs232BaudRateQuery查询RS232波特率
 *Para:hCom 通讯句柄
 *Para:baudRate 波特率 00表示 9600 bps 01表示 19200 bps 02表示 38400 bps03表示115200 bps 04表示230400 bps
 */
	int MsgRs232BaudRateQuery(HANDLE hCom,unsigned char *baudRate);
/**
     *Fun:MsgRs232BaudRateSet设置RS232波特率
     *Para:hCom 通讯句柄
     *Para:baudRate 波特率 00表示 9600 bps 01表示 19200 bps 02表示 38400 bps03表示115200 bps 04表示230400 bps
 */
	int MsgRs232BaudRateSet(HANDLE hCom,unsigned char baudRate);
/**
     *Fun:MsgRs485BaudRateQuery查询RS485波特率
     *Para:hCom 通讯句柄
     *Para:baudRate 波特率 00表示 9600 bps 01表示 19200 bps 02表示 38400 bps03表示115200 bps 04表示230400 bps
 */
	int MsgRs485BaudRateQuery(HANDLE hCom,unsigned char *baudRate);
/**
     *Fun:MsgRs485BaudRateQuery设置RS485波特率
     *Para:hCom 通讯句柄
     *Para:baudRate 波特率 00表示 9600 bps 01表示 19200 bps 02表示 38400 bps03表示115200 bps 04表示230400 bps
 */
	int MsgRs485BaudRateSet(HANDLE hCom,unsigned char baudRate);
/**
     *Fun:MsgRs485AddressQuery 查询本机 RS - 485 地址
     *Para:hCom 通讯句柄
     *Para:address RS-485地址 值范围（1--255）
 */
	int MsgRs485AddressQuery(HANDLE hCom,unsigned char *address);
/**
     *Fun:MsgRs485AddressSet 设置本机 RS - 485 地址
     *Para:hCom 通讯句柄
     *Para:address RS-485地址 值范围（1--255）
 */
	int MsgRs485AddressSet(HANDLE hCom,unsigned char address);
/**
     *Fun:MsgUtcQuery 查询utc时间 8字节
     *Para:hCom 通讯句柄
     *Para:utc utc时间（8字节） 其中前四字节表示时间的秒部分，后四字节表示时间的微秒部分。
 */
	int MsgUtcQuery(HANDLE hCom,unsigned char *utc);
/**
     *Fun:MsgUtcSet 设置utc时间 秒，微妙，
     *Para:hCom 通讯句柄
     *Para:utc utc时间（8字节） 其中前四字节表示时间的秒部分，后四字节表示时间的微秒部分。
 */
	int MsgUtcSet(HANDLE hCom,unsigned char *utc);
/**
     *Fun:MsgCpuVersionQuery 查询应用处理器软件及硬件版本 16字节，
     *Para:hCom 通讯句柄
     *Para:version 应用处理器软件及硬件版本（16字节）  产品型号（1-6 字节 如：YRM801）硬件版本号（7-10 字节 如V1.2）软件版本号（11-16 字节 如V10.01）
 */
	int MsgCpuVersionQuery(HANDLE hCom,unsigned char *version);
/**
     *Fun:MsgmoduleVersionQuery 查询模块产品型号及软硬件版本 16字节，
     *Para:hCom 通讯句柄
     *Para:version 应用处理器软件及硬件版本（16字节）  产品型号（1-6 字节 如：YRM801）硬件版本号（7-10 字节 如V1.2）软件版本号（11-16 字节 如V10.01）
 */
 int MsgmoduleVersionQuery(HANDLE hCom, unsigned char *version);

/**
  *Fun:MsgWifiIpAddressQuery查询wifi ip地址 12字节 
  *Para:hCom 通讯句柄
  *Para:ip IP数据（12字节）读写器 IP：4 字节 读写器子网掩码：4 字节，读写器默认网关：4 字节
 */
	int MsgWifiIpAddressQuery(HANDLE hCom,unsigned char *ip);
/**
     *Fun:MsgWifiIpAddressSet设置wifi ip地址 12字节
     *Para:hCom 通讯句柄
     *Para:ip IP数据（12字节）读写器 IP：4 字节 读写器子网掩码：4 字节，读写器默认网关：4 字节
 */
	int MsgWifiIpAddressSet(HANDLE hCom,unsigned char *ip);
/**
     *Fun:MsgWifiTcpModeQuery 查询应用端的 WIFI 服务端/客户端模式
     *Para:hCom 通讯句柄
     *Para:mode IP数据（3字节）TCP 连接模式： 1 字节0x00 代表服务器模式 0x01 代表客户端模式 服务器模式下 TCP 端口号： 2 字节（1024~65535），默认为 9090
 */
	int MsgWifiTcpModeQuery(HANDLE hCom,unsigned char *mode);
/**
     *Fun:MsgWifiTcpModeSet 设置应用端的 WIFI 服务端/客户端模式
     *Para:hCom 通讯句柄
     *Para:mode IP数据（3字节）TCP 连接模式： 1 字节0x00 代表服务器模式 0x01 代表客户端模式 服务器模式下 TCP 端口号： 2 字节（1024~65535），默认为 9090
 */
	int MsgWifiTcpModeSet(HANDLE hCom,unsigned char *mode);
/**
     *Fun:MsgWifiMacQuery 查询Wifi mac地址 6字节
     *Para:hCom 通讯句柄
     *Para:mac wifi mac地址（6字节）
 */
	int MsgWifiMacQuery(HANDLE hCom,unsigned char *mac);
/**
     *Fun:MsgWifiMacSet 设置Wifi mac地址 6字节
     *Para:hCom 通讯句柄
     *Para:mac wifi mac地址（6字节）
 */
	int MsgWifiMacSet(HANDLE hCom, unsigned char *mac);
/**
     *Fun:MsgWifiHotspotQuery 查询wifi网卡参数
     *Para:hCom 通讯句柄
     *Para:name  热点 ESSID 
     *Para:nameLen  热点 ESSID数据长度（不可为0）
     *Para:pwd  wifi热点密码
     *Para:pwdLen  wifi热点长度  0表示无密码
 */
int MsgWifiHotspotQuery(HANDLE hCom,unsigned char *name,unsigned char *nameLen,unsigned char *pwd,unsigned char *pwdLen);
//设置wifi网卡参数
int MsgWifiHotspotSet(HANDLE hCom,unsigned char *name,unsigned char *nameLen,unsigned char *pwd,unsigned char *pwdLen);
//查询wifi网卡状态̬
/**
 *Fun:MsgWifiStateQuery 查询wifi网卡状态 1字节
 *Para:hCom 通讯句柄
 *Para:state  WIFI 网卡状态  00 表示 WIFI 未开启  01 表示 WIFI 已连接设定网络(客户端模式)/已开启 WIFI 热点（服务端模式）
 *                           02 表示 WIFI 与设定网络连接已断开 (客户端模式)/已关闭 sWIFI 热点（服务端模式）
 *                           03 表示 WIFI 未找到目标网络(客户端模式)
 *                           04 表示 WIFI 访问目标网络密钥不正确(客户端模式)
 *                           05 表示 WIFI 异常
 */
int MsgWifiStateQuery(HANDLE hCom,unsigned char *state);
/**
 *Fun:MsgWifiStateSet 设置wifi网卡状态 1字节
 *Para:hCom 通讯句柄
 *Para:state  WIFI 网卡状态  00 表示 WIFI 未开启  01 表示 WIFI 已连接设定网络(客户端模式)/已开启 WIFI 热点（服务端模式）
 *                           02 表示 WIFI 与设定网络连接已断开 (客户端模式)/已关闭 sWIFI 热点（服务端模式）
 */
int MsgWifiStateSet(HANDLE hCom,unsigned char state);

/**
 *Fun:MsgReadyUpdate 准备升级
 *Para:hCom 通讯句柄
 */
int MsgReadyUpdate(HANDLE hCom);
/**
 *Fun:MsgUpdate 升级
 *Para:hCom 通讯句柄
 *Para:pAdr 帧地址
 *Para:buf 数据内容
 *Para:bufLen 数据内容长度
 */
int MsgUpdate(HANDLE hCom, unsigned char *pAdr, unsigned char *buf, unsigned char bufLen);
/**
 *Fun:MsgTemperature 查询温度
 *Para:hCom 通讯句柄
 *Para:buf 温度  负数时，以补码形式表示【补码是指正数取反加一】；(精度为 0.5℃第一个字节表示温度的整数部分,第二个字节表示温度的小数部分.
【例如】：0x5000 表示 80℃; 0x5080 表示 80.5℃; 0x0080 表示 0.5℃;
0xE700 表示-25℃; 0xE780 表示-25.5℃; 0xFF80 表示-0.5℃;
 */
int MsgTemperature(HANDLE hCom,unsigned char *buf);
//
/**
 *Fun:MsgWifiStateSet RSSI过滤查询 
 *Para:hCom 通讯句柄
 *Para:value  RSSI 值设置为 0 时，表示不启用 RSSI 过滤功能。当 RSSI 值大于 0 时，读
到的标签 RSSI 值小于设置值的情况下，读写器将不上传该标签
 */
int MsgRSSIFilterQuery(HANDLE hCom, unsigned char &value);
/**
 *Fun:MsgWifiStateSet RSSI过滤设置
 *Para:hCom 通讯句柄
 *Para:value  RSSI 值设置为 0 时，表示不启用 RSSI 过滤功能。当 RSSI 值大于 0 时，读
到的标签 RSSI 值小于设置值的情况下，读写器将不上传该标签
 */
int MsgRSSIFilterSet(HANDLE hCom, unsigned char value);
    
#ifdef __cplusplus
}
#endif
#endif
