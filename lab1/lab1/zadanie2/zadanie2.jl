ccall((:srandom,"libc"),Int32,(Cuint,),1)
t = ccall((:random,"libc"),Int32,())
count = 0
while ccall((:random,"libc"),Int32,())!= t
  count = count+1
end

println(k)
