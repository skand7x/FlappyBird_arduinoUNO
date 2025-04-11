# Flappy Bird for Arduino Uno with 2.4" Touch Screen

This is an implementation of the popular Flappy Bird game for Arduino Uno with a 2.4-inch touch screen display.

## Hardware Requirements

- Arduino Uno R3
- 2.4" TFT LCD Touch Screen Display (ILI9341 or compatible)
- USB cable 

## Wiring Connection

### LCD Display Connections

| TFT LCD Pin | Arduino Pin | Description |
|-------------|-------------|-------------|
| LCD_CS      | A3          | Chip Select |
| LCD_CD      | A2          | Command/Data (Register Select) |
| LCD_WR      | A1          | LCD Write |
| LCD_RD      | A0          | LCD Read |
| LCD_RESET   | A4          | Reset |
| SD_CS       | 10          | SD Card Chip Select (if applicable) |
| MOSI        | 11          | SPI Data (Master Out Slave In) |
| MISO        | 12          | SPI Data (Master In Slave Out) |
| CLK/SCK     | 13          | SPI Clock |
| VCC         | 5V          | Power |
| GND         | GND         | Ground |

### Touch Screen Connections

| Touch Pin | Arduino Pin | Description |
|-----------|-------------|-------------|
| YP        | A1          | Y+ (must be analog) |
| XM        | A2          | X- (must be analog) |
| YM        | 7           | Y- (digital) |
| XP        | 6           | X+ (digital) |

## Required Libraries

Before uploading the sketch, install these libraries in the Arduino IDE:

1. Adafruit GFX Library
2. Adafruit TFTLCD Library
3. TouchScreen Library

You can install them via the Arduino Library Manager:
1. In the Arduino IDE, go to Sketch > Include Library > Manage Libraries
2. Search for each library and click Install

## Game Play

- The game starts with a welcome screen
- Touch the screen to start the game
- Tap anywhere on the screen to make the bird flap and fly upwards
- Avoid hitting the pipes, ceiling, or ground
- Each pipe you pass earns you a point
- When you crash, the game over screen will appear
- Touch again to restart

## Touch Screen Calibration

If the touch controls don't respond correctly, you may need to calibrate your screen by adjusting these values in the code:

```
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
#define MINPRESSURE 10
#define MAXPRESSURE 1000
```

## Performance Tweaking

If the game runs too fast or too slow:

1. Adjust the frame rate by changing the value in:
   ```
   if (millis() - lastUpdateTime > 33) { // ~30fps
   ```

2. Adjust the game difficulty by modifying:
   - `GRAVITY` - How quickly the bird falls
   - `JUMP_FORCE` - How high the bird jumps
   - `PIPE_GAP` - Space between top and bottom pipes
   - Pipe speed - Change `pipes[i].x -= 2;` to a different value

## Troubleshooting

- If the screen doesn't display properly, check your wiring and ensure the correct driver is specified (`tft.begin(0x9341)`)
- If touch is not working, ensure pins are correctly connected and try the TouchScreenCalibration sketch from the previous project
- If the bird movement is jittery, try reducing the frame rate
- If the game crashes, make sure your Arduino has sufficient memory available 
