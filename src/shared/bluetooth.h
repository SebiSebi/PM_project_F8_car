#ifndef BLUETOOTH_H__
#define BLUETOOTH_H__

#define CAR_HC_NAME 		"F8_CAR"
#define CAR_HC_PASS 		"7777"
#define CAR_HC_ADDRESS 		"98d3:31:fd4470"

#define BT_MAGIC_CH 	       'S'
#define BT_PAIR_DELAY 		5 	/* seconds */

/** 
 * These must be called before using BT communication in order to pair
 * the master device with the slave device. The master had been already
 * configured to pair only with the MAC address specified by @CAR_HC_ADDRESS.
 */
void BT_init_slave(void);
void BT_init_master(void);

/**
 * Receive a message ending in \r\n aka CR LF. A null character is appended
 * by default at the end of @buf. If LEN_MAX - 1 characters are received,
 * the reception stops but the null character is still inserted at the end
 * (the remaining characters are not read from USART).
 */
void BT_get(char * const buf, unsigned int LEN_MAX = 64);

/**
 * Send a string to the paired device. @buf must be a null-terminated string.
 * Stop characters (e.g. \r\n) should not be included in @buf. They are send
 * automatically as a stop sequence.
 */
void BT_put(const char *buf);

#endif 	/* BLUETOOTH_H__ */
