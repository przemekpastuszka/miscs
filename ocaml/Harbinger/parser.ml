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

open Parsing;;
# 1 "parser.mly"

	open Lexing
	
	let getLine ()=(Parsing.symbol_start_pos ()).pos_lnum;;
	
	let showError str=
		raise (HTypes.HSyntaxError ("",getLine(),str));;
		
	
# 49 "parser.ml"
let yytransl_const = [|
  260 (* Number *);
  261 (* Text *);
  262 (* Bool *);
  263 (* Array *);
  264 (* Struct *);
  272 (* Function *);
  273 (* Return *);
  274 (* While *);
  275 (* For *);
  276 (* Continue *);
  277 (* Break *);
  278 (* If *);
  279 (* Else *);
  280 (* Semicolon *);
  281 (* Coma *);
  282 (* Dot *);
  283 (* Assignment *);
  284 (* Reference *);
  285 (* LParent *);
  286 (* RParent *);
  287 (* LArr *);
  288 (* RArr *);
  289 (* LBracket *);
  290 (* RBracket *);
    0|]

let yytransl_block = [|
  257 (* CNumber *);
  258 (* CText *);
  259 (* CBool *);
  265 (* Identifier *);
  266 (* Logic *);
  267 (* EqGr *);
  268 (* Plus *);
  269 (* Minus *);
  270 (* MulDivMod *);
  271 (* Pow *);
    0|]

let yylhs = "\255\255\
\001\000\001\000\001\000\004\000\004\000\004\000\004\000\004\000\
\005\000\005\000\006\000\007\000\007\000\007\000\003\000\003\000\
\003\000\008\000\008\000\008\000\008\000\008\000\009\000\009\000\
\009\000\009\000\010\000\010\000\010\000\002\000\002\000\002\000\
\002\000\002\000\002\000\002\000\012\000\012\000\014\000\014\000\
\014\000\013\000\013\000\013\000\013\000\013\000\013\000\013\000\
\013\000\013\000\013\000\013\000\013\000\013\000\013\000\013\000\
\015\000\015\000\015\000\015\000\015\000\015\000\015\000\015\000\
\015\000\015\000\015\000\015\000\015\000\011\000\011\000\000\000"

let yylen = "\002\000\
\001\000\001\000\001\000\001\000\001\000\001\000\004\000\002\000\
\001\000\003\000\003\000\001\000\002\000\001\000\003\000\005\000\
\001\000\003\000\002\000\005\000\004\000\001\000\001\000\001\000\
\001\000\001\000\001\000\001\000\001\000\007\000\006\000\009\000\
\008\000\007\000\009\000\002\000\001\000\003\000\001\000\003\000\
\004\000\001\000\001\000\001\000\001\000\003\000\004\000\003\000\
\003\000\003\000\003\000\003\000\003\000\002\000\003\000\001\000\
\004\000\005\000\003\000\004\000\005\000\007\000\011\000\007\000\
\015\000\002\000\002\000\003\000\001\000\001\000\002\000\002\000"

let yydefred = "\000\000\
\000\000\000\000\003\000\000\000\000\000\072\000\001\000\002\000\
\000\000\036\000\006\000\005\000\004\000\000\000\000\000\000\000\
\015\000\000\000\000\000\008\000\000\000\027\000\028\000\023\000\
\024\000\025\000\026\000\029\000\000\000\014\000\000\000\012\000\
\000\000\000\000\000\000\000\000\009\000\000\000\016\000\013\000\
\007\000\022\000\000\000\000\000\000\000\000\000\000\000\011\000\
\000\000\000\000\031\000\000\000\019\000\000\000\000\000\000\000\
\010\000\018\000\069\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\070\000\000\000\000\000\
\030\000\000\000\034\000\000\000\000\000\056\000\042\000\043\000\
\044\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\067\000\066\000\000\000\000\000\033\000\071\000\000\000\000\000\
\000\000\000\000\000\000\021\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\068\000\000\000\039\000\000\000\000\000\059\000\
\040\000\000\000\000\000\000\000\020\000\032\000\035\000\057\000\
\000\000\000\000\046\000\000\000\055\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\060\000\041\000\
\000\000\000\000\058\000\047\000\000\000\000\000\000\000\061\000\
\000\000\000\000\000\000\064\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\063\000\000\000\000\000\
\000\000\065\000"

let yydgoto = "\002\000\
\006\000\007\000\008\000\067\000\038\000\032\000\033\000\046\000\
\028\000\029\000\068\000\104\000\105\000\086\000\070\000"

let yysindex = "\001\000\
\001\255\000\000\000\000\019\255\066\001\000\000\000\000\000\000\
\255\254\000\000\000\000\000\000\000\000\013\255\021\255\076\001\
\000\000\075\001\173\255\000\000\017\255\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\028\255\000\000\050\255\000\000\
\057\255\038\255\007\255\118\255\000\000\123\255\000\000\000\000\
\000\000\000\000\173\255\010\255\069\255\236\254\015\255\000\000\
\071\255\073\255\000\000\068\000\000\000\154\255\034\255\036\255\
\000\000\000\000\000\000\055\255\054\000\065\255\079\255\095\255\
\097\255\098\255\050\255\068\255\230\000\000\000\173\255\127\255\
\000\000\068\000\000\000\068\000\018\255\000\000\000\000\000\000\
\000\000\122\255\054\000\054\000\003\001\080\255\054\000\174\255\
\000\000\000\000\054\000\168\255\000\000\000\000\182\255\054\000\
\054\000\189\255\216\255\000\000\135\255\167\255\176\255\090\255\
\082\001\049\000\208\255\205\000\054\000\054\000\054\000\054\000\
\054\000\054\000\000\000\211\000\000\000\106\255\217\000\000\000\
\000\000\018\001\088\000\054\000\000\000\000\000\000\000\000\000\
\054\000\215\255\000\000\104\255\000\000\191\255\214\255\200\255\
\200\255\208\255\208\255\205\255\054\000\217\255\000\000\000\000\
\024\001\082\001\000\000\000\000\068\000\039\001\068\000\000\000\
\190\255\054\000\213\255\000\000\045\001\223\255\174\255\218\255\
\137\255\068\000\054\000\236\255\238\000\000\000\219\255\068\000\
\003\000\000\000"

let yyrindex = "\000\000\
\000\000\000\000\000\000\237\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\250\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\081\000\000\000\000\000\000\000\104\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\120\255\000\000\113\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\182\000\179\000\161\000\
\170\000\136\000\145\000\000\000\000\000\000\000\000\000\000\000\
\000\000\140\255\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\026\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000"

let yygindex = "\000\000\
\000\000\000\000\000\000\095\000\181\000\216\000\000\000\224\000\
\192\000\000\000\186\255\156\000\198\255\204\255\196\255"

let yytablesize = 609
let yytable = "\069\000\
\003\000\001\000\085\000\101\000\054\000\102\000\042\000\094\000\
\004\000\055\000\011\000\012\000\013\000\014\000\015\000\069\000\
\005\000\078\000\079\000\080\000\081\000\069\000\017\000\069\000\
\107\000\108\000\082\000\009\000\116\000\020\000\083\000\018\000\
\119\000\051\000\043\000\118\000\044\000\122\000\123\000\054\000\
\094\000\094\000\052\000\019\000\056\000\035\000\084\000\103\000\
\069\000\069\000\134\000\135\000\136\000\137\000\138\000\139\000\
\036\000\073\000\037\000\075\000\011\000\012\000\013\000\014\000\
\015\000\145\000\074\000\059\000\076\000\041\000\146\000\011\000\
\012\000\013\000\014\000\015\000\060\000\053\000\153\000\057\000\
\155\000\058\000\150\000\077\000\061\000\062\000\063\000\064\000\
\065\000\066\000\039\000\164\000\094\000\087\000\094\000\157\000\
\069\000\169\000\069\000\016\000\069\000\093\000\069\000\094\000\
\165\000\095\000\161\000\088\000\094\000\069\000\097\000\069\000\
\031\000\034\000\129\000\069\000\069\000\042\000\089\000\130\000\
\090\000\011\000\012\000\013\000\014\000\015\000\091\000\031\000\
\129\000\045\000\045\000\095\000\141\000\148\000\059\000\100\000\
\097\000\050\000\011\000\012\000\013\000\014\000\015\000\060\000\
\037\000\043\000\048\000\049\000\072\000\037\000\106\000\061\000\
\062\000\063\000\064\000\065\000\066\000\011\000\012\000\013\000\
\014\000\015\000\095\000\163\000\038\000\099\000\059\000\097\000\
\126\000\038\000\011\000\012\000\013\000\014\000\015\000\060\000\
\011\000\012\000\013\000\014\000\015\000\071\000\117\000\061\000\
\062\000\063\000\064\000\065\000\066\000\059\000\121\000\120\000\
\049\000\011\000\012\000\013\000\014\000\015\000\060\000\128\000\
\127\000\110\000\111\000\112\000\113\000\114\000\061\000\062\000\
\063\000\064\000\065\000\066\000\059\000\113\000\114\000\124\000\
\011\000\012\000\013\000\014\000\015\000\060\000\114\000\156\000\
\125\000\111\000\112\000\113\000\114\000\061\000\062\000\063\000\
\064\000\065\000\066\000\059\000\017\000\149\000\147\000\011\000\
\012\000\013\000\014\000\015\000\060\000\160\000\158\000\092\000\
\040\000\151\000\162\000\168\000\061\000\062\000\063\000\064\000\
\065\000\066\000\059\000\047\000\098\000\132\000\011\000\012\000\
\013\000\014\000\015\000\060\000\000\000\166\000\000\000\000\000\
\000\000\000\000\000\000\061\000\062\000\063\000\064\000\065\000\
\066\000\062\000\000\000\000\000\000\000\062\000\062\000\062\000\
\062\000\062\000\062\000\000\000\170\000\000\000\000\000\000\000\
\000\000\000\000\062\000\062\000\062\000\062\000\062\000\062\000\
\078\000\079\000\080\000\081\000\000\000\078\000\079\000\080\000\
\081\000\082\000\000\000\062\000\000\000\083\000\082\000\000\000\
\000\000\000\000\083\000\059\000\000\000\000\000\000\000\011\000\
\012\000\013\000\014\000\015\000\060\000\084\000\131\000\000\000\
\000\000\000\000\084\000\000\000\061\000\062\000\063\000\064\000\
\065\000\066\000\039\000\039\000\039\000\039\000\039\000\039\000\
\000\000\109\000\110\000\111\000\112\000\113\000\114\000\000\000\
\039\000\039\000\039\000\000\000\000\000\000\000\039\000\039\000\
\039\000\045\000\045\000\045\000\045\000\045\000\045\000\144\000\
\000\000\000\000\054\000\054\000\054\000\054\000\054\000\045\000\
\045\000\000\000\000\000\000\000\000\000\045\000\000\000\045\000\
\054\000\054\000\000\000\000\000\000\000\000\000\054\000\000\000\
\054\000\052\000\052\000\052\000\052\000\052\000\000\000\000\000\
\000\000\000\000\053\000\053\000\053\000\053\000\053\000\052\000\
\052\000\000\000\000\000\000\000\000\000\052\000\000\000\052\000\
\053\000\053\000\050\000\050\000\050\000\050\000\053\000\000\000\
\053\000\000\000\000\000\051\000\051\000\051\000\051\000\000\000\
\050\000\050\000\000\000\000\000\049\000\049\000\050\000\048\000\
\050\000\051\000\051\000\000\000\000\000\000\000\000\000\051\000\
\000\000\051\000\049\000\049\000\000\000\048\000\048\000\000\000\
\049\000\000\000\049\000\048\000\000\000\048\000\109\000\110\000\
\111\000\112\000\113\000\114\000\109\000\110\000\111\000\112\000\
\113\000\114\000\109\000\110\000\111\000\112\000\113\000\114\000\
\000\000\000\000\133\000\000\000\000\000\000\000\000\000\000\000\
\140\000\024\000\025\000\026\000\027\000\000\000\142\000\109\000\
\110\000\111\000\112\000\113\000\114\000\000\000\000\000\095\000\
\096\000\000\000\000\000\000\000\097\000\039\000\039\000\039\000\
\039\000\000\000\000\000\167\000\109\000\110\000\111\000\112\000\
\113\000\114\000\000\000\039\000\039\000\000\000\000\000\000\000\
\039\000\000\000\115\000\109\000\110\000\111\000\112\000\113\000\
\114\000\109\000\110\000\111\000\112\000\113\000\114\000\000\000\
\000\000\143\000\000\000\000\000\000\000\000\000\000\000\152\000\
\109\000\110\000\111\000\112\000\113\000\114\000\109\000\110\000\
\111\000\112\000\113\000\114\000\000\000\000\000\154\000\000\000\
\000\000\010\000\000\000\000\000\159\000\011\000\012\000\013\000\
\014\000\015\000\030\000\000\000\000\000\000\000\011\000\012\000\
\013\000\014\000\015\000\000\000\021\000\022\000\023\000\024\000\
\025\000\026\000\027\000\109\000\110\000\111\000\112\000\113\000\
\114\000"

let yycheck = "\052\000\
\000\001\001\000\061\000\074\000\025\001\076\000\000\001\068\000\
\008\001\030\001\004\001\005\001\006\001\007\001\008\001\068\000\
\016\001\000\001\001\001\002\001\003\001\074\000\024\001\076\000\
\083\000\084\000\009\001\009\001\087\000\009\001\013\001\033\001\
\091\000\024\001\028\001\088\000\030\001\096\000\097\000\025\001\
\101\000\102\000\033\001\031\001\030\001\029\001\029\001\030\001\
\101\000\102\000\109\000\110\000\111\000\112\000\113\000\114\000\
\029\001\024\001\009\001\024\001\004\001\005\001\006\001\007\001\
\008\001\124\000\033\001\000\001\033\001\032\001\129\000\004\001\
\005\001\006\001\007\001\008\001\009\001\009\001\149\000\009\001\
\151\000\009\001\141\000\029\001\017\001\018\001\019\001\020\001\
\021\001\022\001\034\001\162\000\153\000\029\001\155\000\154\000\
\149\000\168\000\151\000\005\000\153\000\034\001\155\000\164\000\
\163\000\026\001\159\000\029\001\169\000\162\000\031\001\164\000\
\018\000\019\000\025\001\168\000\169\000\000\001\024\001\030\001\
\024\001\004\001\005\001\006\001\007\001\008\001\029\001\033\000\
\025\001\035\000\036\000\026\001\027\001\030\001\000\001\009\001\
\031\001\043\000\004\001\005\001\006\001\007\001\008\001\009\001\
\025\001\028\001\024\001\025\001\054\000\030\001\029\001\017\001\
\018\001\019\001\020\001\021\001\022\001\004\001\005\001\006\001\
\007\001\008\001\026\001\027\001\025\001\071\000\000\001\031\001\
\034\001\030\001\004\001\005\001\006\001\007\001\008\001\009\001\
\004\001\005\001\006\001\007\001\008\001\028\001\009\001\017\001\
\018\001\019\001\020\001\021\001\022\001\000\001\009\001\024\001\
\025\001\004\001\005\001\006\001\007\001\008\001\009\001\024\001\
\034\001\011\001\012\001\013\001\014\001\015\001\017\001\018\001\
\019\001\020\001\021\001\022\001\000\001\014\001\015\001\027\001\
\004\001\005\001\006\001\007\001\008\001\009\001\015\001\034\001\
\009\001\012\001\013\001\014\001\015\001\017\001\018\001\019\001\
\020\001\021\001\022\001\000\001\000\000\033\001\024\001\004\001\
\005\001\006\001\007\001\008\001\009\001\023\001\034\001\067\000\
\033\000\033\001\033\001\033\001\017\001\018\001\019\001\020\001\
\021\001\022\001\000\001\036\000\069\000\106\000\004\001\005\001\
\006\001\007\001\008\001\009\001\255\255\034\001\255\255\255\255\
\255\255\255\255\255\255\017\001\018\001\019\001\020\001\021\001\
\022\001\000\001\255\255\255\255\255\255\004\001\005\001\006\001\
\007\001\008\001\009\001\255\255\034\001\255\255\255\255\255\255\
\255\255\255\255\017\001\018\001\019\001\020\001\021\001\022\001\
\000\001\001\001\002\001\003\001\255\255\000\001\001\001\002\001\
\003\001\009\001\255\255\034\001\255\255\013\001\009\001\255\255\
\255\255\255\255\013\001\000\001\255\255\255\255\255\255\004\001\
\005\001\006\001\007\001\008\001\009\001\029\001\030\001\255\255\
\255\255\255\255\029\001\255\255\017\001\018\001\019\001\020\001\
\021\001\022\001\010\001\011\001\012\001\013\001\014\001\015\001\
\255\255\010\001\011\001\012\001\013\001\014\001\015\001\255\255\
\024\001\025\001\026\001\255\255\255\255\255\255\030\001\031\001\
\032\001\010\001\011\001\012\001\013\001\014\001\015\001\032\001\
\255\255\255\255\010\001\011\001\012\001\013\001\014\001\024\001\
\025\001\255\255\255\255\255\255\255\255\030\001\255\255\032\001\
\024\001\025\001\255\255\255\255\255\255\255\255\030\001\255\255\
\032\001\010\001\011\001\012\001\013\001\014\001\255\255\255\255\
\255\255\255\255\010\001\011\001\012\001\013\001\014\001\024\001\
\025\001\255\255\255\255\255\255\255\255\030\001\255\255\032\001\
\024\001\025\001\010\001\011\001\012\001\013\001\030\001\255\255\
\032\001\255\255\255\255\010\001\011\001\012\001\013\001\255\255\
\024\001\025\001\255\255\255\255\010\001\011\001\030\001\010\001\
\032\001\024\001\025\001\255\255\255\255\255\255\255\255\030\001\
\255\255\032\001\024\001\025\001\255\255\024\001\025\001\255\255\
\030\001\255\255\032\001\030\001\255\255\032\001\010\001\011\001\
\012\001\013\001\014\001\015\001\010\001\011\001\012\001\013\001\
\014\001\015\001\010\001\011\001\012\001\013\001\014\001\015\001\
\255\255\255\255\030\001\255\255\255\255\255\255\255\255\255\255\
\030\001\012\001\013\001\014\001\015\001\255\255\030\001\010\001\
\011\001\012\001\013\001\014\001\015\001\255\255\255\255\026\001\
\027\001\255\255\255\255\255\255\031\001\012\001\013\001\014\001\
\015\001\255\255\255\255\030\001\010\001\011\001\012\001\013\001\
\014\001\015\001\255\255\026\001\027\001\255\255\255\255\255\255\
\031\001\255\255\024\001\010\001\011\001\012\001\013\001\014\001\
\015\001\010\001\011\001\012\001\013\001\014\001\015\001\255\255\
\255\255\024\001\255\255\255\255\255\255\255\255\255\255\024\001\
\010\001\011\001\012\001\013\001\014\001\015\001\010\001\011\001\
\012\001\013\001\014\001\015\001\255\255\255\255\024\001\255\255\
\255\255\000\001\255\255\255\255\024\001\004\001\005\001\006\001\
\007\001\008\001\000\001\255\255\255\255\255\255\004\001\005\001\
\006\001\007\001\008\001\255\255\009\001\010\001\011\001\012\001\
\013\001\014\001\015\001\010\001\011\001\012\001\013\001\014\001\
\015\001"

let yynames_const = "\
  Number\000\
  Text\000\
  Bool\000\
  Array\000\
  Struct\000\
  Function\000\
  Return\000\
  While\000\
  For\000\
  Continue\000\
  Break\000\
  If\000\
  Else\000\
  Semicolon\000\
  Coma\000\
  Dot\000\
  Assignment\000\
  Reference\000\
  LParent\000\
  RParent\000\
  LArr\000\
  RArr\000\
  LBracket\000\
  RBracket\000\
  "

let yynames_block = "\
  CNumber\000\
  CText\000\
  CBool\000\
  Identifier\000\
  Logic\000\
  EqGr\000\
  Plus\000\
  Minus\000\
  MulDivMod\000\
  Pow\000\
  "

let yyact = [|
  (fun _ -> failwith "parser")
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'func) in
    Obj.repr(
# 40 "parser.mly"
            (HTypes.InFunc (_1,getLine()))
# 398 "parser.ml"
               : HTypes.langElement))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'strct) in
    Obj.repr(
# 41 "parser.mly"
         (HTypes.InStruct (_1,getLine()))
# 405 "parser.ml"
               : HTypes.langElement))
; (fun __caml_parser_env ->
    Obj.repr(
# 42 "parser.mly"
         (showError "Function or structure definition/declaration expected")
# 411 "parser.ml"
               : HTypes.langElement))
; (fun __caml_parser_env ->
    Obj.repr(
# 46 "parser.mly"
      (HTypes.TBool)
# 417 "parser.ml"
               : 'vartype))
; (fun __caml_parser_env ->
    Obj.repr(
# 47 "parser.mly"
        (HTypes.TText)
# 423 "parser.ml"
               : 'vartype))
; (fun __caml_parser_env ->
    Obj.repr(
# 48 "parser.mly"
          (HTypes.TNumber)
# 429 "parser.ml"
               : 'vartype))
; (fun __caml_parser_env ->
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'vartype) in
    Obj.repr(
# 49 "parser.mly"
                           (HTypes.TArray _3)
# 436 "parser.ml"
               : 'vartype))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 50 "parser.mly"
                     (HTypes.TStruct _2)
# 443 "parser.ml"
               : 'vartype))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 54 "parser.mly"
            ([_1])
# 450 "parser.ml"
               : 'idents))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'idents) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 55 "parser.mly"
                          (_3::_1)
# 458 "parser.ml"
               : 'idents))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'vartype) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'idents) in
    Obj.repr(
# 59 "parser.mly"
                          (List.map (fun x->(x,_1)) _2)
# 466 "parser.ml"
               : 'varCreation))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'varCreation) in
    Obj.repr(
# 63 "parser.mly"
             (_1)
# 473 "parser.ml"
               : 'field))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'field) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'varCreation) in
    Obj.repr(
# 64 "parser.mly"
                     (_2@_1)
# 481 "parser.ml"
               : 'field))
; (fun __caml_parser_env ->
    Obj.repr(
# 65 "parser.mly"
          (showError "Invalid structure field")
# 487 "parser.ml"
               : 'field))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    Obj.repr(
# 69 "parser.mly"
                              (HTypes.HStruct (HTypes.Declared,_2,[]))
# 494 "parser.ml"
               : 'strct))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 3 : string) in
    let _4 = (Parsing.peek_val __caml_parser_env 1 : 'field) in
    Obj.repr(
# 70 "parser.mly"
                                            (HTypes.HStruct (HTypes.Defined,_2,_4))
# 502 "parser.ml"
               : 'strct))
; (fun __caml_parser_env ->
    Obj.repr(
# 71 "parser.mly"
         (showError "Invalid structure definition/declaration")
# 508 "parser.ml"
               : 'strct))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'vartype) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 75 "parser.mly"
                               ([HTypes.Reference (_3,_2)])
# 516 "parser.ml"
               : 'paramlist))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'vartype) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 76 "parser.mly"
                      ([HTypes.Argument (_2,_1)])
# 524 "parser.ml"
               : 'paramlist))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 4 : 'paramlist) in
    let _4 = (Parsing.peek_val __caml_parser_env 1 : 'vartype) in
    let _5 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 77 "parser.mly"
                                               (HTypes.Reference (_5,_4)::_1)
# 533 "parser.ml"
               : 'paramlist))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'paramlist) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'vartype) in
    let _4 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 78 "parser.mly"
                                     (HTypes.Argument (_4,_3)::_1)
# 542 "parser.ml"
               : 'paramlist))
; (fun __caml_parser_env ->
    Obj.repr(
# 79 "parser.mly"
          (showError "Invalid list of parameters")
# 548 "parser.ml"
               : 'paramlist))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 83 "parser.mly"
        (_1)
# 555 "parser.ml"
               : 'chosenOperators))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 84 "parser.mly"
         (_1)
# 562 "parser.ml"
               : 'chosenOperators))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 85 "parser.mly"
             (_1)
# 569 "parser.ml"
               : 'chosenOperators))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 86 "parser.mly"
       (_1)
# 576 "parser.ml"
               : 'chosenOperators))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 90 "parser.mly"
       (_1)
# 583 "parser.ml"
               : 'operator))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 91 "parser.mly"
        (_1)
# 590 "parser.ml"
               : 'operator))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'chosenOperators) in
    Obj.repr(
# 92 "parser.mly"
                   (_1)
# 597 "parser.ml"
               : 'operator))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 5 : 'vartype) in
    let _3 = (Parsing.peek_val __caml_parser_env 4 : string) in
    let _5 = (Parsing.peek_val __caml_parser_env 2 : 'paramlist) in
    Obj.repr(
# 96 "parser.mly"
                                                                 (HTypes.Function (HTypes.Declared,_3,_2,List.rev _5,[]))
# 606 "parser.ml"
               : 'func))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 4 : 'vartype) in
    let _3 = (Parsing.peek_val __caml_parser_env 3 : string) in
    Obj.repr(
# 97 "parser.mly"
                                                         (HTypes.Function (HTypes.Declared,_3,_2,[],[]))
# 614 "parser.ml"
               : 'func))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 7 : 'vartype) in
    let _3 = (Parsing.peek_val __caml_parser_env 6 : string) in
    let _5 = (Parsing.peek_val __caml_parser_env 4 : 'paramlist) in
    let _8 = (Parsing.peek_val __caml_parser_env 1 : 'seq) in
    Obj.repr(
# 98 "parser.mly"
                                                                               (HTypes.Function (HTypes.Defined,_3,_2,List.rev _5,List.rev _8))
# 624 "parser.ml"
               : 'func))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 6 : 'vartype) in
    let _3 = (Parsing.peek_val __caml_parser_env 5 : string) in
    let _7 = (Parsing.peek_val __caml_parser_env 1 : 'seq) in
    Obj.repr(
# 99 "parser.mly"
                                                                      (HTypes.Function (HTypes.Defined,_3,_2,[],List.rev _7))
# 633 "parser.ml"
               : 'func))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 5 : 'vartype) in
    let _3 = (Parsing.peek_val __caml_parser_env 4 : 'operator) in
    let _5 = (Parsing.peek_val __caml_parser_env 2 : 'paramlist) in
    Obj.repr(
# 100 "parser.mly"
                                                                 (HTypes.Function (HTypes.Declared,_3,_2,List.rev _5,[]))
# 642 "parser.ml"
               : 'func))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 7 : 'vartype) in
    let _3 = (Parsing.peek_val __caml_parser_env 6 : 'operator) in
    let _5 = (Parsing.peek_val __caml_parser_env 4 : 'paramlist) in
    let _8 = (Parsing.peek_val __caml_parser_env 1 : 'seq) in
    Obj.repr(
# 101 "parser.mly"
                                                                             (HTypes.Function (HTypes.Defined,_3,_2,List.rev _5,List.rev _8))
# 652 "parser.ml"
               : 'func))
; (fun __caml_parser_env ->
    Obj.repr(
# 102 "parser.mly"
                  (showError "Invalid function definition/declaration")
# 658 "parser.ml"
               : 'func))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 106 "parser.mly"
      ([_1])
# 665 "parser.ml"
               : 'arglist))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'arglist) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 107 "parser.mly"
                     (_3::_1)
# 673 "parser.ml"
               : 'arglist))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 111 "parser.mly"
            (HTypes.VUsual _1)
# 680 "parser.ml"
               : 'var))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'var) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 112 "parser.mly"
                      (HTypes.VStructEl (_1,_3))
# 688 "parser.ml"
               : 'var))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'var) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'expr) in
    Obj.repr(
# 113 "parser.mly"
                      (HTypes.VArrEl (_1,_3))
# 696 "parser.ml"
               : 'var))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : float) in
    Obj.repr(
# 117 "parser.mly"
         (HTypes.Number _1)
# 703 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 118 "parser.mly"
        (HTypes.Text _1)
# 710 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : bool) in
    Obj.repr(
# 119 "parser.mly"
        (HTypes.Bool _1)
# 717 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'var) in
    Obj.repr(
# 120 "parser.mly"
      (HTypes.Variable _1)
# 724 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : string) in
    Obj.repr(
# 121 "parser.mly"
                             (HTypes.Fun (_1,[]))
# 731 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'arglist) in
    Obj.repr(
# 122 "parser.mly"
                                     (HTypes.Fun (_1,List.rev _3))
# 739 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 123 "parser.mly"
                  (HTypes.Fun (_2,[_1;_3]))
# 748 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 124 "parser.mly"
                 (HTypes.Fun (_2,[_1;_3]))
# 757 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 125 "parser.mly"
                 (HTypes.Fun (_2,[_1;_3]))
# 766 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 126 "parser.mly"
                  (HTypes.Fun (_2,[_1;_3]))
# 775 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 127 "parser.mly"
                      (HTypes.Fun (_2,[_1;_3]))
# 784 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 128 "parser.mly"
                (HTypes.Fun (_2,[_1;_3]))
# 793 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 129 "parser.mly"
                       (HTypes.Fun (_1,[_2]))
# 801 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'expr) in
    Obj.repr(
# 130 "parser.mly"
                       (_2)
# 808 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    Obj.repr(
# 131 "parser.mly"
         (showError "Invalid expression")
# 814 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : string) in
    Obj.repr(
# 137 "parser.mly"
                                       ((getLine(),HTypes.Expression (HTypes.Fun (_1,[]))))
# 821 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 4 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 2 : 'arglist) in
    Obj.repr(
# 138 "parser.mly"
                                               ((getLine(),HTypes.Expression (HTypes.Fun (_1,List.rev _3))))
# 829 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'vartype) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'idents) in
    Obj.repr(
# 139 "parser.mly"
                            ((getLine(),HTypes.Creation (_1,_2)))
# 837 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'var) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'expr) in
    Obj.repr(
# 140 "parser.mly"
                                 ((getLine(),HTypes.Assignment (_1,_3)))
# 845 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 4 : 'var) in
    let _2 = (Parsing.peek_val __caml_parser_env 3 : 'chosenOperators) in
    let _4 = (Parsing.peek_val __caml_parser_env 1 : 'expr) in
    Obj.repr(
# 141 "parser.mly"
                                                 ((getLine(),HTypes.Assignment (_1,HTypes.Fun (_2,[HTypes.Variable _1;_4]))))
# 854 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _3 = (Parsing.peek_val __caml_parser_env 4 : 'expr) in
    let _6 = (Parsing.peek_val __caml_parser_env 1 : 'seq) in
    Obj.repr(
# 142 "parser.mly"
                                                 ((getLine(),HTypes.If (_3,List.rev _6)))
# 862 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _3 = (Parsing.peek_val __caml_parser_env 8 : 'expr) in
    let _6 = (Parsing.peek_val __caml_parser_env 5 : 'seq) in
    let _10 = (Parsing.peek_val __caml_parser_env 1 : 'seq) in
    Obj.repr(
# 143 "parser.mly"
                                                                            ((getLine(),HTypes.IfElse (_3,List.rev _6,List.rev _10)))
# 871 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _3 = (Parsing.peek_val __caml_parser_env 4 : 'expr) in
    let _6 = (Parsing.peek_val __caml_parser_env 1 : 'seq) in
    Obj.repr(
# 144 "parser.mly"
                                                    ((getLine(),HTypes.While (_3,List.rev _6)))
# 879 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _3 = (Parsing.peek_val __caml_parser_env 12 : 'var) in
    let _5 = (Parsing.peek_val __caml_parser_env 10 : 'expr) in
    let _7 = (Parsing.peek_val __caml_parser_env 8 : 'expr) in
    let _9 = (Parsing.peek_val __caml_parser_env 6 : 'var) in
    let _11 = (Parsing.peek_val __caml_parser_env 4 : 'expr) in
    let _14 = (Parsing.peek_val __caml_parser_env 1 : 'seq) in
    Obj.repr(
# 146 "parser.mly"
  ((getLine(),HTypes.For (HTypes.Assignment (_3,_5),_7,HTypes.Assignment (_9,_11),List.rev _14)))
# 891 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    Obj.repr(
# 147 "parser.mly"
                   ((getLine(),HTypes.Break ))
# 897 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    Obj.repr(
# 148 "parser.mly"
                      ((getLine(),HTypes.Continue ))
# 903 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'expr) in
    Obj.repr(
# 149 "parser.mly"
                         ((getLine(),HTypes.Return _2))
# 910 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    Obj.repr(
# 150 "parser.mly"
         (showError "Invalid language statement")
# 916 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'stmt) in
    Obj.repr(
# 153 "parser.mly"
      ([_1])
# 923 "parser.ml"
               : 'seq))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'seq) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'stmt) in
    Obj.repr(
# 154 "parser.mly"
            (_2::_1)
# 931 "parser.ml"
               : 'seq))
(* Entry input *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
|]
let yytables =
  { Parsing.actions=yyact;
    Parsing.transl_const=yytransl_const;
    Parsing.transl_block=yytransl_block;
    Parsing.lhs=yylhs;
    Parsing.len=yylen;
    Parsing.defred=yydefred;
    Parsing.dgoto=yydgoto;
    Parsing.sindex=yysindex;
    Parsing.rindex=yyrindex;
    Parsing.gindex=yygindex;
    Parsing.tablesize=yytablesize;
    Parsing.table=yytable;
    Parsing.check=yycheck;
    Parsing.error_function=parse_error;
    Parsing.names_const=yynames_const;
    Parsing.names_block=yynames_block }
let input (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 1 lexfun lexbuf : HTypes.langElement)
;;
# 157 "parser.mly"


# 959 "parser.ml"
