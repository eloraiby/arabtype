/*
** Arabtype Copyright 2012-2013(c) Wael El Oraiby. All Rights Reserved
** see README.md file for license
*/

#include <stdbool.h>
#include "utf8.h"
#include "arabtype.h"

#define ARABIC_LETTER_START		0x621
#define ARABIC_LETTER_END		0x64A

#define ISOLATED	0
#define ENDING		1
#define INITIAL		2
#define MEDIAL		3

//
// 0: isolated form
// 1: ending form
// 2: beginning form	(if 0, it's a cutting type)
// 3: middle form
//
typedef uint32_t char_form[4];

char_form	arabic_forms_b[]	=
{
	{0xFE80, 0xFE80, 0, 0},			// hamza	(0)
	{0xFE81, 0xFE82, 0, 0},			// 2alif madda	(1)
	{0xFE83, 0xFE84, 0, 0},			// 2alif hamza	(2)
	{0xFE85, 0xFE86, 0, 0},			// waw hamza	(3)
	{0xFE87, 0xFE88, 0, 0},			// 2alif hamza maksoura	(4)
	{0xFE89, 0xFE8A, 0xFE8B, 0xFE8C},	// 2alif maqsoura hamza	(5)
	{0xFE8D, 0xFE8E, 0, 0},			// 2alif	(6)
	{0xFE8F, 0xFE90, 0xFE91, 0xFE92},	// ba2		(7)
	{0xFE93, 0xFE94, 0, 0},			// ta2 marbouta	(8)
	{0xFE95, 0xFE96, 0xFE97, 0xFE98},	// ta2		(9)
	{0xFE99, 0xFE9A, 0xFE9B, 0xFE9C},	// tha2		(10)
	{0xFE9D, 0xFE9E, 0xFE9F, 0xFEA0},	// jim		(11)
	{0xFEA1, 0xFEA2, 0xFEA3, 0xFEA4},	// 7a2		(12)
	{0xFEA5, 0xFEA6, 0xFEA7, 0xFEA8},	// kha2		(13)
	{0xFEA9, 0xFEAA, 0, 0},			// dal		(14)
	{0xFEAB, 0xFEAC, 0, 0},			// dhal		(15)
	{0xFEAD, 0xFEAE, 0, 0},			// ra2		(16)
	{0xFEAF, 0xFEB0, 0, 0},			// zayn		(17)
	{0xFEB1, 0xFEB2, 0xFEB3, 0xFEB4},	// syn		(18)
	{0xFEB5, 0xFEB6, 0xFEB7, 0xFEB8},	// shin		(19)
	{0xFEB9, 0xFEBA, 0xFEBB, 0xFEBC},	// sad		(20)
	{0xFEBD, 0xFEBE, 0xFEBF, 0xFEC0},	// dad		(21)
	{0xFEC1, 0xFEC2, 0xFEC3, 0xFEC4},	// tah		(22)
	{0xFEC5, 0xFEC6, 0xFEC7, 0xFEC8},	// thah		(23)
	{0xFEC9, 0xFECA, 0xFECB, 0xFECC},	// 3ayn		(24)
	{0xFECD, 0xFECE, 0xFECF, 0xFED0},	// ghayn	(25)
	{0, 0, 0, 0},				//		(26)
	{0, 0, 0, 0},				//		(27)
	{0, 0, 0, 0},				//		(28)
	{0, 0, 0, 0},				//		(29)
	{0, 0, 0, 0},				//		(30)
	{0x640, 0x640, 0x640, 0x640},		// wasla	(31)
	{0xFED1, 0xFED2, 0xFED3, 0xFED4},	// fa2		(32)
	{0xFED5, 0xFED6, 0xFED7, 0xFED8},	// qaf		(33)
	{0xFED9, 0xFEDA, 0xFEDB, 0xFEDC},	// kaf		(34)
	{0xFEDD, 0xFEDE, 0xFEDF, 0xFEE0},	// lam		(35)
	{0xFEE1, 0xFEE2, 0xFEE3, 0xFEE4},	// mim		(36)
	{0xFEE5, 0xFEE6, 0xFEE7, 0xFEE8},	// noon		(37)
	{0xFEE9, 0xFEEA, 0xFEEB, 0xFEEC},	// ha2		(38)
	{0xFEED, 0xFEEE, 0, 0},			// waw		(39)
	{0xFEFF, 0xFEF0, 0, 0},			// 2alif maksoura	(40)
	{0xFEF1, 0xFEF2, 0xFEF3, 0xFEF4}	// ya2		(41)
};

bool is_arabic_letter(uint32_t cp)
{
	return ( cp >= ARABIC_LETTER_START && cp <=  ARABIC_LETTER_END );
}

bool is_linking_type(uint32_t cp)
{
	if( is_arabic_letter(cp) )
		if( arabic_forms_b[cp - ARABIC_LETTER_START][INITIAL] || arabic_forms_b[cp - ARABIC_LETTER_START][MEDIAL] )
			return true;
	return false;
}


uint32_t get_presentation_form_b(uint32_t prev, uint32_t next, uint32_t cp)
{
	if( !is_arabic_letter(cp) )
		return cp;	/* not an Arabic letter */

	uint32_t index	= ((is_arabic_letter(next) & is_linking_type(cp)) << 1) | is_linking_type(prev);

	return arabic_forms_b[cp -  ARABIC_LETTER_START][index];
}
