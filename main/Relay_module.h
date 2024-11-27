#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

#define RELAY_1_PIN         GPIO_NUM_19
#define RELAY_2_PIN         GPIO_NUM_18
#define RELAY_3_PIN         GPIO_NUM_2
#define RELAY_4_PIN         GPIO_NUM_27
#define RELAY_5_PIN         GPIO_NUM_26
#define RELAY_6_PIN         GPIO_NUM_25
#define RELAY_7_PIN         GPIO_NUM_33
#define RELAY_8_PIN         GPIO_NUM_32

#define TURN_ON             1
#define TURN_OFF            0

void Rleay_Init();
void Relay_Set(uint8_t relayNumber, bool State);

#endif // RELAY_MODULE_H
