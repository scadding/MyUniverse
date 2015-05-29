
/* Stellar Mass */
double MassTableIa[14] =
  /*  B0  B5  A0  A5  F0  F5   G0   G5  K0  K5  M0  M5  */
    { 16, 16, 14, 11, 10, 8.1, 8.1, 10, 11, 14, 16, 16 };
double MassTableIb[14] =
  /*  B0  B5  A0  A5  F0  F5   G0   G5  K0  K5  M0  M5  */
    { 16, 16, 14, 11, 10, 8.1, 8.1, 10, 11, 14, 16, 16 };
double MassTableII[14] =
 /*  B0  B5  A0  A5  F0  F5   G0   G5  K0  K5  M0  M5  */
   { 16, 16, 14, 11, 10, 8.1, 8.1, 10, 11, 14, 16, 16 };
double MassTableIII[14] =
  /*  B0  B5  A0  A5  F0  F5   G0   G5  K0  K5   M0   M5  */
    { 14, 14, 12,  9,  8,  5, 2.5, 3.2,  4,  5, 6.3, 7.4 };
double MassTableIV[14] =
  /*  B0  B5  A0  A5    F0  F5   G0   G5   K0  K5  M0  M5  */
    { 10,  8,  6,  4,  2.5,  2, 1.75,  2, 2.3,  0,  0,  0 };
double MassTableV[14] =
  /*  B0 B5  A0   A5   F0   F5   G0    G5    K0     K5    M0     M5  */
    { 6, 4, 3.2, 2.1, 1.7, 1.3, 1.04, 0.94, 0.825, 0.57, 0.489, 0.331 };
double MassTableD[14] =
  /*  B0   B5   A0    A5    F0    F5    G0    G5    K0    K5    M0    M5  */
    { 0.3, 0.3, 0.36, 0.36, 0.42, 0.42, 0.63, 0.63, 0.83, 0.83, 1.11, 1.11 };
    
/* Luminosiy Factor */
double LumTableIa[15] =
 /*  B0    B5    A0    A5    F0    F5    G0    G5    K0    K5    M0    M5     M9 */
   { 11.0, 10.0, 6.85, 5.40, 4.95, 4.75, 4.86, 5.22, 5.46, 7.04, 8.24, 11.05, 11.28 };
double LumTableIb[15] =
 /*  B0    B5    A0    A5    F0    F5    G0    G5    K0    K5    M0    M5     M9 */
   { 11.0, 10.0, 6.85, 5.40, 4.95, 4.75, 4.86, 5.22, 5.46, 7.04, 8.24, 11.05, 11.28 };
double LumTableII[15] =
 /*  B0    B5    A0    A5    F0    F5    G0    G5    K0    K5    M0    M5     M9 */
   { 11.0, 10.0, 6.85, 5.40, 4.95, 4.75, 4.86, 5.22, 5.46, 7.04, 8.24, 11.05, 11.28 };
double LumTableIII[15] =
 /*  B0    B5    A0    A5    F0    F5    G0    G5    K0    K5    M0    M5    M9 */
   { 8.00, 6.00, 4.09, 3.08, 2.70, 2.56, 2.66, 2.94, 3.12, 4.23, 4.65, 6.91, 7.20 };
double LumTableIV[15] =
 /*  B0    B5    A0    A5    F0    F5    G0    G5    K0    K5    M0    M5    M9 */
   { 6.00, 4.00, 3.53, 2.47, 2.09, 1.86, 1.60, 1.49, 1.47, 1.27, 1.25, 1.10, 1.02 };
double LumTableV[15] =
 /*  B0    B5    A0    A5    F0    F5    G0    G5    K0    K5    M0    M5    M9 */
   { 4.20, 3.50, 3.08, 2.00, 1.69, 1.37, 1.05, 0.90, 0.81, 0.53, 0.45, 0.29, 0.18 };
double LumTableD[15] =
 /*  B0    B5    A0    A5    F0    F5    G0    G5    K0    K5    M0    M5    M9 */
   { 0.37, 0.31, 0.27, 0.27, 0.13, 0.13, 0.09, 0.09, 0.08, 0.08, 0.07, 0.07, 0.07 };

/* Energy Absorption Factors */
double EnergyAbsorbHZ[11][4] = {
/*        0-3    4-9    A-E    F
/* 0 */ { 0.900, 0.900, 0.740, 0.900 },
/* 1 */ { 0.829, 0.900, 0.697, 0.900 },
/* 2 */ { 0.803, 0.860, 0.672, 0.882 },
/* 3 */ { 0.811, 0.860, 0.676, 0.883 },
/* 4 */ { 0.782, 0.820, 0.648, 0.866 },
/* 5 */ { 0.789, 0.780, 0.613, 0.850 },
/* 6 */ { 0.795, 0.740, 0.577, 0.836 },
/* 7 */ { 0.802, 0.700, 0.539, 0.821 },
/* 8 */ { 0.808, 0.660, 0.500, 0.807 },
/* 9 */ { 0.814, 0.620, 0.500, 0.793 },
/* A */ { 0.818, 0.619, 0.500, 0.773 },
};
double EnergyAbsorbNHZ[11][4] = {
/*        0-3    4-9    A-E    F
/* 0 */ { 0.800, 0.800, 0.680, 0.800 },
/* 1 */ { 0.744, 0.811, 0.646, 0.811 },
/* 2 */ { 0.736, 0.789, 0.635, 0.807 },
/* 3 */ { 0.752, 0.799, 0.644, 0.817 },
/* 4 */ { 0.738, 0.774, 0.625, 0.813 },
/* 5 */ { 0.753, 0.747, 0.599, 0.809 },
/* 6 */ { 0.767, 0.718, 0.570, 0.805 },
/* 7 */ { 0.782, 0.687, 0.537, 0.800 },
/* 8 */ { 0.796, 0.654, 0.500, 0.794 },
/* 9 */ { 0.810, 0.619, 0.500, 0.787 },
/* A */ { 0.818, 0.619, 0.500, 0.773 },
};

    
/* Greenhouse Factor */
double GreenHouse[16] = 
 /* 0     1     2     3     4     5     6     7     */
  { 1.00, 1.00, 1.00, 1.00, 1.05, 1.05, 1.10, 1.10, 
 /* 8     9     A     B     C     D     E     F  */
    1.15, 1.15, 1.15, 1.00, 1.10, 1.15, 1.00, 1.10 };
  
/* Latitude Temperature Modifiers */
double LatitudeMods[11][11] =  {
        /* 0    1    2    3?   4    5?   6     7?    8     9?   10  */
/* 1 */ {  6,   9,  12,  13,  15,  16,  18,   17,   21,   22,   24, },
/* 2 */ {  4,   6,   8,   9,  10,  11,  12,   13,   14,   15,   16, },
/* 3 */ {  2,   3,   4,   4,   5,   5,   6,    6,    7,    7,    8, },
/* 4 */ {  0,   0,   0,   0,   0,   0,   0,    0,    0,    0,    0, },
/* 5 */ { -2,  -3,  -4,  -4,  -5,  -5,  -6,   -6,   -7,   -7,   -8, },
/* 6 */ { -4,  -6,  -8,  -9, -10, -11, -12,  -13,  -14,  -15,  -16, },
/* 7 */ { -6,  -9, -12, -13, -15, -16, -18,  -17,  -21,  -22,  -24, },
/* 8 */ { -8, -12, -16, -18, -20, -22, -24,  -26,  -28,  -30,  -32, },
/* 9 */ {-10, -15, -20, -22, -25, -27, -30,  -32,  -35,  -37,  -40, },
/*10 */ {-12, -18, -24, -27, -30, -33, -36,  -39,  -42,  -45,  -48, },
/*11 */ {-14, -21, -28, -31, -35, -38, -42,  -45,  -49,  -52,  -56, },
};

/* Axial Tilt Effects */
double AxialTiltEffects[11][12] =  {
       /* 0     1-5   6-10  11-15 16-20 21-25 26-30 31-35 36-45 46-60 61-84 85+  */
/* 1 */ { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.25, 0.50, 0.75, 1.00 },
/* 2 */ { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.25, 0.50, 0.75, 1.00, 1.00 },
/* 3 */ { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.25, 0.50, 0.75, 1.00, 1.00, 1.00 },
/* 4 */ { 0.00, 0.00, 0.00, 0.00, 0.00, 0.25, 0.50, 0.75, 1.00, 1.00, 1.00, 1.00 },
/* 5 */ { 0.00, 0.00, 0.00, 0.00, 0.25, 0.50, 0.75, 1.00, 1.00, 1.00, 1.00, 1.00 },
/* 6 */ { 0.00, 0.00, 0.00, 0.25, 0.50, 0.75, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 },
/* 7 */ { 0.00, 0.00, 0.25, 0.50, 0.75, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 },
/* 8 */ { 0.00, 0.00, 0.50, 0.75, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 },
/* 9 */ { 0.00, 0.25, 0.75, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 },
/*10 */ { 0.00, 0.50, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 },
/*11 */ { 0.25, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 },
}; 
