rem sine table maker in 1st basic

n=&hc0000
values=300

count=0

for t=0 to (3.1415927*2) step (3.1415927*2)/values

offset=int(100+(sin(t)*99))
count=count+2

pokew n,offset
n=n+2
next t

bsave "a:\parry1.bin",&hc0000,count