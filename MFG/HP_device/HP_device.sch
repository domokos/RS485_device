EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_8051:AT89C4051-24PU U1
U 1 1 6332D986
P 5500 3850
F 0 "U1" H 5500 5131 50  0000 C CNN
F 1 "AT89C4051-24PU" H 5500 5040 50  0000 C CNN
F 2 "Package_DIP:DIP-20_W7.62mm" H 5500 3850 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc1001.pdf" H 5500 3850 50  0001 C CNN
	1    5500 3850
	1    0    0    -1  
$EndComp
Entry Wire Line
	8100 1400 8200 1300
Entry Wire Line
	8100 1600 8200 1500
Entry Wire Line
	8100 2650 8200 2550
Entry Wire Line
	8100 2850 8200 2750
Text Label 8450 1500 2    50   ~ 0
W_SDO
Text Label 8450 1300 2    50   ~ 0
W_SCK
Text Label 8450 2550 2    50   ~ 0
W_SCK
Wire Wire Line
	8450 2300 8850 2300
Wire Wire Line
	9000 3100 8850 3100
$Comp
L power:GND #PWR0104
U 1 1 6334BE5F
P 9000 3100
F 0 "#PWR0104" H 9000 2850 50  0001 C CNN
F 1 "GND" H 8950 2950 50  0000 C CNN
F 2 "" H 9000 3100 50  0001 C CNN
F 3 "" H 9000 3100 50  0001 C CNN
	1    9000 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 1850 8850 1850
Wire Wire Line
	9000 1900 9000 1850
$Comp
L power:GND #PWR0103
U 1 1 6334B404
P 9000 1900
F 0 "#PWR0103" H 9000 1650 50  0001 C CNN
F 1 "GND" H 9005 1727 50  0000 C CNN
F 2 "" H 9000 1900 50  0001 C CNN
F 3 "" H 9000 1900 50  0001 C CNN
	1    9000 1900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 6334A729
P 8450 2300
F 0 "#PWR0102" H 8450 2150 50  0001 C CNN
F 1 "+5V" H 8465 2473 50  0000 C CNN
F 2 "" H 8450 2300 50  0001 C CNN
F 3 "" H 8450 2300 50  0001 C CNN
	1    8450 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 1050 8850 1050
$Comp
L power:+5V #PWR0101
U 1 1 633490D4
P 8450 1050
F 0 "#PWR0101" H 8450 900 50  0001 C CNN
F 1 "+5V" H 8465 1223 50  0000 C CNN
F 2 "" H 8450 1050 50  0001 C CNN
F 3 "" H 8450 1050 50  0001 C CNN
	1    8450 1050
	1    0    0    -1  
$EndComp
$Comp
L RS485Device:MCP4162-104 U2
U 1 1 633335C2
P 8850 1450
F 0 "U2" H 8850 2031 50  0000 C CNN
F 1 "MCP4162-104" H 8850 1940 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 8850 1450 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/11195c.pdf" H 8850 1450 50  0001 C CNN
	1    8850 1450
	1    0    0    -1  
$EndComp
$Comp
L RS485Device:MCP4162-503 U3
U 1 1 63333DB9
P 8850 2700
F 0 "U3" H 8850 3281 50  0000 C CNN
F 1 "MCP4162-503" H 8850 3190 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 8850 2700 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/11195c.pdf" H 8850 2700 50  0001 C CNN
	1    8850 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 6334123E
P 5650 4950
F 0 "#PWR02" H 5650 4700 50  0001 C CNN
F 1 "GND" H 5655 4777 50  0000 C CNN
F 2 "" H 5650 4950 50  0001 C CNN
F 3 "" H 5650 4950 50  0001 C CNN
	1    5650 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 4950 5500 4950
$Comp
L power:+5V #PWR01
U 1 1 633425AC
P 4800 2700
F 0 "#PWR01" H 4800 2550 50  0001 C CNN
F 1 "+5V" H 4815 2873 50  0000 C CNN
F 2 "" H 4800 2700 50  0001 C CNN
F 3 "" H 4800 2700 50  0001 C CNN
	1    4800 2700
	1    0    0    -1  
$EndComp
Text Notes 10650 600  0    50   ~ 0
OUTPUTS
Wire Wire Line
	8200 2550 8450 2550
Wire Wire Line
	8200 2750 8450 2750
Text Label 8450 2750 2    50   ~ 0
W_SDO
Wire Wire Line
	8200 1300 8450 1300
Wire Wire Line
	8200 1500 8450 1500
Text Notes 1800 5650 2    50   ~ 0
EHS\n230V
Text Notes 1800 6100 2    50   ~ 0
Heating/Cooling mode\n230V
Text Notes 1800 6700 2    50   ~ 0
Electric heater for DHW\nor Backup heater\n230V
Text Notes 1750 7150 2    50   ~ 0
Alarm\n230V
$Comp
L 74xx:74HC595 U4
U 1 1 6336BCEC
P 7250 4100
F 0 "U4" H 7250 4881 50  0000 C CNN
F 1 "74HC595" H 7250 4790 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm" H 7250 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 7250 4100 50  0001 C CNN
	1    7250 4100
	1    0    0    -1  
$EndComp
Entry Wire Line
	6300 3450 6400 3550
Entry Wire Line
	6300 3550 6400 3650
Entry Wire Line
	6300 3650 6400 3750
Entry Wire Line
	6300 3750 6400 3850
Entry Wire Line
	6550 4000 6650 3900
Entry Wire Line
	6550 4300 6650 4200
Entry Wire Line
	6550 4100 6650 4000
Entry Wire Line
	6550 4400 6650 4300
Wire Wire Line
	6650 3700 6850 3700
Wire Wire Line
	6850 3900 6650 3900
Wire Wire Line
	6650 4000 6850 4000
Wire Wire Line
	6850 4200 6650 4200
Wire Wire Line
	6650 4300 6850 4300
$Comp
L power:GND #PWR06
U 1 1 6337C352
P 7350 4850
F 0 "#PWR06" H 7350 4600 50  0001 C CNN
F 1 "GND" H 7355 4677 50  0000 C CNN
F 2 "" H 7350 4850 50  0001 C CNN
F 3 "" H 7350 4850 50  0001 C CNN
	1    7350 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 4850 7350 4800
Wire Wire Line
	7350 4800 7250 4800
$Comp
L power:+5V #PWR03
U 1 1 6337DCF3
P 7000 3500
F 0 "#PWR03" H 7000 3350 50  0001 C CNN
F 1 "+5V" H 7015 3673 50  0000 C CNN
F 2 "" H 7000 3500 50  0001 C CNN
F 3 "" H 7000 3500 50  0001 C CNN
	1    7000 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3500 7250 3500
NoConn ~ 7650 4600
Text Label 6650 4200 0    50   ~ 0
STCP
Text Label 6650 4300 0    50   ~ 0
NOE
Text Label 6650 4000 0    50   ~ 0
NMR
Text Label 6650 3900 0    50   ~ 0
SHCP
Text Label 6100 3450 0    50   ~ 0
SHCP
Text Label 6100 3550 0    50   ~ 0
NMR
Text Label 6100 3750 0    50   ~ 0
NOE
Text Label 6100 3650 0    50   ~ 0
STCP
Wire Wire Line
	6650 3700 6650 3050
Wire Wire Line
	6100 3050 6650 3050
Wire Wire Line
	8450 2650 8150 2650
Wire Wire Line
	8150 2650 8150 2700
Wire Wire Line
	8150 2700 8000 2700
Wire Wire Line
	8000 2700 8000 3050
Text Label 6100 3050 0    50   ~ 0
SERDATA
Wire Wire Line
	6100 3350 6350 3350
Wire Wire Line
	6100 3250 6350 3250
Entry Wire Line
	6350 3250 6450 3350
Text Label 6100 3250 0    50   ~ 0
W_SCK
Text Label 6100 3350 0    50   ~ 0
W_SDO
Entry Wire Line
	6350 3350 6450 3450
$Comp
L Transistor_FET:BS170 Q1
U 1 1 6339C61A
P 7450 1900
F 0 "Q1" H 7654 1946 50  0000 L CNN
F 1 "BS170" H 7654 1855 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 7650 1825 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BS170-D.PDF" H 7450 1900 50  0001 L CNN
	1    7450 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R1
U 1 1 6339DC9F
P 7550 1500
F 0 "R1" H 7609 1546 50  0000 L CNN
F 1 "4.7k" H 7609 1455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 7550 1500 50  0001 C CNN
F 3 "~" H 7550 1500 50  0001 C CNN
	1    7550 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 1600 7550 1650
$Comp
L power:+5V #PWR04
U 1 1 6339F73D
P 7500 1350
F 0 "#PWR04" H 7500 1200 50  0001 C CNN
F 1 "+5V" H 7515 1523 50  0000 C CNN
F 2 "" H 7500 1350 50  0001 C CNN
F 3 "" H 7500 1350 50  0001 C CNN
	1    7500 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 633A1C26
P 7600 2150
F 0 "#PWR05" H 7600 1900 50  0001 C CNN
F 1 "GND" H 7605 1977 50  0000 C CNN
F 2 "" H 7600 2150 50  0001 C CNN
F 3 "" H 7600 2150 50  0001 C CNN
	1    7600 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 1600 8300 1600
Wire Wire Line
	8300 1600 8300 1650
Wire Wire Line
	8300 1650 7550 1650
Connection ~ 7550 1650
Wire Wire Line
	7550 1650 7550 1700
Wire Wire Line
	8050 3150 8050 2950
Wire Wire Line
	8050 2950 8300 2950
Wire Wire Line
	8300 2950 8300 2850
Wire Wire Line
	8300 2850 8450 2850
Wire Bus Line
	6450 3250 8100 3250
Text Label 6100 3150 0    50   ~ 0
W_CSEL
Wire Wire Line
	7250 1900 7200 1900
Wire Wire Line
	7200 1900 7200 3150
Connection ~ 7200 3150
Wire Wire Line
	7200 3150 8050 3150
$Comp
L Relay:G5V-1 K1
U 1 1 633B04EE
P 9350 3850
F 0 "K1" H 9780 3896 50  0000 L CNN
F 1 "G5V-1" H 9780 3805 50  0000 L CNN
F 2 "Relay_THT:Relay_SPDT_Omron_G5V-1" H 10480 3820 50  0001 C CNN
F 3 "http://omronfs.omron.com/en_US/ecb/products/pdf/en-g5v_1.pdf" H 9350 3850 50  0001 C CNN
	1    9350 3850
	1    0    0    -1  
$EndComp
$Comp
L Relay:G5V-1 K3
U 1 1 633B1AE6
P 9350 4950
F 0 "K3" H 9780 4996 50  0000 L CNN
F 1 "G5V-1" H 9780 4905 50  0000 L CNN
F 2 "Relay_THT:Relay_SPDT_Omron_G5V-1" H 10480 4920 50  0001 C CNN
F 3 "http://omronfs.omron.com/en_US/ecb/products/pdf/en-g5v_1.pdf" H 9350 4950 50  0001 C CNN
	1    9350 4950
	1    0    0    -1  
$EndComp
$Comp
L Relay:G5V-1 K5
U 1 1 633B3D09
P 9350 6000
F 0 "K5" H 9780 6046 50  0000 L CNN
F 1 "G5V-1" H 9780 5955 50  0000 L CNN
F 2 "Relay_THT:Relay_SPDT_Omron_G5V-1" H 10480 5970 50  0001 C CNN
F 3 "http://omronfs.omron.com/en_US/ecb/products/pdf/en-g5v_1.pdf" H 9350 6000 50  0001 C CNN
	1    9350 6000
	1    0    0    -1  
$EndComp
$Comp
L Relay:G5V-1 K2
U 1 1 633B5504
P 10450 3850
F 0 "K2" H 10880 3896 50  0000 L CNN
F 1 "G5V-1" H 10850 3800 50  0000 L CNN
F 2 "Relay_THT:Relay_SPDT_Omron_G5V-1" H 11580 3820 50  0001 C CNN
F 3 "http://omronfs.omron.com/en_US/ecb/products/pdf/en-g5v_1.pdf" H 10450 3850 50  0001 C CNN
	1    10450 3850
	1    0    0    -1  
$EndComp
$Comp
L Relay:G5V-1 K6
U 1 1 633B719F
P 10450 6000
F 0 "K6" H 10880 6046 50  0000 L CNN
F 1 "G5V-1" H 10850 5950 50  0000 L CNN
F 2 "Relay_THT:Relay_SPDT_Omron_G5V-1" H 11580 5970 50  0001 C CNN
F 3 "http://omronfs.omron.com/en_US/ecb/products/pdf/en-g5v_1.pdf" H 10450 6000 50  0001 C CNN
	1    10450 6000
	1    0    0    -1  
$EndComp
$Comp
L Relay:G5V-1 K4
U 1 1 633B8DA0
P 10450 4950
F 0 "K4" H 10880 4996 50  0000 L CNN
F 1 "G5V-1" H 10850 4900 50  0000 L CNN
F 2 "Relay_THT:Relay_SPDT_Omron_G5V-1" H 11580 4920 50  0001 C CNN
F 3 "http://omronfs.omron.com/en_US/ecb/products/pdf/en-g5v_1.pdf" H 10450 4950 50  0001 C CNN
	1    10450 4950
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BS170 Q2
U 1 1 633CC5F9
P 8450 3400
F 0 "Q2" H 8300 3500 50  0000 L CNN
F 1 "BS170" H 8200 3300 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8650 3325 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BS170-D.PDF" H 8450 3400 50  0001 L CNN
	1    8450 3400
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BS170 Q3
U 1 1 633CE9B1
P 8450 3950
F 0 "Q3" H 8300 4050 50  0000 L CNN
F 1 "BS170" H 8200 3850 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8650 3875 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BS170-D.PDF" H 8450 3950 50  0001 L CNN
	1    8450 3950
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BS170 Q4
U 1 1 633D05D3
P 8450 4500
F 0 "Q4" H 8300 4600 50  0000 L CNN
F 1 "BS170" H 8200 4400 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8650 4425 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BS170-D.PDF" H 8450 4500 50  0001 L CNN
	1    8450 4500
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BS170 Q6
U 1 1 633D265A
P 8450 5600
F 0 "Q6" H 8300 5700 50  0000 L CNN
F 1 "BS170" H 8200 5500 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8650 5525 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BS170-D.PDF" H 8450 5600 50  0001 L CNN
	1    8450 5600
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BS170 Q5
U 1 1 633D4569
P 8450 5050
F 0 "Q5" H 8300 5150 50  0000 L CNN
F 1 "BS170" H 8200 4950 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8650 4975 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BS170-D.PDF" H 8450 5050 50  0001 L CNN
	1    8450 5050
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BS170 Q7
U 1 1 633D624B
P 8450 6150
F 0 "Q7" H 8300 6250 50  0000 L CNN
F 1 "BS170" H 8200 6050 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8650 6075 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BS170-D.PDF" H 8450 6150 50  0001 L CNN
	1    8450 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1500 9300 1500
Wire Wire Line
	9350 2650 9250 2650
Wire Wire Line
	9400 2750 9250 2750
$Comp
L power:+5V #PWR0105
U 1 1 6345E991
P 9950 5550
F 0 "#PWR0105" H 9950 5400 50  0001 C CNN
F 1 "+5V" H 9800 5600 50  0000 C CNN
F 2 "" H 9950 5550 50  0001 C CNN
F 3 "" H 9950 5550 50  0001 C CNN
	1    9950 5550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0106
U 1 1 63464FB1
P 9550 4450
F 0 "#PWR0106" H 9550 4300 50  0001 C CNN
F 1 "+5V" H 9400 4500 50  0000 C CNN
F 2 "" H 9550 4450 50  0001 C CNN
F 3 "" H 9550 4450 50  0001 C CNN
	1    9550 4450
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0107
U 1 1 634687BB
P 9350 3350
F 0 "#PWR0107" H 9350 3200 50  0001 C CNN
F 1 "+5V" H 9200 3400 50  0000 C CNN
F 2 "" H 9350 3350 50  0001 C CNN
F 3 "" H 9350 3350 50  0001 C CNN
	1    9350 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 6348778A
P 8550 6400
F 0 "#PWR0108" H 8550 6150 50  0001 C CNN
F 1 "GND" H 8400 6350 50  0000 C CNN
F 2 "" H 8550 6400 50  0001 C CNN
F 3 "" H 8550 6400 50  0001 C CNN
	1    8550 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 4500 9150 4650
Entry Wire Line
	8600 3200 8700 3300
Entry Wire Line
	8600 3750 8700 3850
Entry Wire Line
	8600 4300 8700 4400
Entry Wire Line
	8600 4850 8700 4950
Entry Wire Line
	8600 5400 8700 5500
Entry Wire Line
	8600 5950 8700 6050
Entry Wire Line
	9400 6350 9500 6450
Entry Wire Line
	10500 6350 10600 6450
Entry Wire Line
	10550 5300 10650 5400
Entry Wire Line
	9450 5300 9550 5400
Entry Wire Line
	9400 4200 9500 4300
Entry Wire Line
	10500 4200 10600 4300
Connection ~ 8700 4300
Wire Wire Line
	8600 3200 8550 3200
Wire Wire Line
	8600 3750 8550 3750
Wire Wire Line
	8600 4300 8550 4300
Wire Wire Line
	8600 4850 8550 4850
Wire Wire Line
	8600 5400 8550 5400
Wire Wire Line
	8600 5950 8550 5950
Wire Wire Line
	9150 5250 9150 5300
Connection ~ 9950 5550
Wire Wire Line
	9950 5550 10250 5550
Connection ~ 8700 5400
Wire Wire Line
	9150 6300 9150 6350
Wire Wire Line
	9150 4500 9550 4500
Wire Wire Line
	9550 4450 9550 4500
Connection ~ 9550 4500
Wire Wire Line
	9550 4500 10250 4500
Entry Wire Line
	10950 5700 11050 5600
Entry Wire Line
	9900 5700 10000 5600
Entry Wire Line
	9900 4650 10000 4550
Entry Wire Line
	11000 4650 11100 4550
Entry Wire Line
	11000 3550 11100 3450
Entry Wire Line
	9950 3550 10050 3450
Wire Wire Line
	9150 3400 9350 3400
Wire Wire Line
	9150 3400 9150 3550
Wire Wire Line
	9350 3350 9350 3400
Wire Wire Line
	9150 5550 9950 5550
Connection ~ 9350 3400
Wire Wire Line
	9350 3400 10250 3400
Wire Wire Line
	9150 5550 9150 5700
Entry Wire Line
	9700 2950 9800 2850
Entry Wire Line
	9700 2850 9800 2750
Entry Wire Line
	9700 2650 9800 2550
Entry Wire Line
	9700 2550 9800 2450
Entry Wire Line
	9700 2350 9800 2250
Entry Wire Line
	9700 2250 9800 2150
Entry Wire Line
	9700 1950 9800 1850
Entry Wire Line
	9700 1750 9800 1650
Entry Wire Line
	9700 1650 9800 1550
Entry Wire Line
	9700 1450 9800 1350
Entry Wire Line
	9700 1350 9800 1250
Entry Wire Line
	8050 3500 8150 3400
Entry Wire Line
	8050 4050 8150 3950
Entry Wire Line
	8050 4600 8150 4500
Entry Wire Line
	8050 5150 8150 5050
Entry Wire Line
	8050 5700 8150 5600
Entry Wire Line
	8050 6250 8150 6150
NoConn ~ 7650 4400
NoConn ~ 7650 3700
Entry Wire Line
	9700 2050 9800 1950
Wire Wire Line
	9800 2750 10000 2750
Text Label 10000 2750 2    50   ~ 0
LTA
Wire Wire Line
	9950 3550 9650 3550
Text Label 9650 3550 0    50   ~ 0
DHWA
Text Label 9550 4150 0    50   ~ 0
DHWB
Text Label 10750 3550 0    50   ~ 0
ONA
Text Label 10650 4150 0    50   ~ 0
ONB
Entry Wire Line
	10900 4150 11000 4250
Entry Wire Line
	9800 4150 9900 4250
Wire Wire Line
	9550 4150 9800 4150
Text Label 9650 4650 0    50   ~ 0
DSETA
NoConn ~ 9450 4650
NoConn ~ 9450 3550
NoConn ~ 10550 3550
Wire Wire Line
	9650 4650 9900 4650
Connection ~ 11150 4550
Wire Bus Line
	11150 4550 11150 5350
Text Label 9550 5250 0    50   ~ 0
DSETB
Wire Wire Line
	9550 5250 9800 5250
Entry Wire Line
	9800 5250 9900 5350
Connection ~ 11150 5350
Wire Bus Line
	11150 5350 11150 5600
NoConn ~ 10550 4650
Text Label 10750 4650 0    50   ~ 0
OPMA
Text Label 10650 5250 0    50   ~ 0
OPMB
Entry Wire Line
	10950 5250 11050 5350
NoConn ~ 9450 5700
Text Label 9650 5700 0    50   ~ 0
NOCA
Wire Wire Line
	9900 5700 9650 5700
Text Label 9550 6300 0    50   ~ 0
NOCB
Wire Wire Line
	9550 6300 9800 6300
Entry Wire Line
	9800 6300 9900 6400
Wire Bus Line
	11150 6400 11150 5600
Connection ~ 11150 5600
NoConn ~ 10550 5700
Text Label 10750 5700 0    50   ~ 0
LTA
Text Label 10650 6300 0    50   ~ 0
LTB
Entry Wire Line
	10850 6300 10950 6400
Wire Bus Line
	11150 4250 11150 4550
Wire Bus Line
	11150 4250 11150 3450
Connection ~ 11150 4250
Text Label 9150 4200 0    50   ~ 0
C_DHW
Wire Wire Line
	9150 4150 9150 4200
Wire Wire Line
	9400 4200 9150 4200
Text Label 10250 4200 0    50   ~ 0
C_ON
Text Label 9150 5300 0    50   ~ 0
C_DSET
Wire Wire Line
	9150 5300 9450 5300
Text Label 10250 5300 0    50   ~ 0
C_OPM
Text Label 9150 6350 0    50   ~ 0
C_NOC
Wire Wire Line
	9150 6350 9400 6350
Text Label 10250 6350 0    50   ~ 0
C_LT
Text Label 8550 3200 2    50   ~ 0
C_DHW
Text Label 8550 3750 2    50   ~ 0
C_ON
Wire Wire Line
	8800 6350 8800 5800
Wire Wire Line
	8550 3600 8800 3600
Wire Wire Line
	8550 4150 8800 4150
Connection ~ 8800 4150
Wire Wire Line
	8800 4150 8800 3600
Wire Wire Line
	8550 4700 8800 4700
Connection ~ 8800 4700
Wire Wire Line
	8800 4700 8800 4150
Wire Wire Line
	8550 5250 8800 5250
Connection ~ 8800 5250
Wire Wire Line
	8800 5250 8800 4700
Wire Wire Line
	8550 5800 8800 5800
Connection ~ 8800 5800
Wire Wire Line
	8800 5800 8800 5250
Wire Wire Line
	8550 6350 8550 6400
Connection ~ 8550 6350
Wire Wire Line
	8550 6350 8800 6350
Text Label 8550 4850 2    50   ~ 0
C_OPM
Text Label 8550 5400 2    50   ~ 0
C_NOC
Text Label 8550 5950 2    50   ~ 0
C_LT
Text Label 7650 3800 0    50   ~ 0
DHW
Text Label 7650 3900 0    50   ~ 0
ON
Text Label 7650 4000 0    50   ~ 0
DSET
Wire Bus Line
	7950 3450 8050 3450
Text Label 7650 4100 0    50   ~ 0
OPM
Text Label 7650 4200 0    50   ~ 0
NOC
Text Label 7650 4300 0    50   ~ 0
LT
Text Label 8250 3400 2    50   ~ 0
DHW
Text Label 8250 3950 2    50   ~ 0
ON
Entry Wire Line
	7850 4300 7950 4400
Entry Wire Line
	7850 4200 7950 4300
Entry Wire Line
	7850 4100 7950 4200
Entry Wire Line
	7850 4000 7950 4100
Entry Wire Line
	7850 3900 7950 4000
Entry Wire Line
	7850 3800 7950 3900
Connection ~ 6650 3050
Wire Wire Line
	6650 3050 8000 3050
Wire Wire Line
	7650 3800 7850 3800
Wire Wire Line
	7850 3900 7650 3900
Wire Wire Line
	7650 4000 7850 4000
Wire Wire Line
	7850 4100 7650 4100
Wire Wire Line
	7650 4200 7850 4200
Wire Wire Line
	7850 4300 7650 4300
Wire Wire Line
	8150 3400 8250 3400
Wire Wire Line
	8150 3950 8250 3950
Wire Wire Line
	8150 4500 8250 4500
Text Label 8250 4500 2    50   ~ 0
DSET
Wire Wire Line
	8150 5050 8250 5050
Text Label 8250 5050 2    50   ~ 0
OPM
Wire Wire Line
	8150 5600 8250 5600
Text Label 8250 5600 2    50   ~ 0
NOC
Wire Wire Line
	8150 6150 8250 6150
Text Label 8250 6150 2    50   ~ 0
LT
Text GLabel 6250 3950 2    50   Input ~ 0
RXD
Text GLabel 6250 4050 2    50   Input ~ 0
TXD
Wire Wire Line
	7550 1350 7550 1400
Wire Wire Line
	7600 2150 7550 2150
Wire Wire Line
	7550 2150 7550 2100
Wire Wire Line
	7550 1350 7500 1350
Wire Wire Line
	10650 6300 10850 6300
Wire Wire Line
	10250 6350 10250 6300
Wire Wire Line
	10250 6350 10500 6350
Wire Wire Line
	10750 5700 10950 5700
Wire Wire Line
	10250 5700 10250 5550
Wire Wire Line
	10250 5300 10250 5250
Wire Wire Line
	10250 5300 10550 5300
Wire Wire Line
	10650 5250 10950 5250
Wire Wire Line
	10750 4650 11000 4650
Wire Wire Line
	10250 4650 10250 4500
Wire Wire Line
	10650 4150 10900 4150
Wire Wire Line
	10250 4200 10500 4200
Wire Wire Line
	10250 4150 10250 4200
Wire Wire Line
	10250 3550 10250 3400
Wire Wire Line
	10750 3550 11000 3550
Wire Wire Line
	4900 3050 4850 3050
Wire Wire Line
	4850 3050 4850 3000
Wire Wire Line
	4800 2700 4800 2750
Wire Wire Line
	8150 1400 8150 1450
Wire Wire Line
	8150 1450 8000 1450
Wire Wire Line
	8000 1450 8000 2700
Wire Wire Line
	8150 1400 8450 1400
Connection ~ 8000 2700
Text Label 8550 4300 2    50   ~ 0
C_DSET
Wire Wire Line
	6300 3450 6100 3450
Wire Wire Line
	6100 3550 6300 3550
Wire Wire Line
	6300 3650 6100 3650
Wire Wire Line
	6100 3750 6300 3750
Wire Bus Line
	6400 3550 6550 3550
$Comp
L Isolator:4N25 U6
U 1 1 63346F5F
P 3200 5500
F 0 "U6" H 3200 5825 50  0000 C CNN
F 1 "4N25" H 3200 5734 50  0000 C CNN
F 2 "Package_DIP:DIP-6_W7.62mm" H 3000 5300 50  0001 L CIN
F 3 "https://www.vishay.com/docs/83725/4n25.pdf" H 3200 5500 50  0001 L CNN
	1    3200 5500
	1    0    0    -1  
$EndComp
$Comp
L Isolator:4N25 U7
U 1 1 6335D866
P 3200 6050
F 0 "U7" H 3200 6375 50  0000 C CNN
F 1 "4N25" H 3200 6284 50  0000 C CNN
F 2 "Package_DIP:DIP-6_W7.62mm" H 3000 5850 50  0001 L CIN
F 3 "https://www.vishay.com/docs/83725/4n25.pdf" H 3200 6050 50  0001 L CNN
	1    3200 6050
	1    0    0    -1  
$EndComp
$Comp
L Isolator:4N25 U8
U 1 1 63366AEB
P 3200 6600
F 0 "U8" H 3200 6925 50  0000 C CNN
F 1 "4N25" H 3200 6834 50  0000 C CNN
F 2 "Package_DIP:DIP-6_W7.62mm" H 3000 6400 50  0001 L CIN
F 3 "https://www.vishay.com/docs/83725/4n25.pdf" H 3200 6600 50  0001 L CNN
	1    3200 6600
	1    0    0    -1  
$EndComp
$Comp
L Isolator:4N25 U9
U 1 1 63370212
P 3200 7150
F 0 "U9" H 3200 7475 50  0000 C CNN
F 1 "4N25" H 3200 7384 50  0000 C CNN
F 2 "Package_DIP:DIP-6_W7.62mm" H 3000 6950 50  0001 L CIN
F 3 "https://www.vishay.com/docs/83725/4n25.pdf" H 3200 7150 50  0001 L CNN
	1    3200 7150
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R2
U 1 1 63348037
P 2700 5400
F 0 "R2" V 2504 5400 50  0000 C CNN
F 1 "200k" V 2595 5400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P15.24mm_Horizontal" H 2700 5400 50  0001 C CNN
F 3 "~" H 2700 5400 50  0001 C CNN
	1    2700 5400
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R3
U 1 1 6335171B
P 2700 5950
F 0 "R3" V 2504 5950 50  0000 C CNN
F 1 "200k" V 2595 5950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P15.24mm_Horizontal" H 2700 5950 50  0001 C CNN
F 3 "~" H 2700 5950 50  0001 C CNN
	1    2700 5950
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R4
U 1 1 6335A170
P 2700 6500
F 0 "R4" V 2504 6500 50  0000 C CNN
F 1 "200k" V 2595 6500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P15.24mm_Horizontal" H 2700 6500 50  0001 C CNN
F 3 "~" H 2700 6500 50  0001 C CNN
	1    2700 6500
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R5
U 1 1 63362B6A
P 2700 7050
F 0 "R5" V 2504 7050 50  0000 C CNN
F 1 "200K" V 2595 7050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P15.24mm_Horizontal" H 2700 7050 50  0001 C CNN
F 3 "~" H 2700 7050 50  0001 C CNN
	1    2700 7050
	0    1    1    0   
$EndComp
Wire Wire Line
	2350 5500 2350 5400
Wire Wire Line
	2350 5400 2600 5400
Wire Wire Line
	2800 5400 2900 5400
Wire Wire Line
	2650 6100 2650 6150
Wire Wire Line
	2650 6150 2900 6150
Wire Wire Line
	2900 5950 2800 5950
Wire Wire Line
	2800 6500 2900 6500
Wire Wire Line
	2800 7050 2900 7050
Wire Wire Line
	2900 7250 2250 7250
$Comp
L power:+5V #PWR0109
U 1 1 6343E8B2
P 3600 4750
F 0 "#PWR0109" H 3600 4600 50  0001 C CNN
F 1 "+5V" H 3615 4923 50  0000 C CNN
F 2 "" H 3600 4750 50  0001 C CNN
F 3 "" H 3600 4750 50  0001 C CNN
	1    3600 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 4750 3650 4750
Wire Wire Line
	3650 4750 3650 4800
Wire Wire Line
	3650 5200 3650 5500
Wire Wire Line
	3650 5500 3500 5500
NoConn ~ 3500 5400
NoConn ~ 3500 5950
NoConn ~ 3500 6500
NoConn ~ 3500 7050
Wire Wire Line
	3750 5200 3750 6050
Wire Wire Line
	3750 6050 3500 6050
Wire Wire Line
	3850 5200 3850 6600
Wire Wire Line
	3850 6600 3500 6600
Wire Wire Line
	3950 5200 3950 7150
Wire Wire Line
	3950 7150 3500 7150
Wire Wire Line
	3500 5600 3600 5600
Wire Wire Line
	3600 5600 3600 6150
Wire Wire Line
	3500 7250 3600 7250
Connection ~ 3600 7250
Wire Wire Line
	3600 7250 3600 7400
Wire Wire Line
	3500 6700 3600 6700
Connection ~ 3600 6700
Wire Wire Line
	3600 6700 3600 7250
Wire Wire Line
	3500 6150 3600 6150
Connection ~ 3600 6150
Wire Wire Line
	3600 6150 3600 6700
$Comp
L power:GND #PWR0110
U 1 1 634FCDC3
P 3600 7400
F 0 "#PWR0110" H 3600 7150 50  0001 C CNN
F 1 "GND" H 3605 7227 50  0000 C CNN
F 2 "" H 3600 7400 50  0001 C CNN
F 3 "" H 3600 7400 50  0001 C CNN
	1    3600 7400
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Network04 RN1
U 1 1 6343B03E
P 3850 5000
F 0 "RN1" H 4038 5046 50  0000 L CNN
F 1 "4.7k" H 4038 4955 50  0000 L CNN
F 2 "Resistor_THT:R_Array_SIP5" V 4125 5000 50  0001 C CNN
F 3 "http://www.vishay.com/docs/31509/csc.pdf" H 3850 5000 50  0001 C CNN
	1    3850 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R6
U 1 1 63511A1D
P 6650 2800
F 0 "R6" H 6709 2846 50  0000 L CNN
F 1 "4.7k" H 6709 2755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 6650 2800 50  0001 C CNN
F 3 "~" H 6650 2800 50  0001 C CNN
	1    6650 2800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0111
U 1 1 63511A24
P 6650 2500
F 0 "#PWR0111" H 6650 2350 50  0001 C CNN
F 1 "+5V" H 6665 2673 50  0000 C CNN
F 2 "" H 6650 2500 50  0001 C CNN
F 3 "" H 6650 2500 50  0001 C CNN
	1    6650 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R7
U 1 1 63520FEB
P 6950 2800
F 0 "R7" H 7009 2846 50  0000 L CNN
F 1 "4.7k" H 7009 2755 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 6950 2800 50  0001 C CNN
F 3 "~" H 6950 2800 50  0001 C CNN
	1    6950 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 2600 6650 2600
Wire Wire Line
	6950 2600 6950 2700
Wire Wire Line
	6650 2500 6650 2600
Connection ~ 6650 2600
Wire Wire Line
	6650 2600 6650 2700
Wire Wire Line
	6650 2900 6650 3050
Wire Wire Line
	6100 3150 6950 3150
Wire Wire Line
	6950 2900 6950 3150
Connection ~ 6950 3150
Wire Wire Line
	6950 3150 7200 3150
Entry Wire Line
	4000 7150 4100 7250
Wire Wire Line
	4000 7150 3950 7150
Connection ~ 3950 7150
Entry Wire Line
	4000 6600 4100 6700
Entry Wire Line
	4000 6050 4100 6150
Entry Wire Line
	4000 5500 4100 5600
Wire Wire Line
	4000 5500 3650 5500
Connection ~ 3650 5500
Wire Wire Line
	4000 6050 3750 6050
Connection ~ 3750 6050
Wire Wire Line
	4000 6600 3850 6600
Connection ~ 3850 6600
Entry Wire Line
	6350 4250 6450 4350
Entry Wire Line
	6350 4350 6450 4450
Entry Wire Line
	6350 4450 6450 4550
Entry Wire Line
	6350 4550 6450 4650
$Comp
L Connector:Screw_Terminal_01x02 J13
U 1 1 638934B6
P 10200 2450
F 0 "J13" H 10150 2550 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10280 2351 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10200 2450 50  0001 C CNN
F 3 "~" H 10200 2450 50  0001 C CNN
	1    10200 2450
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J12
U 1 1 6388182E
P 10200 2150
F 0 "J12" H 10150 2250 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10280 2051 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10200 2150 50  0001 C CNN
F 3 "~" H 10200 2150 50  0001 C CNN
	1    10200 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 950  9350 950 
$Comp
L Connector:Screw_Terminal_01x02 J11
U 1 1 637CD3E4
P 10200 1850
F 0 "J11" H 10150 1950 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10280 1751 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10200 1850 50  0001 C CNN
F 3 "~" H 10200 1850 50  0001 C CNN
	1    10200 1850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J10
U 1 1 637BAFD1
P 10200 1550
F 0 "J10" H 10150 1650 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10280 1451 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10200 1550 50  0001 C CNN
F 3 "~" H 10200 1550 50  0001 C CNN
	1    10200 1550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J9
U 1 1 637A8B98
P 10200 1250
F 0 "J9" H 10150 1350 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10280 1151 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10200 1250 50  0001 C CNN
F 3 "~" H 10200 1250 50  0001 C CNN
	1    10200 1250
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J8
U 1 1 63796BB5
P 10200 950
F 0 "J8" H 10150 1050 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10280 851 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10200 950 50  0001 C CNN
F 3 "~" H 10200 950 50  0001 C CNN
	1    10200 950 
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J7
U 1 1 6376F002
P 10200 650
F 0 "J7" H 10150 750 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10280 551 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10200 650 50  0001 C CNN
F 3 "~" H 10200 650 50  0001 C CNN
	1    10200 650 
	1    0    0    -1  
$EndComp
Text Label 10000 2850 2    50   ~ 0
LTB
Text Label 10000 2550 2    50   ~ 0
NOCB
Text Label 10000 2450 2    50   ~ 0
NOCA
Wire Wire Line
	9800 2550 10000 2550
Wire Wire Line
	9800 2450 10000 2450
Text Label 10000 2250 2    50   ~ 0
OPMB
Text Label 10000 2150 2    50   ~ 0
OPMA
Wire Wire Line
	9800 2250 10000 2250
Wire Wire Line
	9800 2150 10000 2150
Text Label 10000 1950 2    50   ~ 0
DSETB
Wire Wire Line
	9800 1950 10000 1950
Text Label 10000 1850 2    50   ~ 0
DSETA
Wire Wire Line
	9800 1850 10000 1850
Text Label 10000 1650 2    50   ~ 0
ONB
Text Label 10000 1550 2    50   ~ 0
ONA
Wire Wire Line
	9800 1650 10000 1650
Wire Wire Line
	9800 1550 10000 1550
Wire Wire Line
	9800 1350 10000 1350
Wire Wire Line
	9800 1250 10000 1250
Text Label 10000 1350 2    50   ~ 0
DHWB
Text Label 10000 1250 2    50   ~ 0
DHWA
Wire Wire Line
	9400 1050 10000 1050
Wire Wire Line
	9300 750  10000 750 
Wire Wire Line
	9250 650  10000 650 
Text Notes 10350 2850 0    50   ~ 0
Low tariff contact\nRelay switch
Text Notes 10350 2550 0    50   ~ 0
Night mode contact\nRelay switch
Text Notes 10350 2250 0    50   ~ 0
Heating/Cooling mode\nRelay switch
Text Notes 10350 1950 0    50   ~ 0
Dual set point control\nRelay switch
Text Notes 10350 1650 0    50   ~ 0
On/Off remote contact\nRelay switch
Text Notes 10350 1350 0    50   ~ 0
DHW remote contact\nRelay switch
Text Notes 10350 1050 0    50   ~ 0
DHW temp sensor\nNTC 50k
Text Notes 10350 750  0    50   ~ 0
Outdoor temp sensor \nNTC  100k
$Comp
L Connector:Screw_Terminal_01x02 J14
U 1 1 63937581
P 10200 2750
F 0 "J14" H 10150 2850 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10280 2651 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10200 2750 50  0001 C CNN
F 3 "~" H 10200 2750 50  0001 C CNN
	1    10200 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 2850 10000 2850
Wire Wire Line
	9250 650  9250 1400
Wire Wire Line
	9300 750  9300 1500
Wire Wire Line
	9350 950  9350 2650
Wire Wire Line
	9400 1050 9400 2750
$Comp
L Connector:Screw_Terminal_01x02 J3
U 1 1 63A2CFAC
P 1950 5600
F 0 "J3" H 1900 5700 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 2030 5501 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 1950 5600 50  0001 C CNN
F 3 "~" H 1950 5600 50  0001 C CNN
	1    1950 5600
	-1   0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J4
U 1 1 63A2CFB2
P 1950 6100
F 0 "J4" H 1900 6200 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 2030 6001 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 1950 6100 50  0001 C CNN
F 3 "~" H 1950 6100 50  0001 C CNN
	1    1950 6100
	-1   0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J5
U 1 1 63A2CFB8
P 1950 6600
F 0 "J5" H 1900 6700 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 2030 6501 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 1950 6600 50  0001 C CNN
F 3 "~" H 1950 6600 50  0001 C CNN
	1    1950 6600
	-1   0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J6
U 1 1 63A2CFBE
P 1950 7150
F 0 "J6" H 1900 7250 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 2030 7051 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 1950 7150 50  0001 C CNN
F 3 "~" H 1950 7150 50  0001 C CNN
	1    1950 7150
	-1   0    0    1   
$EndComp
Wire Wire Line
	2150 5500 2350 5500
Wire Wire Line
	2150 5600 2900 5600
Wire Wire Line
	2600 5950 2400 5950
Wire Wire Line
	2150 6000 2400 6000
Wire Wire Line
	2400 6000 2400 5950
Wire Wire Line
	2150 6100 2650 6100
Wire Wire Line
	2150 6500 2600 6500
Wire Wire Line
	2300 6700 2300 6600
Wire Wire Line
	2300 6600 2150 6600
Wire Wire Line
	2300 6700 2900 6700
Wire Wire Line
	2150 7050 2600 7050
Wire Wire Line
	2250 7250 2250 7150
Wire Wire Line
	2250 7150 2150 7150
Text Notes 1150 5300 0    50   ~ 0
INPUTS
Wire Notes Line
	800  5000 1850 5000
Wire Notes Line
	1850 5000 1850 7600
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 63B3486B
P 650 2700
F 0 "J1" H 600 2800 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 730 2601 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 650 2700 50  0001 C CNN
F 3 "~" H 650 2700 50  0001 C CNN
	1    650  2700
	-1   0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 63B34871
P 750 3850
F 0 "J2" H 700 3950 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 830 3751 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 750 3850 50  0001 C CNN
F 3 "~" H 750 3850 50  0001 C CNN
	1    750  3850
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0112
U 1 1 63B463E1
P 1650 3650
F 0 "#PWR0112" H 1650 3500 50  0001 C CNN
F 1 "+5V" H 1665 3823 50  0000 C CNN
F 2 "" H 1650 3650 50  0001 C CNN
F 3 "" H 1650 3650 50  0001 C CNN
	1    1650 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 63B570F7
P 1650 4250
F 0 "#PWR0113" H 1650 4000 50  0001 C CNN
F 1 "GND" H 1655 4077 50  0000 C CNN
F 2 "" H 1650 4250 50  0001 C CNN
F 3 "" H 1650 4250 50  0001 C CNN
	1    1650 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 4250 1650 4150
$Comp
L RS485Device:SN65176BP U5
U 1 1 63BDC577
P 2550 2500
F 0 "U5" H 2550 3081 50  0000 C CNN
F 1 "SN65176BP" H 2550 2990 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 2550 2000 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/sn65176b.pdf" H 2550 2500 50  0001 C CNN
	1    2550 2500
	1    0    0    -1  
$EndComp
Wire Notes Line
	11100 3000 10300 3000
Wire Notes Line
	10300 3000 10300 500 
Text GLabel 6250 4150 2    50   Input ~ 0
DRIVE
Text GLabel 3300 2300 2    50   Input ~ 0
RXD
Text GLabel 3300 2400 2    50   Input ~ 0
TXD
Text GLabel 3300 2650 2    50   Input ~ 0
DRIVE
Wire Wire Line
	3050 2300 3300 2300
Wire Wire Line
	3050 2400 3300 2400
Wire Wire Line
	3300 2650 3150 2650
Wire Wire Line
	3150 2650 3150 2600
Wire Wire Line
	3150 2600 3050 2600
Wire Wire Line
	3050 2700 3150 2700
Wire Wire Line
	3150 2700 3150 2650
Connection ~ 3150 2650
Text Notes 800  2050 0    50   ~ 0
CONTROL & POWER
Text Notes 550  2500 0    50   ~ 0
RS485 Bus
Text Notes 600  3400 0    50   ~ 0
Power input
$Comp
L Device:Crystal Y1
U 1 1 63C9A6AD
P 4450 3350
F 0 "Y1" V 4404 3481 50  0000 L CNN
F 1 "Crystal" V 4495 3481 50  0000 L CNN
F 2 "Crystal:Crystal_HC49-4H_Vertical" H 4450 3350 50  0001 C CNN
F 3 "~" H 4450 3350 50  0001 C CNN
	1    4450 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	4450 3200 4800 3200
Wire Wire Line
	4800 3200 4800 3250
Wire Wire Line
	4800 3250 4900 3250
Wire Wire Line
	4900 3450 4800 3450
Wire Wire Line
	4800 3450 4800 3500
Wire Wire Line
	4800 3500 4450 3500
$Comp
L Device:C C1
U 1 1 63CC4E89
P 3850 3650
F 0 "C1" H 3650 3700 50  0000 L CNN
F 1 "30p" H 3600 3600 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.8mm_W2.6mm_P2.50mm" H 3888 3500 50  0001 C CNN
F 3 "~" H 3850 3650 50  0001 C CNN
	1    3850 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 63CC57B3
P 4150 3650
F 0 "C2" H 4265 3696 50  0000 L CNN
F 1 "30p" H 4265 3605 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.8mm_W2.6mm_P2.50mm" H 4188 3500 50  0001 C CNN
F 3 "~" H 4150 3650 50  0001 C CNN
	1    4150 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 3200 3850 3200
Wire Wire Line
	3850 3200 3850 3500
Connection ~ 4450 3200
Wire Wire Line
	4150 3500 4450 3500
Connection ~ 4450 3500
Wire Wire Line
	4150 3800 4150 3850
Wire Wire Line
	4150 3850 4000 3850
Wire Wire Line
	4000 3850 4000 4000
Wire Wire Line
	3850 3800 3850 3850
Wire Wire Line
	3850 3850 4000 3850
Connection ~ 4000 3850
$Comp
L power:GND #PWR0114
U 1 1 63D45D6E
P 4000 4000
F 0 "#PWR0114" H 4000 3750 50  0001 C CNN
F 1 "GND" H 4005 3827 50  0000 C CNN
F 2 "" H 4000 4000 50  0001 C CNN
F 3 "" H 4000 4000 50  0001 C CNN
	1    4000 4000
	1    0    0    -1  
$EndComp
Text Notes 7450 7500 0    50   ~ 0
Heat Pump controller slave device
$Comp
L power:+5V #PWR0115
U 1 1 63D739E3
P 2150 2050
F 0 "#PWR0115" H 2150 1900 50  0001 C CNN
F 1 "+5V" H 2165 2223 50  0000 C CNN
F 2 "" H 2150 2050 50  0001 C CNN
F 3 "" H 2150 2050 50  0001 C CNN
	1    2150 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 2050 2150 2100
Wire Wire Line
	2150 2100 2550 2100
$Comp
L power:GND #PWR0116
U 1 1 63D9F245
P 2700 3050
F 0 "#PWR0116" H 2700 2800 50  0001 C CNN
F 1 "GND" H 2705 2877 50  0000 C CNN
F 2 "" H 2700 3050 50  0001 C CNN
F 3 "" H 2700 3050 50  0001 C CNN
	1    2700 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 2900 2550 2900
Wire Wire Line
	2700 2900 2700 3050
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 63DCCDFE
P 950 3650
F 0 "#FLG0101" H 950 3725 50  0001 C CNN
F 1 "PWR_FLAG" H 800 3800 50  0000 C CNN
F 2 "" H 950 3650 50  0001 C CNN
F 3 "~" H 950 3650 50  0001 C CNN
	1    950  3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  3650 950  3750
Connection ~ 950  3750
Text Label 3500 5500 0    50   ~ 0
EHS
Text Label 3500 6050 0    50   ~ 0
MODE
Text Label 3500 6600 0    50   ~ 0
BCKUP
Text Label 3500 7150 0    50   ~ 0
ALARM
Text Label 6100 4250 0    50   ~ 0
EHS
Text Label 6100 4350 0    50   ~ 0
MODE
Text Label 6100 4450 0    50   ~ 0
BCKUP
Wire Wire Line
	6100 4250 6350 4250
Wire Wire Line
	6100 4350 6350 4350
Wire Wire Line
	6100 4450 6350 4450
Wire Wire Line
	6100 4550 6350 4550
Wire Bus Line
	4100 5250 6450 5250
Text Label 6100 4550 0    50   ~ 0
ALARM
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 63E71E81
P 950 3950
F 0 "#FLG0102" H 950 4025 50  0001 C CNN
F 1 "PWR_FLAG" H 1100 4100 50  0000 C CNN
F 2 "" H 950 3950 50  0001 C CNN
F 3 "~" H 950 3950 50  0001 C CNN
	1    950  3950
	-1   0    0    1   
$EndComp
Wire Wire Line
	950  3950 950  3850
Wire Wire Line
	950  3850 1150 3850
Connection ~ 950  3850
$Comp
L Device:D D1
U 1 1 63465847
P 1300 3750
F 0 "D1" H 1300 3533 50  0000 C CNN
F 1 "D" H 1300 3624 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P10.16mm_Horizontal" H 1300 3750 50  0001 C CNN
F 3 "~" H 1300 3750 50  0001 C CNN
	1    1300 3750
	-1   0    0    1   
$EndComp
Wire Wire Line
	950  3750 1150 3750
Wire Wire Line
	1650 3750 1650 3650
Wire Wire Line
	1150 3850 1150 4150
$Comp
L Device:C C3
U 1 1 63494D53
P 1500 3950
F 0 "C3" H 1615 3996 50  0000 L CNN
F 1 "C" H 1615 3905 50  0000 L CNN
F 2 "Capacitor_THT:C_Radial_D6.3mm_H11.0mm_P2.50mm" H 1538 3800 50  0001 C CNN
F 3 "~" H 1500 3950 50  0001 C CNN
	1    1500 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 3750 1500 3750
Wire Wire Line
	1500 3800 1500 3750
Connection ~ 1500 3750
Wire Wire Line
	1500 3750 1650 3750
Wire Wire Line
	1150 4150 1500 4150
Wire Wire Line
	1500 4100 1500 4150
Connection ~ 1500 4150
Wire Wire Line
	1500 4150 1650 4150
$Comp
L Device:D D3
U 1 1 63385B64
P 1100 2900
F 0 "D3" H 1100 2800 50  0000 C CNN
F 1 "TVSS" H 1100 3000 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P10.16mm_Horizontal" H 1100 2900 50  0001 C CNN
F 3 "~" H 1100 2900 50  0001 C CNN
	1    1100 2900
	0    -1   -1   0   
$EndComp
$Comp
L Device:D D4
U 1 1 633B4EB1
P 1400 2900
F 0 "D4" H 1400 2800 50  0000 C CNN
F 1 "TVSS" H 1400 3000 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P10.16mm_Horizontal" H 1400 2900 50  0001 C CNN
F 3 "~" H 1400 2900 50  0001 C CNN
	1    1400 2900
	0    -1   -1   0   
$EndComp
$Comp
L Device:D D2
U 1 1 633CC5A9
P 1200 2500
F 0 "D2" H 1200 2283 50  0000 C CNN
F 1 "TVSS" H 1200 2374 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P10.16mm_Horizontal" H 1200 2500 50  0001 C CNN
F 3 "~" H 1200 2500 50  0001 C CNN
	1    1200 2500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2050 2400 1850 2400
Wire Wire Line
	1000 2300 1000 2600
Wire Wire Line
	1000 2600 850  2600
Wire Wire Line
	1200 2350 1200 2300
Connection ~ 1200 2300
Wire Wire Line
	1200 2300 1000 2300
$Comp
L power:GND #PWR0117
U 1 1 63414332
P 1250 3100
F 0 "#PWR0117" H 1250 2850 50  0001 C CNN
F 1 "GND" H 1255 2927 50  0000 C CNN
F 2 "" H 1250 3100 50  0001 C CNN
F 3 "" H 1250 3100 50  0001 C CNN
	1    1250 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 2400 1850 2300
Wire Wire Line
	1200 2300 1700 2300
Wire Wire Line
	2050 2600 1850 2600
Wire Wire Line
	1850 2600 1850 2700
Wire Wire Line
	1850 2700 1700 2700
Wire Wire Line
	1200 2650 1200 2700
Connection ~ 1200 2700
Wire Wire Line
	1200 2700 1100 2700
Wire Wire Line
	1100 2700 1100 2750
Connection ~ 1100 2700
Wire Wire Line
	1100 2700 850  2700
Wire Wire Line
	1400 2700 1400 2750
Connection ~ 1400 2700
Wire Wire Line
	1400 2700 1200 2700
Wire Wire Line
	1400 3050 1250 3050
Wire Wire Line
	1250 3100 1250 3050
Connection ~ 1250 3050
Wire Wire Line
	1250 3050 1100 3050
Wire Notes Line
	1800 4500 1800 1850
Wire Notes Line
	1800 1850 550  1850
Wire Notes Line
	600  4500 1800 4500
$Comp
L Device:R_Small R8
U 1 1 63544C21
P 1700 2500
F 0 "R8" H 1800 2550 50  0000 L CNN
F 1 "150" H 1750 2450 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 1700 2500 50  0001 C CNN
F 3 "~" H 1700 2500 50  0001 C CNN
	1    1700 2500
	-1   0    0    1   
$EndComp
Wire Wire Line
	1700 2400 1700 2300
Connection ~ 1700 2300
Wire Wire Line
	1700 2300 1850 2300
Wire Wire Line
	1700 2600 1700 2700
Connection ~ 1700 2700
Wire Wire Line
	1700 2700 1400 2700
Wire Wire Line
	4800 2750 5500 2750
Wire Wire Line
	6100 4150 6250 4150
Wire Wire Line
	6100 3950 6250 3950
Wire Wire Line
	6100 4050 6250 4050
Wire Bus Line
	6450 3250 6450 3500
Wire Bus Line
	8700 3200 8700 4300
Wire Bus Line
	8700 4300 8700 5400
Wire Bus Line
	8700 5400 8700 6450
Wire Bus Line
	8700 6450 10700 6450
Wire Bus Line
	8700 5400 10750 5400
Wire Bus Line
	8700 4300 10700 4300
Wire Bus Line
	9900 5600 11150 5600
Wire Bus Line
	9900 4550 11150 4550
Wire Bus Line
	9700 3450 11150 3450
Wire Bus Line
	9800 4250 11150 4250
Wire Bus Line
	9800 5350 11150 5350
Wire Bus Line
	9800 6400 11150 6400
Wire Bus Line
	6450 4250 6450 5250
Wire Bus Line
	6550 3550 6550 4450
Wire Bus Line
	6400 3550 6400 3900
Wire Bus Line
	8100 1300 8100 3250
Wire Bus Line
	8050 3450 8050 6350
Wire Bus Line
	7950 3450 7950 4450
Wire Bus Line
	4100 5250 4100 7300
Wire Bus Line
	9700 1250 9700 3450
$EndSCHEMATC
