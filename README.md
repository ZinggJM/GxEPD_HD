# GxEPD_HD
E-Paper display library for Arduino for HD parallel IF displays from Dalian Good Display.

## for HD e-paper displays from Dalian Good Display on DESTM32-T evaluation kit
- http://www.good-display.com/products_detail/productId=370.html

### The E-Paper display classes are subclasses of Adafruit_GFX, to have graphics and text rendering.
- a graphics buffer is used to have fast rendering through Adafruit_GFX.
- parts or full content of the graphics buffer are transferred to the controller buffer for display.
- black / white graphics buffer content is displayed using the fast "differential update" waveform method.
- graphics buffers with gray levels will be added.
- single pixel update to the controller buffer seems possible, to be investigated, may be slow.

### Paged Drawing, Picture Loop
#### - This library uses paged drawing to limit RAM use of graphics buffer for Adafruit_GFX
#### - buffer size can be selected in the application by template parameter page_height, see GxEPD2_Example
#### - Paged drawing is implemented as picture loop, like in U8G2 (Oliver Kraus)
#### - see https://github.com/olikraus/u8glib/wiki/tpictureloop
#### - Paged drawing is also available using drawPaged() and drawCallback(), like in GxEPD
´´´ // GxEPD style paged drawing; drawCallback() is called as many times as needed ´´´
´´´ void drawPaged(void (*drawCallback)(const void*), const void* pv) ´´´
#### - paged drawing is done using Adafruit_GFX methods inside picture loop or drawCallback

### Full Screen Buffer Support
#### - full screen buffer is selected by setting template parameter page_height to display height
#### - drawing to full screen buffer is done using Adafruit_GFX methods without picture loop or drawCallback
#### - and then calling method display()

### Low Level Bitmap Drawing Support
#### - bitmap drawing support to the controller memory and screen is available:
#### - either through the template class instance methods that forward calls to the base display class
#### - or directy using an instance of a base display class and calling its methods directly

### Supporting Arduino Forum Topic:

- Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

### Arduino IDE Board (package) for build:
- processor on demo board is STM32F103VET6
- suitable STM32 Arduino package is:
- STM32 Boards (STM32Duino.com)
- Board: "Generic STM32F103V series"
- Optimize: "Smallest (default)"
- Variant: "STM32F103VE"
- CPU Speed(MHz): "72MHz (Normal)"
- Upload method: "STLink"

### Version 0.0.1
- preliminary
- initial test successful
