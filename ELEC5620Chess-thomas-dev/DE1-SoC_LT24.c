
#include "DE1-SoC_LT24.h"

#define LT24_WRn    0x01
#define LT24_RS     0x02
#define LT24_RDn    0x04
#define LT24_CSn    0x08
#define LT24_RESETn 0x10
#define LT24_LCD_ON 0x20
#define LT24_HW_OPT 0x80

void LCD_WR_REG( unsigned short value )
{
	//Dedicated LCD Command Bus Interface
	volatile unsigned short *LT24_CMD = (unsigned short *) 0xFF200080;
	//Write command to LCD
	*LT24_CMD = value;

	/*
    // PIO Based LCD Control and Data/Command Interface
    volatile unsigned short *LT24_DCx  = (unsigned short *) 0xFF200070;
    volatile unsigned char  *LT24_CNTL = (unsigned char  *) 0xFF200072;
    //Load new register value on D/Cx bus pins
    *LT24_DCx  = value;
    //Perform write to register
    *LT24_CNTL =            LT24_RDn + LT24_LCD_ON + LT24_RESETn;
    *LT24_CNTL = LT24_WRn + LT24_RDn + LT24_LCD_ON + LT24_RESETn;
	*/
}

void LCD_WR_DATA( unsigned short value )
{
	//Dedicated LCD Data Bus Interface
	volatile unsigned short *LT24_DATA = (unsigned short *) 0xFF200084;
	//Write data to LCD
	*LT24_DATA = value;

	/*
    // PIO Based LCD Control and Data/Command Interface
    volatile unsigned short *LT24_DCx  = (unsigned short *) 0xFF200070;
    volatile unsigned char  *LT24_CNTL = (unsigned char  *) 0xFF200072;
    //Load new data value on D/Cx bus pins
    *LT24_DCx  = value;
    //Perform write of data
    *LT24_CNTL =            LT24_RS + LT24_RDn + LT24_LCD_ON + LT24_RESETn;
    *LT24_CNTL = LT24_WRn + LT24_RS + LT24_RDn + LT24_LCD_ON + LT24_RESETn;
	*/
}

void LCD_Enable( void )
{
    // PIO Based LCD Control and Data/Command Interface
    volatile unsigned short *LT24_DCx  = (unsigned short *) 0xFF200070;
    volatile unsigned char  *LT24_CNTL = (unsigned char  *) 0xFF200072;
    //Dummy data on the D/Cx bus
    *LT24_DCx  = 0;
    //Deassert the reset pin and turn LCD on. Don't modify LT24_HW_OPT bit!
    *LT24_CNTL |= (LT24_RESETn | LT24_LCD_ON);
}

void LCD_Disable( void )
{
    // PIO Based LCD Control and Data/Command Interface
    volatile unsigned short *LT24_DCx  = (unsigned short *) 0xFF200070;
    volatile unsigned char  *LT24_CNTL = (unsigned char  *) 0xFF200072;
    //Dummy data on the D/Cx bus
    *LT24_DCx  = 0;
    //Assert the reset pin and turn LCD off. Don't modify LT24_HW_OPT bit!
    *LT24_CNTL &= ~(LT24_RESETn | LT24_LCD_ON);
}


void LCD_Clear(unsigned short Color)
{
    unsigned int index=0; //for loop index
    //Define window as entire display
    LCD_Window(0, 0, LCD_WIDTH, LCD_HEIGHT);
    //Loop through each pixel in the window writing the required colour
    for(index=0;index<(LCD_WIDTH*LCD_HEIGHT);index++)
    {
        LCD_WR_DATA(Color);
    }
}

void LCD_Init( void )
{
	volatile unsigned int  *LT24_DIR  = (unsigned int  *) 0xFF200074;
	volatile unsigned char *LT24_CNTL = (unsigned char  *) 0xFF200072;
    //Set all of the LCD pins to be outputs
	*LT24_DIR = 0x00FFFFFF;
    
    //Initialise LCD control register.
    //PIO Mode (no optimisation):
    //*LT24_CNTL = 0x0;
    //For Hardware optimisation:
    *LT24_CNTL = LT24_HW_OPT;
    
    //LCD requires specific reset sequence: 
    LCD_Enable(); //turn on for 1ms
    Delay_Ms(1);
    LCD_Disable(); //then off for 10ms
    Delay_Ms(10);
    LCD_Enable(); //finally back on and wait 120ms for LCD to wake up
    Delay_Ms(120);
    
    //You don't need to worry about what all these registers are.
    //The LT24 LCDs are complicated things with many settings that need
    //to be configured - Contrast/Brightness/Data Format/etc.
    LCD_WR_REG(0x00EF);
        LCD_WR_DATA(0x0003);
        LCD_WR_DATA(0x0080);
        LCD_WR_DATA(0X0002);
    LCD_WR_REG(0x00CF);
        LCD_WR_DATA(0x0000);
        LCD_WR_DATA(0x0081);
        LCD_WR_DATA(0X00c0);
    LCD_WR_REG(0x00ED);
        LCD_WR_DATA(0x0064);
        LCD_WR_DATA(0x0003);
        LCD_WR_DATA(0X0012);
        LCD_WR_DATA(0X0081);
    LCD_WR_REG(0x00E8);
        LCD_WR_DATA(0x0085);
        LCD_WR_DATA(0x0001);
        LCD_WR_DATA(0x0078);
    LCD_WR_REG(0x00CB);
        LCD_WR_DATA(0x0039);
        LCD_WR_DATA(0x002C);
        LCD_WR_DATA(0x0000);
        LCD_WR_DATA(0x0034);
        LCD_WR_DATA(0x0002);
    LCD_WR_REG(0x00F7);
        LCD_WR_DATA(0x0020);
    LCD_WR_REG(0x00EA);
        LCD_WR_DATA(0x0000);
        LCD_WR_DATA(0x0000);
    //Power control
    LCD_WR_REG(0x00C0);         
        LCD_WR_DATA(0x0023);    //VRH[5:0]
    LCD_WR_REG(0x00C1);
        LCD_WR_DATA(0x0010);    //SAP[2:0];BT[3:0]
    //VCM control
    LCD_WR_REG(0x00C5);         
        LCD_WR_DATA(0x003E);
        LCD_WR_DATA(0x0028);
    LCD_WR_REG(0x00C7);
         LCD_WR_DATA(0X0086);
    // Memory Access Control (MADCTL)
    LCD_WR_REG(0x0036);         
        LCD_WR_DATA(0x0048);
    // More settings...
    LCD_WR_REG(0x003A);
        LCD_WR_DATA(0x0055);
    LCD_WR_REG(0x00B1);
        LCD_WR_DATA(0x0000);
        LCD_WR_DATA(0x001b);
    LCD_WR_REG(0x00B6);
        LCD_WR_DATA(0x0008);    //Non-Display Area Inaccessible
        LCD_WR_DATA(0x0082);    //Normally White, Normal Scan Direction (A2 = Reverse Scan Direction)
        LCD_WR_DATA(0x0027);    //320 Lines
    //3-Gamma Function Disable
    LCD_WR_REG(0x00F2);         
        LCD_WR_DATA(0x0000);
    //Gamma curve selected
    LCD_WR_REG(0x0026);         
        LCD_WR_DATA(0x0001);
    //Set Gamma
    LCD_WR_REG(0x00E0);         
        LCD_WR_DATA(0x000F);
        LCD_WR_DATA(0x0031);
        LCD_WR_DATA(0x002B);
        LCD_WR_DATA(0x000C);
        LCD_WR_DATA(0x000E);
        LCD_WR_DATA(0x0008);
        LCD_WR_DATA(0x004E);
        LCD_WR_DATA(0X00F1);
        LCD_WR_DATA(0x0037);
        LCD_WR_DATA(0x0007);
        LCD_WR_DATA(0x0010);
        LCD_WR_DATA(0x0003);
        LCD_WR_DATA(0x000E);
        LCD_WR_DATA(0x0009);
        LCD_WR_DATA(0x0000);
    LCD_WR_REG(0X00E1);
        LCD_WR_DATA(0x0000);
        LCD_WR_DATA(0x000E);
        LCD_WR_DATA(0x0014);
        LCD_WR_DATA(0x0003);
        LCD_WR_DATA(0x0011);
        LCD_WR_DATA(0x0007);
        LCD_WR_DATA(0x0031);
        LCD_WR_DATA(0x00C1);
        LCD_WR_DATA(0x0048);
        LCD_WR_DATA(0x0008);
        LCD_WR_DATA(0x000F);
        LCD_WR_DATA(0x000C);
        LCD_WR_DATA(0x0031);
        LCD_WR_DATA(0x0036);
        LCD_WR_DATA(0x000f);
    //Frame Rate
    LCD_WR_REG(0x00B1);
        LCD_WR_DATA(0x0000);
        LCD_WR_DATA(0x0001);
    //Interface Control
    LCD_WR_REG(0x00f6);
        LCD_WR_DATA(0x0001);
        LCD_WR_DATA(0x0010);
        LCD_WR_DATA(0x0000);
    //Disable Internal Sleep
    LCD_WR_REG(0x0011);
    //Allow 120ms time for LCD to wake up
    Delay_Ms(120);
    //Turn on display drivers
    LCD_WR_REG(0x0029);
    //And clear the display
    LCD_Clear(LCD_BLACK);
}


unsigned short LCD_MakeColour( unsigned int R, unsigned int G, unsigned int B ) {
    unsigned short colour;
    //Limit the colours to the maximum range
    if (R > 0x1F) R = 0x1F;
    if (G > 0x1F) G = 0x1F;
    if (B > 0x1F) B = 0x1F;
    //Move the RGB values to the correct place in the encoded colour
    colour = (R << 11) + (G << 6) + (B << 0);
    return colour;
}

void LCD_Window( unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height) {
    unsigned int xright, ybottom;
    //Calculate bottom right corner location
    xright = xleft + width - 1;
    ybottom = ytop + height - 1;
    //Ensure end coordinates are in range
    if (xright >= LCD_WIDTH) xright = LCD_WIDTH-1;
    if (ybottom >= LCD_HEIGHT) ybottom = LCD_HEIGHT-1;
    //Ensure start coordinates are in range (top left must be <= bottom right)
    if (xleft > xright) xleft = xright;
    if (ytop > ybottom) ytop = ybottom;
    //Define the left and right of the display
    LCD_WR_REG(0x002A);
		LCD_WR_DATA((xleft >> 8) & 0xFF);
		LCD_WR_DATA(xleft & 0xFF);
		LCD_WR_DATA((xright >> 8) & 0xFF);
		LCD_WR_DATA(xright & 0xFF);
    //Define the top and bottom of the display
    LCD_WR_REG(0x002B);
		LCD_WR_DATA((ytop >> 8) & 0xFF);
		LCD_WR_DATA(ytop & 0xFF);
		LCD_WR_DATA((ybottom >> 8) & 0xFF);
		LCD_WR_DATA(ybottom & 0xFF);
    //Create window and prepare for data
    LCD_WR_REG(0x002c);
}

//Internal function to generate Red/Green corner of test pattern
void LCD_RedGreen(unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height){
	unsigned int i, j;
    unsigned short colour;
    //Define Window
    LCD_Window(xleft,ytop,width,height);
    //Create test pattern
    for (j = 0;j < height;j++){
        for (i = 0;i < width;i++){
            colour = LCD_MakeColour((i * 0x20)/width, (j * 0x20)/height, 0);
            LCD_WR_DATA(colour);
        }
    }
    ResetWDT();
}

//Internal function to generate Green/Blue corner of test pattern
void LCD_GreenBlue(unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height){
	unsigned int i, j;
    unsigned short colour;
    //Define Window
    LCD_Window(xleft,ytop,width,height);
    //Create test pattern
    for (j = 0;j < height;j++){
        for (i = 0;i < width;i++){
            colour = LCD_MakeColour(0, (i * 0x20)/width, (j * 0x20)/height);
            LCD_WR_DATA(colour);
        }
    }
    ResetWDT();
}


void LCD_BlueRed(unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height){
	unsigned int i, j;
    unsigned short colour;
    //Define Window
    LCD_Window(xleft,ytop,width,height);
    //Create test pattern
    for (j = 0;j < height;j++){
        for (i = 0;i < width;i++){
            colour = LCD_MakeColour((j * 0x20)/height, 0, (i * 0x20)/width);
            LCD_WR_DATA(colour);
        }
    }
    ResetWDT();
}

void LCD_ColourBars(unsigned int xleft, unsigned int ytop, unsigned int width, unsigned int height){

	unsigned int i, j;
    unsigned int colourbars[6] = {LCD_RED,LCD_YELLOW,LCD_GREEN,LCD_CYAN,LCD_BLUE,LCD_MAGENTA};
    unsigned short colour;
    //Define Window
    LCD_Window(xleft,ytop,width,height);
    //Generate Colour Bars
    for (j = 0;j < height/2;j++){
        for (i = 0;i < width;i++){
            colour = LCD_MakeColour((i * 0x20)/width, (i * 0x20)/width, (i * 0x20)/width);
            LCD_WR_DATA(colour);
        }
    }
    //Generate tone shades
    for (j = height/2;j < height;j++){
        for (i = 0;i < width;i++){
            LCD_WR_DATA(colourbars[(i*6)/width]);
        }
    }
}


void LCD_TestPattern(){
    LCD_RedGreen  (          0,           0,LCD_WIDTH/2,LCD_HEIGHT/2);
    LCD_GreenBlue (          0,LCD_HEIGHT/2,LCD_WIDTH/2,LCD_HEIGHT/2);
    LCD_BlueRed   (LCD_WIDTH/2,           0,LCD_WIDTH/2,LCD_HEIGHT/2);
    LCD_ColourBars(LCD_WIDTH/2,LCD_HEIGHT/2,LCD_WIDTH/2,LCD_HEIGHT/2);
}

void LCD_Framebuffer(unsigned short framebuffer[], unsigned int length)
{
	while(length--)
	{
		LCD_WR_DATA(*(framebuffer++));
	}
}
