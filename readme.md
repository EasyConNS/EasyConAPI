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

> https://github.com/EasyConNS/EasyMCU



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
// EEPROM or Flash Size = MEM_SIZE + 4 
```

EasyCon的版本号在`EasyCon.c`中，不建议修改，会导致上位机不兼容



#### 回调

主要是这三个，一个是负责EasyCon的运行时钟，一个是串口数据处理，还有一个是每条命令的发包数量回调

```c
/* the main tick for 1 ms
 * need call in a 1 ms timer
 */
extern void EasyCon_tick(void);

/* serial state machine
 * need call when get a new serial date from uart
 * no data return -1
 */
extern void EasyCon_serial_task(int16_t byte);

/* decrement
 * need call when get a report sent
 */
extern void EasyCon_decrease_report_echo(void);
```



#### 接口函数

接口函数比较多，主要是负责可持久化内容的读写、LED的控制、串口数据发送、USB包的修改

```c
/* EasyCon read 1 byte from E2Prom or flash 
 * need implement
 */
extern uint8_t EasyCon_read_byte(uint8_t* addr);

/* EasyCon write 1 byte to E2Prom or flash 
 * need implement
 */
extern void EasyCon_write_byte(uint8_t* addr,uint8_t value);

/* EasyCon read 2 byte from E2Prom or flash 
 * need implement
 */
extern uint16_t EasyCon_read_2byte(uint16_t* addr);

/* EasyCon write 2 byte to E2Prom or flash 
 * need implement
 */
extern void EasyCon_write_2byte(uint16_t* addr,uint16_t value);

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

/* clean echo times in HID
 * need implement
 */
extern void zero_echo(void);

/* reset hid report to default.
 * need implement
 */
extern void reset_hid_report(void);

/* set left stick in hid report.
 * need implement
 */
extern void SetLeftStick(const uint8_t LX, const uint8_t LY);

/* set right stick in hid report.
 * need implement
 */
extern void SetRightStick(const uint8_t RX, const uint8_t RY);

/* set button in hid report.
 * need implement
 */
extern void SetButtons(const uint16_t Button);

/* set button press in hid report.
 * need implement
 */
extern void PressButtons(const uint16_t Button);

/* set buttons release in hid report.
 * need implement
 */
extern void ReleaseButtons(const uint16_t Button);

/* set HAT in hid report.
 * need implement
 */
extern void SetHATSwitch(const uint8_t HAT);
```

还有两个接口是可选实现，主要是给flash上锁解锁使用的，其他情况下一般用不到

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



## 可能有问题的点

1. srand可能在单片机端实现有问题，用个伪随机表替代就行了
2. EasyCon中可能有一些宏是math或者其他专属头文件的，现在没分离出来，后续考虑
3. 整个程序运行理想情况是不会被阻塞的，注意不要在API中使用阻塞时间过长的操作，会影响脚本运行的正确性