fun gcd(m,n):int = if m=n then n
 else if m>n then gcd(m-n,n)
 else gcd(m,n-m);
