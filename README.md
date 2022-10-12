# World of Robot - Kinematica

Dit project bevat de uitwerking van de World of robots - kinematica opdracht 2022.

## Installatie instructies [Linux]

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

Uitvoeren van de applicatie in de standaard opstelling.

```bash
./robotworld
```

Uitvoeren van de applicatie met een aangepaste configuratie file. Deze file moet in de config map geplaatst zijn.

> **```note```** zie config/sensor_noise_config.txt

```bash
./robotworld -config="name_of_config_file.txt"
```

Uitvoeren van de applicatie met een aangepaste configuratie file buiten de map van de applicatie.

> **```note```** zie config/sensor_noise_config.txt

```bash
./robotworld -fromRoot -config="path/from/root/name_of_config_file.txt"
```

### Opdracht

<details>
    <summary> Opdracht progressie </summary>

|#|Beschrijving|Voldaan|
|:---:|:---|:---:|
|01|Ga uit van de basisversie van Robotwereld. In de huidige master zijn heel veel warnings opgelost in RobotWorld zelf. Als je bovendien gebruik maakt van het bijgeleverde autotools build systeem dan heb je vrijwel geen last van allerlei Boost en wxWidgets warnings bij het compileren of het gebruik van CPPCheck.|:heavy_check_mark:|
|02|Vergroot de wereld tot 1024x1024 pixels.|:heavy_check_mark:|
|03|Voeg ten minste 4 muren van tenminste 100 pixels toe aan de wereld.|:heavy_check_mark:|
|04|De robot heeft kennis van de kaart van de wereld.|:heavy_check_mark:|
|05|De robot heeft een positie en oriëntatie die gebruikt wordt voor het bepalen van de route, het rijden en als input voor de sensoren (van waaruit ergemeten moet worden). Daarnaast moet de robot een ’belief’ positie en oriëntatie krijgen.|:heavy_check_mark:|
|06|Het bepalen van de route met A-star en het automatisch rijden naar het doel kan beschout worden als het handmatig besturen / laten rijden van de robot.|:heavy_check_mark: ?|
|07|De robot maakt gebruik van de Kalmanfilter op basis van kompas en odometer om een overtuiging van zijn positie en oriëntatie te vormen.|:heavy_check_mark:|
|08|De robot maakt gebruik van het Particlefilter op basis van kompas en lidar omeen overtuiging van zijn positie en oriëntatie te vormen.|:heavy_check_mark:|
|09|De robot ontwikkelt een overtuiging van de werkelijkheid volgens twee methodes (zie voorgaande twee items). Voeg aan de GUI van robotwereld een optie toe waarmee de gebruiker kan selecteren of de robot gebruik maakt van de overtuiging verkregen met Kalmanfilter of die uit het Particlefilter om een keuze te maken voor de control update.|:heavy_check_mark:|
|10|De controlupdate gaat gepaard met onzekerheid. Bij een snelheid van 10 pixels per stap is de kans 0.7 dat deze uitkomt in een van de pixels in het 3x3 vierkant rondom de doelpixel. De kans is 0.3 dat deze uitkomt in één van de overige pixels in het 5x5 vierkant rondom de doelpixel.|:heavy_check_mark:|
|11|Het kompas van de robot bepaalt de rijrichting van de robot met een configureerbare standaardeviatie. De uitgangswaarde van de standaardeviatie is 2 graden.|:heavy_check_mark:|
|12|De odometer van de robot meet de afgelegde weg van de robot met een configureerbare standaardeviatie. De uitgangswaarde van deze standaardeviatie is 1 pixel per stap van 10 pixels.|:heavy_check_mark:|
|13|De lidar van de robot meet per ronde 180 keer (dat is iedere 2 graden). De afstandsmeting van iedere “beam” heeft een configureerbare standaardeviatie. De uitgangswaarde van deze standaardeviatie is 10 pixels.|:heavy_check_mark:|
|14|Laat de robot naar een willekeurig doel in de wereld rijden. Plot in de robotwereld de werkelijk afgelegde weg, de overtuiging van de afgelegde weg volgens het Kalmanfilter, en de overtuiging van de afgelegde weg volgens het Particlefilter.|:heavy_check_mark:|

</details>

<details>
    <summary> Opdracht progressie (updated: 06-10-2022) </summary>

|#|Beschrijving|Voldaan|
|:---:|:---|:---:|
|01|Ga uit van de WoR-branch van Robotwereld. In die huidige master zijn heel veel warnings opgelost in RobotWorld die in de OSM versie (by design) zitten. Als je bovendien gebruik maakt van het bijgeleverde autotools build systeem dan heb je vrijwel geen last van allerlei Boost en wxWidgets warnings bij het compileren of het gebruik van CPPCheck.|:heavy_check_mark:|
|02|Vergroot de wereld tot 1024x1024 pixels.|:heavy_check_mark:|
|03|Voeg ten minste 4 muren van tenminste 100 pixels toe aan de wereld.|:heavy_check_mark:|
|04|De robot heeft kennis van de kaart van de wereld.|:heavy_check_mark:|
|05|De robot heeft een positie en orïentatie die gebruikt wordt voor het bepalen van de route, het rijden en als input voor de sensoren (van waaruit er gemeten moet worden). Daarnaast moet de robot een ’belief’ positie en ori ̈entatie krijgen.|:heavy_check_mark:|
|06|Het bepalen van de route met A-star en het automatisch rijden naar het doel kan beschouwd worden als het handmatig besturen / laten rijden van de robot.|:heavy_check_mark:|
|07|De robot maakt gebruik van de Kalmanfilter op basis van een kompas en odometer om een overtuiging van zijn positie en orïentatie te vormen.|:heavy_check_mark:|
|08|De robot maakt gebruik van het particlefilter op basis van een lidar en (indien nodig) kompas om een overtuiging van zijn positie en orïentatie te vormen. Het is toegestaan om de lidar te implementeren met een vaste richting, i.e. naar het noorden, parallel aan de y-as.|:heavy_check_mark:|
|09|De robot ontwikkelt een overtuiging van de werkelijkheid volgens twee methodes (zie voorgaande twee items). Voeg aan de GUI van robotwereld een optie toe waarmee de gebruiker kan selecteren of de robot gebruik maakt van de overtuiging verkregen met Kalmanfilter en/of die uit het particlefilter.|:heavy_check_mark:|
|10|Alle updates gaan deterministisch, i.e. daar zit geen onzekerheid in.|:heavy_check_mark:|
|11|Aleen de sensoren zijn probabilistisch.|:heavy_check_mark:|
|12|Het kompas van de robot meet de rijrichting van de robot met een configureerbare standaardeviatie. De uitgangswaarde van de standaardeviatie is 2 graden.|:heavy_check_mark:|
|13|De odometer van de robot meet de afgelegde weg van de robot met een configureerbare standaardeviatie. De uitgangswaarde van deze standaar deviatie is 1 pixel per stap van 10 pixels.|:heavy_check_mark:|
|14|De lidar van de robot meet per ronde 180 keer (dat is iedere 2 graden). De afstandsmeting van iedere “beam” heeft een configureerbare standaardeviatie. De uitgangswaarde van deze standaardeviatie is 10 pixels.|:heavy_check_mark:|
|15|Alle configureerbare waardes moeten uit een tekst-bestand gelezen worden|:heavy_check_mark:|
|16|Laat de robot naar een willekeurig doel in de wereld rijden. Plot in de robotwereld de werkelijk afgelegde weg, de overtuiging van de afgelegde weg volgens het Kalmanfilter, en de overtuiging van de afgelegde weg volgens het Particlefilter.|:heavy_check_mark:|

</details>