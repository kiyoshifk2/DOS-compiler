del %1.lst
del %1.s
del %1.com
del %1.prep
..\prep %1.c %1.prep
if errorlevel 1 goto error
..\banana-c32 %1.prep
if errorlevel 1 goto error
..\nasm -f bin -o %1.com -l %1.lst %1.s
:error