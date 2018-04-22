///*********************************************************************
//This is a library for our Monochrome OLEDs based on SSD1306 drivers
//
//  Pick one up today in the adafruit shop!
//  ------> http://www.adafruit.com/category/63_98
//
//These displays use SPI to communicate, 4 or 5 pins are required to
//interface
//
//Adafruit invests time and resources providing this open source code,
//please support Adafruit and open-source hardware by purchasing
//products from Adafruit!
//
//Written by Limor Fried/Ladyada  for Adafruit Industries.
//BSD license, check license.txt for more information
//All text above, and the splash screen below must be included in any redistribution
//*********************************************************************/
//
//#include "display.h"
//#include "i2cspm.h"
//#include <stdlib.h>
//
//
//// the memory buffer for the LCD
//
//static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] = {
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
//0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x80, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,
//0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xFF,
//0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00,
//0x80, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x8C, 0x8E, 0x84, 0x00, 0x00, 0x80, 0xF8,
//0xF8, 0xF8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80,
//0x00, 0xE0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xC7, 0x01, 0x01,
//0x01, 0x01, 0x83, 0xFF, 0xFF, 0x00, 0x00, 0x7C, 0xFE, 0xC7, 0x01, 0x01, 0x01, 0x01, 0x83, 0xFF,
//0xFF, 0xFF, 0x00, 0x38, 0xFE, 0xC7, 0x83, 0x01, 0x01, 0x01, 0x83, 0xC7, 0xFF, 0xFF, 0x00, 0x00,
//0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0xFF, 0xFF, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7F, 0xFF,
//0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xFF,
//0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x03, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x8F,
//0x8F, 0x9F, 0xBF, 0xFF, 0xFF, 0xC3, 0xC0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC,
//0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x01, 0x03, 0x03, 0x03,
//0x03, 0x03, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01,
//0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00,
//0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
//0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x03,
//0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//#if (SSD1306_LCDHEIGHT == 64)
//0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F,
//0x87, 0xC7, 0xF7, 0xFF, 0xFF, 0x1F, 0x1F, 0x3D, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0x7C, 0x7D, 0xFF,
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x00, 0x30, 0x30, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xC0, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0xC0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F,
//0x0F, 0x07, 0x1F, 0x7F, 0xFF, 0xFF, 0xF8, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xE0,
//0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00,
//0x00, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x0E, 0xFC, 0xF8, 0x00, 0x00, 0xF0, 0xF8, 0x1C, 0x0E,
//0x06, 0x06, 0x06, 0x0C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFC,
//0xFE, 0xFC, 0x00, 0x18, 0x3C, 0x7E, 0x66, 0xE6, 0xCE, 0x84, 0x00, 0x00, 0x06, 0xFF, 0xFF, 0x06,
//0x06, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x06, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0xC0, 0xF8,
//0xFC, 0x4E, 0x46, 0x46, 0x46, 0x4E, 0x7C, 0x78, 0x40, 0x18, 0x3C, 0x76, 0xE6, 0xCE, 0xCC, 0x80,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00,
//0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x03, 0x07, 0x0E, 0x0C,
//0x18, 0x18, 0x0C, 0x06, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x01, 0x0F, 0x0E, 0x0C, 0x18, 0x0C, 0x0F,
//0x07, 0x01, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00,
//0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x07,
//0x07, 0x0C, 0x0C, 0x18, 0x1C, 0x0C, 0x06, 0x06, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//#endif
//};
//
//
//
//// the most basic function, set a single pixel
//void drawPixel(int16_t x, int16_t y, uint16_t color) {
//  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
//    return;
//
//  // check rotation, move pixel around if necessary
//  switch (getRotation()) {
//  case 1:
//    swap(x, y);
//    x = WIDTH - x - 1;
//    break;
//  case 2:
//    x = WIDTH - x - 1;
//    y = HEIGHT - y - 1;
//    break;
//  case 3:
//    swap(x, y);
//    y = HEIGHT - y - 1;
//    break;
//  }
//
//  // x is which column
//  if (color == WHITE)
//    buffer[x+ (y/8)*SSD1306_LCDWIDTH] |= _BV((y%8));
//  else
//    buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~_BV((y%8));
//}
//
//
//Adafruit_SSD1306(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS) : Adafruit_GFX(SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT) {
//  cs = CS;
//  rst = RST;
//  dc = DC;
//  sclk = SCLK;
//  sid = SID;
//}
//
//// initializer for I2C - we only indicate the reset pin!
//Adafruit_SSD1306(int8_t reset) :
//Adafruit_GFX(SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT) {
//  sclk = dc = cs = sid = -1;
//  rst = reset;
//}
//
//
//void begin(uint8_t vccstate, uint8_t i2caddr) {
//  _i2caddr = i2caddr;
//
//    // I2C Init
//	Wire.begin(); // Is this the right place for this?
//  }
//
//  // Setup reset pin direction (used by both SPI and I2C)
////  pinMode(rst, OUTPUT);
////  digitalWrite(rst, HIGH);
//  // VDD (3.3V) goes high at start, lets just chill for a ms
////  delay(1);
//  // bring reset low
////  digitalWrite(rst, LOW);
//  // wait 10ms
////  delay(10);
//  // bring out of reset
////  digitalWrite(rst, HIGH);
//  // turn on VCC (9V?)
//
//   #if defined SSD1306_128_32
//    // Init sequence for 128x32 OLED module
//    ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
//    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
//    ssd1306_command(0x80);                                  // the suggested ratio 0x80
//    ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
//    ssd1306_command(0x1F);
//    ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
//    ssd1306_command(0x0);                                   // no offset
//    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
//    ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
//    if (vccstate == SSD1306_EXTERNALVCC)
//      { ssd1306_command(0x10); }
//    else
//      { ssd1306_command(0x14); }
//    ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
//    ssd1306_command(0x00);                                  // 0x0 act like ks0108
//	ssd1306_command(SSD1306_SEGREMAP | 0x1);
//    ssd1306_command(SSD1306_COMSCANDEC);
//    ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
//    ssd1306_command(0x02);
//    ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
//    ssd1306_command(0x8F);
//    ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
//    if (vccstate == SSD1306_EXTERNALVCC)
//      { ssd1306_command(0x22); }
//    else
//      { ssd1306_command(0xF1); }
//    ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
//    ssd1306_command(0x40);
//    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
//    ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
//  #endif
//
//  #if defined SSD1306_128_64
//    // Init sequence for 128x64 OLED module
//    ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
////    ssd1306_command(0x00);//---set low column address
////    ssd1306_command(0x10);//---set high column address
//    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
//    ssd1306_command(0x80);                                  // the suggested ratio 0x80
//    ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
//    ssd1306_command(0x3F);
//    ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
//    ssd1306_command(0x0);                                   // no offset
//    ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
//    ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
//    if (vccstate == SSD1306_EXTERNALVCC)
//      { ssd1306_command(0x10); }
//    else
//      { ssd1306_command(0x14); }
//    ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
//    ssd1306_command(0x00);                                  // 0x0 act like ks0108
//    ssd1306_command(SSD1306_SEGREMAP | 0x1);
//    ssd1306_command(SSD1306_COMSCANDEC);
//    ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
//    ssd1306_command(0x12);
//    ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
//    if (vccstate == SSD1306_EXTERNALVCC)
//      { ssd1306_command(0x9F); }
//    else
//      { ssd1306_command(0xCF); }
//    ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
//    if (vccstate == SSD1306_EXTERNALVCC)
//      { ssd1306_command(0x22); }
//    else
//      { ssd1306_command(0xF1); }
//    ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
//    ssd1306_command(0x40);
//    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
//    ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
//
//    ssd1306_command(0xb0);
//    ssd1306_command(0x10);
//    ssd1306_command(0x01);//Set original position to (0,0)
//  #endif
//
//  ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel
//}
//
//
//void invertDisplay(uint8_t i) {
//  if (i) {
//    ssd1306_command(SSD1306_INVERTDISPLAY);
//  } else {
//    ssd1306_command(SSD1306_NORMALDISPLAY);
//  }
//}
//
///**************************************************************************//**
//     * @brief
//     * sends command to display
//     * @param[in] i2c
//     *   The I2C peripheral to use (not used).
//     * @param[in] addr
//     *   The I2C address of the sensor.
//     * @param[in] command
//     *   The command to send to device. See the \#define's for details.
//     * @return
//     *   Returns number of bytes read on success. Otherwise returns error codes
//     *   based on the I2CDRV.
//     *****************************************************************************/
//
//static void ssd1306_command(I2C_TypeDef *i2c, uint8_t addr, uint8_t command)
//    {
//      // I2C
//      uint8_t control = 0x00;   // Co = 0, D/C = 0
//      I2C_TransferSeq_TypeDef    seq;
//      I2C_TransferReturn_TypeDef ret;
//      uint8_t                    i2c_read_data[2];
//      uint8_t                    i2c_write_data[1];
//
//      seq.addr  = addr;
//      seq.flags = I2C_FLAG_WRITE_READ;
//      /* Set control to issue */
//      i2c_write_data[0] = control;
//      seq.buf[0].data   = i2c_write_data;
//      seq.buf[0].len    = 1;
//      /* Select location/length of data to be read */
//      seq.buf[1].data = i2c_read_data;
//      seq.buf[1].len  = 2;
//
//      ret = I2CSPM_Transfer(i2c, &seq);
//
//      /* Select command to issue */
//      i2c_write_data[0] = command;
//      seq.buf[0].data   = i2c_write_data;
//      seq.buf[0].len    = 1;
//
//      ret = I2CSPM_Transfer(i2c, &seq);
//
//}
//
//// startscrollright
//// Activate a right handed scroll for rows start through stop
//// Hint, the display is 16 rows tall. To scroll the whole display, run:
//// display.scrollright(0x00, 0x0F)
//void startscrollright(uint8_t start, uint8_t stop){
//	ssd1306_command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
//	ssd1306_command(0X00);
//	ssd1306_command(start);
//	ssd1306_command(0X00);
//	ssd1306_command(stop);
//	ssd1306_command(0X01);
//	ssd1306_command(0XFF);
//	ssd1306_command(SSD1306_ACTIVATE_SCROLL);
//}
//
//// startscrollleft
//// Activate a right handed scroll for rows start through stop
//// Hint, the display is 16 rows tall. To scroll the whole display, run:
//// display.scrollright(0x00, 0x0F)
//void startscrollleft(uint8_t start, uint8_t stop){
//	ssd1306_command(SSD1306_LEFT_HORIZONTAL_SCROLL);
//	ssd1306_command(0X00);
//	ssd1306_command(start);
//	ssd1306_command(0X00);
//	ssd1306_command(stop);
//	ssd1306_command(0X01);
//	ssd1306_command(0XFF);
//	ssd1306_command(SSD1306_ACTIVATE_SCROLL);
//}
//
//// startscrolldiagright
//// Activate a diagonal scroll for rows start through stop
//// Hint, the display is 16 rows tall. To scroll the whole display, run:
//// display.scrollright(0x00, 0x0F)
//void startscrolldiagright(uint8_t start, uint8_t stop){
//	ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
//	ssd1306_command(0X00);
//	ssd1306_command(SSD1306_LCDHEIGHT);
//	ssd1306_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
//	ssd1306_command(0X00);
//	ssd1306_command(start);
//	ssd1306_command(0X00);
//	ssd1306_command(stop);
//	ssd1306_command(0X01);
//	ssd1306_command(SSD1306_ACTIVATE_SCROLL);
//}
//
//// startscrolldiagleft
//// Activate a diagonal scroll for rows start through stop
//// Hint, the display is 16 rows tall. To scroll the whole display, run:
//// display.scrollright(0x00, 0x0F)
//void startscrolldiagleft(uint8_t start, uint8_t stop){
//	ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
//	ssd1306_command(0X00);
//	ssd1306_command(SSD1306_LCDHEIGHT);
//	ssd1306_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
//	ssd1306_command(0X00);
//	ssd1306_command(start);
//	ssd1306_command(0X00);
//	ssd1306_command(stop);
//	ssd1306_command(0X01);
//	ssd1306_command(SSD1306_ACTIVATE_SCROLL);
//}
//
//void stopscroll(void){
//	ssd1306_command(SSD1306_DEACTIVATE_SCROLL);
//}
//
//
//void display(I2C_TypeDef *i2c, uint8_t addr) {
//  ssd1306_command(SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
//  ssd1306_command(SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
//  ssd1306_command(SSD1306_SETSTARTLINE | 0x0); // line #0
//
//
////  // save I2C bitrate
////    uint8_t twbrbackup = TWBR;
////    TWBR = 12; // upgrade to 400KHz!
//
//    I2C_TransferSeq_TypeDef    seq;
//    I2C_TransferReturn_TypeDef ret;
//    uint8_t                    i2c_read_data[2];
//    uint8_t                    i2c_write_data[1];
//
//    seq.addr  = addr;
//    seq.flags = I2C_FLAG_WRITE_READ;
//    /* Select command to issue */
//
//	 seq.buf[0].len    = 1;
//	 /* Select location/length of data to be read */
//	 seq.buf[1].data = i2c_read_data;
//	 seq.buf[1].len  = 2;
//
//
//    // I2C
//    for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++) {
//      // send a bunch of data in one xmission
//
//		 i2c_write_data[0] = 0x40;;
//		 seq.buf[0].data   = i2c_write_data;
//		 ret = I2CSPM_Transfer(i2c, &seq);
//
//		 for (uint8_t x=0; x<16; x++) {
//			 i2c_write_data[0] = buffer[i];
//			 seq.buf[0].data   = i2c_write_data;
//			 ret = I2CSPM_Transfer(i2c, &seq);
//			i++;
//		 }
//			  i--;
//	}
//
////    TWBR = twbrbackup;
//
//}
//
//// clear everything
//void clearDisplay(void) {
//  memset(buffer, 0, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
//}
//
///**************************************************************************//**
// * @brief
// *   Checks if a Si7013 is present on the I2C bus or not.
// * @param[in] i2c
// *   The I2C peripheral to use (Not used).
// * @param[in] addr
// *   The I2C address to probe.
// * @param[out] deviceId
// *   Write device ID from SNB_3 if device responds. Pass in NULL to discard.
// *   Should be 0x0D for Si7013, 0x14 for Si7020 or 0x15 for Si7021
// * @return
// *   True if a Si70xx is detected, false otherwise.
// *****************************************************************************/
//bool ssd1306_Detect(I2C_TypeDef *i2c, uint8_t addr)
//{
//  I2C_TransferSeq_TypeDef    seq;
//  I2C_TransferReturn_TypeDef ret;
//  uint8_t                    i2c_read_data[2];
//  uint8_t                    i2c_write_data[2];
//
//  seq.addr  = addr;
//  seq.flags = I2C_FLAG_WRITE_READ;
//  /* Select command to issue */
//  i2c_write_data[0] = 0xA0;
//  i2c_write_data[1] = 0xA4;
//  seq.buf[0].data   = i2c_write_data;
//  seq.buf[0].len    = 2;
//  /* Select location/length of data to be read */
//  seq.buf[1].data = i2c_read_data;
//  seq.buf[1].len  = 2;
//
//  ret = I2CSPM_Transfer(i2c, &seq);
//  if (ret != i2cTransferDone) {
//    return false;
//  }
//  return true;
//}
//
//
//