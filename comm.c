/***********************************************************************************************************************
 *
 * ID100 Utility
 *
 * (C) 2017 Gergely Budai
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
 **********************************************************************************************************************/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include "crc16.h"
#include "comm.h"

#define dprintf(...) printf(__VA_ARGS__); fflush(stdout)
//#define dprintf(fmt,...)

static const uint8_t STX = 0x02;
static const uint8_t ENQ = 0x10;

static int port = -1;

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void CommOpen(char *devName)
{
  static const speed_t portSpeed = B38400;
  struct termios tty;

  port = open(devName, O_RDWR | O_NOCTTY | O_SYNC);
  if(port < 0) {
    perror("open()");
    exit(EXIT_FAILURE);
  }

  if (tcgetattr(port, &tty) < 0) {
    perror("tcgetattr()");
    exit(EXIT_FAILURE);
  }

  cfsetospeed(&tty, portSpeed);
  cfsetispeed(&tty, portSpeed);

  tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;         /* 8-bit characters */
  tty.c_cflag &= ~PARENB;     /* no parity bit */
  tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
  tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

  /* setup for non-canonical mode */
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tty.c_oflag &= ~OPOST;

  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = 10;

  if (tcsetattr(port, TCSANOW, &tty) != 0) {
    perror("tcsetattr()");
    exit(EXIT_FAILURE);
  }
}

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void CommClose(void)
{
  if(close(port) != 0) {
    perror("close()");
    exit(EXIT_FAILURE);
  }
}

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
static Crc16Type CommSendByte(uint8_t byte, Crc16Type crc)
{
  if(write(port, &byte, 1) != 1) {
    perror("write()");
    exit(EXIT_FAILURE);
  }

  dprintf("%02X ", byte);

  return(Crc16UpdateByte(crc, byte));
}

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
static Crc16Type CommEncodeByte(uint8_t byte, Crc16Type crc)
{
  if((byte == STX) || (byte == ENQ)) {
    crc = CommSendByte(ENQ, crc);
    byte += 0x80;
  }

  crc = CommSendByte(byte, crc);

  return crc;
}

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
void CommSendBuffer(const void *buffer, const uint16_t length)
{
  Crc16Type crc = 0xFFFF;
  uint16_t i;

  dprintf("TX: ");
  // Send STX (Not encoded)
  crc = CommSendByte(STX, crc);
  // Send length
  crc = CommEncodeByte(length >> 8, crc);
  crc = CommEncodeByte(length, crc);
  // Send Buffer as bytes
  for(i = 0; i < length; i++) {
    crc = CommEncodeByte(((uint8_t *)buffer)[i], crc);
  }
  // Send CRC
  CommEncodeByte(crc >> 8, 0);
  CommEncodeByte(crc, 0);
  dprintf("\n");
}

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
static Crc16Type CommReceiveByte(uint8_t *byte, Crc16Type crc)
{
  if(read(port, byte, 1) != 1) {
    perror("read()");
    exit(EXIT_FAILURE);
  }

  dprintf("%02X ", *byte);

  return(Crc16UpdateByte(crc, *byte));
}

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
static Crc16Type CommDecodeByte(uint8_t *byte, Crc16Type crc)
{
  crc = CommReceiveByte(byte, crc);

  if(*byte == ENQ) {
    crc = CommReceiveByte(byte, crc);
    *byte -= 0x80;
  }

  return crc;
}

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
uint16_t CommReceiveBuffer(void *buffer)
{
  Crc16Type crc = 0xFFFF, crcIn;
  uint8_t byte;
  uint16_t i, length;

  dprintf("RX: ");

  // Receive STX (Not encoded)
  crc = CommReceiveByte(&byte, crc);
  if(byte != STX) {
    perror("STX");
    exit(EXIT_FAILURE);
  }

  // Receive length
  crc = CommDecodeByte(&byte, crc);
  length = (uint16_t)byte << 8;
  crc = CommDecodeByte(&byte, crc);
  length |= (uint16_t)byte;
  if(length > COMM_MAX_PARAM_LENGTH) {
    perror("Parameter too big");
    exit(EXIT_FAILURE);
  }

  // Receive Buffer as bytes
  for(i = 0; i < length; i++) {
    crc = CommDecodeByte(&(((uint8_t *)buffer)[i]), crc);
  }

  // Receive CRC
  CommDecodeByte(&byte, 0);
  crcIn = (Crc16Type)byte << 8;
  CommDecodeByte(&byte, 0);
  crcIn |= (Crc16Type)byte;
  dprintf("\n");

  // Check CRC
  if(crc != crcIn) {
    perror("CRC Error");
    exit(EXIT_FAILURE);
  }

  return length;
}
