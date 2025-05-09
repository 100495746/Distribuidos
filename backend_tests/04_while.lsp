(setq i 0)
(defun main ()
  (loop while (< i 3) do
    (progn
      (princ i)
      (princ " ")
      (setf i (+ i 1))))
)
(main)
