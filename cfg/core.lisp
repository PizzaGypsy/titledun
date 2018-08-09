(defpackage "CORE"
	(:use "COMMON-LISP")
	(:export "LOAD-FILES")
	(:export "PROBE-FOLDERS")
	(:export "REGISTER-MODULE")
	(:export "LOAD-REG-FILE")
	(:export "LOAD-PACKAGE-FILE")
	(:export "GET-MODULE-REG")
	(:export "GET-MODULE-ALIST"))

(load "../cfg/terrains.lisp")
(load "../cfg/ui.lisp")

(in-package "CORE")

(defparameter *pmodules* 'nil)

(defun get-module-reg ()
	"Returns our global that contains all module names"
	*pmodules*)

(defun load-files (lst)
	"Loads all files in lst"
	(mapcar #'load lst))

(defun load-package-file (modulepath)
	"Loads the package.lisp at the path"
	(load (merge-pathnames modulepath "package.lisp")))

(defun load-reg-file (modulepath)
	"Loads the reg.lisp at the path"
	(load (merge-pathnames modulepath "reg.lisp")))

(defun verify-module (modulepath)
	"Checks to make sure the package.lisp exists"
	(probe-file (merge-pathnames modulepath "package.lisp")))

(defun probe-folders ()
	"Checks for the presence of folders in modules directory and verifies that a module is there"
	(let ((dirs (directory "../modules/*/"))
				(vlist 'nil))
		(loop :for p :in dirs :do
			 (if (verify-module p)
					 (setq vlist (append vlist `(,p)))
					 'nil))
		vlist))

(defun register-module (alist)
	"Writes the name of the module into *pmodules*"
	(if (assoc (car alist) *pmodules* :test #'string=)
			(progn (princ (format 'nil "package ~A already exists.~%" (car alist)))
						 'nil)
			(progn (push alist *pmodules*)
						 t)))

(defun get-module-alist (name)
	(assoc name *pmodules* :test #'string=))

(in-package "COMMON-LISP-USER")
