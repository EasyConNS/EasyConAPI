# EasyCon API

EasyCon的MCU的实现，只需要根据要求实现`EasyCon_API.c和EasyCon_API.h`即可



## 如何移植

嵌入式工程中添加以下4个文件

- EasyCon.c
- EasyCon.h
- EasyCon_API.c
- EasyCon_API.h



主要需要实现的说明都在`EasyCon_API.h`中，具体实现在`EasyCon_API.c`中

实现的demo在下面的仓库中

> https://github.com/EasyConNS/EasyMCU_STM32



一共是四块内容，都比较简单

#### 变量定义

主要是定义EasyCon可以使用的内存大小，同时也是能使用的E2PROM大小或者Flash大小，根据板子可用情况定义，不要卡极限，出问题不好查。

- 注意E2PROM和Flash需要的内存比 MEM_SIZE 大4字节

```c
/**********************************************************************/
// EasyCon API, you need set the MEM_SIZE that could use in your SRAM and (EEPROM or Flash)
// EEPROM or Flash need 4 bytes more than MEM_SIZE, so make sure you have enough space for it
/**********************************************************************/
// #define MEM_SIZE      924
// need EEPROM or Flash Size = MEM_SIZE + 4 
```

EasyCon的版本号在`EasyCon.c`中，不建议修改，会导致上位机不兼容

- 注意内存是512字节的，需要格外注意`MEM_SIZE`大小，很容易出现溢出的情况



HID报文的输出间隔，有的系统必须要大于等于7ms，否则会导致NS无法识别，有的则不需要

```c
// hid report interval some usb must >7 AVR jsut 2 enough
// too small will make NS cant recognize
#define ECHO_INTERVAL 2
```



#### 回调

主要是这四个：

- 负责EasyCon的运行时钟，
- 串口数据输入处理
- 判断是否需要发送Report
- Report发送后的回调处理

```c
/* the main tick for 1 ms
 * need call in a 1 ms timer
 */
extern void EasyCon_tick(void);

/* serial state machine
 * need call when get a new serial data from uart
 * no data return -1
 */
extern void EasyCon_serial_task(int16_t byte);

/* check if need send report.
 * need call in hid send task
 */
extern bool EasyCon_need_send_report(void);

/* need call when a report sent
 * 
 */
extern void EasyCon_report_send_callback(void);
```



#### 接口函数

接口函数比较多，主要是负责可持久化内容的读写、LED的控制、串口数据发送、USB包的修改

```c
/* EasyCon read 1 byte from E2Prom or flash 
 * need implement
 */
extern uint8_t EasyCon_read_byte(uint16_t addr);

/* EasyCon write n byte to E2Prom or flash 
 * need implement
 */
extern void EasyCon_write_data(uint16_t addr,uint8_t* data,uint16_t len);

/* EasyCon start write to E2Prom or flash callback
 * optional implement
 */
extern void EasyCon_write_start(uint8_t mode);

/* EasyCon write to E2Prom or flash end callback 
 * optional implement
 */
extern void EasyCon_write_end(uint8_t mode);

/* running led on
 * need implement
 */
extern void EasyCon_runningLED_on(void);

/* running led off
 * need implement
 */
extern void EasyCon_runningLED_off(void);

/* data led blink
 * need implement,no block
 */
extern void EasyCon_blink_led(void);

/* serial send 1 byte
 * need implement,block
 */
extern void EasyCon_serial_send(const char DataByte);

// about hid report

/* reset hid report to default.
 * need implement
 */
extern void reset_hid_report(void);

/* set left stick in hid report.
 * need implement
 */
extern void set_left_stick(const uint8_t LX, const uint8_t LY);

/* set right stick in hid report.
 * need implement
 */
extern void set_right_stick(const uint8_t RX, const uint8_t RY);

/* set button in hid report.
 * need implement
 */
extern void set_buttons(const uint16_t Button);

/* set button press in hid report.
 * need implement
 */
extern void press_buttons(const uint16_t Button);

/* set buttons release in hid report.
 * need implement
 */
extern void release_buttons(const uint16_t Button);

/* set HAT in hid report.
 * need implement
 */
extern void set_HAT_switch(const uint8_t HAT);
```

还有两个接口是可选实现，主要是给flash上锁、解锁以及连续写等操作判断用的，其他情况下一般用不到

```
/* EasyCon start write to E2Prom or flash callback
 * optional implement
 */
extern void EasyCon_write_start(uint8_t mode);

/* EasyCon write to E2Prom or flash end callback 
 * optional implement
 */
extern void EasyCon_write_end(uint8_t mode);
```



#### 控制功能

用来初始化、启动、停止EasyCon等

```c
extern void EasyCon_script_init(void);
extern void EasyCon_script_task(void);
extern void EasyCon_script_auto_start(void);
extern bool EasyCon_is_script_running(void);
extern void EasyCon_script_start(void);
extern void EasyCon_script_stop(void);
```



#### 头文件

主要是可能用到的头文件，根据不同板子，框架不同而不同，按需加入即可

```c
/**********************************************************************/
// some incude files for var type, you need change to your device framework files
/**********************************************************************/

```



#### HID

由于不同USB框架实现可能不尽相同，推荐使用目录下的HID文件或者模仿类似的结构

- HID.h
- HID.c

HID中主要是需要实现当echo_ms耗尽时，可以发出一份报文（这个机制主要是用来保证不丢键的）

```c
void Report_Task(void)
{
	if(EasyCon_need_send_report())
	{
		usbd_send((uint8_t *)&next_report, TAG_SEND_BUF);
		EasyCon_report_send_callback();
	}
}
```



## 简单运行

初始化以后，直接运行即可，默认启动

```c
int main(void)
{
    SystemInit();
    CommonInit();
    PCIInit();
    // Initialize script.
    EasyCon_script_init();
    // The USB stack should be initialized last.
    HIDInit();
    // Once that's done, we'll enter an infinite loop.
    while (1)
    {
        // Process local script instructions.
        EasyCon_script_task();
        HIDTask();
    }
}
```



## 检测移植

移植完成以后，可以使用光速过帧的脚本检测EasyCon是否达到最低性能要求：

- 光速过帧联机模式下可以不跑飞，持续至少1分钟
- 光速过帧烧录模式下可以不跑飞，持续至少1分钟
- 光速过帧固件模式下可以不跑飞，持续至少1分钟



以下脚本有几个执行条件，否则可能会出现脚本跑偏的情况：

- switch不过热，并且电量较为充足
- EasyCon上位机没有开启调试模式



```
# <<光速过帧>>
# ver 1.4 (稳定版),可以自定义延迟,支持联机
# 实测每小时约6200帧，必须使用v4或更高版本固件
# 联机速度稍慢5700左右，而且会受cpu100%影响
# 使用方法：
# 1.进系统菜单，关掉时间同步
# 2.打开日期修改，选择一个31天的月份，日期设为1号
# 3.确保再打开时光标在OK按钮上，光标移到最右边后退出
# 4.烧录脚本后运行或者固件模式也行

# 这里帧数你是实际要过的帧数,要过多少填多少
_帧数 = 10000

# 这里是延迟，如果默认30的情况下依然跑飞，建议每次增加10%左右，也就是增加3，然后尝试
_延迟 = 30
# 如果联机将下面的90改为120
$4 = 120

$3 = _延迟
$4 += _延迟
# 检查连接
FOR 10
    RCLICK
    300
NEXT
# 过帧
$1 = 1
$5 = 0
FOR _帧数
    $2 = 1
    $1 += 1
    IF $1 = 31
        $1 = 1
        $2 = 2
    ENDIF
    $5 += 1
    FOR $2
        A 50
        WAIT $4
        LS LEFT
        WAIT $3
        RS LEFT
        WAIT $3
        LEFT DOWN
        WAIT $3
        LS UP
        WAIT $3
        RIGHT DOWN
        WAIT $3
        LS RIGHT
        WAIT $3
        RS RIGHT
        WAIT $3
        LEFT UP
        RIGHT UP
        A 50
        WAIT $4
    NEXT
    PRINT 过了 & $5 & 帧
NEXT
```

以上测试完成基本就算移植成功了



## 可能有问题的点

1. 整个程序运行理想情况是不会被阻塞的，注意不要在API中使用阻塞时间过长的操作，会影响脚本运行的正确性
2. flash操作比较容易出问题，地址需要对齐，写入数据可能也需要成对拼接，操作的时候最好阅读一下源码再进行，否则很容易HardFault