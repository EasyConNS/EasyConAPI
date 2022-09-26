#include "Command.h"
#include "HID.h"
// #include "LED.h"
#include "Common.h"

#include <avr/pgmspace.h>

uint8_t* data_ptr;

const PROGMEM CommandEntryType CommandTable[] = {
  {
    .OP = OP_IDL,
    .CmdDataLength = 0,
    .ExeFunc = ExeEmptyOp
  },
  {
    .OP = OP_PNG,
    .CmdDataLength = 0,
    .ExeFunc = OpPong
  },
  {
    .OP = OP_ACT,
    .CmdDataLength = 7,
    .ExeFunc = ExeReleaseButton
  }
};

void OpPong(const uint8_t* data) {
  SerialSend(RLY_PNG);
}

static void CallCommandFunc(const CommandEntryType* CommandEntry) {
  CmdActionFuncType ExeFunc = pgm_read_ptr(&CommandEntry->ExeFunc);
  ExeFunc(serialbuf);
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
  SerialSend(RLY_ERR);
}

void SerialSend(uint8_t b)
{
    Serial_SendByte(b);
    BlinkLED();
}