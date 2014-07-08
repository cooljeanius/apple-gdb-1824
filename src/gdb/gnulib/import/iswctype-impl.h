/* iswctype-impl.h: Test whether a wide character has a given property.
 * Copyright (C) 2011-2012 Free Software Foundation, Inc.
 * Written by Bruno Haible <bruno@clisp.org>, 2011.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GL_ISWCTYPE_IMPL_H
#define _GL_ISWCTYPE_IMPL_H 1

#if !defined(__WCTYPE_H_)
int iswctype(wint_t wc, wctype_t desc)
{
  return ((int (*)(wint_t))desc)(wc);
}
#else
typedef int iswctype_impl_h_dummy_t;
#endif /* !__WCTYPE_H_ */

#endif /* !_GL_ISWCTYPE_IMPL_H */

/* EOF */
