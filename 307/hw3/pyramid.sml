
fun writeNrec(s, i:int, n:int) =
  if i<n then concat([s,writeNrec(s,i+1,n)])
  else "";
fun pyramidRec(I,N) =
  if I=N+1 then ""
  else concat([writeNrec(" ", 1, N-I),
      writeNrec("*",1,2*I-1),
      "\n",
      pyramidRec(I+1,N)
      ]);
fun pyramid(N) =
  pyramidRec(1,N);

pyramid(6);
