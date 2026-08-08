; CGEN testsuite driver.
; Copyright (C) 2009 Doug Evans
; This file is part of CGEN.

(eval-when (expand load eval)
	   ;; Kept global so it's available to the other .scm files.
	   (define srcdir (dirname (current-filename)))
	   (add-to-load-path srcdir)

	   (use-modules (ice-9 slib))

	   (load-from-path "read")
	   (load-from-path "desc")
	   (load-from-path "desc-cpu")
	   (load-from-path "testsuite"))

(define testsuite-arguments
  (list
   (list "-T" "file" "generate $arch-test.h in <file>"
	 #f
	 (lambda (arg) (file-write arg cgen-test.h)))
   )
)

; Main routine, parses options and calls generators.

(define (cgen-testsuite argv)
  (let ()

    (display-argv argv)

    (cgen #:argv argv
	  #:app-name "testsuite"
	  #:arg-spec testsuite-arguments
	  #:init testsuite-init!
	  #:finish testsuite-finish!
	  #:analyze testsuite-analyze!)
    )
)

(cgen-testsuite (program-arguments))
