/*
 Copyright 2001, 2002 Georges Menie (www.menie.org)
 stdarg version contributed by Christian Ettinger

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 putchar is the only external dependency for this file,
 if you have a working putchar, leave it commented out.
 If not, uncomment the define below and
 replace outbyte(c) by your own function call.

 */

#include <stdarg.h>

#ifdef __thumb__
#define AngelSWI            0xAB
#define AngelSWIInsn        "bkpt"
#define AngelSWIAsm          bkpt
#else
#define AngelSWI            0x123456
#define AngelSWIInsn        "swi"
#define AngelSWIAsm          swi
#endif

#define SYS_WRITE			5

static int __semihost(int reason, const void *arg) {
//	int value;
//
//	asm volatile (
//			"mov r0, %1" "\n\t"
//			"mov r1, %2" "\n\t"
//			AngelSWIInsn " %a3" "\n\t"
//			"mov %0, r0"
//			: "=r" (value) /* output operands             */
//			: "r" (reason), "r" (arg), "i" (AngelSWI) /* input operands              */
//			: "r0", "r1", "r2", "r3", "ip", "lr", "memory", "cc" /* list of clobbered registers */
//	);
//
//	return value;
    register int r0 asm("r0") = reason;
    register const void *r1 asm("r1") = arg;

    asm volatile (
        AngelSWIInsn " %2"
        : "+r" (r0)
        : "r" (r1), "i" (AngelSWI)
        : "memory"
    );

    return r0;
}

static int semihost_write(const unsigned char *buffer, unsigned int length) {
	if (length == 0)
		return 0;

	unsigned int args[3];
	args[0] = 0;
	args[1] = (unsigned int) buffer;
	args[2] = (unsigned int) length;
	return __semihost(SYS_WRITE, args);
}

static int putchar(int ch) {
#ifdef DEBUG
#ifdef SERIAL_CONSOLE
	int UART0_PutChar(int);
	UART0_PutChar(ch);
	if (ch == '\n')
		UART0_PutChar('\r');
#else
	semihost_write((unsigned char*) &ch, 1);
#endif
#endif
	return ch;
}

static void printchar(char **str, int c) {
	if (str) {
		**str = (char) c;
		++(*str);
	} else {
		(void) putchar(c);
	}
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad) {
	register int pc = 0, padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr)
			++len;
		if (len >= width)
			width = 0;
		else
			width -= len;
		if (pad & PAD_ZERO)
			padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for (; width > 0; --width) {
			printchar(out, padchar);
			++pc;
		}
	}
	for (; *string; ++string) {
		printchar(out, *string);
		++pc;
	}
	for (; width > 0; --width) {
		printchar(out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int i, int b, int sg, int width, int pad,
		int letbase) {
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0, pc = 0;
	register unsigned int u = (unsigned int) i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints(out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = (unsigned int) -i;
	}

	s = print_buf + PRINT_BUF_LEN - 1;
	*s = '\0';

	while (u) {
		t = (unsigned int) u % b;
		if (t >= 10)
			t += letbase - '0' - 10;
		*--s = (char) (t + '0');
		u /= b;
	}

	if (neg) {
		if (width && (pad & PAD_ZERO)) {
			printchar(out, '-');
			++pc;
			--width;
		} else {
			*--s = '-';
		}
	}

	return pc + prints(out, s, width, pad);
}

static int print(char **out, const char *format, va_list args) {
	register int width, pad;
	register int pc = 0;
	char scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			++format;
			width = pad = 0;
			if (*format == '\0')
				break;
			if (*format == '%')
				goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				++format;
				pad |= PAD_ZERO;
			}
			for (; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if (*format == 'l') {
				++format;
			}
			if (*format == 's') {
				register char *s = (char*) va_arg(args, int);
				pc += prints(out, s ? s : "(null)", width, pad);
				continue;
			}
			if (*format == 'd') {
				pc += printi(out, va_arg(args, int), 10, 1, width, pad, 'a');
				continue;
			}
			if (*format == 'x') {
				pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'a');
				continue;
			}
			if (*format == 'X') {
				pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'A');
				continue;
			}
			if (*format == 'u') {
				pc += printi(out, va_arg(args, int), 10, 0, width, pad, 'a');
				continue;
			}
			if (*format == 'c') {
				/* char are converted to int then pushed on the stack */
				scr[0] = (char) va_arg(args, int);
				scr[1] = '\0';
				pc += prints(out, scr, width, pad);
				continue;
			}
		} else {
			out: printchar(out, *format);
			++pc;
		}
	}
	if (out)
		**out = '\0';
	va_end(args);
	return pc;
}

int printf(const char *format, ...) {
	va_list args;

	va_start(args, format);
	return print(0, format, args);
}

