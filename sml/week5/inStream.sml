load "TextIO";
load "Option";

type inStream = char Stream.stream

fun create ins = Stream.mkStream (fn _ => Option.valOf (TextIO.input1 ins))
fun read s = (Stream.sHd s, Stream.sTl s)
