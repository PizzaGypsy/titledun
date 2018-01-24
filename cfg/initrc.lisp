(format t "I've run the contents of init.lisp~%")

(defparameter *some-global* 15)
(defparameter *global-str* "hello world")

(compile-file "terrains.lisp")
(load "terrains.lisp")

(defun foo ()
	(format t "We called foo...~%"))

;(setf *standard-output*
 ;     (open (probe-file "stdout-fifo") :direction :output :if-exists :overwrite))
