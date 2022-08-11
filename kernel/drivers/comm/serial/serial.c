#include <kernel/drivers/comm/serial/serial.h>

#include <kernel/arch/io.h>

u8 serial_initialized = 0;

int
serial_init()
{
  serial_initialized = 0;
  outb(PORT + 1, 0x00); // Disable interrupts
  outb(PORT + 3, 0x80); // Set baud rate divisor
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
  outb(PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial
                        // returns same byte)

  // Check if serial is faulty
  if (inb(PORT + 0) != 0xAE) {
    return -1;
  }

  // Set in normal operation mode
  outb(PORT + 4, 0x0F);
  serial_initialized = 1;

  return 0;
}

int
is_transmit_empty()
{
  return inb(PORT + 5) & 0x20;
}

void
serial_putch(char ch)
{
  if (!serial_initialized)
    return;

  while (is_transmit_empty() == 0)
    ;
  outb(PORT, (u8)ch);
}

void
serial_putstr(char *str)
{
  if (!serial_initialized)
    return;

  while (*str != '\0') {
    serial_putch(*str);
    str++;
  }
}
