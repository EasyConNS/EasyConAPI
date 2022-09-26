#include "Command.h"
#include "Common.h"
#include "EasyCon_API.h"
/*

void ExeEmptyOp(const uint8_t* data) {return;}
void OpPong(const uint8_t* data);
void TriggerLED(const uint8_t* data);

*/

uint8_t* data_ptr;

const PROGMEM CommandEntryType CommandTable[] = {
  {
    .OP = CMD_READY,
    .CmdDataLength = 0,
    .ExeFunc = ExeEmptyOp
  },
  {
    .OP = CMD_READY,
    .CmdDataLength = 0,
    .ExeFunc = OpVersion
  },
  {
    .OP = CMD_HELLO,
    .CmdDataLength = 0,
    .ExeFunc = OpPong
  },
  {
    .OP = CMD_LED,
    .CmdDataLength = 0,
    .ExeFunc = TriggerLED
  }
};

void OpPong(const uint8_t* data) {
  SerialSend(REPLY_HELLO);
}

void TriggerLED(const uint8_t* data) {
  _ledflag ^= 0x8;
  // there is no end , for flash one byte force
  EasyCon_write_start(1);
  EasyCon_write_data(LED_SETTING, (uint8_t *)&_ledflag, 1);
  EasyCon_write_end(1);
  EasyCon_runningLED_off();
  EasyCon_serial_send(_ledflag);
}

static void CallCommandFunc(const CommandEntryType* CommandEntry) {
  CmdActionFuncType ExeFunc = pgm_read_ptr(&CommandEntry->ExeFunc);
  ExeFunc(data_ptr);
}

static void cmd_dispatch(uint8_t OPCODE, const uint8_t* data, uint8_t len) {
  uint8_t i;
  bool CommandFound = false;
  for (i = 0; i < ARRAY_COUNT(CommandTable); i++) {
    if (OPCODE == pgm_read_byte(&CommandTable[i].OP)) {
      CommandFound = true;
      data_ptr = data;
      if(len == pgm_read_byte(&CommandTable[i].CmdDataLength)) {
        CallCommandFunc(&CommandTable[i]);
        BlinkLED();
      }else {
        SerialSend(RLY_ERR);
      }
      break;
    }
  }
  if (! CommandFound) {
    SerialSend(RLY_ERR);
    return;
  }
  data_ptr = NULL;
}

static void cmd_senderr(CommandError_t errType, uint8_t arg1, uint8_t arg2)
{
  SerialSend(REPLY_ERROR);
}

void SerialSend(uint8_t b)
{
    Serial_SendByte(b);
    BlinkLED();
}