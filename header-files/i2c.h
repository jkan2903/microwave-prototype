#ifndef I2C_H
#define I2C_H

#include <xc.h>

/**
 * @brief Toggles the colon character on the display.
 *
 * This function enables or disables the colon on a 7-segment display by 
 * sending a specific control byte over I2C (TWI) communication. 
 * The colon state is controlled by writing a specific value to the display's control register.
 *
 * @param toggle 
 *        - `1` to turn the colon ON.
 *        - `0` to turn the colon OFF.
 *
 * @details
 * - Sends the display address (`0x71`) followed by a command byte (`0x77`) to access the colon control register.
 * - Writes `0x10` to enable the colon or `0x00` to disable it.
 * - Stops communication using the TWI stop condition.
 *
 * @note 
 * This function assumes that the TWI interface is already initialized and ready for communication.
 */
void toggleColon(uint8_t toggle);

/**
 * @brief Updates the 7-segment display with a new value.
 *
 * This function sends a string of characters to a 7-segment display over 
 * I2C (TWI) communication. The display is cleared before writing the new value.
 *
 * @param new_value 
 *        Pointer to the string containing the value to be displayed. 
 *        Each character in the string represents a segment display position.
 * @param len 
 *        The number of characters to send to the display.
 *
 * @details
 * - Sends the display address (`0x71`) with write mode.
 * - Sends the clear display command (`0x76`) to reset the display before updating.
 * - Sends each character in the string one at a time to the display.
 * - Stops communication using the TWI stop condition.
 *
 * @note 
 * This function assumes:
 * - The TWI interface is already initialized and ready for communication.
 * - The length of `new_value` does not exceed the maximum displayable characters.
 * 
 * Example usage:
 * @code
 * char value[4] = "1234";
 * updateDisplay(value, 4); // Updates the display to show "1234".
 * @endcode
 */
void updateDisplay(char* data, uint8_t len);

#endif