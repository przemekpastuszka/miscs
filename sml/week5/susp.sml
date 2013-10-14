datatype 'a susp = Val of 'a | Comp of unit -> 'a
type 'a suspension = 'a susp ref

fun delay f = ref (Comp f)
fun force x = case !x of
    Val v => v
    | Comp f =>
        let val v = f () in
        x := Val v;
        v
        end
