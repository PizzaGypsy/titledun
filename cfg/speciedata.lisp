(defstruct species
	name
	markings)

(defparameter *species* `(,(make-species :name "hyena" :markings '(none null))
													 ,(make-species :name "wolf" :markings '(also none))))

(defun get-species-list (splst)
	(mapcar #'species-name splst))

(defun specie-idx (name lst)
	(location name lst))

(specie-idx "hyena" (get-species-list))

