#include "HID.h"
#include "LED.h"
#include "Command.h"

const PROGMEM CommandEntryType CommandTable[] = {
  {
    .OP = 0x00,
    .CmdDataLength = 1,
    .ExeFunc = ExeResetButton
  },
  {
    .OP = 0x01,
    .CmdDataLength = 2,
    .ExeFunc = ExePressButton
  },
  {
    .OP = 0x02,
    .CmdDataLength = 2,
    .ExeFunc = ExeReleaseButton
  },
  {
    .OP = 0x03,
    .CmdDataLength = 1,
    .ExeFunc = ExeSetHAT
  },
  {
    .OP = 0x04,
    .CmdDataLength = 2,
    .ExeFunc = ExeSetLeftStick
  },
  {
    .OP = 0x05,
    .CmdDataLength = 2,
    .ExeFunc = ExeSetRightStick
  },
  {
    .OP = 0x06,
    .CmdDataLength = 7,
    .ExeFunc = ExeSetButtons
  }
};

#define OPCODE opbuf

uint8_t opbuf = 0;
uint8_t serialbuf[BUF_SIZE] = {0};
size_t buffer_length = 0;
CommandAction_t cmd_state = IDLE;

/* TODO:
jc action
script action
flash
*/
void ExeResetButton(const uint8_t* data) {
  if(data[0] == 0)
  {
    ResetReport();
  }
}
void ExePressButton(const uint8_t* data) {
  PressButtons(*(uint16_t *)data);
}
void ExeReleaseButton(const uint8_t* data) {
  ReleaseButtons(*(uint16_t *)data);
}
void ExeSetHAT(const uint8_t* data) {
  SetHATSwitch(data[0]);
}
void ExeSetLeftStick(const uint8_t* data) {
  SetLeftStick(data[0], data[1]);
}
void ExeSetRightStick(const uint8_t* data) {
  SetRightStick(data[0], data[1]);
}

void ExeSetButtons(const uint8_t* data) {
  SetReport((USB_JoystickReport_Input_t*)data);
}

void CommandTick(void) {
  int16_t byte =  Serial_ReceiveByte();
  if (byte < 0) return;
  // SerialRingAdd(&sri,(uint8_t)byte);

  CommandTask(byte);
}

static void OPTask() {
  switch(OPCODE)
  {
    case OP_IDL:
      // do nothing
    break;
    case OP_VER:
      SerialSend(MCU_VERSION);
    break;
    case OP_LED:
      // TODO
    break;
    case OP_PNG:
      SerialSend(RLY_PNG);
    break;
    case OP_SCRIPTSTART:
    break;
    case OP_SCRIPTSTOP:
    break;
    case CMD_UNPAIR:
    break;
    case CMD_CHGMODE:
    break;
    case CMD_CHGCOLOR:
    break;
    case CMD_SAV_AMB:
    break;
    case OP_ACT:
      cmd_state = BUTTON;
      buffer_length = 0;
    break;
    case OP_FLH:
      cmd_state = FLASH;
      buffer_length = 0;
    break;
    default:
      cmd_state = IDLE;
      SerialSend(RLY_ERR);
    break;
  }
}

static void CallCommandFunc(const CommandEntryType* CommandEntry) {
  CmdActionFuncType ExeFunc = pgm_read_ptr(&CommandEntry->ExeFunc);
  ExeFunc(serialbuf);
}

static void ButtonTask(void) {
  uint8_t i;
  bool CommandFound = false;
  uint8_t bt = OPCODE & 0x0F;
  for (i = 0; i < ARRAY_COUNT(CommandTable); i++) {
    if (bt == pgm_read_byte(&CommandTable[i].OP)) {
      CommandFound = true;

      if(buffer_length == pgm_read_byte(&CommandTable[i].CmdDataLength)) {
        CallCommandFunc(&CommandTable[i]);
        cmd_state = IDLE;
        BlinkLED();
      }else {
        SerialSend(0xCC);
      }

      break;
    }
  }
  if (! CommandFound) {
    cmd_state = IDLE;
    SerialSend(RLY_ERR);
    return;
  }
}

static void FlashTask(uint8_t byte) {
  // eeprom += byte
  cmd_state = IDLE;
  SerialSend(RLY_FLH_END);
}

void EasyCon_serial_task(uint8_t byte)
{
  if (byte < 0) return;
  EasyCon_blink_led();
  switch(cmd_state)
  {
    case IDLE:
      memset(serialbuf, 0, BUF_SIZE);
      OPCODE = byte;
      OPTask();
    break;
    case BUTTON:
      serialbuf[buffer_length++] = byte;
      ButtonTask();
    break;
    case FLASH:
      serialbuf[buffer_length++] = byte;
      if(buffer_length == 4)
      {
        cmd_state = FLASH_START;
      }
    break;
    case FLASH_START:
      FlashTask(byte);
    break;
    default:
    break;
  }

  if (buffer_length >= BUF_SIZE)
  {
    buffer_length = 0;
  }
}

void SerialSend(uint8_t b)
{
    Serial_SendByte(b);
    BlinkLED();
}