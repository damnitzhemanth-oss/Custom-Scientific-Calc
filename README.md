
# Navigation 

This repo mainly contains four sections, 
1. CAD for OnShape step files.
2. Images for PCB and CAD images.
3. PCB for project files and production files (POS,CSV,BOM,Gerbers).
4. Firmware for code.

**production folders contain two CSV's, one is manual made with cost and other is KiCad Generated**

# Scientific Calculator 

Scientific calculators are just normal calculator and all other weird math symbols from trigonometry , algebra etc.

These are usually costly and bulky. So I reverse engineered and built one !.

![Hero View](Images/CAD/Hero_View_Final.png)


# Architecture of the REPO

```

Custom-Scientific-Calc
├── CAD
│   ├── Assembly.step
│   ├── Base_Plate.step
│   └── Top_Plate.step
├── Images
│   ├── CAD
│   │   ├── Base_P_B.png
│   │   ├── Base_PlateH.png
│   │   ├── Hero_View_Final.png
│   │   ├── Top_Plate.png
│   │   └── Top_Plate_H.png
│   └── PCB
│       ├── Base
│       │   ├── PCB
│       │   │   ├── 3D.png
│       │   │   ├── 3D_B.png
│       │   │   ├── 3D_Hero.png
│       │   │   ├── B.cu.png
│       │   │   ├── F.cu.png
│       │   │   └── HeroView_PCB.png
│       │   └── Schematic
│       │       ├── Battery.png
│       │       ├── Boosters.png
│       │       ├── Connectors.png
│       │       ├── Crystal.png
│       │       ├── LEDs.png
│       │       ├── MCU_Core.png
│       │       ├── Programming.png
│       │       └── Reset_BTN.png
│       └── Top
│           ├── PCB
│           │   ├── 3DB.png
│           │   ├── 3DF.png
│           │   ├── 3DH.png
│           │   ├── PCBB.cu.png
│           │   ├── PCBF.cu.png
│           │   └── PCBH.png
│           └── Schematic
│               ├── Connectors.png
│               ├── LCD.png
│               ├── Power.png
│               └── Switch_Matrix.png
├── PCB
│   ├── Base_PCB
│   │   ├── CAD
│   │   │   ├── BASE_PCB.cad
│   │   │   └── BASE_PCB.step
│   │   └── Project_Files
│   │       ├── BASE.kicad_sch
│   │       ├── BASE_PCB.kicad_pcb
│   │       ├── BASE_PCB.kicad_pro
│   │       ├── BASE_PCB.step
│   │       └── Production
│   │           ├── BASE.csv
│   │           ├── BASE_PCB-all-pos.csv
│   │           └── BasePCB_Gerbers.zip
│   └── Top_PCB
│       ├── CAD
│       │   ├── TOP.step
│       │   └── Top_PCB.cad
│       └── Project_Files
│           ├── Production
│           │   ├── TOP-all-pos.csv
│           │   ├── TOP.csv
│           │   └── TopPCB_Gerbers.zip
│           ├── TOP.kicad_pcb
│           ├── TOP.kicad_pro
│           ├── TOP.kicad_sch
│           └── TOP.step
├── README.md
└── firmware
    ├── include
    │   ├── board.h
    │   ├── keypad.h
    │   └── lcd.h
    ├── single_file
    │   └── main.c
    └── src
        ├── keyboard.c
        ├── lcd.c
        └── main.c

```

# BOM 

## BASE PCB BOM

| # | Ref | Part / Value | Qty | Footprint | LCSC | Unit $ | Total $ |
|---|-----|--------------|-----|-----------|------|--------|---------|
| 1 | U1 | STM32L431RCT6 | 1 | LQFP-64 10x10mm | C92468 | $3.60 | $3.60 |
| 2 | U02 | XCL108B331ER-G (3.3V boost) | 1 | CL-2025-02 | — | $2.50 | $2.50 |
| 3 | U03 | XCL108B501ER-G (5.0V boost) | 1 | CL-2025-02 | — | $2.50 | $2.50 |
| 4 | U4 | EVQ-P0P02B (reset button) | 1 | SW-SMD_EVQ-P0P02B | C6554563 | $0.44 | $0.44 |
| 5 | BT1 | Keystone 103 CR2032 holder | 1 | BatteryHolder_Keystone_103 | — | $0.55 | $0.55 |
| 6 | Y1 | 32.768 kHz crystal, 12.5pF | 1 | Crystal_SMD_3225-4Pin | C32346 | $0.28 | $0.28 |
| 7 | C1–C5, C14, C16, C18 | 100nF 50V X7R | 8 | C_0402_1005 | C1525 | $0.01 | $0.08 |
| 8 | C6 | 1µF 10V X5R | 1 | C_0402_1005 | C15849 | $0.02 | $0.02 |
| 9 | C7, C15 | 10µF 10V X5R | 2 | C_0402_1005 | C19702 | $0.03 | $0.06 |
| 10 | C10, C11, C12, C17, C21 | 10µF 10V X5R | 5 | C_0805_2012 | C15850 | $0.05 | $0.25 |
| 11 | C19, C20 | 18pF 50V C0G | 2 | C_0402_1005 | C1579 | $0.01 | $0.02 |
| 12 | R1, R2 | 1kΩ 1% | 2 | R_0402_1005 | C11702 | $0.005 | $0.01 |
| 13 | R3, R8 | 10kΩ 1% | 2 | R_0402_1005 | C25744 | $0.005 | $0.01 |
| 14 | R7 | 10Ω 1% | 1 | R_0402_1005 | C103090 | $0.005 | $0.005 |
| 15 | D1 | Green LED 0603 | 1 | LED_0603_1608 | C72043 | $0.03 | $0.03 |
| 16 | D2 | Red LED 0603 | 1 | LED_0603_1608 | C2286 | $0.03 | $0.03 |
| 17 | J1, J2 | 1×13 pin header, 2.00mm | 2 | PinHeader_1x13_P2.00 | — | $0.30 | $0.60 |
| 18 | J4 | 2×3 pin header, 2.54mm (SWD) | 1 | PinHeader_2x03_P2.54 | C65104 | $0.08 | $0.08 |

**BASE Subtotal: ~$11.07**

---

## TOP PCB BOM

| # | Ref | Part / Value | Qty | Footprint | LCSC | Unit $ | Total $ |
|---|-----|--------------|-----|-----------|------|--------|---------|
| 1 | U1–U48 | EVQ-P0P02B tactile switches | 48 | SW-SMD_EVQ-P0P02B_3d | C6554563 | $0.44 | $21.12 |
| 2 | D1–D48 | 1N4148W (anti-ghosting) | 48 | D_SOD-110 | C89641 | $0.01 | $0.48 |
| 3 | SW1 | PS-5824SVA-6PLW (latching power switch) | 1 | SW-SMD_6P-L5.8-W5.8-P2.00 | C47018553 | $1.00 | $1.00 |
| 4 | J1, J2 | 1×13 socket, 2.00mm | 2 | PinSocket_1x13_P2.00 | — | $0.35 | $0.70 |
| 5 | J3 | Hirose FH12-10S-0.5SH (LCD FPC) | 1 | FFC-FPC_Hirose_FH12-10S | C506791 | $0.60 | $0.60 |
| 6 | DS1 | Sharp LS027B7DH01A 2.7" Memory LCD | 1 | *external, soldered via J3* | — | $18.00 | $18.00 |

**TOP Subtotal: ~$41.90**

---

## COMBINED SUMMARY

| Item | Cost |
|------|------|
| BASE PCB components | $11.07 |
| TOP PCB components | $41.90 |
| **Total component cost** | **$52.97** |
| PCB fabrication (2 boards, 5 pcs each, JLCPCB) | ~$15.00 |
| SMT assembly setup fee | ~$8.00 |
| Extended part feeder fees (~15 parts × $1.50) | ~$22.50 |
| Stencil + labor | ~$6.50 |
| **Estimated total (5 units)** | **~$105 per run** |
| **Per unit (5 units)** | **~$21 + shipping** |

---

## Notes

Torex boosters footprint was not available on internet so I created symbol and footprint.

# CAD 

Onshape Link : https://cad.onshape.com/documents/ad7319a20d75441a919925a9/w/2019cd65cfda231ca2b84125/e/7e9bc9015b0e0043ba3c4c11

How assembly works: Base PCB and top PCB latches onto each other and gets sandwiched between Top_Plate and Base_Plate preferably with 16mm screws.

# Schematic 

I used two torex boosters to boost 3v from Coin Cell into 5V for LCD and 3.3V for other components.

And matrix connection was used for 48 switches.

# Firmware

This is a test firmware. Not yet tested yet. Available as single file or 6 files (easier for debugging).

# AI USAGE

AI was used for help of Firmware and debugging problems with OnShape.

# Gallery 

**View more images in *images* folder**

## Gallery

### PCB — Base
![Base PCB Front](Images/PCB/Base/PCB/3D.png)
![Base PCB Back](Images/PCB/Base/PCB/3D_B.png)
![Base PCB Hero](Images/PCB/Base/PCB/3D_Hero.png)
![Base PCB F.Cu](Images/PCB/Base/PCB/F.cu.png)
![Base PCB B.Cu](Images/PCB/Base/PCB/B.cu.png)

### PCB — Top
![Top PCB Front](Images/PCB/Top/PCB/3DF.png)
![Top PCB Back](Images/PCB/Top/PCB/3DB.png)
![Top PCB Hero](Images/PCB/Top/PCB/3DH.png)
![Top PCB F.Cu](Images/PCB/Top/PCB/PCBF.cu.png)
![Top PCB B.Cu](Images/PCB/Top/PCB/PCBB.cu.png)

### Schematic — Base
![MCU Core](Images/PCB/Base/Schematic/MCU_Core.png)
![Boosters](Images/PCB/Base/Schematic/Boosters.png)
![Battery](Images/PCB/Base/Schematic/Battery.png)
![Crystal](Images/PCB/Base/Schematic/Crystal.png)
![Connectors](Images/PCB/Base/Schematic/Connectors.png)
![LEDs](Images/PCB/Base/Schematic/LEDs.png)
![Programming](Images/PCB/Base/Schematic/Programming.png)
![Reset Button](Images/PCB/Base/Schematic/Reset_BTN.png)

### Schematic — Top
![Switch Matrix](Images/PCB/Top/Schematic/Switch_Matrix.png)
![LCD](Images/PCB/Top/Schematic/LCD.png)
![Power](Images/PCB/Top/Schematic/Power.png)
![Connectors Top](Images/PCB/Top/Schematic/Connectors.png)

### CAD
![Base Plate](Images/CAD/Base_PlateH.png)
![Base Plate Back](Images/CAD/Base_P_B.png)
![Top Plate](Images/CAD/Top_Plate.png)
![Top Plate Hero](Images/CAD/Top_Plate_H.png)
![Hero View](Images/CAD/Hero_View_Final.png)

# License

MIT License, freedom to use any of the files in the repo. But credit is appreciated!
