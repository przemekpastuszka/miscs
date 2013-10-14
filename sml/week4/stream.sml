structure Stream : STREAM =
struct
    datatype 'a streamCell = Cons of 'a * 'a stream | Nil
    withtype 'a stream = 'a streamCell Susp.susp

    exception SHd and STl and Nth

    val empty = Susp.delay (fn () => Nil)
    fun singleton x = Susp.delay (fn () => Cons(x, empty))

    fun sHd t = case Susp.force t of Nil => raise SHd | Cons (x, _) => x
    fun sTl t = case Susp.force t of Nil => raise STl | Cons (_, x) => x
    fun merge (a, b) =
        let fun join x = case Susp.force x of Nil => Susp.force b
                | Cons (h, t) => Cons(h, merge (t, b))
        in Susp.delay (fn () => join a) end

    fun mkStream f =
        let fun make n = Cons(f n, Susp.delay (fn () => make (n + 1)))
        in Susp.delay (fn () => make 0) end

    fun from n = mkStream (fn i => i + n)
    fun constStream x = mkStream (fn _ => x)

    fun sMap f s =
        let fun mapper x = case Susp.force x of Nil => Nil
        | Cons(h, t) => Cons(f h, sMap f t)
        in Susp.delay (fn () => mapper s) end

    fun sDrop i s =
        let fun dropper x 0 = x
            | dropper x n = dropper (sTl x) (n - 1)
        in Susp.delay (fn () => Susp.force (dropper s i)) end

    fun sRev s = case Susp.force s of
        Nil => empty
        | Cons(x, t) => merge (sRev t, singleton x)

    fun sTake i s =
        let fun taker x 0 = Nil
            | taker x n = Cons(sHd x, sTake (n - 1) s)
        in Susp.delay (fn () => taker s i) end

    fun sZip (a, b) =
        let fun zipper x y = case Susp.force x of
            Nil => empty
            | Cons(x, t) => merge (singleton (x, sHd y), sZip (t, sTl y))
        in Susp.delay (fn () => Susp.force (zipper a b)) end

(*
    val unzip : ('a*'b) stream -> 'a stream * 'b stream
*)
(*
    fun splice (a, b) =
        let fun splicer n =
            merge (sZip (sTake n a, sRev (sTake n b)), splicer (n + 1))
        in Susp.delay (fn () => Susp.force (splicer 1)) end
*)



(*
    val filter : ('a -> 'b option) -> 'a stream -> 'b stream
    val find : ('a -> bool) -> 'a stream -> 'a stream
    val remove : ('a -> bool) -> 'a stream -> 'a stream
    val findOne : ('a -> bool) -> 'a stream -> 'a option
    val flatten : 'a stream stream -> 'a stream
*)
    fun nth n s = sHd (sDrop (n - 1) s)
(*
    val prefix : ('a -> bool) -> 'a stream -> 'a stream
    val suffix : ('a -> bool) -> 'a stream -> 'a stream
    val split : int -> 'a stream -> 'a stream * 'a stream
    val splitp : ('a -> bool) -> 'a stream -> 'a stream * 'a stream
*)
end
