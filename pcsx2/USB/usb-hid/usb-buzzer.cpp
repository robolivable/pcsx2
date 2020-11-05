/*
 * QEMU USB HID devices
 * 
 * Copyright (c) 2005 Fabrice Bellard
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "vl.h"
#include <windows.h>
#include <setupapi.h>
extern "C" {
#include "../ddk/hidsdi.h"
}

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "ddk/hid.lib")

#if 0
/* HID interface requests */
#define GET_REPORT 0xa101
#define GET_IDLE 0xa102
#define GET_PROTOCOL 0xa103
#define SET_IDLE 0x210a
#define SET_PROTOCOL 0x210b

#define USB_MOUSE 1
#define USB_TABLET 2

typedef struct USBKeyboardState {
    USBDevice dev;
	int keyboard_grabbed;
} USBKeyboardState;

extern HWND gsWnd;

#define VK_BASED

#ifdef VK_BASED
static const uint8_t vk_to_key_code[] = {
0x00, //FAIL: 0x00
0x00, //FAIL: LMOUSE
0x00, //FAIL: RMOUSE
0x00, //FAIL: Break
0x00, //FAIL: MMOUSE
0x00, //FAIL: X1MOUSE
0x00, //FAIL: X2MOUSE
0x00, //FAIL: 0x00
0x2A, //OK: Backspace
0x2B, //OK: Tab
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x9C, //OK: Clear
0x28, //FAIL: ENTER
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: SHIFT
0x00, //FAIL: CTRL
0x00, //FAIL: ALT
0x48, //OK: Pause
0x39, //OK: Caps Lock
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
//0x00, //FAIL: 0x00
//0x00, //FAIL: 0x00
//0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x29, //FAIL: ESC
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x2C, //OK: Spacebar
#ifdef ENABLE_KEYPAD_Fx
0x4B, //FAIL: PAGE UP
0x4E, //FAIL: PAGE DOWN
0x4D, //OK: End
0x4A, //OK: Home
0x50, //FAIL: LEFT ARROW
0x52, //FAIL: UP ARROW
0x4F, //FAIL: RIGHT ARROW
0x51, //FAIL: DOWN ARROW
0x77, //OK: Select
0x00, //FAIL: PRINT
0x74, //OK: Execute
0x46, //FAIL: PRINT SCREEN
0x49, //FAIL: INS
0x4C, //FAIL: DEL
0x75, //OK: Help VK_HOME
#else
0x00, //FAIL: PAGE UP
0x00, //FAIL: PAGE DOWN
0x00, //OK: End
0x00, //OK: Home
0x00, //FAIL: LEFT ARROW
0x00, //FAIL: UP ARROW
0x00, //FAIL: RIGHT ARROW
0x00, //FAIL: DOWN ARROW
0x00, //OK: Select
0x00, //FAIL: PRINT
0x00, //OK: Execute
0x00, //FAIL: PRINT SCREEN
0x00, //FAIL: INS
0x00, //FAIL: DEL
0x00, //OK: Help VK_HOME
#endif
0x27, //OK: 0
0x1E, //OK: 1
0x1F, //OK: 2
0x20, //OK: 3
0x21, //OK: 4
0x22, //OK: 5
0x23, //OK: 6
0x24, //OK: 7
0x25, //OK: 8
0x26, //OK: 9
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: not found
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x04, //OK: A
0x05, //OK: B
0x06, //OK: C
0x07, //OK: D
0x08, //OK: E
0x09, //OK: F
0x0A, //OK: G
0x0B, //OK: H
0x0C, //OK: I
0x0D, //OK: J
0x0E, //OK: K
0x0F, //OK: L
0x10, //OK: M
0x11, //OK: N
0x12, //OK: O
0x13, //OK: P
0x14, //OK: Q
0x15, //OK: R
0x16, //OK: S
0x17, //OK: T
0x18, //OK: U
0x19, //OK: V
0x1A, //OK: W
0x1B, //OK: X
0x1C, //OK: Y
0x1D, //OK: Z
#ifdef ENABLE_KEYPAD_Fx
0xE3, //OK: LGUI
0xE7, //OK: RGUI
0x65, //OK: Application
0x00, //FAIL: 0x00
0x00, //FAIL: SLEEP
0x62, //OK: Keypad 0
0x59, //OK: Keypad 1
0x5A, //OK: Keypad 2
0x5B, //OK: Keypad 3
0x5C, //OK: Keypad 4
0x5D, //OK: Keypad 5
0x5E, //OK: Keypad 6
0x5F, //OK: Keypad 7
0x60, //OK: Keypad 8
0x61, //OK: Keypad 9
0x55, //OK: Keypad *
0x57, //OK: Keypad +
0x9F, //OK: Separator
0x56, //OK: Keypad -
0x63, //OK: Keypad .
0x54, //OK: Keypad /
0x3A, //OK: F1
0x3B, //OK: F2
0x3C, //OK: F3
0x3D, //OK: F4
0x3E, //OK: F5
0x3F, //OK: F6
0x40, //OK: F7
0x41, //OK: F8
0x42, //OK: F9
0x43, //OK: F10
0x44, //OK: F11
0x45, //OK: F12
0x68, //OK: F13
0x69, //OK: F14
0x6A, //OK: F15
0x6B, //OK: F16
0x6C, //OK: F17
0x6D, //OK: F18
0x6E, //OK: F19
0x6F, //OK: F20
0x70, //OK: F21
0x71, //OK: F22
0x72, //OK: F23
0x73, //OK: F24
#else
0x00, //OK: LGUI
0x00, //OK: RGUI
0x00, //OK: Application
0x00, //FAIL: 0x00
0x00, //FAIL: SLEEP
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,
#endif
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x83, //OK: NUM LOCK
0x47, //OK: Scroll Lock
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0xE1, //OK: LSHIFT
0xE5, //OK: RSHIFT
0xE0, //OK: LCONTROL
0xE4, //OK: RCONTROL
0xE3, //OK: LGUI
0xE7, //OK: RGUI
0x00, //FAIL: Windows 2000/XP: Browser Back
0x00, //FAIL: Windows 2000/XP: Browser Forward
0x00, //FAIL: Windows 2000/XP: Browser Refresh
0x00, //FAIL: Windows 2000/XP: Browser Stop
0x00, //FAIL: Windows 2000/XP: Browser Search 
0x00, //FAIL: Windows 2000/XP: Browser Favorites
0x00, //FAIL: Windows 2000/XP: Browser Start and Home
0x00, //FAIL: Windows 2000/XP: Volume Mute
0x00, //FAIL: Windows 2000/XP: Volume Down
0x00, //FAIL: Windows 2000/XP: Volume Up
0x00, //FAIL: Windows 2000/XP: Next Track
0x00, //FAIL: Windows 2000/XP: Previous Track
0x00, //FAIL: Windows 2000/XP: Stop Media
0x00, //FAIL: Windows 2000/XP: Play/Pause Media
0x00, //FAIL: Windows 2000/XP: Start Mail
0x00, //FAIL: Windows 2000/XP: Select Media
0x00, //FAIL: Windows 2000/XP: Start Application 1
0x00, //FAIL: Windows 2000/XP: Start Application 2
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x33, //FAIL: Windows 2000/XP: For the US standard keyboard, the ';:' key 
0x2E, //FAIL: Windows 2000/XP: For any country/region, the '+'
0x36, //FAIL: Windows 2000/XP: For any country/region, the ','
0x2D, //FAIL: Windows 2000/XP: For any country/region, the '-'
0x37, //FAIL: Windows 2000/XP: For any country/region, the '.'
0x38, //FAIL: Windows 2000/XP: For the US standard keyboard, the '/?' key 
0x35, //FAIL: Windows 2000/XP: For the US standard keyboard, the '`~' key 
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: not found
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x2F, //FAIL: Windows 2000/XP: For the US standard keyboard, the '[{' key
0x31, //FAIL: Windows 2000/XP: For the US standard keyboard, the '\|' key
0x30, //FAIL: Windows 2000/XP: For the US standard keyboard, the ']}' key
0x34, //FAIL: Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
0x00, //FAIL: Used for miscellaneous characters; it can vary byboard.
0x00, //FAIL: Reserved
0x00, //FAIL: OEM specific
0x32, //FAIL: Windows 2000/XP: Either the angle bracket or the backslash key on the RT 102-key keyboard
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCEE6
0x00, //FAIL: OEM specific
0x00, //FAIL: Windows 2000/XP: Used to pass Unicode characters as if they E8
0x00, //FAIL: Unassigned
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: 0x00
0x00, //FAIL: Attn
0xA3, //OK: CrSel
0xA4, //OK: ExSel
0x00, //FAIL: Erase EOF
0x00, //FAIL: Play
0x00, //FAIL: Zoom
0x00, //FAIL: Reserved 
0x00, //FAIL: PA1
0x9C, //OK: Clear
0x00, //FAIL: 0x00
};
#else
#ifdef ENABLE_KEYPAD_Fx
static const unsigned char scan_to_usb[] = {
0x00,0x29,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x2f,0x30,0x2a,0x2b,
0x14,0x1a,0x08,0x15,0x17,0x1c,0x18,0x0c,0x12,0x13,0x33,0x2e,0x28,0xe0,0x04,0x16,
0x07,0x09,0x0a,0x0b,0x0d,0x0e,0x0f,0x35,0x34,0x31,0xe1,0x38,0x1d,0x1b,0x06,0x19,
0x05,0x11,0x10,0x36,0x37,0x2d,0xe5,0x55,0xe3,0x2c,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,
0x3f,0x40,0x41,0x42,0x43,0x83,0x47,0x5f,0x60,0x61,0x56,0x5c,0x5d,0x5e,0x57,0x59,
0x5a,0x5b,0x62,0x63,0x46,0x00,0x32,0x44,0x45,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x75,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe4,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x54,0x00,0x00,0xe7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe3,0xe7,0x65,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
#else
static const unsigned char scan_to_usb[] = {
0x00,0x29,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x2f,0x30,0x2a,0x2b,
0x14,0x1a,0x08,0x15,0x17,0x1c,0x18,0x0c,0x12,0x13,0x33,0x2e,0x28,0xe0,0x04,0x16,
0x07,0x09,0x0a,0x0b,0x0d,0x0e,0x0f,0x35,0x34,0x31,0xe1,0x38,0x1d,0x1b,0x06,0x19,
0x05,0x11,0x10,0x36,0x37,0x2d,0xe5,0x00,0xe3,0x2c,0x39,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x83,0x47,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x46,0x00,0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x75,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe4,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe3,0xe7,0x65,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x48,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
#endif
#endif

/* mostly the same values as the Bochs USB Keyboard device */
static const uint8_t qemu_keyboard_dev_descriptor[] = {
	0x12,       /*  u8 bLength; */
	0x01,       /*  u8 bDescriptorType; Device */
	0x10, 0x00, /*  u16 bcdUSB; v1.0 */

	0x00,	    /*  u8  bDeviceClass; */
	0x00,	    /*  u8  bDeviceSubClass; */
	0x00,       /*  u8  bDeviceProtocol; [ low/full speeds only ] */
	0x08,       /*  u8  bMaxPacketSize0; 8 Bytes */

//	0x27, 0x06, /*  u16 idVendor; */
	0x4C, 0x05,
 //	0x01, 0x00, /*  u16 idProduct; */
	0x00, 0x10,
	0x00, 0x00, /*  u16 bcdDevice */

	0x03,       /*  u8  iManufacturer; */
	0x02,       /*  u8  iProduct; */
	0x01,       /*  u8  iSerialNumber; */
	0x01        /*  u8  bNumConfigurations; */
};

static const uint8_t qemu_keyboard_config_descriptor[] = {
	/* one configuration */
	0x09,       /*  u8  bLength; */
	0x02,       /*  u8  bDescriptorType; Configuration */
	0x22, 0x00, /*  u16 wTotalLength; */
	0x01,       /*  u8  bNumInterfaces; (1) */
	0x01,       /*  u8  bConfigurationValue; */
	0x04,       /*  u8  iConfiguration; */
	0xa0,       /*  u8  bmAttributes; 
				 Bit 7: must be set,
				     6: Self-powered,
				     5: Remote wakeup,
				     4..0: resvd */
	50,         /*  u8  MaxPower; */
      
	/* USB 1.1:
	 * USB 2.0, single TT organization (mandatory):
	 *	one interface, protocol 0
	 *
	 * USB 2.0, multiple TT organization (optional):
	 *	two interfaces, protocols 1 (like single TT)
	 *	and 2 (multiple TT mode) ... config is
	 *	sometimes settable
	 *	NOT IMPLEMENTED
	 */

	/* one interface */
	0x09,       /*  u8  if_bLength; */
	0x04,       /*  u8  if_bDescriptorType; Interface */
	0x00,       /*  u8  if_bInterfaceNumber; */
	0x00,       /*  u8  if_bAlternateSetting; */
	0x01,       /*  u8  if_bNumEndpoints; */
	0x03,       /*  u8  if_bInterfaceClass; */
	0x01,       /*  u8  if_bInterfaceSubClass; */
	0x01,       /*  u8  if_bInterfaceProtocol; [usb1.1 or single tt] */
	0x05,       /*  u8  if_iInterface; */
     
        /* HID descriptor */
        0x09,        /*  u8  bLength; */
        0x21,        /*  u8 bDescriptorType; */
        0x01, 0x00,  /*  u16 HID_class */
        0x00,        /*  u8 country_code */
        0x01,        /*  u8 num_descriptors */
        0x22,        /*  u8 type; Report */
        50, 0,       /*  u16 len */

	/* one endpoint (status change endpoint) */
	0x07,       /*  u8  ep_bLength; */
	0x05,       /*  u8  ep_bDescriptorType; Endpoint */
	0x81,       /*  u8  ep_bEndpointAddress; IN Endpoint 1 */
 	0x03,       /*  u8  ep_bmAttributes; Interrupt */
 	0x03, 0x00, /*  u16 ep_wMaxPacketSize; */
	0x0a,       /*  u8  ep_bInterval; (255ms -- usb 2.0 spec) */
};

static const uint8_t qemu_tablet_config_descriptor[] = {
	/* one configuration */
	0x09,       /*  u8  bLength; */
	0x02,       /*  u8  bDescriptorType; Configuration */
	0x22, 0x00, /*  u16 wTotalLength; */
	0x01,       /*  u8  bNumInterfaces; (1) */
	0x01,       /*  u8  bConfigurationValue; */
	0x04,       /*  u8  iConfiguration; */
	0xa0,       /*  u8  bmAttributes; 
				 Bit 7: must be set,
				     6: Self-powered,
				     5: Remote wakeup,
				     4..0: resvd */
	50,         /*  u8  MaxPower; */
      
	/* USB 1.1:
	 * USB 2.0, single TT organization (mandatory):
	 *	one interface, protocol 0
	 *
	 * USB 2.0, multiple TT organization (optional):
	 *	two interfaces, protocols 1 (like single TT)
	 *	and 2 (multiple TT mode) ... config is
	 *	sometimes settable
	 *	NOT IMPLEMENTED
	 */

	/* one interface */
	0x09,       /*  u8  if_bLength; */
	0x04,       /*  u8  if_bDescriptorType; Interface */
	0x00,       /*  u8  if_bInterfaceNumber; */
	0x00,       /*  u8  if_bAlternateSetting; */
	0x01,       /*  u8  if_bNumEndpoints; */
	0x03,       /*  u8  if_bInterfaceClass; */
	0x01,       /*  u8  if_bInterfaceSubClass; */
	0x02,       /*  u8  if_bInterfaceProtocol; [usb1.1 or single tt] */
	0x05,       /*  u8  if_iInterface; */

        /* HID descriptor */
        0x09,        /*  u8  bLength; */
        0x21,        /*  u8 bDescriptorType; */
        0x03, 0x00,  /*  u16 HID_class */
        0x00,        /*  u8 country_code */
        0x01,        /*  u8 num_descriptors */
        0x22,        /*  u8 type; Report */
        74, 0,       /*  u16 len */

	/* one endpoint (status change endpoint) */
	0x07,       /*  u8  ep_bLength; */
	0x05,       /*  u8  ep_bDescriptorType; Endpoint */
	0x81,       /*  u8  ep_bEndpointAddress; IN Endpoint 1 */
 	0x03,       /*  u8  ep_bmAttributes; Interrupt */
 	0x08, 0x00, /*  u16 ep_wMaxPacketSize; */
	0x03,       /*  u8  ep_bInterval; (255ms -- usb 2.0 spec) */
};

static const uint8_t qemu_keyboard_hid_report_descriptor[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard Left Control)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

#define BUZZER_VID 0x054C
#define BUZZER_PID 0x1000
#define BUZZER_PID2 0x0002

unsigned char comp_data[6];
HANDLE usb_buzzer=(HANDLE)-1;
HANDLE readData=(HANDLE)-1;
OVERLAPPED ovl;

static int usb_keyboard_poll(USBKeyboardState *s, uint8_t *buf, int len)
{
	unsigned char data[6];

	ReadFile(usb_buzzer, data, 6, 0, &ovl);

	if(!memcmp(data, comp_data, 6) || (data[0]!=0x00 && data[1]!=0x7F && data[2]!=0x7F)){
		//No event from buzzers
	//	CancelIo(usb_buzzer);
		return USB_RET_STALL;
	}else{
		//We got an event !!!
		memset(buf, 0, 6);
		memcpy(comp_data, data, 6);

		buf[0]=0x7F;
		buf[1]=0x7F;
		buf[2]=data[3];
		buf[3]=data[4];
		buf[4]=data[5]|0xF0;

		return 16;
	}

	return 16;
}

static void usb_keyboard_handle_reset(USBDevice *dev)
{
    USBKeyboardState *s = (USBKeyboardState *)dev;
}

static int usb_keyboard_handle_control(USBDevice *dev, int request, int value,
                                  int index, int length, uint8_t *data)
{
    USBKeyboardState *s = (USBKeyboardState *)dev;
    int ret = 0;
	unsigned char buf[8];

    switch(request) {
    case DeviceRequest | USB_REQ_GET_STATUS:
        data[0] = (1 << USB_DEVICE_SELF_POWERED) |
            (dev->remote_wakeup << USB_DEVICE_REMOTE_WAKEUP);
        data[1] = 0x00;
        ret = 2;
        break;
    case DeviceOutRequest | USB_REQ_CLEAR_FEATURE:
        if (value == USB_DEVICE_REMOTE_WAKEUP) {
            dev->remote_wakeup = 0;
        } else {
            goto fail;
        }
        ret = 0;
        break;
    case DeviceOutRequest | USB_REQ_SET_FEATURE:
        if (value == USB_DEVICE_REMOTE_WAKEUP) {
            dev->remote_wakeup = 1;
        } else {
            goto fail;
        }
        ret = 0;
        break;
    case DeviceOutRequest | USB_REQ_SET_ADDRESS:
        dev->addr = value;
        ret = 0;
        break;
    case DeviceRequest | USB_REQ_GET_DESCRIPTOR:
        switch(value >> 8) {
        case USB_DT_DEVICE:
            memcpy(data, qemu_keyboard_dev_descriptor, 
                   sizeof(qemu_keyboard_dev_descriptor));
            ret = sizeof(qemu_keyboard_dev_descriptor);
            break;
        case USB_DT_CONFIG:
			memcpy(data, qemu_keyboard_config_descriptor, 
				   sizeof(qemu_keyboard_config_descriptor));
			ret = sizeof(qemu_keyboard_config_descriptor);
            break;
        case USB_DT_STRING:
            switch(value & 0xff) {
            case 0:
                /* language ids */
                data[0] = 4;
                data[1] = 3;
                data[2] = 0x09;
                data[3] = 0x04;
                ret = 4;
                break;
            case 1:
                /* serial number */
                ret = set_usb_string(data, "1");
                break;
            case 2:
                /* product description */
			    ret = set_usb_string(data, "Generic USB Keyboard");
                break;
            case 3:
                /* vendor description */
                ret = set_usb_string(data, "PCSX2/QEMU");
                break;
            case 4:
                ret = set_usb_string(data, "HID Keyboard");
                break;
            case 5:
                ret = set_usb_string(data, "Endpoint1 Interrupt Pipe");
                break;
            default:
                goto fail;
            }
            break;
        default:
            goto fail;
        }
        break;
    case DeviceRequest | USB_REQ_GET_CONFIGURATION:
        data[0] = 1;
        ret = 1;
        break;
    case DeviceOutRequest | USB_REQ_SET_CONFIGURATION:
        ret = 0;
        break;
    case DeviceRequest | USB_REQ_GET_INTERFACE:
        data[0] = 0;
        ret = 1;
        break;
    case DeviceOutRequest | USB_REQ_SET_INTERFACE:
        ret = 0;
        break;
        /* hid specific requests */
    case InterfaceRequest | USB_REQ_GET_DESCRIPTOR:
        switch(value >> 8) {
        case 0x22:
			memcpy(data, qemu_keyboard_hid_report_descriptor, 
				   sizeof(qemu_keyboard_hid_report_descriptor));
			ret = sizeof(qemu_keyboard_hid_report_descriptor);
	    break;
        default:
            goto fail;
        }
        break;
    case GET_REPORT:
	    ret = usb_keyboard_poll(s, data, length);
        break;
	case 0x2109:
		ret=0;
		memset(buf, 0, 8);
		buf[2]=data[1];
		buf[3]=data[2];
		buf[4]=data[3];
		buf[5]=data[4];
		CancelIo(usb_buzzer);
		WriteFile(usb_buzzer, buf, 8, 0, &ovl);
		break;
    case SET_IDLE:
        ret = 0;
        break;
    default:
    fail:
        ret = USB_RET_STALL;
        break;
    }
    return ret;
}

static int usb_keyboard_handle_data(USBDevice *dev, int pid, 
                                 uint8_t devep, uint8_t *data, int len)
{
    USBKeyboardState *s = (USBKeyboardState *)dev;
    int ret = 0;
	unsigned char buf[8];

    switch(pid) {
    case USB_TOKEN_IN:
        if (devep == 1) {
			ret = usb_keyboard_poll(s, data, len);
        } else {
            goto fail;
        }
        break;
    case USB_TOKEN_OUT:
		ret=0;
		memset(buf, 0, 8);
		buf[2]=data[1];
		buf[3]=data[2];
		buf[4]=data[3];
		buf[5]=data[4];
		CancelIo(usb_buzzer);
		WriteFile(usb_buzzer, buf, 8, 0, &ovl);
		break;
    default:
    fail:
        ret = USB_RET_STALL;
        break;
    }
    return ret;
}

static void usb_keyboard_handle_destroy(USBDevice *dev)
{
    USBKeyboardState *s = (USBKeyboardState *)dev;

    //qemu_add_keyboard_event_handler(NULL, NULL, 0);
    free(s);
	CloseHandle(usb_buzzer);
}

USBDevice *usb_keyboard_init(void)
{
    USBKeyboardState *s;

    s = (USBKeyboardState *)malloc(sizeof(USBKeyboardState));
    if (!s)
        return NULL;
    memset(s,0,sizeof(USBKeyboardState));
    s->dev.speed = USB_SPEED_FULL;
    s->dev.handle_packet = usb_generic_handle_packet;

    s->dev.handle_reset = usb_keyboard_handle_reset;
    s->dev.handle_control = usb_keyboard_handle_control;
    s->dev.handle_data = usb_keyboard_handle_data;
    s->dev.handle_destroy = usb_keyboard_handle_destroy;

    strncpy(s->dev.devname, "Generic USB Keyboard", sizeof(s->dev.devname));

	int i=0;
	DWORD needed=0;
	unsigned char buf[8];
	HDEVINFO devInfo;
	GUID guid;
	SP_DEVICE_INTERFACE_DATA diData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA didData;
	HIDD_ATTRIBUTES attr;

	readData=CreateEvent(0, 0, 0, 0);
	memset(&ovl, 0, sizeof(OVERLAPPED));
	ovl.hEvent=readData;
	ovl.Offset=0;
	ovl.OffsetHigh=0;

	HidD_GetHidGuid(&guid);
	
	devInfo=SetupDiGetClassDevs(&guid, 0, 0, DIGCF_DEVICEINTERFACE);
	if(!devInfo)return 0;
	
	diData.cbSize=sizeof(diData);

	while(SetupDiEnumDeviceInterfaces(devInfo, 0, &guid, i, &diData)){
		if(usb_buzzer!=INVALID_HANDLE_VALUE)CloseHandle(usb_buzzer);

		SetupDiGetDeviceInterfaceDetail(devInfo, &diData, 0, 0, &needed, 0);

		didData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(needed);
		didData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if(!SetupDiGetDeviceInterfaceDetail(devInfo, &diData, didData, needed, 0, 0)){
			free(didData);
			break;
		}

		usb_buzzer=CreateFile(didData->DevicePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		if(usb_buzzer==INVALID_HANDLE_VALUE){
			free(didData);
			i++;
			continue;
		}

		HidD_GetAttributes(usb_buzzer, &attr);


		if((attr.VendorID==BUZZER_VID) && (attr.ProductID==BUZZER_PID || attr.ProductID==BUZZER_PID2)){
			//We've found our buzzers !!!
			free(didData);
			
			memset(buf, 0, 8);
			buf[2]=0xFF;
			WriteFile(usb_buzzer, buf, 8, 0, &ovl);
			Sleep(100);

			memset(buf, 0, 8);
			buf[3]=0xFF;
			WriteFile(usb_buzzer, buf, 8, 0, &ovl);
			Sleep(100);

			memset(buf, 0, 8);
			buf[4]=0xFF;
			WriteFile(usb_buzzer, buf, 8, 0, &ovl);
			Sleep(100);

			memset(buf, 0, 8);
			buf[5]=0xFF;
			WriteFile(usb_buzzer, buf, 8, 0, &ovl);
			Sleep(100);

			memset(buf, 0, 8);
			WriteFile(usb_buzzer, buf, 8, 0, &ovl);
			break;
		}
		i++;
	}

    return (USBDevice *)s;
}
#endif
