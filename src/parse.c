// SPDX-FileCopyrightText: NONE
// SPDX-License-Identifier: Unlicense

char uwuify_char(char input) {
	switch (input) {
		case 'r':
		case 'l':
			return 'w';
			break;
		case 'R':
		case 'L':
			return 'W';
			break;
		default:
			return input;
			break;
	}
}
