#pragma once

typedef enum
{
    OP_READY,
    DATA_LEN_RECV,
    DATA_RECV,
    DATA_END,
}CommandAction_t;

/*
_isr(test) {
  int16_t byte =  Serial_ReceiveByte();
  if (byte < 0) return;
  CommandTask(byte);
}*/.
void CommandTask(uint8_t byte);