/* archures.c: BFD library support routines for architectures.
   Copyright 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999,
   2000, 2001, 2002, 2003, 2004, 2005
   Free Software Foundation, Inc.
   Hacked by John Gilmore and Steve Chamberlain of Cygnus Support.

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St., 5th Floor, Boston, MA 02110-1301, USA */

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "safe-ctype.h"

/*

SECTION
	Architectures

	BFD keeps one atom in a BFD describing the
	architecture of the data attached to the BFD: a pointer to a
	<<bfd_arch_info_type>>.

	Pointers to structures can be requested independently of a BFD
	so that an architecture's information can be interrogated
	without access to an open BFD.

	The architecture information is provided by each architecture package.
	The set of default architectures is selected by the macro
	<<SELECT_ARCHITECTURES>>.  This is normally set up in the
	@file{config/@var{target}.mt} file of your choice.  If the name is not
	defined, then all the architectures supported are included.

	When BFD starts up, all the architectures are called with an
	initialize method.  It is up to the architecture back end to
	insert as many items into the list of architectures as it wants to;
	generally this would be one for each machine and one for the
	default case (an item with a machine field of 0).

	BFD's idea of an architecture is implemented in	@file{archures.c}.
*/

/*

SUBSECTION
	bfd_architecture

DESCRIPTION
	This enum gives the object file's CPU architecture, in a
	global sense---i.e., what processor family does it belong to?
	Another field indicates which processor within
	the family is in use.  The machine gives a number which
	distinguishes different versions of the architecture,
	containing, for example, 2 and 3 for Intel i960 KA and i960 KB,
	and 68020 and 68030 for Motorola 68020 and 68030.

.enum bfd_architecture
.{
.  bfd_arch_unknown,   {* File arch not known.  *}
.  bfd_arch_obscure,   {* Arch known, not one of these.  *}
.  bfd_arch_m68k,      {* Motorola 68xxx *}
.#define bfd_mach_m68000 1
.#define bfd_mach_m68008 2
.#define bfd_mach_m68010 3
.#define bfd_mach_m68020 4
.#define bfd_mach_m68030 5
.#define bfd_mach_m68040 6
.#define bfd_mach_m68060 7
.#define bfd_mach_cpu32  8
.#define bfd_mach_mcf5200  9
.#define bfd_mach_mcf5206e 10
.#define bfd_mach_mcf5307  11
.#define bfd_mach_mcf5407  12
.#define bfd_mach_mcf528x  13
.#define bfd_mach_mcfv4e   14
.#define bfd_mach_mcf521x   15
.#define bfd_mach_mcf5249   16
.#define bfd_mach_mcf547x   17
.#define bfd_mach_mcf548x   18
.  bfd_arch_vax,       {* DEC Vax *}
.  bfd_arch_i960,      {* Intel 960 *}
.    {* The order of the following is important.
.       lower number indicates a machine type that
.       only accepts a subset of the instructions
.       available to machines with higher numbers.
.       The exception is the "ca", which is
.       incompatible with all other machines except
.       "core".  *}
.
.#define bfd_mach_i960_core      1
.#define bfd_mach_i960_ka_sa     2
.#define bfd_mach_i960_kb_sb     3
.#define bfd_mach_i960_mc        4
.#define bfd_mach_i960_xa        5
.#define bfd_mach_i960_ca        6
.#define bfd_mach_i960_jx	 7
.#define bfd_mach_i960_hx        8
.
.  bfd_arch_or1k,      {* OpenRISC 1000 *}
.#define bfd_mach_or1k           1
.#define bfd_mach_or1knd         2
.
.  bfd_arch_or32,      {* OpenRISC 32 *}
.
.  bfd_arch_a29k,      {* AMD 29000 *}
.  bfd_arch_sparc,     {* SPARC *}
.#define bfd_mach_sparc			1
.{* The difference between v8plus and v9 is that v9 is a true 64 bit env.  *}
.#define bfd_mach_sparc_sparclet	2
.#define bfd_mach_sparc_sparclite	3
.#define bfd_mach_sparc_v8plus		4
.#define bfd_mach_sparc_v8plusa		5 {* with ultrasparc add'ns.  *}
.#define bfd_mach_sparc_sparclite_le	6
.#define bfd_mach_sparc_v9		7
.#define bfd_mach_sparc_v9a		8 {* with ultrasparc add'ns.  *}
.#define bfd_mach_sparc_v8plusb		9 {* with cheetah add'ns.  *}
.#define bfd_mach_sparc_v9b		10 {* with cheetah add'ns.  *}
.{* Nonzero if MACH has the v9 instruction set.  *}
.#define bfd_mach_sparc_v9_p(mach) \
.  ((mach) >= bfd_mach_sparc_v8plus && (mach) <= bfd_mach_sparc_v9b \
.   && (mach) != bfd_mach_sparc_sparclite_le)
.{* Nonzero if MACH is a 64 bit sparc architecture.  *}
.#define bfd_mach_sparc_64bit_p(mach) \
.  ((mach) >= bfd_mach_sparc_v9 && (mach) != bfd_mach_sparc_v8plusb)
.  bfd_arch_spu,       {* PowerPC SPU *}
.#define bfd_mach_spu		256
.  bfd_arch_mips,      {* MIPS Rxxxx *}
.#define bfd_mach_mips3000		3000
.#define bfd_mach_mips_loongson_2e	3001
.#define bfd_mach_mips_loongson_2f	3002
.#define bfd_mach_mips3900		3900
.#define bfd_mach_mips4000		4000
.#define bfd_mach_mips4010		4010
.#define bfd_mach_mips4100		4100
.#define bfd_mach_mips4111		4111
.#define bfd_mach_mips4120		4120
.#define bfd_mach_mips4300		4300
.#define bfd_mach_mips4400		4400
.#define bfd_mach_mips4600		4600
.#define bfd_mach_mips4650		4650
.#define bfd_mach_mips5000		5000
.#define bfd_mach_mips5400		5400
.#define bfd_mach_mips5500		5500
.#define bfd_mach_mips6000		6000
.#define bfd_mach_mips7000		7000
.#define bfd_mach_mips8000		8000
.#define bfd_mach_mips9000		9000
.#define bfd_mach_mips10000		10000
.#define bfd_mach_mips12000		12000
.#define bfd_mach_mips16		16
.#define bfd_mach_mips5                 5
.#define bfd_mach_mips_sb1              12310201 {* octal 'SB', 01 *}
.#define bfd_mach_mipsisa32             32
.#define bfd_mach_mipsisa32r2           33
.#define bfd_mach_mipsisa64             64
.#define bfd_mach_mipsisa64r2           65
.  bfd_arch_i386,      {* Intel 386 *}
.#define bfd_mach_i386_i386 1
.#define bfd_mach_i386_i8086 2
.#define bfd_mach_i386_i386_intel_syntax 3
.#define bfd_mach_x64_32 32
.#define bfd_mach_x64_32_intel_syntax 33
.#define bfd_mach_x86_64 64
.#define bfd_mach_x86_64_intel_syntax 65
.  bfd_arch_l1om,   {* Intel L1OM *}
.#define bfd_mach_l1om 66
.#define bfd_mach_l1om_intel_syntax 67
.  bfd_arch_k1om,   {* Intel K1OM *}
.#define bfd_mach_k1om 68
.#define bfd_mach_k1om_intel_syntax 69
.  bfd_arch_we32k,     {* AT&T WE32xxx *}
.  bfd_arch_tahoe,     {* CCI/Harris Tahoe *}
.  bfd_arch_i860,      {* Intel 860 *}
.  bfd_arch_i370,      {* IBM 360/370 Mainframes *}
.  bfd_arch_romp,      {* IBM ROMP PC/RT *}
.  bfd_arch_alliant,   {* Alliant *}
.  bfd_arch_convex,    {* Convex *}
.  bfd_arch_m88k,      {* Motorola 88xxx *}
.  bfd_arch_m98k,      {* Motorola 98xxx *}
.  bfd_arch_pyramid,   {* Pyramid Technology *}
.  bfd_arch_h8300,     {* Renesas H8/300 (formerly Hitachi H8/300) *}
.#define bfd_mach_h8300    1
.#define bfd_mach_h8300h   2
.#define bfd_mach_h8300s   3
.#define bfd_mach_h8300hn  4
.#define bfd_mach_h8300sn  5
.#define bfd_mach_h8300sx  6
.#define bfd_mach_h8300sxn 7
.  bfd_arch_pdp11,     {* DEC PDP-11 *}
.  bfd_arch_plugin,
.  bfd_arch_powerpc,   {* PowerPC *}
.#define bfd_mach_ppc		32
.#define bfd_mach_ppc64		64
.#define bfd_mach_ppc_403	403
.#define bfd_mach_ppc_403gc	4030
.#define bfd_mach_ppc_505	505
.#define bfd_mach_ppc_601	601
.#define bfd_mach_ppc_602	602
.#define bfd_mach_ppc_603	603
.#define bfd_mach_ppc_ec603e	6031
.#define bfd_mach_ppc_604	604
.#define bfd_mach_ppc_620	620
.#define bfd_mach_ppc_630	630
.#define bfd_mach_ppc_750	750
.#define bfd_mach_ppc_860	860
.#define bfd_mach_ppc_a35	35
.#define bfd_mach_ppc_rs64ii	642
.#define bfd_mach_ppc_rs64iii	643
.#define bfd_mach_ppc_7400	7400
.#define bfd_mach_ppc_e500      500
.#define bfd_mach_ppc_970	970
.  bfd_arch_rs6000,    {* IBM RS/6000 *}
.#define bfd_mach_rs6k		6000
.#define bfd_mach_rs6k_rs1	6001
.#define bfd_mach_rs6k_rsc	6003
.#define bfd_mach_rs6k_rs2	6002
.  bfd_arch_hppa,      {* HP PA RISC *}
.#define bfd_mach_hppa10	10
.#define bfd_mach_hppa11	11
.#define bfd_mach_hppa20	20
.#define bfd_mach_hppa20w	25
.  bfd_arch_d10v,      {* Mitsubishi D10V *}
.#define bfd_mach_d10v		1
.#define bfd_mach_d10v_ts2	2
.#define bfd_mach_d10v_ts3	3
.  bfd_arch_d30v,      {* Mitsubishi D30V *}
.  bfd_arch_dlx,       {* DLX *}
.  bfd_arch_m68hc11,   {* Motorola 68HC11 *}
.  bfd_arch_m68hc12,   {* Motorola 68HC12 *}
.#define bfd_mach_m6812_default 0
.#define bfd_mach_m6812         1
.#define bfd_mach_m6812s        2
.  bfd_arch_m9s12x,   {* Freescale S12X *}
.  bfd_arch_m9s12xg,  {* Freescale XGATE *}
.  bfd_arch_z8k,       {* Zilog Z8000 *}
.#define bfd_mach_z8001		1
.#define bfd_mach_z8002		2
.  bfd_arch_h8500,     {* Renesas H8/500 (formerly Hitachi H8/500) *}
.  bfd_arch_sh,        {* Renesas / SuperH SH (formerly Hitachi SH) *}
.#define bfd_mach_sh            1
.#define bfd_mach_sh2        0x20
.#define bfd_mach_sh_dsp     0x2d
.#define bfd_mach_sh2a       0x2a
.#define bfd_mach_sh2a_nofpu 0x2b
.#define bfd_mach_sh2a_nofpu_or_sh4_nommu_nofpu 0x2a1
.#define bfd_mach_sh2a_nofpu_or_sh3_nommu 0x2a2
.#define bfd_mach_sh2a_or_sh4  0x2a3
.#define bfd_mach_sh2a_or_sh3e 0x2a4
.#define bfd_mach_sh2e       0x2e
.#define bfd_mach_sh3        0x30
.#define bfd_mach_sh3_nommu  0x31
.#define bfd_mach_sh3_dsp    0x3d
.#define bfd_mach_sh3e       0x3e
.#define bfd_mach_sh4        0x40
.#define bfd_mach_sh4_nofpu  0x41
.#define bfd_mach_sh4_nommu_nofpu  0x42
.#define bfd_mach_sh4a       0x4a
.#define bfd_mach_sh4a_nofpu 0x4b
.#define bfd_mach_sh4al_dsp  0x4d
.#define bfd_mach_sh5        0x50
.  bfd_arch_alpha,     {* Dec Alpha *}
.#define bfd_mach_alpha_ev4  0x10
.#define bfd_mach_alpha_ev5  0x20
.#define bfd_mach_alpha_ev6  0x30
.  bfd_arch_arm,       {* Advanced Risc Machines ARM.  *}
.#define bfd_mach_arm_unknown	0
.#define bfd_mach_arm_2		1
.#define bfd_mach_arm_2a	2
.#define bfd_mach_arm_3		3
.#define bfd_mach_arm_3M 	4
.#define bfd_mach_arm_4 	5
.#define bfd_mach_arm_4T 	6
.#define bfd_mach_arm_5 	7
.#define bfd_mach_arm_5T	8
.#define bfd_mach_arm_5TE	9
.#define bfd_mach_arm_XScale	10
.#define bfd_mach_arm_ep9312	11
.#define bfd_mach_arm_iWMMXt	12
.#define bfd_mach_arm_iWMMXt2	13
.#define bfd_mach_arm_6         14
.#define bfd_mach_arm_7         15
.#define bfd_mach_arm_7f        16
.#define bfd_mach_arm_7s        17
.#define bfd_mach_arm_7k        18
.  bfd_arch_nds32,     {* Andes NDS32 *}
.#define bfd_mach_n1            1
.#define bfd_mach_n1h           2
.#define bfd_mach_n1h_v2        3
.#define bfd_mach_n1h_v3        4
.#define bfd_mach_n1h_v3m       5
.  bfd_arch_ns32k,     {* National Semiconductors ns32000 *}
.  bfd_arch_w65,       {* WDC 65816 *}
.  bfd_arch_tic30,     {* Texas Instruments TMS320C30 *}
.  bfd_arch_tic4x,     {* Texas Instruments TMS320C3X/4X *}
.#define bfd_mach_tic3x         30
.#define bfd_mach_tic4x         40
.  bfd_arch_tic54x,    {* Texas Instruments TMS320C54X *}
.  bfd_arch_tic6x,     {* Texas Instruments TMS320C6X *}
.  bfd_arch_tic80,     {* TI TMS320c80 (MVP) *}
.  bfd_arch_v850,      {* NEC V850 *}
.  bfd_arch_v850_rh850,{* NEC V850 (using RH850 ABI) *}
.#define bfd_mach_v850          1
.#define bfd_mach_v850e 	'E'
.#define bfd_mach_v850e1	'1'
.#define bfd_mach_v850e2        0x4532
.#define bfd_mach_v850e2v3      0x45325633
.#define bfd_mach_v850e3v5      0x45335635 {* ('E'|'3'|'V'|'5') *}
.  bfd_arch_arc,       {* ARC Cores *}
.#define bfd_mach_arc_5         5
.#define bfd_mach_arc_6         6
.#define bfd_mach_arc_7         7
.#define bfd_mach_arc_8         8
. bfd_arch_m32c,     {* Renesas M16C/M32C.  *}
.#define bfd_mach_m16c        0x75
.#define bfd_mach_m32c        0x78
.  bfd_arch_m32r,      {* Renesas M32R (formerly Mitsubishi M32R/D) *}
.#define bfd_mach_m32r		1 {* For backwards compatibility.  *}
.#define bfd_mach_m32rx		'x'
.#define bfd_mach_m32r2		'2'
.  bfd_arch_mn10200,   {* Matsushita MN10200 *}
.  bfd_arch_mn10300,   {* Matsushita MN10300 *}
.#define bfd_mach_mn10300		300
.#define bfd_mach_am33		330
.#define bfd_mach_am33_2	332
.  bfd_arch_fr30,
.#define bfd_mach_fr30		0x46523330
.  bfd_arch_frv,
.#define bfd_mach_frv		1
.#define bfd_mach_frvsimple	2
.#define bfd_mach_fr300		300
.#define bfd_mach_fr400		400
.#define bfd_mach_fr450		450
.#define bfd_mach_frvtomcat	499	{* fr500 prototype *}
.#define bfd_mach_fr500		500
.#define bfd_mach_fr550		550
.  bfd_arch_moxie,       {* The moxie processor *}
.#define bfd_mach_moxie		1
.  bfd_arch_ft32,       {* The ft32 processor *}
.#define bfd_mach_ft32		1
.  bfd_arch_mcore,
.  bfd_arch_mep,
.#define bfd_mach_mep		1
.#define bfd_mach_mep_h1	0x6831
.#define bfd_mach_mep_c5	0x6335
.  bfd_arch_metag,
.#define bfd_mach_metag		1
.  bfd_arch_ia64,      {* HP/Intel ia64 *}
.#define bfd_mach_ia64_elf64	64
.#define bfd_mach_ia64_elf32	32
.  bfd_arch_ip2k,      {* Ubicom IP2K microcontrollers. *}
.#define bfd_mach_ip2022	1
.#define bfd_mach_ip2022ext	2
. bfd_arch_iq2000,     {* Vitesse IQ2000.  *}
.#define bfd_mach_iq2000        1
.#define bfd_mach_iq10          2
.  bfd_arch_epiphany,  {* Adapteva EPIPHANY *}
.#define bfd_mach_epiphany16   1
.#define bfd_mach_epiphany32   2
.  bfd_arch_ms1,
.  bfd_arch_mt,
.#define bfd_mach_ms1           1
.#define bfd_mach_mrisc2        2
.#define bfd_mach_ms2           3
.  bfd_arch_pj,
.  bfd_arch_avr,       {* Atmel AVR microcontrollers.  *}
.#define bfd_mach_avr1		1
.#define bfd_mach_avr2		2
.#define bfd_mach_avr3		3
.#define bfd_mach_avr4		4
.#define bfd_mach_avr5		5
.  bfd_arch_bfin,        {* ADI Blackfin *}
.#define bfd_mach_bfin          1
.  bfd_arch_cr16,       {* National Semiconductor CompactRISC (i.e. CR16). *}
.#define bfd_mach_cr16          1
.  bfd_arch_cr16c,       {* National Semiconductor CompactRISC. *}
.#define bfd_mach_cr16c		1
.  bfd_arch_crx,       {*  National Semiconductor CRX.  *}
.#define bfd_mach_crx		1
.  bfd_arch_cris,      {* Axis CRIS *}
.#define bfd_mach_cris_v0_v10	255
.#define bfd_mach_cris_v32	32
.#define bfd_mach_cris_v10_v32	1032
.  bfd_arch_rl78,
.#define bfd_mach_rl78	0x75
.  bfd_arch_rx,        {* Renesas RX.  *}
.#define bfd_mach_rx            0x75
.  bfd_arch_s390,      {* IBM s390 *}
.#define bfd_mach_s390_31       31
.#define bfd_mach_s390_64       64
.  bfd_arch_score,     {* Sunplus score *}
.#define bfd_mach_score3         3
.#define bfd_mach_score7         7
.  bfd_arch_openrisc,  {* OpenRISC *}
.  bfd_arch_mmix,      {* Donald Knuth's educational processor.  *}
.  bfd_arch_xstormy16,
.#define bfd_mach_xstormy16	1
.  bfd_arch_msp430,    {* Texas Instruments MSP430 architecture.  *}
.#define bfd_mach_msp11          11
.#define bfd_mach_msp110         110
.#define bfd_mach_msp12          12
.#define bfd_mach_msp13          13
.#define bfd_mach_msp14          14
.#define bfd_mach_msp15          15
.#define bfd_mach_msp16          16
.#define bfd_mach_msp21          21
.#define bfd_mach_msp31          31
.#define bfd_mach_msp32          32
.#define bfd_mach_msp33          33
.#define bfd_mach_msp41          41
.#define bfd_mach_msp42          42
.#define bfd_mach_msp43          43
.#define bfd_mach_msp44          44
.#define bfd_mach_msp430x        45
.#define bfd_mach_msp46          46
.#define bfd_mach_msp47          47
.#define bfd_mach_msp54          54
.  bfd_arch_xc16x,     {* Infineon's XC16X Series.               *}
.#define bfd_mach_xc16x         1
.#define bfd_mach_xc16xl        2
.#define bfd_mach_xc16xs        3
.  bfd_arch_xgate,   {* Freescale XGATE *}
.#define bfd_mach_xgate         1
.  bfd_arch_xtensa,    {* Tensilica's Xtensa cores.  *}
.#define bfd_mach_xtensa	1
.   bfd_arch_maxq,     {* Dallas MAXQ 10/20 *}
.#define bfd_mach_maxq10    10
.#define bfd_mach_maxq20    20
.  bfd_arch_z80,
.#define bfd_mach_z80strict      1 {* No undocumented opcodes.  *}
.#define bfd_mach_z80            3 {* With ixl, ixh, iyl, and iyh.  *}
.#define bfd_mach_z80full        7 {* All undocumented instructions.  *}
.#define bfd_mach_r800           11 {* R800: successor with multiplication.  *}
.  bfd_arch_lm32,      {* Lattice Mico32 *}
.#define bfd_mach_lm32      1
.  bfd_arch_microblaze,{* Xilinx MicroBlaze. *}
.  bfd_arch_tilepro,   {* Tilera TILEPro *}
.  bfd_arch_tilegx, {* Tilera TILE-Gx *}
.#define bfd_mach_tilepro   1
.#define bfd_mach_tilegx    1
.#define bfd_mach_tilegx32  2
.  bfd_arch_aarch64,   {* AArch64  *}
.#define bfd_mach_aarch64 0
.#define bfd_mach_aarch64_ilp32	32
.  bfd_arch_nios2,	{* Nios II *}
.#define bfd_mach_nios2		0
.#define bfd_mach_nios2r1	1
.#define bfd_mach_nios2r2	2
.  bfd_arch_visium,	{* Visium *}
.#define bfd_mach_visium	1
.  bfd_arch_wasm32,    {* WebAssembly.  *}
.#define bfd_mach_wasm32	1
.  bfd_arch_pru,       {* PRU.  *}
.#define bfd_mach_pru		0
.  bfd_arch_nfp,       {* Netronome Flow Processor *}
.#define bfd_mach_nfp3200	0x3200
.#define bfd_mach_nfp6000	0x6000
.  bfd_arch_csky,      {* C-SKY.  *}
.#define bfd_mach_ck_unknown    0
.#define bfd_mach_ck510		1
.#define bfd_mach_ck610		2
.#define bfd_mach_ck801		3
.#define bfd_mach_ck802		4
.#define bfd_mach_ck803		5
.#define bfd_mach_ck807		6
.#define bfd_mach_ck810		7
.#define bfd_mach_ck860		8
.  bfd_arch_loongarch,       {* LoongArch *}
.#define bfd_mach_loongarch32	1
.#define bfd_mach_loongarch64	2
.  bfd_arch_amdgcn,     {* AMDGCN *}
.#define bfd_mach_amdgcn_unknown 0x000
.#define bfd_mach_amdgcn_gfx900  0x02c
.#define bfd_mach_amdgcn_gfx904  0x02e
.#define bfd_mach_amdgcn_gfx906  0x02f
.#define bfd_mach_amdgcn_gfx908  0x030
.#define bfd_mach_amdgcn_gfx90a  0x03f
.#define bfd_mach_amdgcn_gfx1010 0x033
.#define bfd_mach_amdgcn_gfx1011 0x034
.#define bfd_mach_amdgcn_gfx1012 0x035
.#define bfd_mach_amdgcn_gfx1030 0x036
.#define bfd_mach_amdgcn_gfx1031 0x037
.#define bfd_mach_amdgcn_gfx1032 0x038
.#define bfd_mach_amdgcn_gfx1100 0x041
.#define bfd_mach_amdgcn_gfx1101 0x046
.#define bfd_mach_amdgcn_gfx1102 0x047
.  bfd_arch_last
.  };
*/

/*
SUBSECTION
	bfd_arch_info

DESCRIPTION
	This structure contains information on architectures for use
	within BFD.

.
.typedef struct bfd_arch_info
.{
.  int bits_per_word;
.  int bits_per_address;
.  int bits_per_byte;
.  enum bfd_architecture arch;
.  unsigned long mach;
.  const char *arch_name;
.  const char *printable_name;
.  unsigned int section_align_power;
.  {* TRUE if this is the default machine for the architecture.
.     The default arch should be the first entry for an arch so that
.     all the entries for that arch can be accessed via <<next>>.  *}
.  bfd_boolean the_default;
.  const struct bfd_arch_info * (*compatible)
.    (const struct bfd_arch_info *a, const struct bfd_arch_info *b);
.
.  bfd_boolean (*scan) (const struct bfd_arch_info *, const char *);
.
.#ifdef USE_NEW_ELF_BFD_STRUCT_MEMBERS
.  {* Allocate via bfd_malloc and return a fill buffer of size COUNT.  If
.     IS_BIGENDIAN is TRUE, the order of bytes is big endian.  If CODE is
.     TRUE, the buffer contains code.  *}
.  void *(*fill) (bfd_size_type count, bfd_boolean is_bigendian,
.                 bfd_boolean code);
.#endif {* USE_NEW_ELF_BFD_STRUCT_MEMBERS *}
.  const struct bfd_arch_info *next;
.}
.bfd_arch_info_type;
.
*/

extern const bfd_arch_info_type bfd_a29k_arch;
extern const bfd_arch_info_type bfd_aarch64_arch;
extern const bfd_arch_info_type bfd_alpha_arch;
extern const bfd_arch_info_type bfd_arc_arch;
extern const bfd_arch_info_type bfd_arm_arch;
extern const bfd_arch_info_type bfd_avr_arch;
extern const bfd_arch_info_type bfd_bfin_arch;
extern const bfd_arch_info_type bfd_cr16c_arch;
extern const bfd_arch_info_type bfd_cris_arch;
extern const bfd_arch_info_type bfd_crx_arch;
extern const bfd_arch_info_type bfd_d10v_arch;
extern const bfd_arch_info_type bfd_d30v_arch;
extern const bfd_arch_info_type bfd_dlx_arch;
extern const bfd_arch_info_type bfd_epiphany_arch;
extern const bfd_arch_info_type bfd_fr30_arch;
extern const bfd_arch_info_type bfd_frv_arch;
extern const bfd_arch_info_type bfd_h8300_arch;
extern const bfd_arch_info_type bfd_h8500_arch;
extern const bfd_arch_info_type bfd_hppa_arch;
extern const bfd_arch_info_type bfd_i370_arch;
extern const bfd_arch_info_type bfd_i386_arch;
extern const bfd_arch_info_type bfd_i860_arch;
extern const bfd_arch_info_type bfd_i960_arch;
extern const bfd_arch_info_type bfd_ia64_arch;
extern const bfd_arch_info_type bfd_ip2k_arch;
extern const bfd_arch_info_type bfd_iq2000_arch;
extern const bfd_arch_info_type bfd_k1om_arch;
extern const bfd_arch_info_type bfd_l1om_arch;
extern const bfd_arch_info_type bfd_lm32_arch;
extern const bfd_arch_info_type bfd_loongarch_arch;
extern const bfd_arch_info_type bfd_m32c_arch;
extern const bfd_arch_info_type bfd_m32r_arch;
extern const bfd_arch_info_type bfd_m68hc11_arch;
extern const bfd_arch_info_type bfd_m68hc12_arch;
extern const bfd_arch_info_type bfd_m9s12x_arch;
extern const bfd_arch_info_type bfd_m9s12xg_arch;
extern const bfd_arch_info_type bfd_m68k_arch;
extern const bfd_arch_info_type bfd_m88k_arch;
extern const bfd_arch_info_type bfd_maxq_arch;
extern const bfd_arch_info_type bfd_mcore_arch;
extern const bfd_arch_info_type bfd_mep_arch;
extern const bfd_arch_info_type bfd_metag_arch;
extern const bfd_arch_info_type bfd_microblaze_arch;
extern const bfd_arch_info_type bfd_mips_arch;
extern const bfd_arch_info_type bfd_mmix_arch;
extern const bfd_arch_info_type bfd_mn10200_arch;
extern const bfd_arch_info_type bfd_mn10300_arch;
extern const bfd_arch_info_type bfd_moxie_arch;
extern const bfd_arch_info_type bfd_ft32_arch;
extern const bfd_arch_info_type bfd_msp430_arch;
extern const bfd_arch_info_type bfd_ms1_arch;
extern const bfd_arch_info_type bfd_mt_arch;
extern const bfd_arch_info_type bfd_nds32_arch;
extern const bfd_arch_info_type bfd_nios2_arch;
extern const bfd_arch_info_type bfd_ns32k_arch;
extern const bfd_arch_info_type bfd_openrisc_arch;
extern const bfd_arch_info_type bfd_or1k_arch;
extern const bfd_arch_info_type bfd_or32_arch;
extern const bfd_arch_info_type bfd_pdp11_arch;
extern const bfd_arch_info_type bfd_plugin_arch;
extern const bfd_arch_info_type bfd_pj_arch;
extern const bfd_arch_info_type bfd_powerpc_archs[];
#define bfd_powerpc_arch bfd_powerpc_archs[0]
extern const bfd_arch_info_type bfd_rs6000_arch;
extern const bfd_arch_info_type bfd_rl78_arch;
extern const bfd_arch_info_type bfd_rx_arch;
extern const bfd_arch_info_type bfd_s390_arch;
extern const bfd_arch_info_type bfd_score_arch;
extern const bfd_arch_info_type bfd_sh_arch;
extern const bfd_arch_info_type bfd_sparc_arch;
extern const bfd_arch_info_type bfd_spu_arch;
extern const bfd_arch_info_type bfd_tic30_arch;
extern const bfd_arch_info_type bfd_tic4x_arch;
extern const bfd_arch_info_type bfd_tic54x_arch;
extern const bfd_arch_info_type bfd_tic6x_arch;
extern const bfd_arch_info_type bfd_tic80_arch;
extern const bfd_arch_info_type bfd_tilegx_arch;
extern const bfd_arch_info_type bfd_tilepro_arch;
extern const bfd_arch_info_type bfd_v850_arch;
extern const bfd_arch_info_type bfd_v850_rh850_arch;
extern const bfd_arch_info_type bfd_vax_arch;
extern const bfd_arch_info_type bfd_visium_arch;
extern const bfd_arch_info_type bfd_we32k_arch;
extern const bfd_arch_info_type bfd_w65_arch;
extern const bfd_arch_info_type bfd_xstormy16_arch;
extern const bfd_arch_info_type bfd_xtensa_arch;
extern const bfd_arch_info_type bfd_xc16x_arch;
extern const bfd_arch_info_type bfd_xgate_arch;
extern const bfd_arch_info_type bfd_z80_arch;
extern const bfd_arch_info_type bfd_z8k_arch;

static const bfd_arch_info_type * const bfd_archures_list[] =
  {
#ifdef SELECT_ARCHITECTURES
    SELECT_ARCHITECTURES,
#else
    &bfd_a29k_arch,
    &bfd_alpha_arch,
    &bfd_arc_arch,
    &bfd_arm_arch,
    &bfd_avr_arch,
    &bfd_bfin_arch,
    &bfd_cr16c_arch,
    &bfd_cris_arch,
    &bfd_crx_arch,
    &bfd_d10v_arch,
    &bfd_d30v_arch,
    &bfd_dlx_arch,
    &bfd_epiphany_arch,
    &bfd_fr30_arch,
    &bfd_frv_arch,
    &bfd_h8300_arch,
    &bfd_h8500_arch,
    &bfd_hppa_arch,
    &bfd_i370_arch,
    &bfd_i386_arch,
    &bfd_i860_arch,
    &bfd_i960_arch,
    &bfd_ia64_arch,
    &bfd_ip2k_arch,
    &bfd_iq2000_arch,
    &bfd_k1om_arch,
    &bfd_l1om_arch,
    &bfd_lm32_arch,
    &bfd_loongarch_arch,
    &bfd_m32c_arch,
    &bfd_m32r_arch,
    &bfd_m68hc11_arch,
    &bfd_m68hc12_arch,
    &bfd_m9s12x_arch,
    &bfd_m9s12xg_arch,
    &bfd_m68k_arch,
    &bfd_m88k_arch,
    &bfd_maxq_arch,
    &bfd_mcore_arch,
    &bfd_mep_arch,
    &bfd_metag_arch,
    &bfd_microblaze_arch,
    &bfd_mips_arch,
    &bfd_mmix_arch,
    &bfd_mn10200_arch,
    &bfd_mn10300_arch,
    &bfd_moxie_arch,
    &bfd_ft32_arch,
    &bfd_ms1_arch,
    &bfd_msp430_arch,
    &bfd_mt_arch,
    &bfd_nds32_arch,
    &bfd_nios2_arch,
    &bfd_ns32k_arch,
    &bfd_openrisc_arch,
    &bfd_or1k_arch,
    &bfd_or32_arch,
    &bfd_pdp11_arch,
    &bfd_powerpc_arch,
    &bfd_rs6000_arch,
    &bfd_rl78_arch,
    &bfd_rx_arch,
    &bfd_s390_arch,
    &bfd_score_arch,
    &bfd_sh_arch,
    &bfd_sparc_arch,
    &bfd_spu_arch,
    &bfd_tic30_arch,
    &bfd_tic4x_arch,
    &bfd_tic54x_arch,
    &bfd_tic6x_arch,
    &bfd_tic80_arch,
    &bfd_tilegx_arch,
    &bfd_tilepro_arch,
    &bfd_v850_arch,
    &bfd_v850_rh850_arch,
    &bfd_vax_arch,
    &bfd_visium_arch,
    &bfd_w65_arch,
    &bfd_we32k_arch,
    &bfd_xstormy16_arch,
    &bfd_xtensa_arch,
    &bfd_xc16x_arch,
    &bfd_xgate_arch,
    &bfd_z80_arch,
    &bfd_z8k_arch,
#endif /* SELECT_ARCHITECTURES */
  0
};

/*
FUNCTION
	bfd_printable_name

SYNOPSIS
	const char *bfd_printable_name (bfd *abfd);

DESCRIPTION
	Return a printable string representing the architecture and machine
	from the pointer to the architecture info structure.

*/

const char *
bfd_printable_name (bfd *abfd)
{
  return abfd->arch_info->printable_name;
}

/*
FUNCTION
	bfd_scan_arch

SYNOPSIS
	const bfd_arch_info_type *bfd_scan_arch (const char *string);

DESCRIPTION
	Figure out if BFD supports any cpu which could be described with
	the name @var{string}.  Return a pointer to an <<arch_info>>
	structure if a machine is found, otherwise NULL.
*/

const bfd_arch_info_type *
bfd_scan_arch (const char *string)
{
  const bfd_arch_info_type * const *app, *ap;

  /* Look through all the installed architectures.  */
  for (app = bfd_archures_list; *app != NULL; app++)
    {
      for (ap = *app; ap != NULL; ap = ap->next)
	{
	  if (ap->scan (ap, string))
	    return ap;
	}
    }

  return NULL;
}

/*
FUNCTION
	bfd_arch_list

SYNOPSIS
	const char **bfd_arch_list (void);

DESCRIPTION
	Return a freshly malloced NULL-terminated vector of the names
	of all the valid BFD architectures.  Do not modify the names.
*/

const char **
bfd_arch_list (void)
{
  int vec_length = 0;
  const char **name_ptr;
  const char **name_list;
  const bfd_arch_info_type * const *app;
  bfd_size_type amt;

  /* Determine the number of architectures: */
  vec_length = 0;
  for (app = bfd_archures_list; *app != NULL; app++)
    {
      const bfd_arch_info_type *ap;
      for (ap = *app; ap != NULL; ap = ap->next)
	{
	  vec_length++;
	}
    }

  amt = (bfd_size_type)((size_t)(vec_length + 1) * sizeof(char **));
  name_list = (const char **)bfd_malloc(amt);
  if (name_list == NULL)
    return NULL;

  /* Point the list at each of the names: */
  name_ptr = name_list;
  for (app = bfd_archures_list; *app != NULL; app++)
    {
      const bfd_arch_info_type *ap;
      for (ap = *app; ap != NULL; ap = ap->next)
	{
	  *name_ptr = ap->printable_name;
	  name_ptr++;
	}
    }
  *name_ptr = NULL;

  return name_list;
}

/*
FUNCTION
	bfd_arch_get_compatible

SYNOPSIS
	const bfd_arch_info_type *bfd_arch_get_compatible
	  (const bfd *abfd, const bfd *bbfd, bfd_boolean accept_unknowns);

DESCRIPTION
	Determine whether two BFDs' architectures and machine types
	are compatible.  Calculates the lowest common denominator
	between the two architectures and machine types implied by
	the BFDs and returns a pointer to an <<arch_info>> structure
	describing the compatible machine.
*/

const bfd_arch_info_type *
bfd_arch_get_compatible(const bfd *abfd, const bfd *bbfd,
                        bfd_boolean accept_unknowns)
{
  const bfd *ubfd = (bfd *)NULL;

  /* Look for an unknown architecture: */
  if (((ubfd = abfd) && (ubfd->arch_info->arch == bfd_arch_unknown))
      || ((ubfd = bbfd) && (ubfd->arch_info->arch == bfd_arch_unknown)))
    {
      /* We can allow an unknown architecture if accept_unknowns
	 is true, or if the target is the "binary" format, which
	 has an unknown architecture.  Since the binary format can
	 only be set by explicit request from the user, it is safe
	 to assume that they know what they are doing.  */
      if (accept_unknowns
	  || (strcmp(bfd_get_target(ubfd), "binary") == 0))
	return ubfd->arch_info;
      return NULL;
    }

  BFD_ASSERT((abfd != NULL) && (bbfd != NULL));
  BFD_ASSERT((abfd->arch_info != NULL) && (bbfd->arch_info != NULL));

  /* Otherwise architecture-specific code has to decide: */
  return abfd->arch_info->compatible(abfd->arch_info, bbfd->arch_info);
}

/*
INTERNAL_DEFINITION
	bfd_default_arch_struct

DESCRIPTION
	The <<bfd_default_arch_struct>> is an item of
	<<bfd_arch_info_type>> which has been initialized to a fairly
	generic state.  A BFD starts life by pointing to this
	structure, until the correct back end has determined the real
	architecture of the file.

.extern const bfd_arch_info_type bfd_default_arch_struct;
*/

const bfd_arch_info_type bfd_default_arch_struct = {
  32, 32, 8, bfd_arch_unknown, 0, "unknown", "unknown", 2, TRUE,
  bfd_default_compatible,
  bfd_default_scan,
  0,
};

/*
FUNCTION
	bfd_set_arch_info

SYNOPSIS
	void bfd_set_arch_info (bfd *abfd, const bfd_arch_info_type *arg);

DESCRIPTION
	Set the architecture info of @var{abfd} to @var{arg}.
*/

void
bfd_set_arch_info (bfd *abfd, const bfd_arch_info_type *arg)
{
  abfd->arch_info = arg;
}

/*
INTERNAL_FUNCTION
	bfd_default_set_arch_mach

SYNOPSIS
	bfd_boolean bfd_default_set_arch_mach
	  (bfd *abfd, enum bfd_architecture arch, unsigned long mach);

DESCRIPTION
	Set the architecture and machine type in BFD @var{abfd}
	to @var{arch} and @var{mach}.  Find the correct
	pointer to a structure and insert it into the <<arch_info>>
	pointer.
*/

bfd_boolean
bfd_default_set_arch_mach(bfd *abfd, enum bfd_architecture arch,
			  unsigned long mach)
{
  abfd->arch_info = bfd_lookup_arch(arch, mach);
  if (abfd->arch_info != NULL)
    return TRUE;

  abfd->arch_info = &bfd_default_arch_struct;
  bfd_set_error(bfd_error_bad_value);
  return FALSE;
}

/*
FUNCTION
	bfd_get_arch

SYNOPSIS
	enum bfd_architecture bfd_get_arch (bfd *abfd);

DESCRIPTION
	Return the enumerated type which describes the BFD @var{abfd}'s
	architecture.
*/

enum bfd_architecture
bfd_get_arch (bfd *abfd)
{
  return abfd->arch_info->arch;
}

/*
FUNCTION
	bfd_get_mach

SYNOPSIS
	unsigned long bfd_get_mach (bfd *abfd);

DESCRIPTION
	Return the long type which describes the BFD @var{abfd}'s
	machine.
*/

unsigned long
bfd_get_mach (bfd *abfd)
{
  return abfd->arch_info->mach;
}

/*
FUNCTION
	bfd_arch_bits_per_byte

SYNOPSIS
	unsigned int bfd_arch_bits_per_byte (bfd *abfd);

DESCRIPTION
	Return the number of bits in one of the BFD @var{abfd}'s
	architecture's bytes.
*/

unsigned int
bfd_arch_bits_per_byte(bfd *abfd)
{
  return (unsigned int)abfd->arch_info->bits_per_byte;
}

/*
FUNCTION
	bfd_arch_bits_per_address

SYNOPSIS
	unsigned int bfd_arch_bits_per_address (bfd *abfd);

DESCRIPTION
	Return the number of bits in one of the BFD @var{abfd}'s
	architecture's addresses.
*/

unsigned int
bfd_arch_bits_per_address(bfd *abfd)
{
  return (unsigned int)abfd->arch_info->bits_per_address;
}

/*
INTERNAL_FUNCTION
	bfd_default_compatible

SYNOPSIS
	const bfd_arch_info_type *bfd_default_compatible
	  (const bfd_arch_info_type *a, const bfd_arch_info_type *b);

DESCRIPTION
	The default function for testing for compatibility.
*/

const bfd_arch_info_type *
bfd_default_compatible (const bfd_arch_info_type *a,
			const bfd_arch_info_type *b)
{
  if (a->arch != b->arch)
    return NULL;

  if (a->bits_per_word != b->bits_per_word)
    return NULL;

  if (a->mach > b->mach)
    return a;

  if (b->mach > a->mach)
    return b;

  return a;
}

/*
INTERNAL_FUNCTION
	bfd_default_scan

SYNOPSIS
	bfd_boolean bfd_default_scan
	  (const struct bfd_arch_info *info, const char *string);

DESCRIPTION
	The default function for working out whether this is an
	architecture hit and a machine hit.
*/

bfd_boolean
bfd_default_scan (const bfd_arch_info_type *info, const char *string)
{
  const char *ptr_src;
  const char *ptr_tst;
  unsigned long number;
  enum bfd_architecture arch;
  const char *printable_name_colon;

  /* Exact match of the architecture name (ARCH_NAME) and also the
     default architecture?  */
  if ((strcasecmp(string, info->arch_name) == 0)
      && info->the_default)
    return TRUE;

  /* Exact match of the machine name (PRINTABLE_NAME)?  */
  if (strcasecmp(string, info->printable_name) == 0)
    return TRUE;

  /* Given that printable_name contains no colon, attempt to match:
     ARCH_NAME [ ":" ] PRINTABLE_NAME?  */
  printable_name_colon = strchr(info->printable_name, ':');
  if (printable_name_colon == NULL)
    {
      size_t strlen_arch_name = strlen(info->arch_name);
      if (strncasecmp(string, info->arch_name, strlen_arch_name) == 0)
	{
	  if (string[strlen_arch_name] == ':')
	    {
	      if (strcasecmp((string + strlen_arch_name + 1),
                             info->printable_name) == 0)
		return TRUE;
	    }
	  else
	    {
	      if (strcasecmp((string + strlen_arch_name),
                             info->printable_name) == 0)
		return TRUE;
	    }
	}
    }

  /* Given that PRINTABLE_NAME has the form: <arch> ":" <mach>;
     Attempt to match: <arch> <mach>?  */
  if (printable_name_colon != NULL)
    {
      size_t colon_index;
      colon_index = (size_t)(printable_name_colon - info->printable_name);
      if ((strncasecmp(string, info->printable_name, colon_index) == 0)
	  && (strcasecmp((string + colon_index),
			 (info->printable_name + colon_index + 1)) == 0))
	return TRUE;
    }

  /* Given that PRINTABLE_NAME has the form: <arch> ":" <mach>; Do not
     attempt to match just <mach>, it could be ambiguous.  This test
     is left until later.  */

  /* NOTE: The below is retained for compatibility only.  Please do
     not add to this code.  */

  /* See how much of the supplied string matches with the
     architecture, eg the string m68k:68020 would match the 68k entry
     up to the :, then we get left with the machine number.  */

  for (ptr_src = string, ptr_tst = info->arch_name;
       *ptr_src && *ptr_tst;
       ptr_src++, ptr_tst++)
    {
      if (*ptr_src != *ptr_tst)
	break;
    }

  /* Chewed up as much of the architecture as will match, skip any
     colons.  */
  if (*ptr_src == ':')
    ptr_src++;

  if (*ptr_src == 0)
    {
      /* Nothing more, then only keep this one if it is the default
       * machine for this architecture: */
      return info->the_default;
    }

  number = 0UL;
  while (ISDIGIT(*ptr_src))
    {
      number = ((number * 10UL) + (unsigned long)*ptr_src - '0');
      ptr_src++;
    }

  /* NOTE: The below is retained for compatibility only.
   * PLEASE DO NOT ADD TO THIS CODE: */
  switch (number)
    {
      /* FIXME: These are needed to parse IEEE objects.  */
      /* The following 7 cases are here only for compatibility w/older
       * binutils (at least IEEE objs from binutils 2.9.1 require them): */
    case bfd_mach_m68000:
    case bfd_mach_m68010:
    case bfd_mach_m68020:
    case bfd_mach_m68030:
    case bfd_mach_m68040:
    case bfd_mach_m68060:
    case bfd_mach_cpu32:
      arch = bfd_arch_m68k;
      break;
    case 68000:
      arch = bfd_arch_m68k;
      number = bfd_mach_m68000;
      break;
    case 68010:
      arch = bfd_arch_m68k;
      number = bfd_mach_m68010;
      break;
    case 68020:
      arch = bfd_arch_m68k;
      number = bfd_mach_m68020;
      break;
    case 68030:
      arch = bfd_arch_m68k;
      number = bfd_mach_m68030;
      break;
    case 68040:
      arch = bfd_arch_m68k;
      number = bfd_mach_m68040;
      break;
    case 68060:
      arch = bfd_arch_m68k;
      number = bfd_mach_m68060;
      break;
    case 68332:
      arch = bfd_arch_m68k;
      number = bfd_mach_cpu32;
      break;
    case 5200:
      arch = bfd_arch_m68k;
      number = bfd_mach_mcf5200;
      break;
    case 5206:
      arch = bfd_arch_m68k;
      number = bfd_mach_mcf5206e;
      break;
    case 5307:
      arch = bfd_arch_m68k;
      number = bfd_mach_mcf5307;
      break;
    case 5407:
      arch = bfd_arch_m68k;
      number = bfd_mach_mcf5407;
      break;
    case 5282:
      arch = bfd_arch_m68k;
      number = bfd_mach_mcf528x;
      break;

    case 32000:
      arch = bfd_arch_we32k;
      break;

    case 3000:
      arch = bfd_arch_mips;
      number = bfd_mach_mips3000;
      break;

    case 4000:
      arch = bfd_arch_mips;
      number = bfd_mach_mips4000;
      break;

    case 6000:
      arch = bfd_arch_rs6000;
      break;

    case 7410:
      arch = bfd_arch_sh;
      number = bfd_mach_sh_dsp;
      break;

    case 7708:
      arch = bfd_arch_sh;
      number = bfd_mach_sh3;
      break;

    case 7729:
      arch = bfd_arch_sh;
      number = bfd_mach_sh3_dsp;
      break;

    case 7750:
      arch = bfd_arch_sh;
      number = bfd_mach_sh4;
      break;

    default:
      return FALSE;
    }

  if (arch != info->arch)
    return FALSE;

  if (number != info->mach)
    return FALSE;

  return TRUE;
}

/*
FUNCTION
	bfd_get_arch_info

SYNOPSIS
	const bfd_arch_info_type *bfd_get_arch_info (bfd *abfd);

DESCRIPTION
	Return the architecture info struct in @var{abfd}.
*/

const bfd_arch_info_type *
bfd_get_arch_info (bfd *abfd)
{
  return abfd->arch_info;
}

/*
FUNCTION
	bfd_lookup_arch

SYNOPSIS
	const bfd_arch_info_type *bfd_lookup_arch
	  (enum bfd_architecture arch, unsigned long machine);

DESCRIPTION
	Look for the architecture info structure which matches the
	arguments @var{arch} and @var{machine}. A machine of 0 matches the
	machine/architecture structure which marks itself as the
	default.
*/

const bfd_arch_info_type *
bfd_lookup_arch (enum bfd_architecture arch, unsigned long machine)
{
  const bfd_arch_info_type * const *app, *ap;

  for (app = bfd_archures_list; *app != NULL; app++)
    {
      for (ap = *app; ap != NULL; ap = ap->next)
	{
	  if (ap->arch == arch
	      && (ap->mach == machine
		  || (machine == 0 && ap->the_default)))
	    return ap;
	}
    }

  return NULL;
}

/*
FUNCTION
	bfd_printable_arch_mach

SYNOPSIS
	const char *bfd_printable_arch_mach
	  (enum bfd_architecture arch, unsigned long machine);

DESCRIPTION
	Return a printable string representing the architecture and
	machine type.

	This routine is depreciated.
*/

const char *
bfd_printable_arch_mach (enum bfd_architecture arch, unsigned long machine)
{
  const bfd_arch_info_type *ap = bfd_lookup_arch (arch, machine);

  if (ap)
    return ap->printable_name;
  return "UNKNOWN!";
}

/*
FUNCTION
	bfd_octets_per_byte

SYNOPSIS
	unsigned int bfd_octets_per_byte (bfd *abfd);

DESCRIPTION
	Return the number of octets (8-bit quantities) per target byte
        (minimum addressable unit).  In most cases, this will be one, but some
        DSP targets have 16, 32, or even 48 bits per byte.
*/

unsigned int
bfd_octets_per_byte (bfd *abfd)
{
  return bfd_arch_mach_octets_per_byte (bfd_get_arch (abfd),
					bfd_get_mach (abfd));
}

/*
FUNCTION
	bfd_arch_mach_octets_per_byte

SYNOPSIS
	unsigned int bfd_arch_mach_octets_per_byte
	  (enum bfd_architecture arch, unsigned long machine);

DESCRIPTION
	See bfd_octets_per_byte.

        This routine is provided for those cases where a bfd * is not
        available
*/

unsigned int
bfd_arch_mach_octets_per_byte(enum bfd_architecture arch,
                              unsigned long mach)
{
  const bfd_arch_info_type *ap = bfd_lookup_arch(arch, mach);

  if (ap) {
    return (unsigned int)((unsigned int)ap->bits_per_byte / 8U);
  }
  return 1U;
}

/*
INTERNAL_FUNCTION
	bfd_arch_default_fill

SYNOPSIS
	void *bfd_arch_default_fill (bfd_size_type count,
				     bfd_boolean is_bigendian,
				     bfd_boolean code);

DESCRIPTION
	Allocate via bfd_malloc and return a fill buffer of size COUNT.
	If IS_BIGENDIAN is TRUE, the order of bytes is big endian.  If
	CODE is TRUE, the buffer contains code.
*/

void *
bfd_arch_default_fill(bfd_size_type count,
		      bfd_boolean is_bigendian ATTRIBUTE_UNUSED,
		      bfd_boolean code ATTRIBUTE_UNUSED)
{
  void *fill = bfd_malloc(count);
  if (fill != NULL)
    memset(fill, 0, (size_t)count);
  return fill;
}

/* End of archures.c */
