type token =
  | CNumber of (float)
  | CText of (string)
  | CBool of (bool)
  | Number
  | Text
  | Bool
  | Array
  | Struct
  | Identifier of (string)
  | Logic of (string)
  | EqGr of (string)
  | Plus of (string)
  | Minus of (string)
  | MulDivMod of (string)
  | Pow of (string)
  | Function
  | Return
  | While
  | For
  | Continue
  | Break
  | If
  | Else
  | Semicolon
  | Coma
  | Dot
  | Assignment
  | Reference
  | LParent
  | RParent
  | LArr
  | RArr
  | LBracket
  | RBracket

val input :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> HTypes.langElement
