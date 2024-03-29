# GxEPD_HD
E-Paper display library for Arduino for HD parallel interface displays from Dalian Good Display.

## for HD e-paper displays from Dalian Good Display on DESTM32-T evaluation kit
- http://www.good-display.com/products_detail/productId=370.html
## for HD e-paper displays from Waveshare with matching IT8951 Driver HAT

### The E-Paper display classes are subclasses of Adafruit_GFX, to have graphics and text rendering.
- a graphics buffer is used to have fast rendering through Adafruit_GFX.
- parts or full content of the graphics buffer are transferred to the controller buffer for display.
- black / white graphics buffer content is displayed using the fast "differential update" waveform method.
- graphics buffers with gray levels will be added.
- single pixel update to the controller buffer seems possible, to be investigated, may be slow.

### Paged Drawing, Picture Loop
- This library uses paged drawing to limit RAM use of graphics buffer for Adafruit_GFX
- buffer size can be selected in the application by template parameter page_height, see GxEPD_HD_Example
- Paged drawing is implemented as picture loop, like in U8G2 (Oliver Kraus)
- see https://github.com/olikraus/u8glib/wiki/tpictureloop
- Paged drawing is also available using drawPaged() and drawCallback(), like in GxEPD
- ` // GxEPD style paged drawing; drawCallback() is called as many times as needed `
- ` void drawPaged(void (*drawCallback)(const void*), const void* pv) `
- paged drawing is done using Adafruit_GFX methods inside picture loop or drawCallback

### Full Screen Buffer Support
- full screen buffer is selected by setting template parameter page_height to display height
- drawing to full screen buffer is done using Adafruit_GFX methods without picture loop or drawCallback
- and then calling method display()

### Low Level Bitmap Drawing Support
- bitmap drawing support to the controller memory and screen is available:
- either through the template class instance methods that forward calls to the base display class
- or directy using an instance of a base display class and calling its methods directly

### Supporting Arduino Forum Topic:

- Good Display ePaper for Arduino: https://forum.arduino.cc/index.php?topic=436411.0

### Arduino IDE Board (package) for build:
- processor on demo board is STM32F103VET6
- suitable STM32 Arduino package is:
- official STM32 package "STM32 Boards (select from submenu)"
- Board: "Generic STM32F1 series"
- Board part number: "Generic F103VE"
- also suitable Arduino package is Roger Clark version:
- STM32 Boards (STM32Duino.com)
- Board: "Generic STM32F103V series"
- Optimize: "Smallest (default)"
- Variant: "STM32F103VE"
- CPU Speed(MHz): "72MHz (Normal)"
- Upload method: "STLink"
- ESP32 package for use with my proto boards, or ED060SCT on IT8951 Driver HAT

### Version 1.0.4
- added support for Waveshare ED060KC1 on matching IT8951 Driver HAT
- added support for Waveshare ED060KC2 on matching IT8951 Driver HAT
- added support for Waveshare ES103TC1 on matching IT8951 Driver HAT
- added example GxEPD_HD_SD_Example
- updated GxEPD_HD_WiFi_Example, fixes for BMP handling, updated https certificates and use
- updated BMP handling for GxEPD_HD_SerialFlash_Annotated, GxEPD_HD_SerialFlash_Example
- updated BMP handling for GxEPD_HD_Spiffs_Example
- updated certificates for GxEPD_HD_SerialFlash_Loader, GxEPD_HD_Spiffs_Loader
#### Version 1.0.3
- added grey levels graphics support GxEPD_HD_4G.h, GxEPD_HD_16G.h, GxEPD_HD_GreyGraphics.ino
- added support for Waveshare ED060SCT on IT8951 Driver HAT
- added touch support for GxGDE060F3-T (initial version, GxFT5436 touch driver for FT5436 controller)
- added fix for GFX_FontExtension (added adapted getTextBounds)
- now declared supported with official STM32 package, re-tested
#### Version 1.0.2
- fixed controller buffer partial overwrite issue with GDE060F3 or GDEW080T5 (higher buffer base address)
#### Version 1.0.1
- added GxGDE060F3 for panel GDE060F3 6" 1024x758
- uses my Adafruit_GFX clone GFX from my library GFX_Root, available by Library Manager
- search for GFX_Root in Library Manager to install GFX_Root
#### Version 1.0.0
- uses my Adafruit_GFX clone Fruitless_GFX
- download Fruitless_GFX as ZIP from https://github.com/ZinggJM/Fruitless_GFX
- install Fruitless_GFX using Library Manager Add .ZIP Library method
- GFX_FontExtension doesn't need namespaces with Fruitless_GFX or actual Adafruit_GFX
#### Version 0.0.14
- caution: Adafruit_GFX Version 1.8.0 and higher doesn't compile for STM32 Boards (STM32Duino.com)
- use Adafruit_GFX Version 1.7.5 for use with DESTM32-T
- GFX_FontExtension therefore still needs to use namespaces
- fixed writeImage, writeImagePart, drawImage, drawImagePart for "odd" bitmap widths
#### Version 0.0.13
- added GFX_FontExtension, UTF-8 fonts extension for up to 256 code range pages
- GFX_FontExtension is a preliminary version, released for tests
- GFX_FontExtension uses namespaces until new gfxfont.h is released with Adafruit_GFX
- added Fonts/Hindi12pt8b.h for use with GFX_FontExtension
- additional fonts can be created with fontconverter from https://github.com/Bodmer/GFX_Font_Converter
- added example GxEPD_HD_FontExtension, shows how to use the actual GFX_FontExtension with Hindi12pt8b.h
- removed support for old DESTM32L, is now in library GxEPDxL
- caution: it is Version 13
#### Version 0.0.12
- updated waveform table with actual version from Good Display
- added vcom parameter to constructor (take value from sticker on flex connector)
- added examples GxEPD_HD_WiFi_Example, GxEPD_HD_Spiffs_Example, GxEPD_HD_Spiffs_Loader
#### Version 0.0.11
- added missing #include to the examples
- adapted GxEPD_HD_SerialFlash_Example and GxEPD_HD_SerialFlash_Annotated for GxDESP32T_BP
- fixed long transfers in GxDESP32T_BP, e.g. for clearScreen()
#### Version 0.0.10
- added hibernate() support, switches off TCon-11 for minimal power use
- added GxDESP32T_BP driver class for use with ESP32 with my other ESP32-Tcon-11 proto board
- the proto board uses a STM32F103C8T6 (Blue Pill) for most TCon-11 control and data lines.
- the wiring used is documented in the DESP32T_PB_wiring.h header file
- the driver code for the STM32F103C8T6 is in examples/STM32F103C8T6_IO_DRIVER
- the GxDESP32T_BP driver class is in experimental state
- support for the old DESTM32L is deprecated and moved to extras/deprecated
#### Version 0.0.9
- fixed powerOff() handling: power on for refresh if powered off
#### Version 0.0.8
- added GxDESP32T driver class for use with ESP32 with my ESP32-Tcon-11 proto board
- the proto board uses a MCP23S17 breakout for the AVT6203A 16bit interface
- the wiring used is documented in the DESP32T_wiring.h header file
- the GxDESP32T driver class is in experimental state
#### Version 0.0.7
- added grey level support to GxEPD_HD_SerialFlash_Example
- added delayed refresh option to GxEPD_HD_SerialFlash_Example functions
- added transfer() method: transfer graphics buffer to controller buffer without refresh
- added norefresh parameter to nextPage() method, default is false
- added GxEPD_HD_SerialFlash_Annotated example, shows use of delayed refresh
#### Version 0.0.6
- cleanup: moved demo part to class DESTM32T_DEMO
- SPI usage: SPI (SPI1) is no longer used or initialized by the library itself
- SPI usage: GxEPD_HD_SerialFlash_Example releases all SPI pins after every drawBitmapFromSerialFlash
- SPI usage: GxEPD_HD_SerialFlash_Loader asks for confirmation before using SPI
- SPI usage: GxEPD_HD_SerialFlash_Loader releases all SPI pins at the end
- SPI usage: onboard SPI-flash (N_J16) can safely be connected in parallel to ESP32 or ESP8266
- SPI usage: make sure only one processor uses SPI at any point in time, if connected in parallel
- SPI usage: SPI1 may be used for interprocessor communication in a future version
#### Version 0.0.5
- added example GxEPD_HD_SerialFlash_Loader (runs on ESP8266, ESP32)
- added example GxEPD_HD_SerialFlash_Example
- these examples use a modified SerialFlash library, see example source
- initial versions, GxEPD_HD_SerialFlash_Example shows b/w only
#### Version 0.0.4
- renamed GxGDE06BA to GxGDE060BA
- added GxGDE043A2
- added GxGDEW080T5
- some fixes
- preliminary
#### Version 0.0.3
- interim check-in for old big red STM32F103VE DESTM32L board (ex GxEPD)
- preliminary
#### Version 0.0.2
- flexible selection of Serialx for diagnostic output
- preliminary
#### Version 0.0.1
- preliminary
- for panel GDE60BA (may work with GDE043A2)
- initial test successful
