(defparameter *max-x* 2)
(defparameter *max-y* 2)

(defstruct terrain
	name
	texture1
	x
	y)

(defparameter 0-0 (make-terrain :name "0-0" :texture1 "NULL" :x 0 :y 0))
(defparameter 0-1 (make-terrain :name "0-1" :texture1 "NULL" :x 0 :y 1))
(defparameter 1-0 (make-terrain :name "1-0" :texture1 "NULL" :x 1 :y 0))
(defparameter 0-2 (make-terrain :name "0-2" :texture1 "NULL" :x 0 :y 2))
(defparameter 2-0 (make-terrain :name "0-2" :texture1 "NULL" :x 2 :y 0))
(defparameter 1-1 (make-terrain :name "1-1" :texture1 "NULL" :x 1 :y 1))
(defparameter 1-2 (make-terrain :name "1-2" :texture1 "NULL" :x 1 :y 2))
(defparameter 2-1 (make-terrain :name "2-1" :texture1 "NULL" :x 2 :y 1))
(defparameter 2-2 (make-terrain :name "2-2" :texture1 "NULL" :x 2 :y 2))

(defparameter *terrains* (list 0-0 0-1 1-0 0-2 2-0 1-1 1-2 2-1 2-2))
;(defparameter *terrains* (list 0-0 0-1))

