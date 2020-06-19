// SeQDoT_SF_Merger: utility for Display Library for HD e-Paper displays from Dalian Good Display Inc. (parallel interface).
//
// Display Library based on Demo Example available from Good Display
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD_HD

#include <GxEPD_HD.h>

#include "bitmaps/Bitmap_Tiger_320x240x24_4Bit.h"

#include <SerialFlash.h>
#include <SPI.h>

// digital pin for flash chip CS pin:
//const int FlashChipSelect = SS; // for standard slave select pin
//const int FlashChipSelect = 5; // use D1 on my Wemos D1 mini wired for e-papers
//const int FlashChipSelect = 17; // as used with my ESP32 breadboard
const int FlashChipSelect = 32; // for W25Q16BVSIG on my DESP32T_BP proto board

void setup()
{
  Serial.begin(115200);
  if (!SerialFlash.begin(FlashChipSelect))
  {
    Serial.println("Unable to access SPI Flash chip");
    SPI.end(); // release SPI pins
    pinMode(FlashChipSelect, INPUT); // CS also!
    return;
  }
  Serial.println("SerialFlash started");
  writeBitmaps();
}

void loop()
{
}

uint8_t filldata_1[] = {0x0, 0x23, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0x0};
uint8_t filldata_4[] = {0x0, 0x0, 0x0, 0x0, 0x12, 0xB, 0x0, 0x0, 0x12, 0xB, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0x11, 0x11, 0x11,
                        0xFF, 0x22, 0x22, 0x22, 0xFF, 0x33, 0x33, 0x33, 0xFF, 0x44, 0x44, 0x44, 0xFF, 0x55, 0x55, 0x55, 0xFF, 0x66, 0x66, 0x66, 0xFF, 0x77, 0x77, 0x77,
                        0xFF, 0x88, 0x88, 0x88, 0xFF, 0x99, 0x99, 0x99, 0xFF, 0xAA, 0xAA, 0xAA, 0xFF, 0xBB, 0xBB, 0xBB, 0xFF, 0xCC, 0xCC, 0xCC, 0xFF, 0xDD, 0xDD, 0xDD,
                        0xFF, 0xEE, 0xEE, 0xEE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                       };

void writeBitmaps()
{
  writeBitmap("GxBitmap_Tiger_320x240x24_4Bit.bmp", "07_Tiger_320x240x24_4Bit_Hex.txt", 320, 240, 4);
}

void writeBitmap(const char filename[], const char sourcename[], int16_t w, int16_t h, uint16_t depth)
{
  SerialFlashFile bitmapFile, sourceFile;
  uint32_t ppb = 8 / depth; // pixel per byte
  uint32_t rowSizeCode = (w + ppb - 1) / ppb;
  // BMP rows are padded (if needed) to 4-byte boundary
  uint32_t rowSizeBMP = (rowSizeCode + 3) & ~3;
  Serial.print("writeBitmap(\""); Serial.print(filename); Serial.println("\")");
  Serial.print("rowSizeCode "); Serial.println(rowSizeCode);
  Serial.print("rowSizeBMP  "); Serial.println(rowSizeBMP);
  uint32_t headerSize = 40;
  uint32_t imageOffset = depth == 1 ? 62 : 118; // until known better
  uint32_t fileSize = imageOffset + h * rowSizeBMP;
  SerialFlash.create(filename, fileSize);
  bitmapFile = SerialFlash.open(filename);
  if (!bitmapFile) Serial.println("bitmap file open error");
  sourceFile = SerialFlash.open(sourcename);
  if (!sourceFile) Serial.println("source file open error");
  if (bitmapFile && sourceFile)
  {
    bitmapFile.seek(0);
    write16(bitmapFile, 0x4D42); // BMP signature
    write32(bitmapFile, fileSize); // fileSize
    write32(bitmapFile, 0); // creator bytes
    write32(bitmapFile, imageOffset); // image offset
    write32(bitmapFile, headerSize); // Header size
    write32(bitmapFile, w); // image width
    write32(bitmapFile, h); // image height
    write16(bitmapFile, 1); // # planes
    write16(bitmapFile, depth); // // bits per pixel
    write32(bitmapFile, 0); // format uncompressed

    //Serial.print("first position "); Serial.println(bitmapFile.position());
    uint32_t j = 0;
    uint8_t* filldata = filldata_1;
    uint32_t filldata_size = sizeof(filldata);
    if (depth == 4)
    {
      filldata = filldata_4;
      filldata_size = sizeof(filldata_4);
    }
    for (uint32_t i = bitmapFile.position(); i < imageOffset; i++)
    {
      write8(bitmapFile, j < filldata_size ? filldata[j++] : 0); // remaining header bytes
    }
    Serial.print("start position "); Serial.println(bitmapFile.position());
    for (uint16_t row = 0; row < h; row++) // for each line
    {
      uint32_t rowidx = (h - row - 1) * rowSizeBMP * 2; // in reverse order
      uint32_t colidx;
      sourceFile.seek(rowidx);
      for (colidx = 0; colidx < rowSizeBMP; colidx++)
      {
        uint8_t data = 16 * fromHex(read8(sourceFile)) + fromHex(read8(sourceFile));
        write8(bitmapFile, data);
      }
      Serial.print(row); Serial.print(" "); Serial.print(rowidx); Serial.print(" "); Serial.println(bitmapFile.position());
    }
    Serial.println(bitmapFile.position());
    bitmapFile.close();
    Serial.println("done");
  }
  else Serial.print("open file for write failed!");
}

int fromHex(int v)
{
  if ((v >= int('0')) && (v <= int('9'))) return v - int('0');
  if ((v >= int('A')) && (v <= int('F'))) return v - int('A') + 10;
  if ((v >= int('a')) && (v <= int('f'))) return v - int('a') + 10;
  return 0;
}

uint8_t read8(SerialFlashFile& f)
{
  uint8_t result;
  f.read((void*)&result, 1);
  return result;
}

void write8(SerialFlashFile & f, uint8_t v)
{
  f.write((void*)&v, 1);
}

void write16(SerialFlashFile & f, uint16_t v)
{
  // BMP data is stored little-endian, same as Arduino.
  f.write((void*)&v, 2);
}

void write32(SerialFlashFile & f, uint32_t v)
{
  // BMP data is stored little-endian, same as Arduino.
  f.write((void*)&v, 4);
}
