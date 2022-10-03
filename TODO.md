# TODO list

|#|Beschrijving|Voldaan|
|:---:|:---|:---:|
|01|Ga uit van de basisversie van Robotwereld. In de huidige master zijn heel veel warnings opgelost in RobotWorld zelf. Als je bovendien gebruik maakt van het bijgeleverde autotools build systeem dan heb je vrijwel geen last van allerlei Boost en wxWidgets warnings bij het compileren of het gebruik van CPPCheck.|:heavy_check_mark:|
|02|Vergroot de wereld tot 1024x1024 pixels.|:x:|
|03|Voeg ten minste 4 muren van tenminste 100 pixels toe aan de wereld.|:x:|
|04|De robot heeft kennis van de kaart van de wereld.|:x:|
|05|De robot heeft een positie en oriëntatie die gebruikt wordt voor het bepalen van de route, het rijden en als input voor de sensoren (van waaruit ergemeten moet worden). Daarnaast moet de robot een ’belief’ positie en oriëntatie krijgen.|:x:|
|06|Het bepalen van de route met A-star en het automatisch rijden naar het doel kan beschout worden als het handmatig besturen / laten rijden van de robot.|:x:|
|07|De robot maakt gebruik van de Kalmanfilter op basis van kompas en odometer om een overtuiging van zijn positie en oriëntatie te vormen.|:x:|
|08|De robot maakt gebruik van het Particlefilter op basis van kompas en lidar omeen overtuiging van zijn positie en oriëntatie te vormen.|:x:|
|09|De robot ontwikkelt een overtuiging van de werkelijkheid volgens twee methodes (zie voorgaande twee items). Voeg aan de GUI van robotwereld een optie toe waarmee de gebruiker kan selecteren of de robot gebruik maakt van de overtuiging verkregen met Kalmanfilter of die uit het Particlefilter om een keuze te maken voor de control update.|:x:|
|10|De controlupdate gaat gepaard met onzekerheid. Bij een snelheid van 10 pixels per stap is de kans 0.7 dat deze uitkomt in een van de pixels in het 3x3 vierkant rondom de doelpixel. De kans is 0.3 dat deze uitkomt in één van de overige pixels in het 5x5 vierkant rondom de doelpixel.|:x:|
|11|Het kompas van de robot bepaalt de rijrichting van de robot met een configureerbare standaardeviatie. De uitgangswaarde van de standaardeviatie is 2 graden.|:x:|
|12|De odometer van de robot meet de afgelegde weg van de robot met een configureerbare standaardeviatie. De uitgangswaarde van deze standaardeviatie is 1 pixel per stap van 10 pixels.|:x:|
|13|De lidar van de robot meet per ronde 180 keer (dat is iedere 2 graden). De afstandsmeting van iedere “beam” heeft een configureerbare standaardeviatie. De uitgangswaarde van deze standaardeviatie is 10 pixels.|:x:|
|14|Laat de robot naar een willekeurig doel in de wereld rijden. Plot in de robotwereld de werkelijk afgelegde weg, de overtuiging van de afgelegde weg volgens het Kalmanfilter, en de overtuiging van de afgelegde weg volgens het Particlefilter.|:x:|
