# Arabtype

A small and simple implementation that transform isolated arabic utf8 character
strings into contextual forms useful for rendering (rendering example is provided).

This is mostly suitable for lightweight UIs (embedded systems, games, media players...).
It's worthy to note that this is far more lightweight than HarfBuzz (used by Qt/Pango...).
Freetype 2 on the other hand lacks this functionality.

### Usage

One call will convert a utf8 string to a sequence of code points:

```C
size_t get_presentation_form_b(size_t in_len,
                               unsigned char *in_str,
                               size_t out_len,
                               uint32_t* out_cp);
```

Where

`in_len`: the input utf8 string length in bytes

`in_str`: the input utf8 string

`out_len`: output code point buffer size (in bytes)

`out_cp`: output code point buffer	

`return`: return the total number of code points transformed

### Details
Arabic letters have 4 forms: Isolated, Initial, Medial and Ending. An arabic letter will have one of these forms depending on the letters preceding and succeeding it: For instance take the letter ﺡ : This is the isolated form (i.e. nothing precedes nor succeeds it), if it comes at the start of a syllable it will have the initial form ( ﺣ ). If it ends a syllable it will have the ending form ( ﺢ ) and if it is in the middle of a syllable it will have the medial form ( ﺤ ).  Some even have more complicated ligature forms (such as Lam and Alef together: ﻻ ) . A utf8 arabic string usually only comprises of isolated letters from [Arabic Unicode Block](http://en.wikipedia.org/wiki/Arabic_%28Unicode_block%29). With this library you will transform it to the rendering/presentation form: [Arabic Presentation Forms B](http://en.wikipedia.org/wiki/Arabic_Presentation_Forms-B)


### Example ([Khalil Gibran](http://en.wikipedia.org/wiki/Kahlil_Gibran)'s Al-Nay)

![ScreenShot](https://raw.github.com/eloraiby/arabtype/master/example.png)


### LICENSE

Arabtype, Copyrights 2012-2015(c) Wael El Oraiby. All rights reserved.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License 3 for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the [GCC Runtime Library Exception](https://www.gnu.org/licenses/gcc-exception-3.1.en.html), version 3.1,
as published by the Free Software Foundation.

Roughly speaking: it means that you can statically link with the code but you are obliged
to make open your bug fixes and/or enhancement to the arabtype.c/arabtype.h code.

-----------------------------------------------------------------------
The utf8.c file is under the following license:

Copyright (c) 2008-2009 Bjoern Hoehrmann &lt;bjoern@hoehrmann.de&gt;

See <A HREF="http://bjoern.hoehrmann.de/utf-8/decoder/dfa/">http://bjoern.hoehrmann.de/utf-8/decoder/dfa/</A> for details.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the &quot;Software&quot;), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED &quot;AS IS&quot;, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
