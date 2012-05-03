/* Shim added to the 'bastard' disassembler to implement a plugin API found in some JVMs.
 * The disassembler project is http://bastard.sourceforge.net/.
 * The symbol 'decode_instructions' is the entry point to the plugin, which is dynamically loaded.
 *
 * Copyright (C) 2008 John R. Rose.  This package may be used for any
 * purpose, provided that this copyright notice is preserved intact.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "libdis.h"
#include <stdio.h>

static intptr_t addr_from_operand( x86_op_t *opnd ) {
	x86_insn_t *insn;
	intptr_t field = 0;
	int size = !opnd ? 0 : x86_operand_size(opnd);
	if (size <= 0 || size > sizeof(qword_t))
		return 0;
	// compute operand field
	switch ( opnd->datatype ) {
	case op_byte:  field = (intptr_t) opnd->data.sbyte;  break;
	case op_word:  field = (intptr_t) opnd->data.sword;  break;
	case op_dword: field = (intptr_t) opnd->data.sdword; break;
	case op_qword: field = (intptr_t) opnd->data.sqword;
		if ((qword_t) field != opnd->data.sqword)
			return 0;  // overflow in intptr_t
		break;
	default:
		return 0;
	}
	if (field && !(opnd->flags & op_signed) && size < sizeof(field)) {
		// clear unrepresented sign bits
		field &= ~( (intptr_t)-1 << (8 * size) );
	}
	switch (opnd->type) {
	case op_immediate:
		return field;
	case op_relative_near:
	case op_relative_far:
		insn = (x86_insn_t*) opnd->insn;
		return insn->addr + insn->size + field;
	default:
		return 0;
	}
}

#define FORMAT_BYTES_PER_LINE 12 /* break raw byte output at 12 bytes */

/* default xml handler does nothing (ignores markup) */
static void *empty_putxml(void *putxml_data, const char *element_format, void *element_data) {
	FILE* fp = (FILE *) putxml_data;
	if (fp) {
		fprintf(fp, "<");
		fprintf(fp, element_format, element_data);
		fprintf(fp, ">");
	}
	return NULL;
}

void *decode_instructions(
	void *buf,
	void *buf_limit,
	void *(*putxml_fptr)(void *putxml_data, const char *element_format, void *element_data),
	void *putxml_data,
	void *(*printf_fptr)(void *printf_data, const char *format, ...),
	void *printf_data,
	const char *config_string)
{
#define PUTXML(efmt, edata)	(*putxml_fptr)(putxml_data, (const char*)(efmt), (void*)(edata))
#define PRINTF(fmt, x)		(*printf_fptr)(printf_data, (const char*)(fmt), x)
#define PRINTF2(fmt, x, y)	(*printf_fptr)(printf_data, (const char*)(fmt), x, y)

	size_t buf_size = (char*)buf_limit - (char*)buf;
	unsigned char *uc_buf = (unsigned char*) buf;
	int have_opnd, which;
	x86_insn_t insn;
	x86_op_t *opnd;
	intptr_t addr;
	char sbuf[MAX_OP_STRING * 2];
	size_t pos, size;

	if (!putxml_fptr) {
		putxml_fptr = empty_putxml;
	}
	if (!printf_fptr) {
		intptr_t stdio_fprintf_addr = (intptr_t) &fprintf;
		printf_fptr = (void * (*)(void *, const char *, ...)) stdio_fprintf_addr;
		if (!printf_data)  printf_data = stdout;
	}

	x86_init(0, 0, 0);
	PUTXML("insns", buf);
	PUTXML("mach name='%s'/", "i386(base-hsdis)");
	PUTXML("format bytes-per-line='%p'/", (intptr_t)FORMAT_BYTES_PER_LINE);

	for (pos = 0; pos < buf_size; pos += size) {
		size = x86_disasm(uc_buf, buf_size, (intptr_t) buf, pos, &insn);
		PUTXML("insn", (char*)buf + pos);
		if (size == 0) {
			PRINTF("invalid\t0x%02x", uc_buf[pos]);
			size = 1;  /* to make progress */
		} else {
			PRINTF2("%s%s", insn.prefix_string, insn.mnemonic);
			have_opnd = 0;
			for (which = 0; which < 3; which++) {
				/* dest comes first, then src and/or imm */
				switch (which) {
				case 0:  opnd = x86_operand_1st(&insn); break;
				case 1:  opnd = x86_operand_2nd(&insn); break;
				default: opnd = x86_operand_3rd(&insn); break;
				}
				if (!opnd)  continue;
				if (opnd->flags & op_implied)  continue;
				if (!have_opnd++)
					PRINTF("\t", 0);
				else	PRINTF(", ", 0);
				addr = addr_from_operand(opnd);
				if (!(addr && PUTXML("addr/", addr))) {
					x86_format_operand(opnd,
							   sbuf, sizeof(sbuf),
							   intel_syntax);
					PRINTF("%s", sbuf);
				}
			}
		}
		PUTXML("/insn", (char*)buf + pos + size);
		PRINTF("\n", 0);
	}

	PUTXML("/insns", (char*)buf + pos);
	x86_cleanup();
	return (char*)buf + pos;
}


/*
 * For Emacs:
 * Local Variables:
 * mode: C
 * indent-tabs-mode: t
 * c-indent-level: 8
 * eval: (c-set-style "linux")
 * End:
 */
