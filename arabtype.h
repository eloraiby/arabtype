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
#ifndef ARABTYPE_H
#define ARABTYPE_H
#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus

/**
 * @brief get_presentation_form_b
 * @param prev previous character
 * @param next next character
 * @param cp the current character
 * @return 0 ignore the code point, the transformed code point otherise
 */
uint32_t get_presentation_form_b(uint32_t prev, uint32_t next, uint32_t cp);

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif // ARABTYPE_H
