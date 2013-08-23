v 20070216 1
C 40000 40000 0 0 0 title-B.sym
C 49400 46500 1 0 0 output-2.sym
{
T 50300 46700 5 10 1 0 0 0 1
net=PINCEPF:1
T 49600 47200 5 10 0 0 0 0 1
device=OUTPUT
T 50300 46600 5 10 1 1 0 1 1
value=PINCEPF
}
C 45600 50300 1 180 0 ds2405.sym
{
T 44500 49350 5 10 1 1 180 0 1
refdes=U107
T 45400 49350 5 10 1 1 180 0 1
device=DS2405
T 43500 48500 5 10 0 0 180 0 1
footprint=TO92a
}
C 46600 48500 1 0 0 TLP3063.sym
{
T 46900 50500 5 10 0 0 0 0 1
device=TLP3063
T 48400 50300 5 10 1 1 0 6 1
refdes=U207
T 46900 50700 5 10 0 0 0 0 1
footprint=DIL 6 300
}
C 50700 48800 1 90 1 triac.sym
{
T 49800 48500 5 10 0 0 270 2 1
device=TRIAC
T 49800 48700 5 10 1 1 0 2 1
refdes=D107
T 50100 48500 5 10 0 0 270 2 1
footprint=TO220W
}
N 43400 49500 43400 50200 4
N 43400 50200 44100 50200 4
N 43600 48800 43600 49600 4
N 43600 49600 44100 49600 4
N 45600 49900 46000 49900 4
N 46000 49900 46000 48800 4
N 46000 48800 46600 48800 4
C 44100 50500 1 0 0 resistor-1.sym
{
T 44400 50900 5 10 0 0 0 0 1
device=RESISTOR
T 44300 50800 5 10 1 1 0 0 1
refdes=R107
T 44200 50400 5 10 1 0 0 0 1
value=1k
T 44100 50500 5 10 0 0 0 0 1
footprint=ACY400
}
N 43000 50600 44100 50600 4
N 45000 50600 46400 50600 4
N 46400 50600 46400 49900 4
N 46400 49900 46600 49900 4
C 49100 49800 1 0 0 resistor-1.sym
{
T 49400 50200 5 10 0 0 0 0 1
device=RESISTOR
T 49300 50100 5 10 1 1 0 0 1
refdes=R207
T 49200 49600 5 10 1 0 0 0 1
value=100
T 49100 49800 5 10 0 0 0 0 1
footprint=ACY400
}
N 50200 50000 50200 49900 4
N 50200 49900 50000 49900 4
N 49100 49900 48700 49900 4
N 50000 48100 49200 48100 4
N 49200 48100 49200 48800 4
N 49200 48800 48700 48800 4
N 50400 48800 50400 50200 4
N 49000 46600 49400 46600 4
{
T 49000 46600 5 10 0 0 0 0 1
netname=PINCEPF
}
N 49000 46600 49000 47400 4
N 49000 47400 50400 47400 4
N 50400 47400 50400 47900 4
C 44800 45500 1 180 0 ds2405.sym
{
T 43700 44550 5 10 1 1 180 0 1
refdes=U108
T 44600 44550 5 10 1 1 180 0 1
device=DS2405
T 42700 43700 5 10 0 0 180 0 1
footprint=TO92a
}
C 45800 43700 1 0 0 TLP3063.sym
{
T 46100 45700 5 10 0 0 0 0 1
device=TLP3063
T 47600 45500 5 10 1 1 0 6 1
refdes=U208
T 46100 45900 5 10 0 0 0 0 1
footprint=DIL 6 300
}
C 49900 44000 1 90 1 triac.sym
{
T 49000 43700 5 10 0 0 270 2 1
device=TRIAC
T 49000 43900 5 10 1 1 0 2 1
refdes=D108
T 49300 43700 5 10 0 0 270 2 1
footprint=TO220W
}
N 44800 45100 45200 45100 4
N 45200 45100 45200 44000 4
N 45200 44000 45800 44000 4
C 43300 45700 1 0 0 resistor-1.sym
{
T 43600 46100 5 10 0 0 0 0 1
device=RESISTOR
T 43500 46000 5 10 1 1 0 0 1
refdes=R108
T 43400 45600 5 10 1 0 0 0 1
value=1k
T 43300 45700 5 10 0 0 0 0 1
footprint=ACY400
}
N 44200 45800 45600 45800 4
N 45600 45800 45600 45100 4
N 45600 45100 45800 45100 4
C 48300 45000 1 0 0 resistor-1.sym
{
T 48600 45400 5 10 0 0 0 0 1
device=RESISTOR
T 48500 45300 5 10 1 1 0 0 1
refdes=R208
T 48400 44800 5 10 1 0 0 0 1
value=100
T 48300 45000 5 10 0 0 0 0 1
footprint=ACY400
}
N 48300 45100 47900 45100 4
N 49200 43300 48400 43300 4
N 48400 43300 48400 44000 4
N 48400 44000 47900 44000 4
N 43300 44800 42600 44800 4
N 42600 44800 42600 48400 4
N 42600 48400 42700 48400 4
N 42700 48400 42700 48800 4
N 43300 45400 42800 45400 4
N 42800 45400 42800 49500 4
N 43000 45800 43300 45800 4
N 43000 45800 43000 50600 4
N 49200 45100 51800 45100 4
N 51800 45100 51800 49600 4
N 51800 49600 50400 49600 4
N 49600 44000 49600 45100 4
C 50000 42500 1 0 0 output-2.sym
{
T 50900 42700 5 10 1 0 0 0 1
net=PINCERAD:1
T 50200 43200 5 10 0 0 0 0 1
device=OUTPUT
T 50900 42600 5 10 1 1 0 1 1
value=PINCERAD
}
N 49600 43100 49600 42600 4
N 49600 42600 50000 42600 4
{
T 49600 42600 5 10 0 0 0 0 1
netname=PINCERAD
}
T 50300 41000 9 10 1 0 0 0 1
Kazan Vezerles
T 50000 40100 9 10 1 0 0 0 1
2
T 51500 40100 9 10 1 0 0 0 1
2
T 54000 40400 9 10 1 0 0 0 1
1
T 54000 40100 9 10 1 0 0 0 1
Molnar Domokos
C 41100 47900 1 0 0 input-1.sym
{
T 41100 48200 5 10 0 0 0 0 1
device=INPUT
T 39700 48000 5 10 1 0 0 0 1
net=VCC:1
T 40100 47800 5 10 1 0 0 0 1
value=VCC
}
C 41000 47300 1 0 0 input-1.sym
{
T 41000 47600 5 10 0 0 0 0 1
device=INPUT
T 39500 47500 5 10 1 0 0 0 1
net=GND:1
T 39800 47300 5 10 1 0 0 0 1
value=GND
}
C 41000 46500 1 0 0 input-1.sym
{
T 41000 46800 5 10 0 0 0 0 1
device=INPUT
T 39600 46700 5 10 1 0 0 0 1
net=OW:1
T 39900 46500 5 10 1 0 0 0 1
value=OW
}
C 51000 50400 1 0 0 input-1.sym
{
T 51000 50700 5 10 0 0 0 0 1
device=INPUT
T 51700 50700 5 10 1 0 0 0 1
net=SW230:1
T 52000 50400 5 10 1 0 0 0 1
value=SW230
}
N 41900 48000 43000 48000 4
N 41800 47400 42800 47400 4
N 42800 49500 43400 49500 4
N 41800 46600 42600 46600 4
N 42700 48800 43600 48800 4
N 51800 50500 52000 50500 4
N 52000 50500 52000 50200 4
N 52000 50200 50400 50200 4
N 50200 50000 50400 50000 4