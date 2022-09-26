#pragma once

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

void ExeEmptyOp(const uint8_t* data) {return;}
void OpPong(const uint8_t* data);

void SerialSend(uint8_t b);