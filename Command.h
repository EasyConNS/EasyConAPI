#pragma once

// CA1E 0.1 beta
#define MCU_VERSION 0x47

#define BUF_SIZE 16

#define OP_IDL 0xA5
#define OP_ACT 0x80
#define OP_PNG 0x81
#define OP_FLH 0x82
#define OP_SCRIPTSTART 0x83
#define OP_SCRIPTSTOP 0x84
#define OP_VER 0x85
#define OP_LED 0x86
#define CMD_UNPAIR 0x87
#define CMD_CHGMODE 0x88
#define CMD_CHGCOLOR 0x89
#define CMD_SAV_AMB 0x90

#define RLY_ERR 0x00
#define RLY_ACK 0xFF
#define RLY_BSY 0xFE
#define RLY_PNG 0x80
#define RLY_FLH 0x81
#define RLY_FLH_END 0x82
#define RLY_SCRIPTACK 0x83

typedef void (*CmdActionFuncType) (const uint8_t* data);

typedef enum
{
    IDLE,
    BUTTON,
    FLASH,
    FLASH_START,
}CommandAction_t;

typedef struct {
    uint8_t OP;
    uint8_t CmdDataLength;
    CmdActionFuncType ExeFunc;
}CommandEntryType;

#define ARRAY_COUNT(x)  (sizeof(x) / sizeof(x[0]))