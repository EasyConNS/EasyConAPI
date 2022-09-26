#pragma once

#include <stdbool.h>

#define MCU_VERSION 0x47

// serial protocal control bytes and replies
#define CMD_READY 0xA5
#define CMD_DEBUG 0x80
#define CMD_HELLO 0x81
#define CMD_FLASH 0x82
#define CMD_SCRIPTSTART 0x83
#define CMD_SCRIPTSTOP 0x84
#define CMD_VERSION 0x85
#define CMD_LED 0x86
#define CMD_UNPAIR 0x87
#define CMD_CHGMODE 0x88
#define CMD_CHGCOLOR 0x89
#define CMD_SAV_AMB 0x90

#define REPLY_ERROR 0x00
#define REPLY_ACK 0xFF
#define REPLY_BUSY 0xFE
#define REPLY_HELLO 0x80
#define REPLY_FLASHSTART 0x81
#define REPLY_FLASHEND 0x82
#define REPLY_SCRIPTACK 0x83

#define cmd_senderrE(op)    cmd_senderr(op, 0,0)

typedef enum
{
    ERR_OP,
    ERR_LEN,
    ERR_CRC,
    ERR_UKN,
}CommandError_t;

typedef void (*CmdActionFuncType) (const uint8_t* data);

typedef struct {
    uint8_t OP;
    uint8_t CmdDataLength;
    CmdActionFuncType ExeFunc;
}CommandEntryType;

bool cmd_checkop(uint8_t opcode) {return true;}
void cmd_dispatch(uint8_t opcode, const uint8_t* data, uint8_t len);
void cmd_senderr(CommandError_t errType, uint8_t arg1, uint8_t arg2);

/*
_isr(test) {
  int16_t byte =  Serial_ReceiveByte();
  if (byte < 0) return;
  CommandTask(byte);
}*/
extern void EasyCon_serial_task(uint8_t byte);
