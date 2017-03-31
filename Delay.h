#ifndef DELAY_H_
#define DELAY_H_

//Delay for x microseconds
void usleep(int x);

//Delay for x milliseconds
void Delay_Ms(int x);

//Function to reset watchdog
void ResetWDT(void);

void utimer_irq(int);

#endif //DELAY_H_
