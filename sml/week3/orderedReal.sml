structure OrderedReal : ORDERED =
struct
    type T = real
    fun leq a b = a <= b
end
