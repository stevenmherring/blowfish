val maxFun : int ref = ref 0;

fun extract ((x,y)::xs) = y;

fun max(x:int,y:int) =
  if (x>y) then x
  else (y);

fun calc_fun(t:int, r:int, f:int, l:(int)list) = if l=[] then maxFun:=max(!maxFun, f)
else if t > 0
      then if List.nth(l,0) > 0
        then calc_fun(t - 1 + r, r, f + List.nth(l, 0), tl(l))
       else
             calc_fun(t - 1 + r, r, f + List.nth(l, 0), tl(l))
else ();

fun total_fun(x:int, y:int, z:int, l::ls) = calc_fun(y, z, 0, extract(l));

print(total_fun(5, 4, 2, [(1,2),(2,3),(3,4),(4,5)]));
