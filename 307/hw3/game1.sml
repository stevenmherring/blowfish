fun for(N) = if N=1 then [1]
   else for(N-1)@[N];

fun findPay(Current: int, ListPays:(int * int)list) = if #1(hd(ListPays)) = Current then #2(hd(ListPays)) else findPay(Current,tl(ListPays));

fun refill(A,R,C) = if A+R > C then C
     else A+R;

fun findMaxFun3(A:int,L) = if L = [] then []
      else hd(L)+A::findMaxFun3(A,tl(L));

fun findMaxFun(Current, Amount, N, Cap, Refill, ListPays) =
  if Current=N+1 then [0]
  else findMaxFun2(for(Amount),Current,Amount,N,Cap,Refill,ListPays)
and

 findMaxFun2(ListAmounts, Current,Amount,N,Cap,Refill,ListPays) =
   if ListAmounts = [] then []
   else findMaxFun3 (hd(ListAmounts)*findPay(Current,ListPays),
     findMaxFun(Current-1,refill(Amount-hd(ListAmounts),Refill,Cap),
     N, Cap,Refill,ListPays)) @ findMaxFun2(tl(ListAmounts), Current, Amount, N, Cap, Refill, ListPays);

fun fold f [] base = base
       | fold f (x::xs) base = fold f xs (f x base);

fun max [] =  0
       | max (x::xs) = fold (fn a => fn b => if a > b then a else b) (x::xs) x;

fun total_fun(N,Cap,Refill,ListPays)=
 max(findMaxFun(1,Cap,N,Cap,Refill,ListPays));
