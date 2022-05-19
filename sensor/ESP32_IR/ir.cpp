// IR Receiver
#include <IRremote.h>
#include <IRremoteInt.h>

#include "ir.hpp"

const int ir_receive_pin = 25;

void (*valid_signal_callback) (unsigned int signal);
void (*invalid_signal_callback) (unsigned int signal);


void ir_init(void (*valid_cb) (unsigned int signal), void (*invalid_cb) (unsigned int signal)) {
    IrReceiver.begin(ir_receive_pin);
    valid_signal_callback = valid_cb;
    invalid_signal_callback = invalid_cb;
}


unsigned int last_id = 0x00000000;
const unsigned int signal_id_mask = 0x000000FF;

int isValidSignal(unsigned int signal) {
  if (!(signal & 0xFFF00000)) return 1; // short signal with no id
  unsigned int id = signal & signal_id_mask;
  int result = 0;
  if (id == last_id) result = 1; 
  last_id = id;
  return result;
}

void ir_scan() {
    if (IrReceiver.decode()) {
    unsigned int ir_signal = IrReceiver.decodedIRData.decodedRawData;
    if (ir_signal < 0x100 && ir_signal > 0.) ir_signal += 0x10000;

    if (ir_signal != 0) {
      if (!isValidSignal(ir_signal)) invalid_signal_callback(ir_signal);
      else valid_signal_callback(ir_signal);
    }
    IrReceiver.resume();
  }
}