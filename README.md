# World of Robot - Kinematica

Dit project bevat de uitwerking van de World of robots - kinematica opdracht 2022.

## Instalatie instructies [Linux]

> **```note```** Als beide Boost en wxWidgets geïnstalleerd en ge-compiled zijn als beschreven in de installatie handleiding kunnen de onderstaande commando's gebruikt worden voor het compileren van de robotworld applicatie.

> **```note```** De onderstaande commando's moeten uitgevoerd worden vanaf de root folder van het project. bijvoorbeeld ```'~/sources/robotworld/'```, waar ```'~'``` de home folder van de gebruiker betekend. 

### Compileren van de applicatie

```bash
mkdir build
cd build
cmake ..
make -j4 2>$1 | tee build.log
```

> **```note```** Het onderdeel ```'-j4'``` staat voor het aantal gebruikte cores van de CPU voor het compileren van de applicatie. Dit getal kan aangepast worden naar het aantal gewenste cores.

### Uitvoeren van de executable

```bash
./robotworld
```