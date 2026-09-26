// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#ifndef LIBUWU_H
#define LIBUWU_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/// @cond INTERNAL

typedef struct uwu_internal_instance_internal uwu_instance_internal;

/// @endcond

/**
 * @brief The breaking version the program is built against
 */
#define UWU_BREAKING_RELEASE_BUILT_AGAINST 2

/**
 * @brief Struct containing information, passed along to various functions, with
 * the exception of 'stutter_chance', do not modify manually.
 *
 * Must be initalized with 'uwu_init(&instance)', NEVER use an uninitalized
 * instance!
 */
typedef struct
{
	uwu_instance_internal *internal; // internal use only
	uint8_t stutter_chance;
	uint64_t rng;
	char *errwu;
} uwu_instance;

/**
 * @brief Information string describing the libuwu version.
 */
extern const char *uwu_INFO;

/**
 * @brief Version number of the libuwu library.
 *
 * [0] = breaking
 * [1] = release
 * [2] = patch
 *
 * As long as the breaking release is the same, code and binaries made for an
 * older release will be compatible with newer releases.
 */
extern const uint16_t uwu_VERSION[3];

/**
 * @brief Number of default replacements
 */
extern const uint8_t uwu_number_of_default_replacements;

/**
 * @brief Initalizes a uwu_instance.
 *
 * @param instance Pointer to a uwu_instance struct
 * @return 0 on success, 1 in failure with errwu being set (on the uninitalized
 * instance)
 */
uint8_t uwu_init(uwu_instance *instance);

/**
 * @brief Uninitalizes an uwu_instance.
 *
 * @param instance Pointer to a uwu_instance struct
 * @return Nothing is returned, function is guaranteed to succeed!
 */
void uwu_close(uwu_instance *instance);

/**
 * @brief Uwuifies text
 *
 * @param instance Pointer to a uwu_instance struct
 * @return An uwuified string on success and NULL on error with errwu being set
 * (see uwu_perrwu)
 */
char *uwu_uwuify(uwu_instance *instance, char *input);

/**
 * @brief Prints errwu information, simalarly to perror. Outputs only messsage
 * if errwu is unset.
 *
 * @param instance Pointer to a uwu_instance struct
 * @param message Message printed before error (simalarly to perror)
 * @return Nothing is returned, function is guaranteed to succeed!
 */
void uwu_perrwu(uwu_instance *instance, char *messsage);

/**
 * @brief Prints errwu information, simalarly to uwu_perrwu, but outputs to stream parameter
 * instead of stdout.
 *
 * @param instance Pointer to a uwu_instance struct
 * @param message Message printed before error (simalarly to perror)
 * @return Nothing is returned, function is guaranteed to succeed!
 */
void uwu_fperrwu(FILE *stream, uwu_instance *instance, char *messsage);

/**
 * @brief Sets the function used to generate a random number
 *
 * @param function Pointer to a uwu_instance struct, must match 'uint64_t name(uint64_t
 * *state)'.
 */
void uwu_rng_change(uwu_instance *instance, uint64_t (*func)(uint64_t *));

/**
 * @brief Removes item from replacement dictionary
 *
 * @param instance Pointer to a uwu_instance struct
 * @param key String containing key value of item to remove
 * @return 0 on success, 1 in failure with errwu being set
 */
uint8_t uwu_replacement_remove(uwu_instance *instance, const char *key);

/**
 * @brief Updates or adds item to replacement dictionary
 *
 * @param instance Pointer to a uwu_instance struct
 * @param key Text to replace
 * @param value What the text is replaced with
 * @return 0 on success, 1 in failure with errwu being set
 */
uint8_t uwu_replacement_update(uwu_instance *instance, const char *key, const char *value);

/**
 * @brief Loads defaults
 *
 * @param instance Pointer to a uwu_instance struct
 * @return 0 on success, the amount of defaults it failed to set on failure
 */
uint8_t uwu_replacement_load_defaults(uwu_instance *instance);

/**
 * @brief Gets value of key in replacement dictionary
 *
 * @param instance Pointer to a uwu_instance struct
 * @param key The key that is being looked up.
 * @return Returns the value or NULL if it is not found.
 */
char *uwu_replacement_get_value(uwu_instance *instance, const char *key);

#endif
