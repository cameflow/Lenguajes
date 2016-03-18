#lang racket
(define (dia-siguiente a)
  (define D (car a))
  (define M (cadr a))
  (define A (caddr a))

  (cond
        ;Caso para cuando es el 31 de diciembre (Dia y Mes se vuelven 1 y el año aumenta)
        [(and (= M 12) (= D 31))(list 1 1 (+ 1 A))]
        ;Caso para los meses que tienen 31 días (Día se vuelve 1, se aumenta mes y se mantiene el año
        [(and (= D 31)(or (or (or (or (= M 1) (= M 3)) (= M 5)) (= M 8)) (= M 10)))(list 1 (+ M 1) A)]
        ;Caso para los meses que tienen 30 días (Día se vuelve 1, se aumenta mes y se mantiene el año
        [(and (= D 30)(or (or (or (= M 4) (= M 6)) (= M 9))(= M 11)))(list 1 (+ M 1) A)]
        ;Caso para el 29 de febrero
        [(and (= D 29)(= M 2))(list 1 3 A)]
        ;Caso para los años bisiestos
        [(and(and (= D 28)(= M 2))(=(modulo A 4) 0))(list 29 2 A)]
        ;Caso para los años que no son bisiestos
        [(and (= D 28)(= M 2))(list 1 3 A)]
        ;Caso para todos los demás días
        [(< D 30)(list(+ D 1) M A)]
  )
)

(define (lista-de-fechas-siguientes a)
  (cond
    ;Revisa cuando solo queda una fecha
    [(= (length a) 1) (list(dia-siguiente (car a)))]
    ;Recursividad que va llamando a la misma función con el resto de la lista y va creando una lista nueva
    [(append (list(dia-siguiente (car a))) (lista-de-fechas-siguientes (cdr a))) ]
   )
  )