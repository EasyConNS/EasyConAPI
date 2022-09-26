#include "Packet.h"
#include "Command.h"
#include "crc8.h"

#define BUF_SIZE 32

uint8_t opcode = 0;
uint8_t serialbuf[BUF_SIZE + 1] = {0};
uint8_t buffer_current = 0;
uint8_t buffer_length = 0;
CommandAction_t cmd_state = OP_READY;

void EasyCon_serial_task(uint8_t byte)
{
  switch(cmd_state)
  {
    case OP_READY:
      opcode = byte;
      if(!cmd_checkop(opcode)) {
        cmd_senderrE(ERR_OP);
        goto error;
      }
      cmd_state = DATA_LEN_RECV;
      memset(serialbuf, 0, BUF_SIZE);
    break;
    case DATA_LEN_RECV:
      if(byte >= BUF_SIZE) {
        cmd_senderrE(ERR_LEN);
        goto error;
      }
      buffer_length = byte;
      // if data length is 0 then skipp DATA_RECV
      if(buffer_length == 0) {
        cmd_state = DATA_END;
      } else {
        cmd_state = DATA_RECV;
        buffer_current = 0;
      }
    break;
    case DATA_RECV:
      serialbuf[buffer_current++] = byte;
      if(buffer_current == buffer_length) {
        cmd_state = DATA_END;
        return;
      }
    break;
    case DATA_END:
      // crc check
      uint8_t crcdata = byte;
      int8_t crcsum = 0;
      // default crc should be 0 if buffer_length is 0
      if(buffer_length != 0) {
        crcsum = crc8(serialbuf, buffer_length);
      }
      if(crcsum != crcdata) {
        cmd_senderr(ERR_CRC, crcdata, crcsum);
        goto error;
      } else {
        // all good, dispatch op commands
        cmd_dispatch(opcode, serialbuf, buffer_length);
        cmd_state = OP_READY;
        return;
      }
    break;
    default:
      // unknown state error
      cmd_senderrE(ERR_UKN);
error:
      cmd_state = OP_READY;
    break;
  }
  // loop
}
