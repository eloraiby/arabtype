/*
** Arabtype Copyright 2012-2015(c) Wael El Oraiby. All Rights Reserved
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.

** Under Section 7 of GPL version 3, you are granted additional
** permissions described in the GCC Runtime Library Exception, version
** 3.1, as published by the Free Software Foundation.

** You should have received a copy of the GNU General Public License and
** a copy of the GCC Runtime Library Exception along with this program;
** see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
** <http://www.gnu.org/licenses/>.
**
** see README.md file for more information
*/

#include <stdbool.h>
#include <stdio.h>
#include "utf8.h"
#include "arabtype.h"

#define ARABIC_LETTER_START		0x621
#define ARABIC_LETTER_END		0x64A

#define ISOLATED	0
#define ENDING		1
#define INITIAL		2
#define MEDIAL		3

#define UNICODE_LAM	0x644

//
// 0: isolated form
// 1: ending form
// 2: beginning form	(if 0, it's a cutting type)
// 3: middle form
//
typedef uint32_t char_form_t[4];
typedef char_form_t	all_form_t[2];

static all_form_t arabic_forms_b[]	= {
	{ {0xFE80, 0xFE80,      0,      0}, {0, 0, 0, 0} },	// hamza	(0)
	{ {0xFE81, 0xFE82,      0,      0}, {0, 0, 0xFEF5, 0xFEF6} },	// 2alif madda	(1)
	{ {0xFE83, 0xFE84,      0,      0}, {0, 0, 0xFEF7, 0xFEF8} },	// 2alif hamza	(2)
	{ {0xFE85, 0xFE86,      0,      0}, {0, 0, 0, 0} },	// waw hamza	(3)
	{ {0xFE87, 0xFE88,      0,      0}, {0, 0, 0xFEF9, 0xFEFA} },	// 2alif hamza maksoura	(4)
	{ {0xFE89, 0xFE8A, 0xFE8B, 0xFE8C}, {0, 0, 0, 0} },	// 2alif maqsoura hamza	(5)
	{ {0xFE8D, 0xFE8E,      0,      0}, {0, 0, 0xFEFB, 0xFEFC} },	// 2alif	(6)
	{ {0xFE8F, 0xFE90, 0xFE91, 0xFE92}, {0, 0, 0, 0} },	// ba2		(7)
	{ {0xFE93, 0xFE94,      0,      0}, {0, 0, 0, 0} },	// ta2 marbouta	(8)
	{ {0xFE95, 0xFE96, 0xFE97, 0xFE98}, {0, 0, 0, 0} },	// ta2		(9)
	{ {0xFE99, 0xFE9A, 0xFE9B, 0xFE9C}, {0, 0, 0, 0} },	// tha2		(10)
	{ {0xFE9D, 0xFE9E, 0xFE9F, 0xFEA0}, {0, 0, 0, 0} },	// jim		(11)
	{ {0xFEA1, 0xFEA2, 0xFEA3, 0xFEA4}, {0, 0, 0, 0} },	// 7a2		(12)
	{ {0xFEA5, 0xFEA6, 0xFEA7, 0xFEA8}, {0, 0, 0, 0} },	// kha2		(13)
	{ {0xFEA9, 0xFEAA,      0,      0}, {0, 0, 0, 0} },	// dal		(14)
	{ {0xFEAB, 0xFEAC,      0,      0}, {0, 0, 0, 0} },	// dhal		(15)
	{ {0xFEAD, 0xFEAE,      0,      0}, {0, 0, 0, 0} },	// ra2		(16)
	{ {0xFEAF, 0xFEB0,      0,      0}, {0, 0, 0, 0} },	// zayn		(17)
	{ {0xFEB1, 0xFEB2, 0xFEB3, 0xFEB4}, {0, 0, 0, 0} },	// syn		(18)
	{ {0xFEB5, 0xFEB6, 0xFEB7, 0xFEB8}, {0, 0, 0, 0} },	// shin		(19)
	{ {0xFEB9, 0xFEBA, 0xFEBB, 0xFEBC}, {0, 0, 0, 0} },	// sad		(20)
	{ {0xFEBD, 0xFEBE, 0xFEBF, 0xFEC0}, {0, 0, 0, 0} },	// dad		(21)
	{ {0xFEC1, 0xFEC2, 0xFEC3, 0xFEC4}, {0, 0, 0, 0} },	// tah		(22)
	{ {0xFEC5, 0xFEC6, 0xFEC7, 0xFEC8}, {0, 0, 0, 0} },	// thah		(23)
	{ {0xFEC9, 0xFECA, 0xFECB, 0xFECC}, {0, 0, 0, 0} },	// 3ayn		(24)
	{ {0xFECD, 0xFECE, 0xFECF, 0xFED0}, {0, 0, 0, 0} },	// ghayn	(25)
	{ {     0,      0,      0,      0}, {0, 0, 0, 0} },	//		(26)
	{ {     0,      0,      0,      0}, {0, 0, 0, 0} },	//		(27)
	{ {     0,      0,      0,      0}, {0, 0, 0, 0} },	//		(28)
	{ {     0,      0,      0,      0}, {0, 0, 0, 0} },	//		(29)
	{ {     0,      0,      0,      0}, {0, 0, 0, 0} },	//		(30)
	{ { 0x640,  0x640,  0x640,  0x640}, {0, 0, 0, 0} },	// wasla	(31)
	{ {0xFED1, 0xFED2, 0xFED3, 0xFED4}, {0, 0, 0, 0} },	// fa2		(32)
	{ {0xFED5, 0xFED6, 0xFED7, 0xFED8}, {0, 0, 0, 0} },	// qaf		(33)
	{ {0xFED9, 0xFEDA, 0xFEDB, 0xFEDC}, {0, 0, 0, 0} },	// kaf		(34)
	{ {0xFEDD, 0xFEDE, 0xFEDF, 0xFEE0}, {0, 0, 0, 0} },	// lam		(35)
	{ {0xFEE1, 0xFEE2, 0xFEE3, 0xFEE4}, {0, 0, 0, 0} },	// mim		(36)
	{ {0xFEE5, 0xFEE6, 0xFEE7, 0xFEE8}, {0, 0, 0, 0} },	// noon		(37)
	{ {0xFEE9, 0xFEEA, 0xFEEB, 0xFEEC}, {0, 0, 0, 0} },	// ha2		(38)
	{ {0xFEED, 0xFEEE,      0,      0}, {0, 0, 0, 0} },	// waw		(39)
	{ {0xFEFF, 0xFEF0,      0,      0}, {0, 0, 0, 0} },	// 2alif maksoura	(40)
	{ {0xFEF1, 0xFEF2, 0xFEF3, 0xFEF4}, {0, 0, 0, 0} },	// ya2		(41)
};



static inline bool is_arabic_letter(uint32_t cp)		{ return ( cp >= ARABIC_LETTER_START && cp <=  ARABIC_LETTER_END ); }
static inline bool is_lam_alef(uint32_t cp, uint32_t next)	{ return cp == UNICODE_LAM &&
									 is_arabic_letter(next) &&
									 arabic_forms_b[next - ARABIC_LETTER_START][1][INITIAL] != 0; }
static inline bool is_alef_prev_lam(uint32_t prev, uint32_t cp)	{ return prev == UNICODE_LAM &&
									 is_arabic_letter(cp) &&
									 arabic_forms_b[cp - ARABIC_LETTER_START][1][INITIAL] != 0; }
static inline bool is_linking_type(uint32_t cp) {
	if( is_arabic_letter(cp) && ( arabic_forms_b[cp - ARABIC_LETTER_START][0][INITIAL] || arabic_forms_b[cp - ARABIC_LETTER_START][0][MEDIAL] ) ) {
		return true;
	} else {
		return false;
	}
}


uint32_t get_presentation_form_b(uint32_t prev, uint32_t next, uint32_t cp) {
	if( !is_arabic_letter(cp) ) {
		return cp;	/* not an Arabic letter */
	}

	if( is_lam_alef(cp, next) ) {
		uint32_t index	= (is_linking_type(cp) << 1) | is_linking_type(prev);
		return arabic_forms_b[next -  ARABIC_LETTER_START][1][index];

	} else {
		if( is_alef_prev_lam(prev, cp) ) {
			return -1;	// skip previously processed lam alef
		} else {
			uint32_t index	= ((is_arabic_letter(next) & is_linking_type(cp)) << 1) | is_linking_type(prev);

			return arabic_forms_b[cp -  ARABIC_LETTER_START][0][index];
		}
	}
}
