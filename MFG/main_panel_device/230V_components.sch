v 20121123 2
C 9200 84400 1 180 0 CONN3.sym
{
T 9100 82900 5 10 0 0 180 0 1
device=CONN3
T 9100 83100 5 10 0 0 180 0 1
footprint=CONN3
T 9200 84700 5 10 1 1 180 0 1
refdes=CONN_PUMPS
}
C 18000 84100 1 180 0 CONN3.sym
{
T 17900 82600 5 10 0 0 180 0 1
device=CONN3
T 17900 82800 5 10 0 0 180 0 1
footprint=CONN3
T 18000 84400 5 10 1 1 180 0 1
refdes=CONN_VALVES
}
C 11200 82700 1 0 0 BS170.sym
{
T 12100 83100 5 10 0 0 0 0 1
device=BS170
T 12100 82900 5 10 0 0 0 0 1
footprint=TO92
T 12100 83200 5 10 1 1 0 0 1
refdes=Q111
}
C 11700 82200 1 0 0 gnd-1.sym
N 11800 82500 11800 82700 4
N 11800 83700 11800 83900 4
N 11800 83900 12200 83900 4
C 11900 85500 1 0 0 vcc-1.sym
C 12200 84500 1 90 0 SMALLRES.sym
{
T 11550 84600 5 10 0 0 90 0 1
device=SMALLRES
T 11700 84600 5 10 0 0 90 0 1
footprint=ACY300
T 11900 85100 5 10 1 1 180 0 1
refdes=R111
T 12200 84800 5 10 1 1 90 0 1
value=1k
}
N 12200 84400 12100 84400 4
N 12100 84400 12100 84500 4
N 12100 85400 12100 85500 4
T 7500 84300 9 10 1 0 0 0 1
Rad pump
T 7500 84000 9 10 1 0 0 0 1
Floor pump
T 7500 83700 9 10 1 0 0 0 1
Hidr pump
T 16400 84000 9 10 1 0 0 0 1
HW pump
T 15700 83700 9 10 1 0 0 0 1
Basement floor valve
T 15700 83400 9 10 1 0 0 0 1
Basement rad valve
C 1300 87700 1 180 0 io-1.sym
{
T 1100 87000 5 10 0 0 180 0 1
device=none
T 1200 87300 5 10 1 1 180 0 1
net=RadPump:1
T 900 87600 5 10 1 1 180 1 1
value=IO
}
C 1600 83600 1 180 0 io-1.sym
{
T 1400 82900 5 10 0 0 180 0 1
device=none
T 1300 83300 5 10 1 1 180 0 1
net=FloorPump:1
T 1200 83500 5 10 1 1 180 1 1
value=IO
}
C 1300 79800 1 180 0 io-1.sym
{
T 1100 79100 5 10 0 0 180 0 1
device=none
T 1000 79500 5 10 1 1 180 0 1
net=HidrPump:1
T 900 79700 5 10 1 1 180 1 1
value=IO
}
C 10600 87400 1 180 0 io-1.sym
{
T 10400 86700 5 10 0 0 180 0 1
device=none
T 10500 87000 5 10 1 1 180 0 1
net=HWPump:1
T 10200 87300 5 10 1 1 180 1 1
value=IO
}
C 10600 83300 1 180 0 io-1.sym
{
T 10400 82600 5 10 0 0 180 0 1
device=none
T 10400 82900 5 10 1 1 180 0 1
net=BasFlValve:1
T 10200 83200 5 10 1 1 180 1 1
value=IO
}
C 10200 79500 1 180 0 io-1.sym
{
T 10000 78800 5 10 0 0 180 0 1
device=none
T 10200 79200 5 10 1 1 180 0 1
net=BasRadValve:1
T 9800 79400 5 10 1 1 180 1 1
value=IO
}
C 12000 83500 1 0 0 TLP3063.sym
{
T 12695 84900 5 10 0 0 0 0 1
device=TLP3063
T 12695 85100 5 10 0 0 0 0 1
footprint=DIL_6_300
T 13200 84700 5 10 1 1 0 0 1
refdes=U111
}
C 14800 86200 1 180 0 io-1.sym
{
T 14200 85500 5 10 0 0 180 0 1
device=none
T 14800 86000 5 10 1 1 180 0 1
net=SW230L:1
T 14300 86100 5 10 1 1 180 7 1
value=IO
}
N 13800 84500 13900 84500 4
N 13900 84500 13900 85100 4
N 13900 85100 14000 85100 4
N 14900 85100 15400 85100 4
N 15400 84500 15400 86100 4
N 14800 86100 15400 86100 4
N 15400 83600 16300 83600 4
C 10900 78900 1 0 0 BS170.sym
{
T 11800 79300 5 10 0 0 0 0 1
device=BS170
T 11800 79100 5 10 0 0 0 0 1
footprint=TO92
T 11800 79400 5 10 1 1 0 0 1
refdes=Q121
}
C 11400 78200 1 0 0 gnd-1.sym
N 11500 78500 11500 78900 4
N 11500 79900 11500 80100 4
N 11500 80100 11900 80100 4
C 11600 81700 1 0 0 vcc-1.sym
C 11900 80700 1 90 0 SMALLRES.sym
{
T 11250 80800 5 10 0 0 90 0 1
device=SMALLRES
T 11400 80800 5 10 0 0 90 0 1
footprint=ACY300
T 11600 81300 5 10 1 1 180 0 1
refdes=R121
T 11900 81000 5 10 1 1 90 0 1
value=1k
}
N 11900 80600 11800 80600 4
N 11800 80600 11800 80700 4
N 11800 81600 11800 81700 4
C 11700 79700 1 0 0 TLP3063.sym
{
T 12395 81100 5 10 0 0 0 0 1
device=TLP3063
T 12395 81300 5 10 0 0 0 0 1
footprint=DIL_6_300
T 12900 80900 5 10 1 1 0 0 1
refdes=U121
}
C 14500 82400 1 180 0 io-1.sym
{
T 13900 81700 5 10 0 0 180 0 1
device=none
T 14500 82200 5 10 1 1 180 0 1
net=SW230L:1
T 14000 82300 5 10 1 1 180 7 1
value=IO
}
N 13500 80700 13600 80700 4
N 13600 80700 13600 81300 4
N 13600 81300 13700 81300 4
N 14600 81300 15100 81300 4
N 15100 80700 15100 82300 4
N 14500 82300 15100 82300 4
N 10200 79400 10900 79400 4
N 15100 79800 15100 79500 4
N 16100 83300 16300 83300 4
C 14900 85200 1 180 0 SMALLRES.sym
{
T 14800 84550 5 10 0 0 180 0 1
device=SMALLRES
T 14800 84700 5 10 0 0 180 0 1
footprint=ACY300
T 14300 85300 5 10 1 1 0 0 1
refdes=R112
T 14600 85200 5 10 1 1 180 0 1
value=100
}
C 14600 81400 1 180 0 SMALLRES.sym
{
T 14500 80750 5 10 0 0 180 0 1
device=SMALLRES
T 14500 80900 5 10 0 0 180 0 1
footprint=ACY300
T 14000 81500 5 10 1 1 0 0 1
refdes=R122
T 14300 81400 5 10 1 1 180 0 1
value=100
}
C 11100 86800 1 0 0 BS170.sym
{
T 12000 87200 5 10 0 0 0 0 1
device=BS170
T 12000 87000 5 10 0 0 0 0 1
footprint=TO92
T 12000 87300 5 10 1 1 0 0 1
refdes=Q131
}
C 11600 86300 1 0 0 gnd-1.sym
N 11700 86600 11700 86800 4
N 11700 87800 11700 88000 4
N 11700 88000 12100 88000 4
C 11800 89600 1 0 0 vcc-1.sym
C 12100 88600 1 90 0 SMALLRES.sym
{
T 11450 88700 5 10 0 0 90 0 1
device=SMALLRES
T 11600 88700 5 10 0 0 90 0 1
footprint=ACY300
T 11800 89200 5 10 1 1 180 0 1
refdes=R131
T 12100 88900 5 10 1 1 90 0 1
value=1k
}
N 12100 88500 12000 88500 4
N 12000 88500 12000 88600 4
N 12000 89500 12000 89600 4
C 11900 87600 1 0 0 TLP3063.sym
{
T 12595 89000 5 10 0 0 0 0 1
device=TLP3063
T 12595 89200 5 10 0 0 0 0 1
footprint=DIL_6_300
T 13100 88800 5 10 1 1 0 0 1
refdes=U131
}
C 14700 90300 1 180 0 io-1.sym
{
T 14100 89600 5 10 0 0 180 0 1
device=none
T 14700 90100 5 10 1 1 180 0 1
net=SW230L:1
T 14200 90200 5 10 1 1 180 7 1
value=IO
}
N 13700 88600 13800 88600 4
N 13800 88600 13800 89200 4
N 13800 89200 13900 89200 4
N 14800 89200 15300 89200 4
N 15300 88600 15300 90200 4
N 14700 90200 15300 90200 4
C 14800 89300 1 180 0 SMALLRES.sym
{
T 14700 88650 5 10 0 0 180 0 1
device=SMALLRES
T 14700 88800 5 10 0 0 180 0 1
footprint=ACY300
T 14200 89400 5 10 1 1 0 0 1
refdes=R132
T 14500 89300 5 10 1 1 180 0 1
value=100
}
N 15300 87700 15300 86700 4
N 16200 83900 16300 83900 4
C 2000 83000 1 0 0 BS170.sym
{
T 2900 83400 5 10 0 0 0 0 1
device=BS170
T 2900 83200 5 10 0 0 0 0 1
footprint=TO92
T 2900 83500 5 10 1 1 0 0 1
refdes=Q151
}
C 2500 82500 1 0 0 gnd-1.sym
N 2600 82800 2600 83000 4
N 2600 84000 2600 84200 4
N 2600 84200 3000 84200 4
C 2700 85800 1 0 0 vcc-1.sym
C 3000 84800 1 90 0 SMALLRES.sym
{
T 2350 84900 5 10 0 0 90 0 1
device=SMALLRES
T 2500 84900 5 10 0 0 90 0 1
footprint=ACY300
T 2700 85400 5 10 1 1 180 0 1
refdes=R151
T 3000 85100 5 10 1 1 90 0 1
value=1k
}
N 3000 84700 2900 84700 4
N 2900 84700 2900 84800 4
N 2900 85700 2900 85800 4
C 2800 83800 1 0 0 TLP3063.sym
{
T 3495 85200 5 10 0 0 0 0 1
device=TLP3063
T 3495 85400 5 10 0 0 0 0 1
footprint=DIL_6_300
T 4000 85000 5 10 1 1 0 0 1
refdes=U151
}
C 5600 86500 1 180 0 io-1.sym
{
T 5000 85800 5 10 0 0 180 0 1
device=none
T 5600 86300 5 10 1 1 180 0 1
net=SW230L:1
T 5100 86400 5 10 1 1 180 7 1
value=IO
}
N 4600 84800 4700 84800 4
N 4700 84800 4700 85400 4
N 4700 85400 4800 85400 4
N 5700 85400 6200 85400 4
N 6200 84800 6200 86400 4
N 5600 86400 6200 86400 4
C 1700 79200 1 0 0 BS170.sym
{
T 2600 79600 5 10 0 0 0 0 1
device=BS170
T 2600 79400 5 10 0 0 0 0 1
footprint=TO92
T 2600 79700 5 10 1 1 0 0 1
refdes=Q141
}
C 2200 78500 1 0 0 gnd-1.sym
N 2300 78800 2300 79200 4
N 2300 80200 2300 80400 4
N 2300 80400 2700 80400 4
C 2400 82000 1 0 0 vcc-1.sym
C 2700 81000 1 90 0 SMALLRES.sym
{
T 2050 81100 5 10 0 0 90 0 1
device=SMALLRES
T 2200 81100 5 10 0 0 90 0 1
footprint=ACY300
T 2400 81600 5 10 1 1 180 0 1
refdes=R141
T 2700 81300 5 10 1 1 90 0 1
value=1k
}
N 2700 80900 2600 80900 4
N 2600 80900 2600 81000 4
N 2600 81900 2600 82000 4
C 2500 80000 1 0 0 TLP3063.sym
{
T 3195 81400 5 10 0 0 0 0 1
device=TLP3063
T 3195 81600 5 10 0 0 0 0 1
footprint=DIL_6_300
T 3700 81200 5 10 1 1 0 0 1
refdes=U141
}
C 5300 82700 1 180 0 io-1.sym
{
T 4700 82000 5 10 0 0 180 0 1
device=none
T 5300 82500 5 10 1 1 180 0 1
net=SW230L:1
T 4800 82600 5 10 1 1 180 7 1
value=IO
}
N 4300 81000 4400 81000 4
N 4400 81000 4400 81600 4
N 4400 81600 4500 81600 4
N 5400 81600 5900 81600 4
N 5900 81000 5900 82600 4
N 5300 82600 5900 82600 4
N 5900 80100 5900 79800 4
N 5900 79800 7500 79800 4
N 7500 79800 7500 83600 4
C 5700 85500 1 180 0 SMALLRES.sym
{
T 5600 84850 5 10 0 0 180 0 1
device=SMALLRES
T 5600 85000 5 10 0 0 180 0 1
footprint=ACY300
T 5100 85600 5 10 1 1 0 0 1
refdes=R152
T 5400 85500 5 10 1 1 180 0 1
value=100
}
C 5400 81700 1 180 0 SMALLRES.sym
{
T 5300 81050 5 10 0 0 180 0 1
device=SMALLRES
T 5300 81200 5 10 0 0 180 0 1
footprint=ACY300
T 4800 81800 5 10 1 1 0 0 1
refdes=R142
T 5100 81700 5 10 1 1 180 0 1
value=100
}
C 1900 87100 1 0 0 BS170.sym
{
T 2800 87500 5 10 0 0 0 0 1
device=BS170
T 2800 87300 5 10 0 0 0 0 1
footprint=TO92
T 2800 87600 5 10 1 1 0 0 1
refdes=Q161
}
C 2400 86600 1 0 0 gnd-1.sym
N 2500 86900 2500 87100 4
N 2500 88100 2500 88300 4
N 2500 88300 2900 88300 4
C 2600 89900 1 0 0 vcc-1.sym
C 2900 88900 1 90 0 SMALLRES.sym
{
T 2250 89000 5 10 0 0 90 0 1
device=SMALLRES
T 2400 89000 5 10 0 0 90 0 1
footprint=ACY300
T 2600 89500 5 10 1 1 180 0 1
refdes=R161
T 2900 89200 5 10 1 1 90 0 1
value=1k
}
N 2900 88800 2800 88800 4
N 2800 88800 2800 88900 4
N 2800 89800 2800 89900 4
C 2700 87900 1 0 0 TLP3063.sym
{
T 3395 89300 5 10 0 0 0 0 1
device=TLP3063
T 3395 89500 5 10 0 0 0 0 1
footprint=DIL_6_300
T 3900 89100 5 10 1 1 0 0 1
refdes=U161
}
C 5500 90600 1 180 0 io-1.sym
{
T 4900 89900 5 10 0 0 180 0 1
device=none
T 5500 90400 5 10 1 1 180 0 1
net=SW230L:1
T 5000 90500 5 10 1 1 180 7 1
value=IO
}
N 4500 88900 4600 88900 4
N 4600 88900 4600 89500 4
N 4600 89500 4700 89500 4
N 5600 89500 6100 89500 4
N 6100 88900 6100 90500 4
N 5500 90500 6100 90500 4
C 5600 89600 1 180 0 SMALLRES.sym
{
T 5500 88950 5 10 0 0 180 0 1
device=SMALLRES
T 5500 89100 5 10 0 0 180 0 1
footprint=ACY300
T 5000 89700 5 10 1 1 0 0 1
refdes=R162
T 5300 89600 5 10 1 1 180 0 1
value=100
}
N 6100 88000 6100 87000 4
N 6100 87000 7500 87000 4
N 7500 87000 7500 84200 4
N 1300 79700 1700 79700 4
N 6200 83900 7500 83900 4
N 16200 83900 16200 86700 4
N 16200 86700 15300 86700 4
N 16100 83300 16100 79500 4
N 16100 79500 15100 79500 4
N 1300 87600 1900 87600 4
N 1600 83500 2000 83500 4
N 10600 87300 11100 87300 4
N 10600 83200 11200 83200 4
C 10100 90100 1 180 0 CONN2.sym
{
T 9900 89100 5 10 0 0 180 0 1
device=CONN2
T 10000 90300 5 10 1 1 180 0 1
refdes=CONN_230L_IN
T 9400 89300 5 10 0 0 180 0 1
footprint=CONN2
}
C 7900 90000 1 180 0 io-1.sym
{
T 7300 89300 5 10 0 0 180 0 1
device=none
T 7900 89800 5 10 1 1 180 0 1
net=SW230L:1
T 7400 89900 5 10 1 1 180 7 1
value=IO
}
N 8400 89600 8100 89600 4
N 8100 89600 8100 89900 4
N 8100 89900 7900 89900 4
C 15600 88600 1 90 1 TRIAC.sym
{
T 14700 88300 5 10 0 0 270 2 1
device=TRIAC
T 16000 87800 5 10 1 1 0 6 1
refdes=D131
T 15250 87900 5 8 0 1 270 2 1
footprint=TO220_TRIAC
}
N 14900 87900 13700 87900 4
C 15700 84500 1 90 1 TRIAC.sym
{
T 14800 84200 5 10 0 0 270 2 1
device=TRIAC
T 15000 84100 5 10 1 1 0 6 1
refdes=D111
T 15350 83800 5 8 0 1 270 2 1
footprint=TO220_TRIAC
}
N 15000 83800 13800 83800 4
C 15400 80700 1 90 1 TRIAC.sym
{
T 14500 80400 5 10 0 0 270 2 1
device=TRIAC
T 14800 80500 5 10 1 1 0 6 1
refdes=D121
T 15050 80000 5 8 0 1 270 2 1
footprint=TO220_TRIAC
}
N 14700 80000 13500 80000 4
C 6200 81000 1 90 1 TRIAC.sym
{
T 5300 80700 5 10 0 0 270 2 1
device=TRIAC
T 6600 80200 5 10 1 1 0 6 1
refdes=D141
T 5850 80300 5 8 0 1 270 2 1
footprint=TO220_TRIAC
}
N 4300 80300 5500 80300 4
C 6500 84800 1 90 1 TRIAC.sym
{
T 5600 84500 5 10 0 0 270 2 1
device=TRIAC
T 6900 84000 5 10 1 1 0 6 1
refdes=D151
T 6150 84100 5 8 0 1 270 2 1
footprint=TO220_TRIAC
}
N 5800 84100 4600 84100 4
C 6400 88900 1 90 1 TRIAC.sym
{
T 5500 88600 5 10 0 0 270 2 1
device=TRIAC
T 6800 88100 5 10 1 1 0 6 1
refdes=D161
T 6050 88200 5 8 0 1 270 2 1
footprint=TO220_TRIAC
}
N 5700 88200 4500 88200 4
