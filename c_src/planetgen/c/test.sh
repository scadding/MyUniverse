#!/bin/bash

w=640
h=480

for i in a c g h m M p q s S
do
    echo $i
    ./planet -C burrowsB.col -w $w -h $h -g 10 -G 10 -p$i > bmp/$i.bmp
done



#./planet -w 640 -h 480 -g 10 -G 10 -pa > bmp/a.bmp 
#./planet -w 640 -h 480 -g 10 -G 10 -pc > bmp/c.bmp 
#./planet -w 640 -h 480 -g 10 -G 10 -pg > bmp/g.bmp 
#./planet -w 640 -h 480 -g 10 -G 10 -ph > bmp/h.bmp 
#./planet -w 640 -h 480 -g 10 -G 10 -pm > bmp/m.bmp 
#./planet -w 640 -h 480 -g 10 -G 10 -pM > bmp/M.bmp 
#./planet -w 640 -h 480 -g 10 -G 10 -pp > bmp/p.bmp 
#./planet -w 640 -h 480 -g 10 -G 10 -pq > bmp/q.bmp 
#./planet -w 640 -h 480 -g 10 -G 10 -ps > bmp/s.bmp 
#./planet -w 640 -h 480 -g 10 -G 10 -pS > bmp/S.bmp 

