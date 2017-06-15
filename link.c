/***********************************************************************************************************************
 *
 * ID100 Utility
 * Link Layer
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
#include "link.h"

#include <stdio.h>
#include <stdint.h>
#include "crc16.h"
#include "utils.h"
#include "phy.h"

#define dprintf(...) printf(__VA_ARGS__); fflush(stdout)
//#define dprintf(fmt,...)

static const uint8_t STX = 0x02;
static const uint8_t ENQ = 0x10;

/***********************************************************************************************************************
 * Connect the link and lower layers
 **********************************************************************************************************************/
void LinkConnect(void *ctx)
{
  PhyOpen(ctx);
}

/***********************************************************************************************************************
 * Disconnect the link and lower layers
 **********************************************************************************************************************/
void LinkDisconnect(void)
{
  PhyClose();
}

/***********************************************************************************************************************
 * Send one byte to the physical layer and update CRC
 **********************************************************************************************************************/
static Crc16Type LinkSendByte(uint8_t byte, Crc16Type crc)
{
  PhySendByte(byte);
  dprintf("%02X ", byte);

  return(Crc16UpdateByte(crc, byte));
}

/***********************************************************************************************************************
 * Encode and send special bytes and update CRC
 **********************************************************************************************************************/
static Crc16Type LinkEncodeByte(uint8_t byte, Crc16Type crc)
{
  if((byte == STX) || (byte == ENQ)) {
    crc = LinkSendByte(ENQ, crc);
    byte += 0x80;
  }

  crc = LinkSendByte(byte, crc);

  return crc;
}

/***********************************************************************************************************************
 * Build a frame around the given buffer and send to physical layer
 **********************************************************************************************************************/
void LinkSendBuffer(const void *buffer, const uint16_t length)
{
  Crc16Type crc = 0xFFFF;
  uint16_t i;

  dprintf("TX: ");
  // Send STX (Not encoded)
  crc = LinkSendByte(STX, crc);
  // Send length
  crc = LinkEncodeByte(length >> 8, crc);
  crc = LinkEncodeByte(length, crc);
  // Send Buffer as bytes
  for(i = 0; i < length; i++) {
    crc = LinkEncodeByte(((uint8_t *)buffer)[i], crc);
  }
  // Send CRC
  LinkEncodeByte(crc >> 8, 0);
  LinkEncodeByte(crc, 0);
  dprintf("\n");
}

/***********************************************************************************************************************
 * Receive one byte over the link layer and update CRC
 **********************************************************************************************************************/
static Crc16Type LinkReceiveByte(uint8_t *byte, Crc16Type crc)
{
  *byte = PhyReceiveByte();
  dprintf("%02X ", *byte);

  return(Crc16UpdateByte(crc, *byte));
}

/***********************************************************************************************************************
 * Receive and decode special bytes and update CRC
 **********************************************************************************************************************/
static Crc16Type LinkDecodeByte(uint8_t *byte, Crc16Type crc)
{
  crc = LinkReceiveByte(byte, crc);

  if(*byte == ENQ) {
    crc = LinkReceiveByte(byte, crc);
    *byte -= 0x80;
  }

  return crc;
}

/***********************************************************************************************************************
 * Receive and check a frame from the physical layer
 **********************************************************************************************************************/
uint16_t LinkReceiveBuffer(void *buffer, uint16_t bufLen)
{
  Crc16Type crc = 0xFFFF, crcIn;
  uint8_t byte;
  uint16_t i, length;

  dprintf("RX: ");

  // Receive STX (Not encoded)
  crc = LinkReceiveByte(&byte, crc);
  if(byte != STX) {
    ExitWithError("Bad packet start byte: %02X", byte);
  }

  // Receive length
  crc = LinkDecodeByte(&byte, crc);
  length = (uint16_t)byte << 8;
  crc = LinkDecodeByte(&byte, crc);
  length |= (uint16_t)byte;
  if(length > bufLen) {
    ExitWithError("Receive data too big: %u", length);
  }

  // Receive Buffer as bytes
  for(i = 0; i < length; i++) {
    crc = LinkDecodeByte(&(((uint8_t *)buffer)[i]), crc);
  }

  // Receive CRC
  LinkDecodeByte(&byte, 0);
  crcIn = (Crc16Type)byte << 8;
  LinkDecodeByte(&byte, 0);
  crcIn |= (Crc16Type)byte;
  dprintf("\n");

  // Check CRC
  if(crc != crcIn) {
    ExitWithError("CRC Error, Calcualted: %04X, Received: %04X", crc, crcIn);
  }

  return length;
}
