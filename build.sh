#!/bin/sh


echo Compiling ...

make
chmod +x ./emul

echo 
echo Done!
echo 

./emul
