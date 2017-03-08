# gsoc_gamser
TMath::GamSer with AVX

gsoc_gamser/gamser.h
TMath::GamSer: Original version in ROOT(https://root.cern).
TMath::GamSer2: Edited with AVX support.

gsoc_gamser/main.cpp
Code to test two functions.

On Haswell@2.3GHz (arukas.io docker Xeon E5-2650v3/512MB)
TESTTIME = 10000000
Compile command: g++ main.cpp -lm -mavx -O2 -o main
Without AVX: 1.476307s
With AVX: 0.916580s

On Broadwell@2.2GHz (Core i5 5200U/8G Dell Laptop)
TESTTIME = 1000000
Compile command: g++ -O3 -lm -mavx main.cpp -o main_gcc.exe
Without AVX: 0.876214s
With AVX: 0.527415s
