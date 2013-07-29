
#include <avr/io.h>
#include <avr/interrupt.h>

#define LONGPRESS_MSEC 3000
#define TIMER_MSEC     32

volatile unsigned char g_press_counter_;

ISR(TIMER0_OVF_vect) {
  TCNT0 = 0;

  unsigned int msec = g_press_counter_ * TIMER_MSEC;
  if (bit_is_clear(PIND, PD4)) {
    if ((g_press_counter_ * TIMER_MSEC) < (LONGPRESS_MSEC + TIMER_MSEC)) {
      g_press_counter_++;
    }

    if (msec >= LONGPRESS_MSEC && msec < (LONGPRESS_MSEC + TIMER_MSEC)) {
      if (bit_is_clear(PIND, PD3)) {
        PORTD |= _BV(PD3);
      }
      else {
        PORTD &= ~_BV(PD3);
      }
    }
  }
  else {
    if (msec > 0 && msec < LONGPRESS_MSEC) {
      if (bit_is_clear(PIND, PD5)) {
        PORTD |= _BV(PD5);
      }
      else {
        PORTD &= ~_BV(PD5);
      }
    }

    g_press_counter_ = 0;
  }
}

void set_timer(void) {
  g_press_counter_ = 0;
  TCNT0 = 0;
  TIMSK = 0b00000010;
}

int main(void) {
  TCCR0B = 0b00000101;
  DDRD = 0b00101000;
  PORTD = 0b00010000;
  set_timer();
  sei();

  for (;;);

  return 0;
}
