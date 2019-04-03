# SA_nfc_mp3_player_realTag_WDT_IRQ3

Programme développé par Simon Buoro et Lionel Delteil pour l'exposition Age of Classics (AoC) du Musée Saint-Raymond de Toulouse (MSR) dans le cadre d'un partenariat entre le MSR et Science Animation (SA).

Le MSR a demandé à SA de concevoir un scénario de visite pour l'exposition AoC, ainsi qu'un dispositif de visite permettant de suivre le scénario.

Nous avons développé un "kit de fouille" répondant à plusieurs fonctions :
- détection de tags RFID/NFC pour déclencher des sons
- led UV pour faire apparaitre de l'encre invisible
- miroir
- possibilité d'accrocher le carnet de visite et crayon gris pour écrire sur le carnet
- autonome en énergie avec recharge par induction dans son emplacement de rangement

Le programme SA_NFC_mp3_player_realTag_WDT_IRQ3 permet la lecture de tag RFID/NFC et le déclenchement de sons mp3 selon le tags détectés.

Le montage électronique est composé :
- d'une carte Arduino UNO rev3
- d'un shield Tune MP3 de Snootlab
- d'un shield PN532NFC d'Adafruit
- d'un aplificateur mono 2,5W PAM8302A d'Adafruit
- d'un speaker mono 3W 4Ohm d'Adafruit
- d'un bouton reset
- une powerbank MakeZens 3200mAh

/!\ ATTENTION
Le shield PN532NFC a été modifié de la manière suivante :
- la piste entre le pin 2 et l'IRQ a été coupée
- un strap a été soudé entre la piste 3 et l'IRQ
En effet, les librairies Tune et Adafruit_PN532 utilisent toutes les deux le pin 2 pour communiquer avec la carte Arduino. On choisit donc d'utiliser dans ce cas le pin 3 pour l'IRQ du shield PN532NFC.

Nous avons rajouté un bouton reset sur le kit de fouille afin de régler le problème suivant : lorsqu'on enlève le kit de fouille de son poste de recharge, il y a une chute de tension qui fait redémarrer la carte Arduino. Or, la carte Arduino redémarre mal la plupart du temps. Le bouton reset permet de redémarrer manuellement la carte et lui permet de fonctionner à nouveau.
Certaines variables du programme sont enregistrées dans la mémoire EEPROM afin de permettre au visiteur de reprendre la visite là où il en était si le problème survient lors de son parcours.
Un watchdog time (WDT) a également été mis en place pour palier à ce problème s'il survient de manière aléatoire.

Le programme SA_NFC_mp3_player_realTag_WDT_IRQ3 est commenté pour comprendre son fonctionnement.
/!\ si vous souhaitez le réutiliser à modifier les valeurs des tags RFID/NFC selon ceux que vous possédez (chaque tag RFID/NFC a sa propre identité !).

