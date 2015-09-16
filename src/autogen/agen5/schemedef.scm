
;;; Time-stamp:        "2012-01-07 09:25:09 bkorb"
;;;
;;; This file is part of AutoGen.
;;; AutoGen Copyright (c) 1992-2012 by Bruce Korb - all rights reserved
;;;
;;; AutoGen is free software: you can redistribute it and/or modify it
;;; under the terms of the GNU General Public License as published by the
;;; Free Software Foundation, either version 3 of the License, or
;;; (at your option) any later version.
;;;
;;; AutoGen is distributed in the hope that it will be useful, but
;;; WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
;;; See the GNU General Public License for more details.
;;;
;;; You should have received a copy of the GNU General Public License along
;;; with this program.  If not, see <http://www.gnu.org/licenses/>.
;;;
;;; This module defines all the scheme initialization for AutoGen.
;;; It gets sucked up into directives.h as a single ANSI-C string.
;;; Comments, blank lines and leading white space are omitted.
;;;
;;; The contents of this file get converted to a C language static string
;;; and is then fed to the guile library at start up time.
;;; Blank lines, commends, leading and trailing white space and spaces
;;; before closing parentheses are all stripped.
;;;
(use-modules (ice-9 common-list))

(define identifier?
  (lambda (x) (or (string? x) (symbol? x))))

(define normalize-identifier
  (lambda (x)
    (if (string? x) (string->symbol x) x)))

(define coerce->string
  (lambda (x)
    (let ((char->string (lambda (x) (make-string 1 x)))
          (coercable? (lambda (x)
            (or (string? x) (boolean? x) (char? x)
                (symbol? x) (list? x) (number? x)) )) )

      (if (not (coercable? x))
          (error "Wrong type to coerce->string" x))

      (cond
        ((string? x)  (string-append
            (char->string #\") x (char->string #\")  ))

        ; Probably not what was wanted, but fun
        ((boolean? x) (if x "#t" "#f"))
        ((char? x)    (char->string x))
        ((number? x)  (number->string x))
        ((symbol? x)  (symbol->string x))
        ((list? x)    (if (every coercable? x)
            (apply string-append (map coerce->string x))  ))
) ) ) )


;;; alist->autogen-def:
;;; take a scheme alist of values, and create autogen assignments.
;;; recursive alists are handled. Using a bare list as a value to be
;;; assigned is not a terribly good idea, though it should work if it
;;; doesn't look too much like an alist The returned string doesn't
;;; contain opening and closing brackets.

(define alist->autogen-def
  (lambda (lst . recursive)
    (if (null? recursive) (set! recursive #f)
        (set! recursive #t))
    (let ((res (if recursive "{\n" ""))
          (list-nnul? (lambda (x) (and (list? x) (not (null? x))))))
      (do ((i lst (cdr i)))
          ((null? i) (if recursive
                          (string-append res "}")
                           res))
        (let* ((kvpair (car i))
               (value (cdr kvpair))
               (value-is-alist (if (and (list-nnul? value)
                                        (list-nnul? (car value))
                                        (list-nnul? (caar value))
                                        (identifier? (caaar value)))
                                   #t #f)))
          (set! res (string-append res
                (coerce->string (normalize-identifier (car kvpair)))
                " = "
                (if value-is-alist
                    (alist->autogen-def (car value) 1)
                    (coerce->string (cdr kvpair)))
                ";\n"
) ) ) ) ) )         )

(define shell-cleanup "")
(define add-cleanup (lambda (t)
   (set! shell-cleanup (string-append shell-cleanup "\n" t "\n"))  ))
(define tmp-dir "")

(define header-file     "")
(define header-guard    "")
(define autogen-version "AUTOGEN_VERSION")
(define c-file-line-fmt "#line %2$d \"%1$s\"\n")

(define-macro (defined-as predicate symbol)
  `(and (defined? ',symbol) (,predicate ,symbol)))

;;; /*=gfunc   html_escape_encode
;;;  *
;;;  * what:   encode html special characters
;;;  * general-use:
;;;  *
;;;  * exparg: str , string to make substitutions in
;;;  *
;;;  * doc:    This function will replace replace the characters @code{'&'},
;;;  *         @code{'<'} and @code{'>'} characters with the HTML/XML
;;;  *         escape-encoded strings (@code{"&amp;"}, @code{"&lt;"}, and
;;;  *         @code{"&gt;"}, respectively).
;;; =*/
;;;
(define html-escape-encode (lambda (str)
    (string-substitute str
          '("&"      "<"     ">")
          '("&amp;"  "&lt;"  "&gt;") ) ))

(define stt-table   (make-hash-table 31))
(define stt-curr    stt-table)
(define stt-idx-tbl stt-table)
(define stt-idx     0)

;;; /*=gfunc   string_table_new
;;;  *
;;;  * what:   create a string table
;;;  * general-use:
;;;  *
;;;  * exparg: st-name , the name of the array of characters
;;;  *
;;;  * doc:
;;;  *   This function will create an array of characters.  The companion
;;;  *   functions, (@xref{SCM string-table-add},
;;;  *   @xref{SCM string-table-add-ref}, and
;;;  *   @pxref{SCM emit-string-table}) will insert text and emit the
;;;  *   populated table.
;;;  *
;;;  *   With these functions, it should be much easier to construct
;;;  *   structures containing string offsets instead of string pointers.
;;;  *   That can be very useful when transmitting, storing or sharing data
;;;  *   with different address spaces.
;;;  *
;;;  *   @noindent
;;;  *   Here is a brief example copied from the strtable.test test:
;;;  *
;;;  *   @example
;;;  *      [+ (string-table-new "scribble")
;;;  *    `'   (out-push-new) ;; redirect output to temporary
;;;  *    `'   (define ct 1)  +][+
;;;  *
;;;  *      FOR str IN that was the week that was +][+
;;;  *    `'  (set! ct (+ ct 1))
;;;  *      +]
;;;  *    `'    [+ (string-table-add-ref "scribble" (get "str")) +],[+
;;;  *      ENDFOR  +]
;;;  *      [+ (out-suspend "main")
;;;  *    `'   (emit-string-table "scribble")
;;;  *    `'   (ag-fprintf 0 "\nchar const *ap[%d] = @{" ct)
;;;  *    `'   (out-resume "main")
;;;  *    `'   (out-pop #t) ;; now dump out the redirected output +]
;;;  *    `'    NULL @};
;;;  *   @end example
;;;  *
;;;  *   @noindent
;;;  *   Some explanation:
;;;  *
;;;  *   @noindent
;;;  *   I added the @code{(out-push-new)} because the string table text is
;;;  *   diverted into an output stream named, ``scribble'' and I want to
;;;  *   have the string table emitted before the string table references.
;;;  *   The string table references are also emitted inside the @code{FOR}
;;;  *   loop.  So, when the loop is done, the current output is suspended
;;;  *   under the name, ``main'' and the ``scribble'' table is then emitted
;;;  *   into the primary output.  (@code{emit-string-table} inserts its
;;;  *   output directly into the current output stream.  It does not need to
;;;  *   be the last function in an AutoGen macro block.)  Next I
;;;  *   @code{ag-fprintf} the array-of-pointer declaration directly into the
;;;  *   current output.  Finally I restore the ``main'' output stream and
;;;  *   @code{(out-pop #t)}-it into the main output stream.
;;;  *
;;;  *   Here is the result.  Note that duplicate strings are not repeated
;;;  *   in the string table:
;;;  *
;;;  *   @example
;;;  *      static char const scribble[18] =
;;;  *    `'    "that\0" "was\0"  "the\0"  "week\0";
;;;  *
;;;  *      char const *ap[7] = @{
;;;  *    `'    scribble+0,
;;;  *    `'    scribble+5,
;;;  *    `'    scribble+9,
;;;  *    `'    scribble+13,
;;;  *    `'    scribble+0,
;;;  *    `'    scribble+5,
;;;  *    `'    NULL @};
;;;  *   @end example
;;;  *
;;;  *   These functions use the global name space @code{stt-*} in addition to
;;;  *   the function names.
;;;  *
;;;  *   If you utilize this in your programming, it is recommended that you
;;;  *   prevent printf format usage warnings with the GCC option
;;;  *   @code{-Wno-format-contains-nul}
;;; =*/
;;;
(define string-table-new (lambda (st-name) (begin
   (set! stt-curr (make-hash-table 31))
   (hash-create-handle! stt-table st-name stt-curr)
   (out-push-new)
   (out-suspend st-name)
   (set! stt-idx-tbl (make-hash-table 31))
   (hash-create-handle! stt-curr "string-indexes" stt-idx-tbl)
   (hash-create-handle! stt-curr "current-index"  0)
   ""
)))

;;; /*=gfunc   string_table_add
;;;  * general-use:
;;;  *
;;;  * what:   Add an entry to a string table
;;;  *
;;;  * exparg: st-name , the name of the array of characters
;;;  * exparg: str-val , the (possibly) new value to add
;;;  *
;;;  * doc:    Check for a duplicate string and, if none, then insert a new
;;;  *         string into the string table.  In all cases, returns the
;;;  *         character index of the beginning of the string in the table.
;;;  *
;;;  *         The returned index can be used in expressions like:
;;;  *         @example
;;;  *         string_array + <returned-value>
;;;  *         @end example
;;;  *         @noindent
;;;  *         that will yield the address of the first byte of the inserted
;;;  *         string.  See the @file{strtable.test} AutoGen test for a usage
;;;  *         example.
;;; =*/
;;;
(define string-table-add (lambda (st-name str-val) (begin
   (set! stt-curr    (hash-ref stt-table   st-name))
   (set! stt-idx-tbl (hash-ref stt-curr    "string-indexes"))
   (set! stt-idx     (hash-ref stt-idx-tbl str-val))
   (if (not (number? stt-idx))
       (begin
          (set! stt-idx (hash-ref stt-curr "current-index"))
          (ag-fprintf st-name "/* %5d */ %s \"\\0\"\n"
                      stt-idx (c-string str-val))
          (hash-create-handle! stt-idx-tbl str-val stt-idx)
          (hash-set! stt-curr "current-index"
                    (+ stt-idx (string-length str-val) 1)  )
   )   )
   stt-idx
)))

;;; /*=gfunc   string_table_add_ref
;;;  *
;;;  * what:   Add an entry to a string table, get reference
;;;  * general-use:
;;;  *
;;;  * exparg: st-name , the name of the array of characters
;;;  * exparg: str-val , the (possibly) new value to add
;;;  *
;;;  * doc:    Identical to string-table-add, except the value returned
;;;  *         is the string "st-name" '+' and the index returned by
;;;  *         string-table-add.
;;; =*/
;;;
(define string-table-add-ref (lambda (st-name str-val)
   (string-append st-name "+"
      (number->string (string-table-add st-name str-val)) ) ))

;;; /*=gfunc   emit_string_table
;;;  *
;;;  * what:   output a string table
;;;  *
;;;  * exparg: st-name , the name of the array of characters
;;;  *
;;;  * doc:    Emit into the current output stream a
;;;  *         @code{static char const} array named @code{st-name}
;;;  *         that will have @code{NUL} bytes between each inserted string.
;;; =*/
;;;
(define emit-string-table (lambda (st-name) (begin
   (set! stt-curr (hash-ref stt-table   st-name))
   (set! stt-idx  (hash-ref stt-curr "current-index"))
   (ag-fprintf 0 "\nstatic char const %s[%d] =\n" st-name stt-idx)
   (out-resume st-name)

   ;; Columnize the output.
   ;; Remove any leading spaces -- columns adds them itself.
   ;; Glue the "\0" string to its preceding text.
   ;; End the last line with a semi-colon
   ;;
   (emit (shell (string-append
     "sed 's/^ /      /
	$s/\" \"\\\\0\"/\";/
	s/\" \"\\\\0/\\\\0/
	' <<\\_EOF_\n"
     (out-pop #t)
     "_EOF_")))
   (emit "\n")
)))

;;; /*=gfunc   string_table_size
;;;  *
;;;  * what:   print the current size of a string table
;;;  * general-use:
;;;  *
;;;  * exparg: st-name , the name of the array of characters
;;;  *
;;;  * doc:    Returns the current byte count of the string table.
;;; =*/
;;;
(define string-table-size (lambda (st-name)
  (hash-ref (hash-ref stt-table st-name) "current-index") ))

;;; /*=gfunc   gperf_code
;;;  *
;;;  * what:   emit the source of the generated gperf program
;;;  * general-use:
;;;  *
;;;  * exparg: st-name , the name of the gperf hash list
;;;  *
;;;  * doc:
;;;  *  Returns the contents of the emitted code, suitable
;;;  *  for inclusion in another program.  The interface contains
;;;  *  the following elements:
;;;  *
;;;  *  @table @samp
;;;  *  @item struct @i{<st-name>}_index
;;;  *  containg the fields: @code{@{char const * name, int const id; @};}
;;;  *
;;;  *  @item @i{<st-name>}_hash()
;;;  *  This is the hashing function with local only scope (static).
;;;  *
;;;  *  @item @i{<st-name>}_find()
;;;  *  This is the searching and validation function.  The first argument
;;;  *  is the string to look up, the second is its length.
;;;  *  It returns a pointer to the corresponding @code{@i{<st-name>}_index}
;;;  *  entry.
;;;  *  @end table
;;;  *
;;;  *  Use this in your template as follows where "@i{<st-name>}" was
;;;  *  set to be "@code{lookup}":
;;;  *
;;;  *  @example
;;;  *  [+ (make-gperf "lookup" (join "\n" (stack "name_list")))
;;;  *     (gperf-code "lookup") +]
;;;  *  void my_fun(char * str) @{
;;;  *  struct lookup_index * li = lookup_find(str, strlen(str));
;;;  *  if (li != NULL) printf("%s yields %d\n", str, li->idx);
;;;  *  @end example
;;; =*/
;;;
(define gperf-code (lambda (gp-name) (shellf
  "sed -e '1,/^#line/d' \
       -e '/#include/d' \
       -e '/#line/d' \
       -e '/^[ \t]*$/d' \
       -e 's/^const struct /static const struct /' \
       -e '/^int main(/,$d' ${gpdir}/%s.c"
  gp-name
)))

(use-modules (ice-9 debug))

(read-enable 'positions)

;;; /*=gfunc   stack_join
;;;  *
;;;  * what:   stack values then join them
;;;  *
;;;  * exparg: join , string between each element
;;;  * exparg: ag-name , name of autogen values to stack
;;;  *
;;;  * doc:    This function will collect all the values named @code{ag-name}
;;;  *         (see the @pxref{SCM stack, stack function}) and join them
;;;  *         separated by the @code{join} string (see the
;;;  *         @pxref{SCM join, join function}).
;;; =*/
;;;
(define stack-join (lambda (j-str ag-name)
  (join j-str (stack ag-name))))

;;; end of agen5/schemedef.scm
