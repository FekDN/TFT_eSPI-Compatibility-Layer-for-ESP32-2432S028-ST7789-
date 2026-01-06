# `TFT_eSPI_Compat.h`: TFT_eSPI Compatibility Layer for ESP32-2432S028 (ST7789)
This file is a drop-in compatibility layer that allows sketches written for the popular TFT_eSPI library to run on the ESP32-2432S028 board without code modification.


It is specifically configured for the board version that features:
*   A **2.8-inch display**
*   An **ST7789** controller (not ILI9341)
*   No "R" at the end of the model name (e.g., `ESP32-2432S028`)

This header uses the powerful **LovyanGFX** library in the background, pre-configured with the correct pinout and driver settings for this specific board.

### How to Use

1.  **Install LovyanGFX**: Before you start, install the `LovyanGFX` library from the Arduino IDE Library Manager.
2.  **Place the File**: Put this `TFT_eSPI_Compat.h` file in the same folder as your main `.ino` sketch.
3.  **Modify Your Sketch**: In your code, replace the original library include with this one.

    **Instead of this:**
    ```cpp
    // #include <TFT_eSPI.h>
    // #include <SPI.h>
    ```
    **Use this:**
    ```cpp
    #include "TFT_eSPI_Compat.h" 
    ```
    *(Note the use of `"` instead of `<>`)*

4.  **Compile and Run**: Your existing code, which uses `TFT_eSPI tft;`, will now work correctly on this display.

    **Example:**
    ```cpp
    #include "TFT_eSPI_Compat.h" // The only include you need for the display

    TFT_eSPI tft; // Or TFT_eSPI tft = TFT_eSPI(); - both options work. Your code thinks this is TFT_eSPI, but it's LovyanGFX!

    void setup() {
      tft.init();
      tft.fillScreen(TFT_BLUE);
      tft.setCursor(10, 10);
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(2);
      tft.println("Hello, World!");
    }

    void loop() {
      // Your code here
    }
    ```
    
## License

The source code of this project is licensed under the **Apache License 2.0**. A full copy of the license is available in the `LICENSE` file in the root directory of this repository and can also be viewed at [https://www.apache.org/licenses/LICENSE-2.0](https://www.apache.org/licenses/LICENSE-2.0).

---
### Copyright (c) 2026 Dmitry Feklin (FeklinDN@gmail.com)
---
    
