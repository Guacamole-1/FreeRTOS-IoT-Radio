/**
 * @file rda5807m.h
 * @brief RDA5807M FM Radio Receiver Driver API.
 *
 * High-level driver to control the RDA5807M FM radio receiver
 * connected to the LPC1769 via I2C.
 *
 * Provides power control, frequency tuning, audio configuration,
 * signal strength monitoring and RDS enable.
 *
 * @version 1.0
 * @date 21 Jul 2025
 * @author PSampaio
 *
 * @note Software provided for educational purposes.
 * @warning Provided "AS IS", without warranties.
 */

#ifndef RDA5807M_H_
#define RDA5807M_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup DRIVERS
 * @{
 */

/**
 * @defgroup RDA5807M RDA5807M — FM Radio Receiver Driver
 * @brief Driver for the RDA5807M FM receiver IC.
 *
 * This module abstracts the radio hardware and provides control over:
 * - Power state
 * - Frequency tuning
 * - Volume control
 * - Mono/stereo selection
 * - Bass boost
 * - Mute control
 * - RSSI monitoring
 * - RDS enable
 * @{
 */

/**
 * @brief Initializes the RDA5807M radio receiver.
 *
 * @retval true  Device initialized successfully
 * @retval false Initialization failed
 */
bool RDA5807M_Init(void);

/** @brief Powers on the radio receiver. */
void RDA5807M_PowerOn(void);

/** @brief Powers off the radio receiver. */
void RDA5807M_PowerOff(void);

/** @brief Resets the radio to default state. */
void RDA5807M_Reset(void);

/**
 * @brief Sets the audio output volume.
 *
 * @param vol Volume level (0–15).
 */
void RDA5807M_Volume(uint8_t vol);

/**
 * @brief Enables or disables bass boost.
 *
 * @param on true = enable, false = disable.
 */
void RDA5807M_BassBoost(bool on);

/**
 * @brief Forces mono or stereo mode.
 *
 * @param on true = mono, false = stereo.
 */
void RDA5807M_Mono(bool on);

/**
 * @brief Checks if mono mode is active.
 *
 * @retval true  Mono
 * @retval false Stereo
 */
bool RDA5807M_IsMono(void);

/**
 * @brief Mutes or unmutes audio output.
 *
 * @param on true = mute, false = unmute.
 */
void RDA5807M_Mute(bool on);

/**
 * @brief Sets tuned frequency in MHz.
 *
 * @param freq Frequency in MHz (example: 101.7).
 */
void RDA5807M_SetFrequencyFloat(float freq);

/**
 * @brief Gets tuned frequency in MHz.
 *
 * @return Frequency in MHz.
 */
float RDA5807M_GetFrequencyFloat(void);

/**
 * @brief Gets the received signal strength.
 *
 * @return RSSI value (0–127).
 */
uint8_t RDA5807M_GetRSSI(void);


/** @} */  // RDA5807M
/** @} */  // DRIVERS

#ifdef __cplusplus
}
#endif

#endif /* RDA5807M_H_ */
