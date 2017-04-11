#ifndef DE1_SoC_LT24_H_
#define DE1_SoC_LT24_H_

#include "Delay.h" //some useful delay routines

//Size of the LCD
#define LCD_WIDTH  240
#define LCD_HEIGHT 320

//Some basic colours
#define LCD_BLACK   (0x0000)
#define LCD_WHITE   (0xFFFF)
#define LCD_RED     (0x1F << 11)
#define LCD_GREEN   (0x1F << 6)
#define LCD_BLUE    (0x1F << 0)
#define LCD_YELLOW  (LCD_RED | LCD_GREEN)
#define LCD_CYAN    (LCD_GREEN | LCD_BLUE)
#define LCD_MAGENTA (LCD_BLUE | LCD_RED)
#define LCD_DARKBROWN (0x5246)
#define LCD_LIGHTBROWN (0xA4CB)

//Initialise LCD
void LCD_Init ( void );
//Enable LCD
void LCD_Enable ( void );
//Disable LCD
void LCD_Disable( void );
//Convert RGB to LCD colour values
unsigned short LCD_MakeColour( unsigned int R, unsigned int G, unsigned int B );
//Define window on LCD (where data will be written to)
void LCD_Window( unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height);
//Clear the LCD setting background colour
void LCD_Clear( unsigned short colour );
//Display Test Pattern
void LCD_TestPattern( void );
//Copy Data Buffer to LCD Frame Buffer (define window first)
void LCD_Framebuffer(unsigned short buffer[], unsigned int length);

//Write to LCD Register
void LCD_WR_REG ( unsigned short value );
//Write one pixel to LCD internal frame buffer
void LCD_WR_DATA( unsigned short value );

#endif /*DE1_SoC_LT24_H_*/
