# hoogtemeter-modelraket
Simpele hoogtemeter voor modelraketten

## features
- toon maximaal bereikte hoogte
- enkele LiPo batterij
- goedkoop
  
##  Benodigde onderdelen
- arduino pro mini (3.3V, 8MHz)
- USB - TTL converter voor programmeren, 3.3V
- BMP280 druk/hoogte meter I2C, 3.3V
- I2C OLED 0.91 inch 128x32 schermpje
- LiPo batterijtje (3.7V > 200 mAh)
- gaatjes print, 0.1 inch gaatjes, vertind, stevig printplaat
- soldeerboutje
- 0.5 mm soldeer met harskern, met lood voor lager smeltpunt. 
- dunne draad

  linkjes naar webshops voor onderdelen in links directory

  Tips voor bouw:
  Soldeer de header voor de arduino pro mini op zo'n manier dat je in de uiteindelijke print montage er nog bij kan met de programmer jumper kabels.
  Gebruik NOOIT de 5V, dat kan de boel slopen
  Als je soldeert, doe dit in een geventileerde ruimte

  Aansluitingen:
  3.7V kan direct op pin vcc van arduino
  sensors op i2c pinnen, sda en scl  

 