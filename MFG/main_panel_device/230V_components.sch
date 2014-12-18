v 20121123 2
C 9200 84400 1 180 0 CONN3.sym
{
T 9100 82900 5 10 0 0 180 0 1
device=CONN3
T 9100 83100 5 10 0 0 180 0 1
footprint=CONN3
T 9200 84700 5 10 1 1 180 0 1
refdes=CONN_PUMPS_1
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
N 12200 84400 12100 84400 4
N 12100 84400 12100 84500 4
T 7500 84300 9 10 1 0 0 0 1
Rad pump
T 7500 84000 9 10 1 0 0 0 1
Floor pump
T 7500 83700 9 10 1 0 0 0 1
Hidr pump
T 16400 84000 9 10 1 0 0 0 1
HW pump
T 15700 83400 9 10 1 0 0 0 1
Basement floor valve
T 15700 83700 9 10 1 0 0 0 1
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
N 15400 84500 15400 86100 4
N 14800 86100 15400 86100 4
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
N 11900 80600 11800 80600 4
N 11800 80600 11800 80700 4
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
N 15100 80700 15100 82300 4
N 14500 82300 15100 82300 4
N 10200 79400 10900 79400 4
N 15100 79800 15100 79500 4
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
N 12100 88500 12000 88500 4
N 12000 88500 12000 88600 4
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
N 15300 88600 15300 90200 4
N 14700 90200 15300 90200 4
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
N 3000 84700 2900 84700 4
N 2900 84700 2900 84800 4
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
N 2700 80900 2600 80900 4
N 2600 80900 2600 81000 4
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
N 5900 81000 5900 82600 4
N 5300 82600 5900 82600 4
N 5900 80100 5900 79800 4
N 5900 79800 7500 79800 4
N 7500 79800 7500 83600 4
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
N 2900 88800 2800 88800 4
N 2800 88800 2800 88900 4
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
N 6100 88900 6100 90500 4
N 5500 90500 6100 90500 4
N 6100 88000 6100 87000 4
N 6100 87000 7500 87000 4
N 7500 87000 7500 84200 4
N 1300 79700 1700 79700 4
N 6200 83900 7500 83900 4
N 16200 83900 16200 86700 4
N 16200 86700 15300 86700 4
N 16100 79500 16100 83600 4
N 16100 79500 15100 79500 4
N 1300 87600 1900 87600 4
N 1600 83500 2000 83500 4
N 10600 87300 11100 87300 4
N 10600 83200 11200 83200 4
C 10600 73100 1 180 0 io-1.sym
{
T 10000 72400 5 10 0 0 180 0 1
device=none
T 10600 72900 5 10 1 1 180 0 1
net=SW230L:1
T 10100 73000 5 10 1 1 180 7 1
value=IO
}
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
C 3900 75600 1 180 0 io-1.sym
{
T 3000 75600 5 10 1 1 180 0 1
net=Buffer_Pump:1
T 3700 74900 5 10 0 0 180 0 1
device=none
T 3500 75500 5 10 1 1 180 1 1
value=IO
}
C 3800 71100 1 180 0 io-1.sym
{
T 3400 71000 5 10 1 1 180 1 1
value=IO
T 2900 71100 5 10 1 1 180 0 1
net=Spare:1
T 3600 70400 5 10 0 0 180 0 1
device=none
}
C 5400 75000 1 0 0 BS170.sym
{
T 6300 75400 5 10 0 0 0 0 1
device=BS170
T 6300 75200 5 10 0 0 0 0 1
footprint=TO92
T 6300 75500 5 10 1 1 0 0 1
refdes=Q181
}
C 5900 74500 1 0 0 gnd-1.sym
N 6000 74800 6000 75000 4
N 6000 76000 6000 76200 4
N 6000 76200 6400 76200 4
N 6400 76700 6300 76700 4
N 6300 76700 6300 76800 4
C 6200 75800 1 0 0 TLP3063.sym
{
T 6895 77200 5 10 0 0 0 0 1
device=TLP3063
T 6895 77400 5 10 0 0 0 0 1
footprint=DIL_6_300
T 7400 77000 5 10 1 1 0 0 1
refdes=U181
}
C 9000 78500 1 180 0 io-1.sym
{
T 8400 77800 5 10 0 0 180 0 1
device=none
T 9000 78300 5 10 1 1 180 0 1
net=SW230L:1
T 8500 78400 5 10 1 1 180 7 1
value=IO
}
N 8000 76800 8100 76800 4
N 8100 76800 8100 77400 4
N 9600 76800 9600 78400 4
N 9000 78400 9600 78400 4
C 9900 76800 1 90 1 TRIAC.sym
{
T 9000 76500 5 10 0 0 270 2 1
device=TRIAC
T 9550 76100 5 8 0 1 270 2 1
footprint=TO220_TRIAC
T 10300 76000 5 10 1 1 0 6 1
refdes=D181
}
N 9200 76100 8000 76100 4
C 4800 70500 1 0 0 BS170.sym
{
T 5700 70900 5 10 0 0 0 0 1
device=BS170
T 5700 70700 5 10 0 0 0 0 1
footprint=TO92
T 5700 71000 5 10 1 1 0 0 1
refdes=Q171
}
C 5300 70000 1 0 0 gnd-1.sym
N 5400 70300 5400 70500 4
N 5400 71500 5400 71700 4
N 5400 71700 5800 71700 4
N 5800 72200 5700 72200 4
N 5700 72200 5700 72300 4
C 5600 71300 1 0 0 TLP3063.sym
{
T 6295 72700 5 10 0 0 0 0 1
device=TLP3063
T 6295 72900 5 10 0 0 0 0 1
footprint=DIL_6_300
T 6800 72500 5 10 1 1 0 0 1
refdes=U171
}
C 8400 74000 1 180 0 io-1.sym
{
T 7800 73300 5 10 0 0 180 0 1
device=none
T 8400 73800 5 10 1 1 180 0 1
net=SW230L:1
T 7900 73900 5 10 1 1 180 7 1
value=IO
}
N 7400 72300 7500 72300 4
N 7500 72300 7500 72900 4
N 7500 72900 7600 72900 4
N 9000 72300 9000 73900 4
N 8400 73900 9000 73900 4
N 9000 71400 9000 70400 4
C 9300 72300 1 90 1 TRIAC.sym
{
T 8400 72000 5 10 0 0 270 2 1
device=TRIAC
T 8950 71600 5 8 0 1 270 2 1
footprint=TO220_TRIAC
T 9700 71500 5 10 1 1 0 6 1
refdes=D171
}
N 8600 71600 7400 71600 4
N 9600 75900 9600 72500 4
N 9600 72500 11000 72500 4
N 9000 70400 10400 70400 4
N 10400 70400 10400 72200 4
N 10400 72200 11000 72200 4
N 3900 75500 5400 75500 4
N 3800 71000 4800 71000 4
C 12700 73000 1 180 0 CONN3.sym
{
T 12600 71500 5 10 0 0 180 0 1
device=CONN3
T 12600 71700 5 10 0 0 180 0 1
footprint=CONN3
T 12700 73300 5 10 1 1 180 0 1
refdes=CONN_PUMPS_2
}
N 10600 73000 10800 73000 4
N 10800 73000 10800 72800 4
N 10800 72800 11000 72800 4
C 21000 89500 1 0 0 io-1.sym
{
T 21600 90200 5 10 0 0 0 0 1
device=none
T 21000 89700 5 10 1 1 0 0 1
net=SW230L:1
T 21500 89600 5 10 1 1 0 7 1
value=IO
}
N 21000 89600 20600 89600 4
C 20900 87200 1 0 0 vcc-1.sym
N 21100 87200 21100 86900 4
N 21100 86900 20600 86900 4
C 18500 88200 1 270 0 io-1.sym
{
T 19200 87600 5 10 0 0 270 0 1
device=none
T 18500 87900 5 10 1 1 270 0 1
net=I32:1
}
C 13900 89100 1 0 0 io-1.sym
{
T 14500 89800 5 10 0 0 0 0 1
device=none
T 14200 89100 5 10 1 1 0 0 1
net=I32:1
}
C 18500 85400 1 270 0 io-1.sym
{
T 19200 84800 5 10 0 0 270 0 1
device=none
T 18500 85100 5 10 1 1 270 0 1
net=I31:1
}
C 12100 88600 1 90 0 io-1.sym
{
T 11400 89200 5 10 0 0 90 0 1
device=none
T 12100 88900 5 10 1 1 90 0 1
net=I31:1
}
N 18600 88500 18600 88200 4
C 18800 88200 1 270 0 io-1.sym
{
T 19500 87600 5 10 0 0 270 0 1
device=none
T 18800 87900 5 10 1 1 270 0 1
net=I42:1
}
C 19100 88200 1 270 0 io-1.sym
{
T 19800 87600 5 10 0 0 270 0 1
device=none
T 19100 87900 5 10 1 1 270 0 1
net=I52:1
}
C 19400 88200 1 270 0 io-1.sym
{
T 20100 87600 5 10 0 0 270 0 1
device=none
T 19400 87900 5 10 1 1 270 0 1
net=I62:1
}
C 19700 88200 1 270 0 io-1.sym
{
T 20400 87600 5 10 0 0 270 0 1
device=none
T 19700 87900 5 10 1 1 270 0 1
net=I72:1
}
C 18200 88200 1 270 0 io-1.sym
{
T 18900 87600 5 10 0 0 270 0 1
device=none
T 18200 87900 5 10 1 1 270 0 1
net=I22:1
}
C 17900 88200 1 270 0 io-1.sym
{
T 18600 87600 5 10 0 0 270 0 1
device=none
T 17900 87900 5 10 1 1 270 0 1
net=I12:1
}
N 19800 88200 19800 88500 4
N 19500 88500 19500 88200 4
N 19200 88200 19200 88500 4
N 18900 88500 18900 88200 4
N 18300 88200 18300 88500 4
N 18000 88500 18000 88200 4
C 14000 85000 1 0 0 io-1.sym
{
T 14600 85700 5 10 0 0 0 0 1
device=none
T 14300 85000 5 10 1 1 0 0 1
net=I12:1
}
C 13700 81200 1 0 0 io-1.sym
{
T 14300 81900 5 10 0 0 0 0 1
device=none
T 14000 81200 5 10 1 1 0 0 1
net=I22:1
}
C 4500 81500 1 0 0 io-1.sym
{
T 5100 82200 5 10 0 0 0 0 1
device=none
T 4800 81500 5 10 1 1 0 0 1
net=I42:1
}
C 4800 85300 1 0 0 io-1.sym
{
T 5400 86000 5 10 0 0 0 0 1
device=none
T 5100 85300 5 10 1 1 0 0 1
net=I52:1
}
C 4700 89400 1 0 0 io-1.sym
{
T 5300 90100 5 10 0 0 0 0 1
device=none
T 5000 89400 5 10 1 1 0 0 1
net=I62:1
}
C 7600 72800 1 0 0 io-1.sym
{
T 8200 73500 5 10 0 0 0 0 1
device=none
T 7900 72800 5 10 1 1 0 0 1
net=I72:1
}
C 17900 88500 1 0 0 resistorpack9-1.sym
{
T 18000 89900 5 10 0 0 0 0 1
device=RESISTORPACK_9
T 18000 89700 5 10 1 1 0 0 1
refdes=R200
T 18600 89700 5 10 1 1 0 0 1
value=100R
T 17900 88500 5 10 1 1 0 0 1
footprint=SIP9
}
C 20000 88200 1 270 0 io-1.sym
{
T 20700 87600 5 10 0 0 270 0 1
device=none
T 20000 87900 5 10 1 1 270 0 1
net=I82:1
}
N 20100 88500 20100 88200 4
C 8300 77300 1 0 0 io-1.sym
{
T 8900 78000 5 10 0 0 0 0 1
device=none
T 8600 77300 5 10 1 1 0 0 1
net=I82:1
}
N 8300 77400 8100 77400 4
C 17900 85800 1 0 0 resistorpack9-1.sym
{
T 18000 87200 5 10 0 0 0 0 1
device=RESISTORPACK_9
T 18000 87000 5 10 1 1 0 0 1
refdes=R300
T 18600 87000 5 10 1 1 0 0 1
value=1k
T 17900 85800 5 10 1 1 0 0 1
footprint=SIP9
}
N 18600 85400 18600 85800 4
C 18200 85400 1 270 0 io-1.sym
{
T 18900 84800 5 10 0 0 270 0 1
device=none
T 18200 85100 5 10 1 1 270 0 1
net=I21:1
}
C 17900 85400 1 270 0 io-1.sym
{
T 18600 84800 5 10 0 0 270 0 1
device=none
T 17900 85100 5 10 1 1 270 0 1
net=I11:1
}
C 18800 85400 1 270 0 io-1.sym
{
T 19500 84800 5 10 0 0 270 0 1
device=none
T 18800 85100 5 10 1 1 270 0 1
net=I41:1
}
C 19100 85400 1 270 0 io-1.sym
{
T 19800 84800 5 10 0 0 270 0 1
device=none
T 19100 85100 5 10 1 1 270 0 1
net=I51:1
}
C 19400 85400 1 270 0 io-1.sym
{
T 20100 84800 5 10 0 0 270 0 1
device=none
T 19400 85100 5 10 1 1 270 0 1
net=I61:1
}
C 20000 85400 1 270 0 io-1.sym
{
T 20700 84800 5 10 0 0 270 0 1
device=none
T 20000 85100 5 10 1 1 270 0 1
net=I71:1
}
C 19700 85400 1 270 0 io-1.sym
{
T 20400 84800 5 10 0 0 270 0 1
device=none
T 19700 85100 5 10 1 1 270 0 1
net=I81:1
}
N 19500 85400 19500 85800 4
N 19200 85800 19200 85400 4
N 18900 85800 18900 85400 4
N 18300 85800 18300 85400 4
N 18000 85800 18000 85400 4
C 11700 80700 1 270 1 io-1.sym
{
T 12400 81300 5 10 0 0 90 2 1
device=none
T 11700 81000 5 10 1 1 90 2 1
net=I21:1
}
C 12000 84500 1 270 1 io-1.sym
{
T 12700 85100 5 10 0 0 90 2 1
device=none
T 12000 84800 5 10 1 1 90 2 1
net=I11:1
}
C 2500 81000 1 270 1 io-1.sym
{
T 3200 81600 5 10 0 0 90 2 1
device=none
T 2500 81300 5 10 1 1 90 2 1
net=I41:1
}
C 2800 84800 1 270 1 io-1.sym
{
T 3500 85400 5 10 0 0 90 2 1
device=none
T 2800 85100 5 10 1 1 90 2 1
net=I51:1
}
C 2700 88900 1 270 1 io-1.sym
{
T 3400 89500 5 10 0 0 90 2 1
device=none
T 2700 89200 5 10 1 1 90 2 1
net=I61:1
}
C 5600 72300 1 270 1 io-1.sym
{
T 6300 72900 5 10 0 0 90 2 1
device=none
T 5600 72600 5 10 1 1 90 2 1
net=I71:1
}
C 6200 76800 1 270 1 io-1.sym
{
T 6900 77400 5 10 0 0 90 2 1
device=none
T 6200 77100 5 10 1 1 90 2 1
net=I81:1
}
N 20100 85400 20100 85800 4
N 19800 85800 19800 85400 4
N 16100 83600 16300 83600 4
N 16300 83300 15400 83300 4
N 15400 83300 15400 83600 4
