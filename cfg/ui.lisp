(defstruct ui-button
	name
	posx
	posy
	scale
	fn)

;;our simple test button
(defparameter tt (make-ui-button
 :name "test button"
 :posx 0
 :posy -0.5
 :scale 0.1
 :fn '(defparameter *show-me* '(1 2 3)) ))
