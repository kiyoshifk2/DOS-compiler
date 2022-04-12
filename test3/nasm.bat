del %1.lst
del %1.com
..\nasm -f bin -o %1.com -l %1.lst %1.s
:error