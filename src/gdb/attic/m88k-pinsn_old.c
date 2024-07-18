/* m88k-pinsn.c */
/* This file has been modified by Data General Corporation, November 1989. */

#include <stdio.h>
#if defined(__GNUC__) && (__GNUC__ >= 4)
# pragma GCC diagnostic ignored "-Wmissing-braces"
#endif /* __GNUC__ >= 4 */
#include "m88k-opcode_old.h"
#ifdef AND
# undef AND
#endif /* AND */
#ifdef mode
# undef mode
#endif /* mode */
#include "defs.h"
#include "symtab.h"

extern int print_insn(CORE_ADDR, FILE *);
extern int m88kdis(int, int *, char *);
extern void sprintop(char *, OPSPEC *, UINT, int, int);
extern void init_disasm(void);
extern void install(INSTAB *);
extern void sprint_address(CORE_ADDR, char *);

/* Changed hashtab to hashtable to avoid naming conflict
 *  with libdc.o (used for tdesc) for m88k.
 */

INSTAB  *hashtable[HASHVAL] = {0};

/*
 *		Disassemble an M88000 Instruction
 *
 *
 *       This module decodes the first instruction in inbuf.  It uses the pc
 *	to display pc-relative displacements.  It writes the disassembled
 *	instruction in outbuf.
 *
 *			Revision History
 *
 *       Revision 1.0    11/08/85        Creation date by Motorola
 *			05/11/89	R. Trawick adapted to GDB interface.
 */
#define MAXLEN 20

int
print_insn(CORE_ADDR memaddr, FILE *stream)
{
  unsigned char buffer[MAXLEN];
  /* should be expanded if disassembler prints symbol names */
  char outbuf[100];
  int n;

  /* Instruction addresses may have low two bits set. Clear them: */
  memaddr &= 0xfffffffc;
#if defined(MAXLEN) && (MAXLEN < 20)
  read_memory(memaddr, buffer, MAXLEN);
#endif /* (MAXLEN < 20) */

  n = m88kdis((int)memaddr, (int *)buffer, outbuf);

  fputs(outbuf, stream);

  return n;
}

/*
 * disassemble the first instruction in 'inbuf'.
 * 'pc' should be the address of this instruction, it will
 *   be used to print the target address if this is a relative jump or call
 * 'outbuf' gets filled in with the disassembled instruction.  It should
 *   be long enough to hold the longest disassembled instruction.
 *   100 bytes is certainly enough, unless symbol printing is added later
 * The function returns the length of this instruction in bytes.
 */
int
m88kdis(int pc, int *inbuf, char *outbuf)
{
    static int		ihashtab_initialized = 0;
    int			instruction;
    unsigned int	opcode;
    INSTAB		*entry_ptr;
    int	        	opmask;
    int			myclass;

    instruction = *inbuf;

    if (!ihashtab_initialized) {
	init_disasm();
    }

    /* create a the appropriate mask to isolate the opcode */
    opmask = DEFMASK;
    myclass = (instruction & DEFMASK);
    if ((myclass >= SFU0) && (myclass <= SFU7)) {
	if (instruction < SFU1) {
	    opmask = CTRLMASK;
	} else {
	    opmask = SFUMASK;
	}
    } else if (myclass == RRR) {
	opmask = RRRMASK;
    } else if (myclass == RRI10) {
	opmask = RRI10MASK;
    }

    /* isolate the opcode */
    opcode = (instruction & opmask);

    /* search the hash table with the isolated opcode */
    for (entry_ptr = hashtable[opcode % HASHVAL];
	 (entry_ptr != NULL) && (entry_ptr->opcode != opcode);
	 entry_ptr= entry_ptr->next) {
      ;
    }

    if (entry_ptr == NULL) {
      snprintf(outbuf, 100, "word\t%08x", instruction);
    } else {
      snprintf(outbuf, 100, "%s\t", entry_ptr->mnemonic);
      sprintop(&outbuf[strlen(outbuf)], &(entry_ptr->op1), instruction, pc, 1);
      sprintop(&outbuf[strlen(outbuf)], &(entry_ptr->op2), instruction, pc, 0);
      sprintop(&outbuf[strlen(outbuf)], &(entry_ptr->op3), instruction, pc, 0);
    }

    return 4;
}


/*
*                      Decode an Operand of an Instruction
*
*			Functional Description
*
*       This module formats and writes an operand of an instruction to buf
*       based on the operand specification.  When the first flag is set this
*       is the first operand of an instruction.  Undefined operand types
*       cause a <dis error> message.
*
*			Parameters
*	char	*buf		buffer where the operand may be printed
*       OPSPEC  *opptr          Pointer to an operand specification
*       UINT    inst            Instruction from which operand is extracted
*	UINT    pc		PC of instruction; used for pc-relative disp.
*       int     first           Flag which if nonzero indicates the first
*                               operand of an instruction
*
*			Output
*
*       The operand specified is extracted from the instruction and is
*       written to buf in the format specified. The operand is preceded
*       by a comma if it is not the first operand of an instruction and it
*       is not a register indirect form.  Registers are preceded by 'r' and
*       hex values by '0x'.
*
*			Revision History
*
*       Revision 1.0    11/08/85        Creation date
*/

void
sprintop(char *buf, OPSPEC *opptr, UINT inst, int pc, int first)
{
   int	  extracted_field;
   const char *cond_mask_sym;
   char cond_mask_sym_buf[9];

   if (opptr->width == 0)
      return;

   switch(opptr->type) {
      case CRREG:
		       if (!first)
			   *buf++= ',';
		       snprintf(buf, 16, "cr%d", UEXT(inst, opptr->offset, opptr->width));
		       break;

      case FCRREG:
		       if (!first)
			   *buf++= ',';
		       snprintf(buf, 16, "fcr%d", UEXT(inst, opptr->offset, opptr->width));
		       break;

      case REGSC:
		       snprintf(buf, 16, "[r%d]", UEXT(inst, opptr->offset, opptr->width));
		       break;

      case REG:
		       if (!first)
			   *buf++= ',';
		       snprintf(buf, 16, "r%d", UEXT(inst, opptr->offset, opptr->width));
		       break;

      case HEX:
		        if (!first)
			   *buf++ = ',';
		        extracted_field = UEXT(inst, opptr->offset, opptr->width);
		        if (extracted_field == 0) {
			    snprintf(buf, 16, "0");
		        } else {
			    snprintf(buf, 16, "0x%02x", extracted_field);
		        }
		        break;

      case CONDMASK:
		        if (!first)
			   *buf++ = ',';
		        extracted_field = UEXT(inst, opptr->offset, opptr->width);
		        switch (extracted_field & 0x0f) {
			  case 0x1:	cond_mask_sym = "gt0";
					break;
			  case 0x2:	cond_mask_sym = "eq0";
					break;
			  case 0x3:	cond_mask_sym = "ge0";
					break;
			  case 0xc:	cond_mask_sym = "lt0";
					break;
			  case 0xd:	cond_mask_sym = "ne0";
					break;
			  case 0xe:	cond_mask_sym = "le0";
					break;
			  default:	cond_mask_sym = cond_mask_sym_buf;
					snprintf(cond_mask_sym_buf,
     						 sizeof(cond_mask_sym_buf),
						 "%x", extracted_field);
					break;
			}
			strcpy(buf, cond_mask_sym);
			break;
			
      case PCREL:
		        if (!first)
			   *buf++= ',';
			sprint_address(pc + (4 * SEXT(inst, opptr->offset, opptr->width)),
                                       buf);
		        break;

      case CONT:
		       snprintf(buf, 16, "%d,r%d",
                                UEXT(inst, opptr->offset, 5),
			        UEXT(inst, (opptr->offset + 5), 5));
		       break;

      case BF:
		       if (!first)
			   *buf++ = ',';
		       snprintf(buf, 16, "%d<%d>",
                                UEXT(inst, (opptr->offset + 5), 5),
			        UEXT(inst, opptr->offset, 5));
		       break;

      default:
		       snprintf(buf, 32, "<dis error: %08x>", inst);
    }

}

/*
*                 Initialize the Disassembler Instruction Table
*
*       Initialize the hash table and instruction table for the disassembler.
*       This should be called once before the first call to disasm().
*
*			Parameters
*
*			Output
*
*       If the debug option is selected, certain statistics about the hashing
*       distribution are written to stdout.
*
*			Revision History
*
*       Revision 1.0    11/08/85        Creation date
*/

void
init_disasm(void)
{
   int i, size;

   for (i = 0; i < HASHVAL ; i++)
      hashtable[i] = NULL;

   for (i = 0, size = (sizeof(instructions) / sizeof(INSTAB)); i < size;
       install(&instructions[i++]));

}

/*
*       Insert an instruction into the disassembler table by hashing the
*       opcode and inserting it into the linked list for that hash value.
*
*			Parameters
*
*       INSTAB *instptr         Pointer to the entry in the instruction table
*                               to be installed
*
*       Revision 1.0    11/08/85        Creation date
*			05/11/89	R. TRAWICK ADAPTED FROM MOTOROLA
*/

void
install(INSTAB *instptr)
{
   UINT i;

   i = (instptr->opcode) % HASHVAL;
   instptr->next = hashtable[i];
   hashtable[i] = instptr;
}


/* adapted from print_address in printcmd by R. Trawick 5/15/89.  The two should
 * be combined.
 */
void sprint_address(CORE_ADDR addr, char *buffer)
{
	register int	i;
	struct symbol	*fs;
	const char	*name;
	int		name_location;

	snprintf(buffer, 16, "0x%x", addr);

	fs = find_pc_function(addr);

	if (!fs) {
#if 0
	    i = find_pc_misc_function(addr);
#else
	    i = 0;
#endif /* 0 */

	    if (i < 0) return;	/* If nothing comes through, don't
				   print anything symbolic */
#if 0
	    name = misc_function_vector[i].name;
	    name_location = misc_function_vector[i].address;
#else
            name = "";
            name_location = 0;
#endif /* 0 */
	} else {
#if 0
	    name = fs->name;
#else
            name = "";
#endif /* 0 */
#if defined(BLOCK_START) && defined(SYMBOL_BLOCK_VALUE)
	    name_location = BLOCK_START(SYMBOL_BLOCK_VALUE(fs));
#else
	    name_location = 0;
#endif /* BLOCK_START && SYMBOL_BLOCK_VALUE */
	}

	if (addr - name_location) {
	    snprintf(buffer, 16, " <%s+%d>", name, (addr - name_location));
	} else {
	    snprintf(buffer, 16, " <%s>", name);
	}
}

/* EOF */
