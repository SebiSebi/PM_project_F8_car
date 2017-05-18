#ifndef BLUETOOTH_H__
#define BLUETOOTH_H__

#define CAR_HC_NAME "F8_CAR"
#define CAR_HC_PASS "7777"
#define CAR_HC_ADDRESS "98d3:31:fd4470"

/** 
 * These must be called before using BT communication in order to pair
 * the master device with the slave device. The master had been already
 * configured to pair only with the MAC address specified by @CAR_HC_ADDRESS.
 */
void BT_init_slave(void);
void BT_init_master(void);


/* In order to use this function you have to boot the device in AT mode. */
void BT_list_config(void);

/**
 * Receive a message ending in \r\n aka CR LF. A null character is appended
 * by default at the end of @buf. If LEN_MAX - 1 characters are received,
 * the reception stops but the null character is still inserted at the end
 * (the remaining characters are not read from USART).
 */
void BT_get(char *buf, unsigned int LEN_MAX = 64);

#endif 	/* BLUETOOTH_H__ */
