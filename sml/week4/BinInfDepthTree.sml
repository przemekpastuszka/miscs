load "Susp";

datatype 'a tree = E | T of 'a tree Susp.susp * 'a * 'a tree Susp.susp
datatype dir = L | R

fun constTree a =
    let
        val root = ref E
        val delay = Susp.delay (fn () => !root)
        val x = T(delay, a, delay)
    in
        root := x;
        x
    end

fun tMap f E = E
    | tMap f (T(l, x, r)) =
        let
            fun apply z = Susp.delay (fn () => tMap f (Susp.force z))
        in
            T(apply l, f x, apply r)
        end

fun subTree [] t = t
    | subTree _ E = E
    | subTree (L :: tail) (T(l, _, _)) = subTree tail (Susp.force l)
    | subTree (R :: tail) (T(_, _, r)) = subTree tail (Susp.force r)

fun mirror E = E
    | mirror (T(l, x, r)) =
        let
            fun apply z = Susp.delay (fn () => mirror (Susp.force z))
        in
            T(apply r, x, apply l)
        end

val naturalTree =
    let fun createT x =
        T(
            Susp.delay (fn () => createT (2 * x)),
            x,
            Susp.delay (fn () => createT (2 * x + 1)))
    in createT 1 end
