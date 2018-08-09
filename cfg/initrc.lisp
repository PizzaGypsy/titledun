(format t "I've run the contents of init.lisp~%")

(load "../cfg/core.lisp")

(load "../modules/default/package.lisp")
(load "../cfg/speciedata.lisp")

;(setf *standard-output*
;     (open (probe-file "stdout-fifo") :direction :output :if-exists :overwrite))
