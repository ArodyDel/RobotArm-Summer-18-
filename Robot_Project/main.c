#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <constants.h>
#include <time.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/timer.h"


void button_setup(void);
void onButtonDown(void);
int coinFlip(void);
void placePiece(int);
int determine_winner(void);
void print_winner(void);
void humanTurn(void);
int whereCanRobotWin(void);
int whereCanHumanWin(void);
void gameOver(void);
void initializeGameBoard(void);
int blockHuman(void);
int robotWinningMove(void);
int findRandomOpenSpot(void);
void setupCenterTrap(int);
int rollRandom4(void);
void playOutTiedGame(void);
void findEmptyEdge(void);
void findEmptyCorner(void);
int whereCanPlayerWin(int player);
void force_sensor_setup(void);
float TrigSensor(void);
void displayH_OR_R_7SEG(int);
void flashWinner7seg(int);
void easyMode(void);
void challengeMode(void);
int findSetupForWin(void);
int rollRandom8(void);
int rollRandom9(void);
void humanVsHuman(void);
void humanVsRobot(void);


extern void movement_setup(void);
extern void moveBlock(int position);
extern void get_block_from_slide(int);
extern void initialize(void);
extern void test_slides_exhaustively(void);

unsigned char Mask7seg=0x01;
int count=-1;
int robotWonFlip;
int currentSlide = X;
int humanTurnDone = 0;
int gameBoard[9] = {-1, -1, -1,
                    -1, -1, -1,
                    -1, -1, -1};
int startNewGameSelected = 0;
uint32_t ui32Value;
uint32_t randseed;
int offset = 0;

int rightButtonCounter = 0;

int main(void)
{

    movement_setup();
    force_sensor_setup();

    randseed = 0;
    while(TrigSensor() > 10000){
        randseed++;
        if(randseed == 3000000000){
            randseed = 0;
        }
    }

    button_setup();



    robotWonFlip = coinFlip(); // coinFlip returning a 1 means robot won, and is assigned X in Tic Tac Toe, else is assigned O.

    //easyMode();

    if(robotWonFlip){ // robot goes first

      displayH_OR_R_7SEG(ROBOT);

      if(coinFlip()){ // if coinFlip == 1, robot plays first X in center of gameBoard
         placePiece(POS4);//turn 1
         humanTurn(); //turn 2
         int humanTurn2Pos = count;

        if(humanTurn2Pos==0||humanTurn2Pos==2||humanTurn2Pos==6||humanTurn2Pos==8)
        {
          int saveTurn3Move;
          if(humanTurn2Pos==8)
          {
          placePiece(POS0);
          saveTurn3Move=POS0;
          }
          else
          {
          placePiece(humanTurn2Pos+2);
          saveTurn3Move=humanTurn2Pos+2;
          }
          humanTurn();//turn4
          int humanTurn4Pos=0;
          humanTurn4Pos=count;
          if(humanTurn4Pos==0||humanTurn4Pos==2||humanTurn4Pos==6||humanTurn4Pos==8)
          {
            if(robotWinningMove())
              {
                    gameOver();
              }
            else
            {
              blockHuman();//turn 5
            }
            humanTurn();//turn 6
            if(findRandomOpenSpot()){
              gameOver();
            }//turn 7
            humanTurn();//turn 8
            findRandomOpenSpot();//turn 9
            gameOver();

          }
          else if(humanTurn4Pos==1||humanTurn4Pos==3||humanTurn4Pos==5||humanTurn4Pos==7)
          {
                if(humanTurn2Pos==0||humanTurn2Pos==2||humanTurn2Pos==6||humanTurn2Pos==8)
                {
                  if(robotWinningMove())
                  {
                    gameOver();
                  }
                  else{
                    blockHuman();
                  }

                  humanTurn();
                  if(robotWinningMove())
                  {
                    gameOver();
                  }


                }
                else{
                  setupCenterTrap(saveTurn3Move);
                }
                humanTurn();
                robotWinningMove();
                gameOver();
          }



        }

        else
        {
          placePiece(POS8);
        }
       humanTurn();//turn 4;

       if(robotWinningMove())
        {
                   gameOver();

       }
       else{
         blockHuman();
       }


       humanTurn();//turn 6

       if(robotWinningMove())
        {
                   gameOver();
        }
      }
      else{ // robot plays first X in position 0 (lower left corner of gameBoard)

         placePiece(POS6);//turn 1
         humanTurn();
         int humanTurn2Pos=0;
         humanTurn2Pos=count;


          if(humanTurn2Pos==1||humanTurn2Pos==7||humanTurn2Pos==8) //turn 2 played position 1,7,or8
             {

                 placePiece(POS0);
                 humanTurn();
                  //check can i win
               //if true
               if(gameBoard[3]!=O)
               {
                 placePiece(POS3);
                 gameOver();
                }

               //if not true
                else{ placePiece(POS8);}
                 humanTurn();
                 if(gameBoard[4]==O)
                 {
                 placePiece(POS7);
                 }
                 else{placePiece(POS4);}
                 gameOver();

             }
             else if(humanTurn2Pos==4) //turn 2 played center
             {
                 placePiece(2);//turn3
                 humanTurn();//turn4
                 if(gameBoard[0]==O)
                 {
                   placePiece(8);
                 }
                 else if(gameBoard[8]==O)
                 {
                   placePiece(0);
                 }
                 else
                 {
                   //block human
                   blockHuman();
                 }
                 humanTurn();//turn 6
                 if(robotWinningMove())
                 {
                   gameOver();
                 }
                 else
                 {
                   blockHuman();
                 }

                 humanTurn();//turn8
                 if(robotWinningMove())
                 {
                   gameOver();
                 }
                 else
                 {
                   blockHuman();
                 }
                gameOver();

             }
             else //turn 2 played any other spot
             {
                 placePiece(POS8);//turn 3
                 humanTurn();//turn 4
                //check can i win

               //if true
               if(gameBoard[7]!=O)
               {
                 placePiece(POS7);//turn 5
                 gameOver();
               }

               //if not true
                 placePiece(POS2);//turn 5
                 humanTurn();//turn 6
                 if(gameBoard[4]==O)
                 {
                 placePiece(POS5);//turn 7
                 }
                 placePiece(POS4);//turn 7
                 gameOver();
              }
      }
    }
    else{ // human goes first
        displayH_OR_R_7SEG(HUMAN);
        humanTurn();    //turn 1
        int humanTurn1 = count;
        if (humanTurn1 == 0 || humanTurn1 == 2 || humanTurn1 == 6 || humanTurn1 == 8) //human played corner
        {   //turn2
            placePiece(POS4);
            humanTurn();//turn 3
            if(!blockHuman())
            {
                //turn 4
                if (count == 1 ||  count == 7)
                {
                    if(gameBoard[count+1]==-1)
                    {
                        placePiece(count+1);
                    }
                    else
                    {
                        placePiece(count-1);
                    }

                }
                else if(count == 3||count==5)
                {
                    if(gameBoard[count+3]==-1)
                    {
                        placePiece(count+3);
                    }
                    else
                    {
                        placePiece(count-3);
                    }
                }
                else
                {
                    findEmptyEdge();

                }
                playOutTiedGame();
            }
            else
            {
                playOutTiedGame();
            }
        }
        else if (humanTurn1 == 1 || humanTurn1 == 7 || humanTurn1 == 3 || humanTurn1 == 5)//human played edge
        {
            //turn 2
            if (humanTurn1 == 3 || humanTurn1 == 5)
            {
                placePiece(humanTurn1 + 3);
            }
            else
            {
                placePiece(humanTurn1 + 1);
            }
            humanTurn(); //turn 3
            if(!blockHuman())
            {
                placePiece(POS4);
            }
            playOutTiedGame();

        }
        else //human played center
        {
            int randCorner=rollRandom4()*3;
            if (randCorner == 1 || randCorner == 3)
            {
                randCorner--;
            }
            placePiece(randCorner);//turn2

            humanTurn(); //turn 3
            //UARTprintf("end turn 3\n");
            if (blockHuman()) //turn 4
            {
                //UARTprintf("entering playOutTiedGame\n");
                playOutTiedGame();

            }
            else
            {
                //UARTprintf("entering findEmptyCorner\n");
                findEmptyCorner();//turn 4
                playOutTiedGame();
            }
        }




    }
    return 0;
}

// interrupt service routine
void onButtonDown(void) {



    if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_4) {       //sw1 button handler
//        GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
//        UARTprintf("\nI am in button 1\n");
        // PF4 was interrupt cause
        //UARTprintf("Button Down\n");
        //TrigSensor();
        // Configure PF4 for rising edge trigger

        //GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);

        if(count == 8){count = -1;}
        if(gameBoard[count + 1] == -1){
            count++;
        }
        else{
            int i = 0;
            count++;
            while(gameBoard[count] != -1){
                count++;
                i++;
                if(count > 8){count = -1;}
                if(i == 9) { break;}
            }
            if(i == 9) { initialize();}
        }

        switch (count)
        {
        case 0:
            Mask7seg=0x3F;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 1:
            Mask7seg=0x06;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 2:
            Mask7seg=0x5B;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 3:
            Mask7seg=0x4F;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 4:
            Mask7seg=0x66;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 5:
            Mask7seg=0x6D;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 6:
            Mask7seg=0x7D;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 7:
            Mask7seg=0x07;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 8:
            Mask7seg=0x7F;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        case 9:
            Mask7seg=0x67;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        default:
            Mask7seg=0x40;
            GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                         GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
            break;
        }

//        GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
        SysCtlDelay(4000000); //delay
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);  // Clear interrupt flag
        //humanTurnDone = 1;

    }
    else if (GPIOIntStatus(GPIO_PORTF_BASE, false) & GPIO_PIN_0) {       // sw2 interrupt handle
        //rightButtonCounter++;
//        UARTprintf("\n%d\n", rightButtonCounter);

//        GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
//        UARTprintf("\nI am at button 2\n");

        Mask7seg=0x40;
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                                                   GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );

        if(gameBoard[count] == -1){
            placePiece(count);
            humanTurnDone = 1;
        }


//        GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
        SysCtlDelay(4000000);
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);  // Clear interrupt flag

    }
}

void button_setup(void){
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 );


    // Pin F4 and F0 setup
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        // Enable port F
    HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;//Disable the device on F0
    HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= GPIO_PIN_0;

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 |GPIO_PIN_0);  // Init PF4 and PF0 as input
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 |GPIO_PIN_0,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);  // Enable weak pullup resistor for PF4

    // Interrupt setup
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);        // Disable interrupt for PF4 (in case it was enabled)
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);      // Clear pending interrupts for PF4
    GPIOIntRegister(GPIO_PORTF_BASE, onButtonDown);     // Register our handler function for port F
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0,
                   GPIO_FALLING_EDGE);             // Configure PF4 for falling edge trigger
//    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
}

float TrigSensor(void)
{
    ADCProcessorTrigger(ADC0_BASE,0);   //start sample
    while(!ADCIntStatus(ADC0_BASE,0, false));  // wait till the status changes
    {

    }
    ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value); // read data
    float FsrVolt=ui32Value*5/1023.0;
    float FsrResist=3270.0*(5/FsrVolt-1.0); //voltage divider using a 3270 ohm resistor
    float FsrG=1.0/FsrResist;
    float force;
    if(FsrResist<=600)
    {
        force=(FsrG-0.00075)/0.00000032639;
    }
    else
    {
        force=FsrG/0.000000642857;
    }
    float test = 65.0004;
    return FsrResist;
//    UARTprintf("Resistance: %d ohms \n",(int)FsrResist);
//    UARTprintf("Resistance: %d g\n",(int)force);
}

void force_sensor_setup(void){
    //ADC setup
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);    //connected to PE3
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    {

    }
    ADCSequenceConfigure(ADC0_BASE,0, ADC_TRIGGER_PROCESSOR,0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
                             ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
    ADCSequenceEnable(ADC0_BASE,0);
    ADCProcessorTrigger(ADC0_BASE,0);
    while(!ADCIntStatus(ADC0_BASE,0, false));
    {

    }
    ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);
}

int coinFlip(void){
      uint32_t shifter = 0x01 << offset;
      uint32_t randNum = (randseed & shifter) >> offset;
      offset++;
      if(offset == 29){offset = 0;}
      return randNum;
}

int rollRandom4(void)
{
      uint32_t shifter = 0x01 << offset;
      uint32_t bit0 = (randseed & shifter) >> offset;
      shifter = shifter << 1;
      uint32_t bit1 = (randseed & shifter) >> offset;
      offset++;
      if(offset == 29){offset = 0;}
      uint32_t randNum = bit0 + bit1;
      return randNum;
}

int rollRandom8(void)
{
      uint32_t shifter = 0x01 << offset;
      uint32_t bit0 = (randseed & shifter) >> offset;
      shifter = shifter << 1;
      uint32_t bit1 = (randseed & shifter) >> offset;
      shifter = shifter << 1;
      uint32_t bit2 = (randseed & shifter) >> offset;
      offset++;
      if(offset == 29){offset = 0;}
      uint32_t randNum = bit0 + bit1 + bit2;
      return randNum;
}

int rollRandom9(void){
    if(coinFlip() && coinFlip() && coinFlip()){
        return 8;
    }
    else{
        return rollRandom8();
    }
}

void placePiece(int pos){
    get_block_from_slide(currentSlide);
    moveBlock(pos);
    if(robotWonFlip == 1){
        gameBoard[pos] = currentSlide;
    }
    else{ //human won flip
        if(currentSlide == X){gameBoard[pos] = O;}
        else{gameBoard[pos] = X;}
    }
    if(currentSlide == X) {currentSlide = O;}
    else{ currentSlide = X;}
}

void print_winner(void){
  char c ;
  if(determine_winner() == X){c = 'X'; printf("%s\n", &c);}
  else if(determine_winner() == O){c = 'O'; printf("%s\n", &c);}
  else{printf("%d\n", determine_winner());}
}


void humanTurn(void){
    displayH_OR_R_7SEG(HUMAN);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    while(!humanTurnDone){ //wait on human to take their turn
        //GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    }
    GPIOIntDisable(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    humanTurnDone=0;     // reset flag
    displayH_OR_R_7SEG(ROBOT);
}

int determine_winner(void){

    int i;
    int winner = NO_WINNER;
    // check rows of gameBoard (3)
    for(i = 0; i < 3; i++){
        if(gameBoard[3*i] == gameBoard[3*i+1] && gameBoard[3*i+1] == gameBoard[3*i+2] && gameBoard[3*i] != -1){
            winner = gameBoard[3*i];
        }
    }

    // check columns of gameBoard (3)
    for(i = 0; i < 3; i++){
            if(gameBoard[i] == gameBoard[i + 3] && gameBoard[i + 3] == gameBoard[i + 6] && gameBoard[i] != -1){
                winner = gameBoard[i];
            }
    }

    // check diagonals of gameBoard (2)
    if(gameBoard[0] == gameBoard[4] && gameBoard[4] == gameBoard[8] && gameBoard[0] != -1){
        winner = gameBoard[0];
    }
    if(gameBoard[2] == gameBoard[4] && gameBoard[4] == gameBoard[6] && gameBoard[2] != -1){
            winner = gameBoard[2];
    }

    return winner;
}


int blockHuman(void)
{
         //UARTprintf("entering blockHuman\n");
         int check=whereCanPlayerWin(O);
         if(check!=-1)
         {
           placePiece(check);
           //UARTprintf("returned 1 from blockHuman\n");
           return 1;
         }
         //UARTprintf("returned 0 from blockHuman\n");
         return 0;
}
int robotWinningMove(void)
{
         int check=whereCanPlayerWin(X);
         if(check!=-1)
         {
           placePiece(check);
           return 1;
         }
         return 0;
}


void playOutTiedGame(void)
{
    humanTurn();//turn 5
    if (robotWinningMove() == 1)
    {
        //UARTprintf("robot won the game\n");
        //turn 6
        gameOver();
    }
    else
    {
        //UARTprintf("entering turn 6 (miracle on ice)\n");
        if (blockHuman() == 0)
        {
            //UARTprintf("turn 6 random\n");
            findRandomOpenSpot(); //turn 6
        }
        //UARTprintf("human turn 7\n");
        humanTurn();//turn 7

        if (robotWinningMove())
        {
            gameOver();
        }
        else
        {
            if(blockHuman() == 0)
            {
                findRandomOpenSpot();
            }
            humanTurn();
            gameOver();
        }


    }
}

//find randomOpenSpot checks if we can make a winning move overwise move towards a tied game
int findRandomOpenSpot(void)
{
  int check=whereCanPlayerWin(X);
  int i;
         if(check==-1)
         {
           for(i=0;i<9;i++)
           {
             if(gameBoard[i]==-1&& (i==1||i==3||i==5||i==7))
             {
               placePiece(i);
               return 0;
             }
           }
           for(i=0;i<9;i++)
           {
             if(gameBoard[i]==-1&& (i==0||i==2||i==6||i==8))
             {
               placePiece(i);
               return 0;
             }
           }
         }
         else
         {
           placePiece(check);
           return 1;
         }

}

void findEmptyEdge(void)
{
    //find a corner
    int i;
    for (i = 1; i < 9; i += 2)
    {
        if (gameBoard[i] == -1)
        {
            placePiece(i);
            break;
        }
    }
}

void findEmptyCorner(void)
{
    //find a corner
    int i;
    for (i = 0; i < 9; i += 2)
    {
        if (gameBoard[i] == -1)
        {
            placePiece(i);
            break;
        }
    }
}
void setupCenterTrap(int saveTurn3Move)
{
  if(saveTurn3Move==0 || saveTurn3Move==6)
                    {
                      if(gameBoard[saveTurn3Move+1]==-1)
                      {
                        placePiece(saveTurn3Move+2);
                      }
                      else
                      {
                        if(saveTurn3Move==0)
                        {
                          placePiece(POS6);
                        }
                        else
                        {
                          placePiece(POS0);
                        }
                      }

                    }

                    else{
                      if(gameBoard[saveTurn3Move-1]==-1)
                      {
                        placePiece(saveTurn3Move-2);
                      }
                      else
                      {
                        if(saveTurn3Move==2)
                        {
                          placePiece(POS8);
                        }
                        else
                        {
                          placePiece(POS2);
                        }
                      }

                    }

}
// Returns position 0-8 where X can win by playing an X or -1 if there is no winning move
int whereCanPlayerWin(int player){
  if(player != X && player != O) {return NO_WINNER;} // reject invalid argument
  int i, j;
  int winningPosition = NO_WINNER;
  // check rows of gameBoard (3)
  for(i = 0; i < 3; i++){
    if(gameBoard[3*i] + gameBoard[3*i+1] + gameBoard[3*i+2] == (2 * player - 1)){ // There are two Xs and one -1 in this row
      for(j = 3*i; j < 3*i + 3; j++){
        if(gameBoard[j] == -1){ winningPosition = j;}
      }
    }
  }

  // check columns of gameBoard (3)
  for(i = 0; i < 3; i++){
    if(gameBoard[i] + gameBoard[i + 3] + gameBoard[i + 6] == (2 * player - 1)){
      for(j = i; j < i + 7; j += 3){
        if(gameBoard[j] == -1){ winningPosition = j; }
      }
    }
  }

  // check diagonals of gameBoard (2)
  if(gameBoard[0] + gameBoard[4] + gameBoard[8] == (2 * player - 1)){
    for(j = 0; j < 9; j += 4){
      if(gameBoard[j] == -1){ winningPosition = j; }
    }
  }
  if(gameBoard[2] + gameBoard[4] + gameBoard[6] == (2 * player - 1)){
    for(j = 2; j < 7; j += 2){
      if(gameBoard[j] == -1){ winningPosition = j; }
    }
  }

  return winningPosition;
}

void gameOver(void){
  int winner = determine_winner();
  if(winner == -1){
      flashWinner7seg(TIE);
  }
  else if(winner == X){
      flashWinner7seg(ROBOT);
  }
  else if(winner == O){
      flashWinner7seg(HUMAN);
  }
  while(1){/*waiting for reset button press*/}
}

void initializeGameBoard(void){
  int i;
  for(i = 0; i < 9; i++){
    gameBoard[i] = -1;
  }
}

void displayH_OR_R_7SEG(int player){
    switch (player)
    {
    case HUMAN:
        Mask7seg=0x74;  // Display h on 7 seg when it's the human's turn
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                     GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
        break;
    case ROBOT:
        Mask7seg=0x50;  // Display r on 7 seg when it's the human's turn
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                     GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
        break;
    case TIE:
        Mask7seg=0x41;  // Display r on 7 seg when it's the human's turn
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
        break;
    default:
        break;
    }
}

void flashWinner7seg(int player){
    while(1){
        displayH_OR_R_7SEG(player);
        SysCtlDelay(2500000);
        Mask7seg=0x00;
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                     GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,Mask7seg );
        SysCtlDelay(2500000);
    }
}

void easyMode(void){

    int turnCount = 0;

    if(robotWonFlip){ // robot goes first
        displayH_OR_R_7SEG(ROBOT);
        placePiece(rollRandom9());
        turnCount++;
        while(1){
            if(turnCount == 9){gameOver();}
            if(determine_winner() != -1){
                gameOver();
            }
            humanTurn();

            turnCount++;
            if(turnCount == 9){gameOver();}
            if(determine_winner() != -1){
                gameOver();
            }
            if(robotWinningMove()){gameOver();}
            if(!blockHuman()){
                int setupPos = findSetupForWin();


                if(setupPos != -1){
                    placePiece(setupPos);
                }
                else{
                    findRandomOpenSpot();
                }
            }
            turnCount++;
        }


    }
    else{ //human goes first
        turnCount = 0;
        while(1){
            if(turnCount == 9){gameOver();}
            if(determine_winner() != -1){
                gameOver();
            }
            humanTurn();

            turnCount++;
            if(turnCount == 9){gameOver();}
            if(determine_winner() != -1){
                gameOver();
            }
            if(robotWinningMove()){gameOver();}
            if(!blockHuman()){
                int setupPos = findSetupForWin();


                if(setupPos != -1){
                    placePiece(setupPos);
                }
                else{
                    findRandomOpenSpot();
                }
            }
            turnCount++;


        }

    }

}




void challengeMode(void){

}

int findSetupForWin(void){
    int gameBoardCopy[9];
    int i;
    for(i = 0; i < 9; i++){
        gameBoardCopy[i] = gameBoard[i];
    }
    for(i = 0; i < 9; i++){
        if(gameBoard[i] == -1){
            gameBoard[i] = X;
            if(whereCanPlayerWin(X) != -1){
              int j;
              for(j = 0; j < 9; j++){
                  gameBoard[j] = gameBoardCopy[j];
              }
                return i;
            }
            else{
                gameBoard[i] = -1;
            }
        }
    }
    for(i = 0; i < 9; i++){
        gameBoard[i] = gameBoardCopy[i];
    }
    return -1;
}

