load "Susp";

datatype 'a tree = T of 'a * 'a forest
withtype 'a forest = 'a tree Stream.stream

fun forestMap f forest = Stream.sMap (fn t => treeMap f t) forest
and
treeMap f (T(x, forest)) = T(f x, forestMap f forest)

fun subTree [] t = t
    | subTree (i :: ns) (T(_, fr)) = subTree ns (Stream.nth i fr)

fun constTree a =
    let
        val stream = ref (Susp.delay (fn () => Stream.Nil))
        val root = ref (T(a, Susp.delay (fn () => Stream.Nil)))

        val delay = Susp.delay (fn () => Stream.Cons(!root, !stream))
        val x = T(a, delay)
    in
        root := x;
        stream := delay;
        x
    end
