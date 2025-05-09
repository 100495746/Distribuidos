(setq a 1)
(defun main ()
  (if (> a 2)
    (progn
      (princ "gt")
      )
    (progn
      (princ "le")
      )
  )
)
(main)
