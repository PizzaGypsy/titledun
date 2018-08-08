(defparameter *max-x* 4)
(defparameter *max-y* 4)
(defparameter *terrains* (make-array `(,*max-x* ,*max-y*)))

(defstruct terrain
	name
	texture1
	x
	y)

(defun generate-terrain-block (x y)
	"Generates a single terrain object with x and y set to input x and y"
	(make-terrain
	 :name (concatenate 'string (write-to-string x) "-" (write-to-string y))
	 :texture1 "NULL"
	 :x x
	 :y y))

(defun generate-terrain-array (x y)
	"Makes an array of terrains with size x by y"
	(loop :for x2 :below x :do
		 (loop :for y2 :below y :do
				(setf (aref *terrains* x2 y2) (generate-terrain-block x2 y2)))))

(generate-terrain-array *max-x* *max-y*)
(setf (terrain-texture1 (aref *terrains* 1 1)) "unexisting_tex")

#|
;;I really oughta write a function to make these for me.
(defparameter 0-0 (make-terrain :name "0-0" :texture1 "NULL" :x 0 :y 0))
(defparameter 0-1 (make-terrain :name "0-1" :texture1 "NULL" :x 0 :y 1))
(defparameter 1-0 (make-terrain :name "1-0" :texture1 "NULL" :x 1 :y 0))
(defparameter 0-2 (make-terrain :name "0-2" :texture1 "NULL" :x 0 :y 2))
(defparameter 2-0 (make-terrain :name "0-2" :texture1 "NULL" :x 2 :y 0))
(defparameter 1-1 (make-terrain :name "1-1" :texture1 "NULL" :x 1 :y 1))
(defparameter 1-2 (make-terrain :name "1-2" :texture1 "NULL" :x 1 :y 2))
(defparameter 2-1 (make-terrain :name "2-1" :texture1 "NULL" :x 2 :y 1))
(defparameter 2-2 (make-terrain :name "2-2" :texture1 "NULL" :x 2 :y 2))

(defparameter 0-3 (make-terrain :name "0-3" :texture1 "NULL" :x 0 :y 3))
(defparameter 1-3 (make-terrain :name "1-3" :texture1 "NULL" :x 1 :y 3))
(defparameter 2-3 (make-terrain :name "2-3" :texture1 "NULL" :x 2 :y 3))
(defparameter 3-0 (make-terrain :name "3-0" :texture1 "NULL" :x 3 :y 0))
(defparameter 3-1 (make-terrain :name "3-1" :texture1 "NULL" :x 3 :y 1))
(defparameter 3-2 (make-terrain :name "3-2" :texture1 "NULL" :x 3 :y 2))
(defparameter 3-3 (make-terrain :name "3-3" :texture1 "NULL" :x 3 :y 3))

(defparameter 4-0 (make-terrain :name "4-0" :texture1 "NULL" :x 4 :y 0))
(defparameter 4-1 (make-terrain :name "4-1" :texture1 "NULL" :x 4 :y 1))
(defparameter 4-2 (make-terrain :name "4-2" :texture1 "NULL" :x 4 :y 2))
(defparameter 4-3 (make-terrain :name "4-3" :texture1 "NULL" :x 4 :y 3))
(defparameter 4-4 (make-terrain :name "4-4" :texture1 "NULL" :x 4 :y 4))
(defparameter 3-4 (make-terrain :name "3-4" :texture1 "NULL" :x 3 :y 4))
(defparameter 2-4 (make-terrain :name "2-4" :texture1 "NULL" :x 2 :y 4))
(defparameter 1-4 (make-terrain :name "1-4" :texture1 "NULL" :x 1 :y 4))
(defparameter 0-4 (make-terrain :name "0-4" :texture1 "NULL" :x 0 :y 4))

(defparameter *terrains* (make-array `(,*max-x* ,*max-y*)))
(setf (aref *terrains* 0 0) 0-0)
(setf (aref *terrains* 0 1) 0-1)
(setf (aref *terrains* 1 0) 1-0)
(setf (aref *terrains* 0 2) 0-2)
(setf (aref *terrains* 2 0) 2-0)
(setf (aref *terrains* 1 1) 1-1)
(setf (aref *terrains* 1 2) 1-2)
(setf (aref *terrains* 2 1) 2-1)
(setf (aref *terrains* 2 2) 2-2)

(setf (aref *terrains* 0 3) 0-3)
(setf (aref *terrains* 1 3) 1-3)
(setf (aref *terrains* 2 3) 2-3)
(setf (aref *terrains* 3 0) 3-0)
(setf (aref *terrains* 3 1) 3-1)
(setf (aref *terrains* 3 2) 3-2)
(setf (aref *terrains* 3 3) 3-3)

(setf (aref *terrains* 4 0) 4-0)
(setf (aref *terrains* 4 1) 4-1)
(setf (aref *terrains* 4 2) 4-2)
(setf (aref *terrains* 4 3) 4-3)
(setf (aref *terrains* 4 4) 4-4)
(setf (aref *terrains* 3 4) 3-4)
(setf (aref *terrains* 2 4) 2-4)
(setf (aref *terrains* 1 4) 1-4)
(setf (aref *terrains* 0 4) 0-4)
|#
