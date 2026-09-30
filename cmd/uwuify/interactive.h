// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

#ifndef INTERACTIVE_H
#define INTERACTIVE_H

extern uwu_instance instance; // extern'd here
void handle_keyboard_exit(int sig);
int interactive_mode(void);

#endif
