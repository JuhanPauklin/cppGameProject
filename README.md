# Tohoo Tonti! 
Liikmed: Joonas Tiitson, Juhan Pauklin

## Lühikokkuvõte:

Projektiks on SFML teegiga tehtud mäng. Mäng on Space Invadersi või Touhou Projecti laadne bullet hell, kus mängija peab vastase kuulide eest põiklema ning neid vastu laskma.

## Projekti buildimine:

Kasutasime projekti loomisel Microsoft Visual Studio 2022 (MITTE Visual Studio CODEi), mis teeb projekti haldamise hulga mugavamaks, kui Visual Studio Code.

Esmalt tehke kindlaks, et teil on installeeritud CMake ->
Koodi buildimiseks avage Microsoft Visual Studio 2022 -> 
vajutage "Open a project or solution" -> 
valige \cppGameProject-master\cppGameProject.sln -> 
kui üleval keskel ribal on "Debug" siis valige rippmenüüst "Release" ->
ülevale ribal valige Build>Build CppGameProjekt (või vajutage klahvikombinatsiooni Ctrl+B)

Executable file peaks olema tekkinud kausta \cppGameProject-master\x64\Release  

Release kausta peab ka lisama teksti fondi failid, kujutised ning SFML teegi failid, et kood saaks neile ligi.

## Välised teegid:
CMake (https://cmake.org/)  
SFML (https://www.sfml-dev.org/)

## Koodi käivitamise eeldatav tulemus:

Käivitub uus ekraan koos mänguga. Esialgu avaneb stardi ekraan, kus tuleb mängu käivitamiseks Enter klahvi vajutada.  
Mängija saab liigutada end WASD klahvidega ning tühiku klahviga lasta. Vasaku shift nupuga saab aeglustada enda liikumist täpsemaks liikumiseks.  
Vastased (tondid) liiguvad edasi tagasi ning lasevad mängija tegelast punaste kuulidega. Kuuliga pihta saades läheb elusi maha. Kui elud jõuavad nulli oled kaotanud (Game Over ekraan)  
Kui võidad kõik tondid ekraanil, siis tulevad uued tontide lained ning alistades kõik vastased võidad sa mängu (Võidu ekraan).   

## Muud kaasa antud failid Release kaustas:
.ttf - font failid teksti kasutamiseks.  
cppGameProject.pdb - debugging informatsioon programmi kohta  
.dll - Dünaamilised SFML teegi failid  
