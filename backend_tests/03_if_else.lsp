(setq a 1)
(defun main ()
  (if (> a 2)
    (progn
      (princ "gt")
      (terpri))
    (progn
      (princ "le")
      (terpri))
  )
)
(main)
