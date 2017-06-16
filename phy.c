/***********************************************************************************************************************
 *
 * ID100 Utility
 * Physical Layer
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
#include "phy.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/file.h>
#include "utils.h"

static int port = -1;

/***********************************************************************************************************************
 * Open serial port
 **********************************************************************************************************************/
void PhyOpen(char *devName)
{
  static const speed_t portSpeed = B38400;
  struct termios tty;

  port = open(devName, O_RDWR | O_NOCTTY | O_SYNC);
  if(port < 0) {
    ExitWithError("Could not open device: %s", devName);
  }

  if(flock(port, LOCK_EX) != 0) {
    ExitWithError("Could not lock device: %s", devName);
  }

  if(tcgetattr(port, &tty) < 0) {
    ExitWithError("Could not get attributes");
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
    ExitWithError("Could not set attributes");
  }
}

/***********************************************************************************************************************
 * Close serial port
 **********************************************************************************************************************/
void PhyClose(void)
{
  if(flock(port, LOCK_UN) != 0) {
    ExitWithError("Could not unlock device");
  }

  if(close(port) != 0) {
    ExitWithError("Could not close device");
  }
}

/***********************************************************************************************************************
 * Send one byte to serial port
 **********************************************************************************************************************/
void PhySendByte(uint8_t byte)
{
  if(write(port, &byte, 1) != 1) {
    ExitWithError("Could not send byte");
  }
}

/***********************************************************************************************************************
 * Receive one byte from serial port
 **********************************************************************************************************************/
uint8_t PhyReceiveByte(void)
{
  uint8_t byte;

  if(read(port, &byte, 1) != 1) {
    ExitWithError("Could not receive byte");
  }

  return byte;
}
