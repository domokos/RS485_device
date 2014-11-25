v 20130925 2
C 4900 88900 1 180 0 SN65176BP.sym
{
T 3300 87200 5 10 1 1 180 6 1
refdes=U1
T 4905 88900 5 10 0 1 180 0 1
footprint=DIP8
T 4905 88900 5 10 0 1 180 0 1
device=SN65176BP
}
C 13300 86700 1 0 0 TA8030S.sym
{
T 15200 88950 5 10 1 1 0 0 1
refdes=U3
T 14100 88500 5 10 0 1 0 0 1
device=TA8030S
T 13300 86700 5 10 0 0 0 0 1
footprint=SIP7
}
C 6600 85400 1 0 0 AT89C4051-1.sym
{
T 9100 89400 5 10 1 1 0 6 1
refdes=U2
T 6595 85395 5 10 0 1 0 0 1
footprint=DIP20
T 6595 85395 5 10 0 1 0 0 1
device=AT89C4051
}
C 10900 89500 1 0 0 vcc-1.sym
N 9400 88900 10200 88900 4
N 8700 84400 8700 85400 4
N 7900 84400 7900 85400 4
N 7900 84700 8000 84700 4
C 8200 83100 1 0 0 gnd-1.sym
N 7900 83500 8700 83500 4
N 8300 83500 8300 83400 4
N 13100 88000 13300 88000 4
C 14200 89600 1 0 0 vcc-1.sym
C 11400 85700 1 0 0 gnd-1.sym
N 13300 87500 12600 87500 4
N 14400 89600 14400 89200 4
N 11500 86000 11500 86500 4
N 11500 86500 11600 86500 4
N 12500 86500 12900 86500 4
N 12900 86500 12900 87500 4
N 16100 88600 16100 89200 4
N 16100 87700 16100 87400 4
N 16900 89100 16900 89200 4
N 16900 88200 16900 88100 4
C 4000 84300 1 0 0 vcc-1.sym
C 4100 82200 1 0 0 gnd-1.sym
N 3100 87600 800 87600 4
N 2200 88000 3100 88000 4
N 2000 86300 2200 86300 4
N 4900 87800 5200 87800 4
N 4900 88100 5200 88100 4
C 16700 86900 1 0 0 BS170.sym
{
T 17600 87300 5 10 0 0 0 0 1
device=BS170
T 17600 87400 5 10 1 1 0 0 1
refdes=Q4
T 17600 87100 5 10 0 0 0 0 1
footprint=TO92
}
N 15700 87400 16700 87400 4
N 17300 87900 17300 88100 4
N 17300 86900 17300 86100 4
N 17300 86100 11500 86100 4
C 200 83300 1 0 0 CONN2.sym
{
T 400 84300 5 10 0 0 0 0 1
device=CONN2
T 200 84100 5 10 1 1 0 0 1
refdes=CONN_RS_VCC
T 900 84100 5 10 0 0 0 0 1
footprint=CONN2
}
C 200 84500 1 0 0 CONN2.sym
{
T 400 85500 5 10 0 0 0 0 1
device=CONN2
T 200 85300 5 10 1 1 0 0 1
refdes=CONN_RS485
T 900 85300 5 10 0 0 0 0 1
footprint=CONN2
}
C 2000 86400 1 180 0 SMALLRES.sym
{
T 1900 85750 5 10 0 0 180 0 1
device=SMALLRES
T 1800 86100 5 10 1 1 180 0 1
refdes=R6
T 1900 85900 5 10 0 0 180 0 1
footprint=ACY300
T 1700 86400 5 10 1 1 180 0 1
value=100R
}
C 11700 87400 1 0 0 SMALLRES.sym
{
T 11800 88050 5 10 0 0 0 0 1
device=SMALLRES
T 12300 87300 5 10 1 1 180 0 1
refdes=R3
T 11800 87900 5 10 0 0 0 0 1
footprint=ACY300
T 12000 87400 5 10 1 1 0 0 1
value=20k
}
C 10200 88800 1 0 0 SMALLRES.sym
{
T 10300 89450 5 10 0 0 0 0 1
device=SMALLRES
T 10800 89200 5 10 1 1 180 0 1
refdes=R1
T 10300 89300 5 10 0 0 0 0 1
footprint=ACY300
T 10500 88800 5 10 1 1 0 0 1
value=4.7k
}
C 16200 87700 1 90 0 SMALLRES.sym
{
T 15550 87800 5 10 0 0 90 0 1
device=SMALLRES
T 16500 88300 5 10 1 1 180 0 1
refdes=R5
T 15700 87800 5 10 0 0 90 0 1
footprint=ACY300
T 16200 88000 5 10 1 1 90 0 1
value=4.7k
}
C 17000 88200 1 90 0 SMALLRES.sym
{
T 16350 88300 5 10 0 0 90 0 1
device=SMALLRES
T 17300 88800 5 10 1 1 180 0 1
refdes=R4
T 16500 88300 5 10 0 0 90 0 1
footprint=ACY300
T 17000 88500 5 10 1 1 90 0 1
value=4.7k
}
C 2000 88500 1 180 0 SURGE.sym
{
T 1950 87550 5 10 0 0 180 0 1
device=SURGE
T 1750 87850 5 10 1 1 180 0 1
refdes=TVS2
T 1980 87700 5 10 0 0 180 0 1
footprint=ACY400
}
C 2000 89400 1 180 0 SURGE.sym
{
T 1950 88450 5 10 0 0 180 0 1
device=SURGE
T 1750 88750 5 10 1 1 180 0 1
refdes=TVS3
T 1980 88600 5 10 0 0 180 0 1
footprint=ACY400
}
C 2000 87500 1 180 0 SURGE.sym
{
T 1950 86550 5 10 0 0 180 0 1
device=SURGE
T 1750 86850 5 10 1 1 180 0 1
refdes=TVS1
T 1980 86700 5 10 0 0 180 0 1
footprint=ACY400
}
N 800 87200 800 87600 4
N 2000 87200 2200 87200 4
N 2600 87600 2600 89100 4
N 2600 89100 2000 89100 4
N 2200 88200 2000 88200 4
N 800 89900 800 88200 4
C 8000 84600 1 0 0 CRYSTAL.sym
{
T 8200 85100 5 10 0 0 0 0 1
device=CRYSTAL
T 8200 84900 5 10 1 1 0 0 1
refdes=CR1
T 8200 85500 5 10 0 0 0 0 1
footprint=CRYSTAL 300
}
C 11600 86300 1 0 0 BIGCERCAP.sym
{
T 11800 87000 5 10 0 0 0 0 1
device=BIGCERCAP
T 11800 86800 5 10 1 1 0 0 1
refdes=C8
T 11800 87400 5 10 0 0 0 0 1
footprint=ACY200
T 12200 86600 5 10 1 1 0 0 1
value=330nF
}
C 12200 87800 1 0 0 SMALLCERCAP.sym
{
T 12400 88500 5 10 0 0 0 0 1
device=SMALLCERCAP
T 12400 88300 5 10 1 1 0 0 1
refdes=C6
T 12400 88900 5 10 0 0 0 0 1
footprint=ACY150
T 12800 88100 5 10 1 1 0 0 1
value=2.2nF
}
C 8100 83500 1 90 0 SMALLCERCAP.sym
{
T 7400 83700 5 10 0 0 90 0 1
device=SMALLCERCAP
T 7800 84200 5 10 1 1 180 0 1
refdes=C5
T 7000 83700 5 10 0 0 90 0 1
footprint=ACY150
T 7400 83700 5 10 1 1 0 0 1
value=33pF
}
C 8900 83500 1 90 0 SMALLCERCAP.sym
{
T 8200 83700 5 10 0 0 90 0 1
device=SMALLCERCAP
T 8600 84200 5 10 1 1 180 0 1
refdes=C7
T 7800 83700 5 10 0 0 90 0 1
footprint=ACY150
T 8200 83700 5 10 1 1 0 0 1
value=33pF
}
N 4900 87500 5700 87500 4
N 12200 88000 11800 88000 4
N 11800 88000 11800 90000 4
N 11800 90000 6100 90000 4
N 11400 89200 16900 89200 4
N 11400 89200 11400 87500 4
N 11400 87500 11700 87500 4
N 4200 83600 4200 84300 4
C 4000 83600 1 270 0 SMALLELCO.sym
{
T 4700 83400 5 10 0 0 270 0 1
device=SMALLELCO
T 4400 82900 5 10 1 1 0 0 1
refdes=C9
T 5200 83400 5 10 0 0 270 0 1
footprint=RCY100P
T 3400 83100 5 10 1 1 0 0 1
value=100uF
}
N 4200 82500 4200 82700 4
N 2100 82600 4200 82600 4
N 2100 83800 1900 83800 4
N 3200 83500 3500 83500 4
N 3500 83500 3500 83800 4
N 3500 83800 4200 83800 4
N 2300 83500 1900 83500 4
N 2100 83800 2100 82600 4
N 2200 88200 2200 84700 4
N 2200 84700 1900 84700 4
N 2600 87600 2600 85000 4
N 2600 85000 1900 85000 4
C 2300 83300 1 0 0 DIODE_300.sym
{
T 2700 83900 5 10 0 0 0 0 1
device=DIODE
T 2600 83800 5 10 1 1 0 0 1
refdes=D1
T 2400 84400 5 10 0 0 0 0 1
footprint=DIODE300
}
C 4800 89500 1 0 0 gnd-1.sym
N 4900 89800 4900 89900 4
N 4900 89900 800 89900 4
N 4200 88900 4200 89900 4
C 3500 89200 1 0 0 vcc-1.sym
N 3700 89200 3700 88900 4
N 5700 87500 5700 88900 4
N 5700 88900 6600 88900 4
N 6600 88400 4900 88400 4
N 1100 86300 900 86300 4
N 900 86300 900 85700 4
N 900 85700 2600 85700 4
C 10200 88400 1 0 0 SMALLRES.sym
{
T 10300 89050 5 10 0 0 0 0 1
device=SMALLRES
T 10800 88300 5 10 1 1 180 0 1
refdes=R2
T 10300 88900 5 10 0 0 0 0 1
footprint=ACY300
T 10500 88400 5 10 1 1 0 0 1
value=4.7k
}
N 13300 87000 13200 87000 4
N 13200 87000 13200 89200 4
B 100 81700 18600 8900 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
T 16800 81900 9 10 1 0 0 0 2
RS485 communication
component
B 16700 81700 2000 700 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
C 10100 83300 1 0 0 io-1.sym
{
T 11000 83300 5 10 1 1 0 0 1
net=RadPump:1
T 10300 84000 5 10 0 0 0 0 1
device=none
T 10500 83400 5 10 1 1 0 1 1
value=IO
}
C 10100 82900 1 0 0 io-1.sym
{
T 11000 82900 5 10 1 1 0 0 1
net=FloorPump:1
T 10300 83600 5 10 0 0 0 0 1
device=none
T 10500 83000 5 10 1 1 0 1 1
value=IO
}
C 10100 83700 1 0 0 io-1.sym
{
T 11000 83700 5 10 1 1 0 0 1
net=HidrPump:1
T 10300 84400 5 10 0 0 0 0 1
device=none
T 10500 83800 5 10 1 1 0 1 1
value=IO
}
C 10100 82500 1 0 0 io-1.sym
{
T 11000 82500 5 10 1 1 0 0 1
net=HWPump:1
T 10300 83200 5 10 0 0 0 0 1
device=none
T 10500 82600 5 10 1 1 0 1 1
value=IO
}
C 10100 84100 1 0 0 io-1.sym
{
T 11000 84100 5 10 1 1 0 0 1
net=BasFlValve:1
T 10300 84800 5 10 0 0 0 0 1
device=none
T 10500 84200 5 10 1 1 0 1 1
value=IO
}
C 10100 82100 1 0 0 io-1.sym
{
T 11000 82100 5 10 1 1 0 0 1
net=BasRadValve:1
T 10300 82800 5 10 0 0 0 0 1
device=none
T 10500 82200 5 10 1 1 0 1 1
value=IO
}
N 10200 88500 9400 88500 4
N 11100 88500 11100 89500 4
N 10200 88500 10200 84600 4
N 9400 88100 10000 88100 4
N 10000 88100 10000 84200 4
N 9400 87700 9900 87700 4
N 9900 87700 9900 83800 4
N 9800 83400 9800 87300 4
N 9800 87300 9400 87300 4
N 9400 86900 9700 86900 4
N 9700 86900 9700 83000 4
N 9600 82600 9600 86500 4
N 9600 86500 9400 86500 4
N 5500 85600 5500 87900 4
N 5500 87900 6600 87900 4
N 5500 85600 4700 85600 4
N 4700 85600 4700 83400 4
N 4700 83400 5200 83400 4
C 6900 83600 1 180 0 CONN3.sym
{
T 6800 82100 5 10 0 0 180 0 1
device=CONN3
T 6800 82300 5 10 0 0 180 0 1
footprint=CONN3
T 6800 83900 5 10 1 1 180 0 1
refdes=CONN_OWBUS
}
N 4200 82600 4900 82600 4
N 4900 82600 4900 82800 4
N 4900 82800 5200 82800 4
N 5200 83100 4500 83100 4
N 4500 83100 4500 83800 4
N 4500 83800 4200 83800 4
N 9400 86100 9400 85300 4
N 9400 85300 5200 85300 4
N 5200 85300 5200 88100 4
C 13100 84300 1 0 0 MCP4161.sym
{
T 14700 85700 5 10 1 1 0 6 1
refdes=U4
T 15295 84700 5 10 0 1 0 0 1
footprint=DIP8
T 14995 85200 5 10 0 1 0 0 1
device=MCP4161
}
N 7300 85400 7300 84900 4
N 7300 84900 5900 84900 4
N 5900 84900 5900 90300 4
N 5900 90300 18200 90300 4
N 18200 90300 18200 88100 4
N 16900 88100 18200 88100 4
C 15600 84400 1 0 0 POTMETER.sym
{
T 16400 85300 5 10 0 0 0 0 1
device=VARIABLE_RESISTOR
T 15900 84200 5 10 1 1 0 0 1
refdes=TR2
T 16400 85900 5 10 0 0 0 0 1
footprint=TRIMPOT200
}
C 15300 85700 1 180 1 POTMETER.sym
{
T 16100 84800 5 10 0 0 180 6 1
device=VARIABLE_RESISTOR
T 15900 85300 5 10 1 1 180 6 1
refdes=TR1
T 16100 84200 5 10 0 0 180 6 1
footprint=TRIMPOT200
}
N 15100 85600 15300 85600 4
N 16100 85000 16100 85100 4
N 16100 85100 16500 85100 4
N 16500 85100 16500 84800 4
N 16500 84800 16800 84800 4
C 18500 85300 1 180 0 CONN2.sym
{
T 18300 84300 5 10 0 0 180 0 1
device=CONN2
T 18400 85500 5 10 1 1 180 0 1
refdes=CONN_THERMO
T 17800 84500 5 10 0 0 180 0 1
footprint=CONN2
}
N 16800 85100 16700 85100 4
N 16700 85100 16700 85900 4
N 16700 85900 15100 85900 4
N 6200 84400 7300 84400 4
N 7300 84400 7300 81900 4
N 7300 81900 12500 81900 4
C 12800 82500 1 0 0 BS170.sym
{
T 13700 82900 5 10 0 0 0 0 1
device=BS170
T 13700 83000 5 10 1 1 0 0 1
refdes=Q3
T 13700 82700 5 10 0 0 0 0 1
footprint=TO92
}
N 12500 81900 12500 83000 4
N 12500 83000 12800 83000 4
C 13300 81900 1 0 0 gnd-1.sym
N 13400 82200 13400 82500 4
N 13400 83700 13400 83500 4
C 14200 82600 1 0 0 vcc-1.sym
T 7200 86600 6 10 1 0 0 0 1
Heat
T 8100 86200 6 10 1 0 0 0 1
Comm_dir
T 8100 89000 6 10 1 0 0 0 1
nCS
T 7200 87600 6 10 1 0 0 0 1
Watchdog
T 8000 88600 6 10 1 0 0 0 1
BasRadValve
T 7200 88000 6 10 1 0 0 0 1
OWBus
T 8100 88200 6 10 1 0 0 0 1
BasFlValve
T 8100 86600 6 10 1 0 0 0 1
HWPump
T 8100 87800 6 10 1 0 0 0 1
HidrPump
T 8100 87000 6 10 1 0 0 0 1
FloorPump
T 8100 87400 6 10 1 0 0 0 1
RadPump
T 7200 87100 6 10 1 0 0 0 1
SDI/SDO
T 7200 86200 6 10 1 0 0 0 1
SCK
C 14800 82400 1 0 0 RELAY_G6D.sym
{
T 15600 83550 5 10 0 1 0 0 1
device=G6D_1A
T 15600 83250 5 10 1 0 0 0 1
refdes=U5
T 15700 83500 5 10 0 1 0 0 1
footprint=RELAY_G6D
}
C 18500 84100 1 180 0 CONN2.sym
{
T 18300 83100 5 10 0 0 180 0 1
device=CONN2
T 18500 83300 5 10 1 1 180 0 1
refdes=CONN_HEAT
T 17800 83300 5 10 0 0 180 0 1
footprint=CONN2
}
N 15400 83600 15400 83900 4
N 15400 83900 16800 83900 4
N 15400 82400 15400 82200 4
N 15400 82200 16500 82200 4
N 16500 82200 16500 82900 4
N 16500 82900 16700 82900 4
N 16700 82900 16700 83600 4
N 16700 83600 16800 83600 4
N 10200 84600 9300 84600 4
N 9300 84600 9300 82200 4
N 9300 82200 10100 82200 4
N 6600 86100 6500 86100 4
N 6200 84400 6200 86500 4
N 6200 86500 6600 86500 4
N 6600 87400 6100 87400 4
N 6100 87400 6100 90000 4
N 14400 82600 14400 82200 4
N 14400 82200 14900 82200 4
N 14900 82200 14900 82400 4
N 14900 83600 14900 83700 4
N 14900 83700 13400 83700 4
N 13100 85400 10100 85400 4
N 10100 85400 10100 88900 4
N 13100 85000 10600 85000 4
N 10600 85000 10600 85200 4
N 10600 85200 6500 85200 4
N 6500 85200 6500 86100 4
N 6600 86900 6400 86900 4
N 6400 86900 6400 84500 4
N 6400 84500 12400 84500 4
N 12400 84500 12400 84600 4
N 12400 84600 13100 84600 4
N 15100 84500 15100 85900 4
N 15100 84500 14900 84500 4
N 14900 85000 15800 85000 4
N 15800 85000 15800 85100 4
N 15600 84500 15300 84500 4
N 15300 84500 15300 85000 4
N 10100 82600 9600 82600 4
N 9700 83000 10100 83000 4
N 10100 84200 10000 84200 4
N 10100 83400 9800 83400 4
N 10100 83800 9900 83800 4
