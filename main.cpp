#include "mbed.h"
#include "N5110.h"

//vcc, sce, rst, dc, mosi, clk, led
N5110 lcd(p7,p8,p9,p10,p11,p13,p21); //pwm for led backlight

int nx = 84; //84 x axis pixels
int ny = 48; //48 y axis pixels
AnalogIn pot(p20);  //potentiometer
DigitalIn enable(p5);  //button
unsigned char next[84][48]; //buffer for next generation
AnalogIn ain(p15);

void clearCells()              //loop through cells, and clear
{
    for (int i = 0; i < nx ; i++) {
        for (int j = 0; j < ny ; j++) {
            lcd.clearPixel(i,j);
        }
    }
    lcd.refresh(); //must refresh to write buffer to display
}

//setting up definition of n/neighbours

int checkNeighbours(int i, int j)
{
    int n = 0; // number of neighbours

    if (lcd.getPixel(i-1,j-1))  // pixel to top-left
        n++;
    if (lcd.getPixel(i-1,j))    // pixel to left
        n++;
    if (lcd.getPixel(i-1,j+1))  // pixel to bottom-left
        n++;
    if (lcd.getPixel(i,j-1))    // pixel to top-left
        n++;
    if (lcd.getPixel(i,j+1))    // pixel to bottom
        n++;
    if (lcd.getPixel(i+1,j-1))  // pixel to top-right
        n++;
    if (lcd.getPixel(i+1,j))    // pixel to right
        n++;
    if (lcd.getPixel(i+1,j+1))  // pixel to bottom right
        n++;
    return(n);
}

int main()
{

    lcd.init(); // initialise the display

    //display intro screen

    lcd.printString("Welcome to",15,1); // display splash screen
    lcd.printString("Conway's",20,2);   // X pixel, Y bank
    lcd.printString("Game of Life",6,3);
    lcd.printString("Geoff Grevers",4,5);
    wait(2);
    clearCells();       // clears screen
    srand(ain); //change seed of random to the analogue in pin
    lcd.randomiseBuffer();
    lcd.drawRect(0,0,83,47,0);
    lcd.refresh();      //updates screen from buffer

    while(1) {

        int n;
        for (int i = 1; i < nx-1 ; i++) {
            for (int j = 1; j < ny-1 ; j++) {
                n = checkNeighbours(i,j);
                if (lcd.getPixel(i,j)) {  // if alive
                    next[i][j] = 1;
                    if ((n < 2) || (n > 3))
                        next[i][j] = 0;     //clears pixel if neighbours are less than 2 or more than 3
                } else {  // dead
                    next[i][j] = 0;
                    if (n == 3)
                        next[i][j] = 1;   //turns pixel on if it has exactly 3 neighbours
                }
            }
        }
        for (int i = 1; i < nx-1 ; i++) {
            for (int j = 1; j < ny-1 ; j++) {
                if (next[i][j])
                    lcd.setPixel(i,j);
                else
                    lcd.clearPixel(i,j);
            }
        }
        if(enable)  {     
            clearCells(); //resets
            lcd.printString("RESET",30,2);
            wait(2);
            lcd.randomiseBuffer();
            lcd.drawRect(0,0,83,47,0);
        }
        lcd.refresh();
        wait(pot);        //this wait enables each cycle of the game to be viewed, and can be adjusted using the potentiometer
    }
    
}