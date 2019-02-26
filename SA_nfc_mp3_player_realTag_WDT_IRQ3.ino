/**************************************************************************/
/*
   Boitier/livret de visite Age of Classics (MSR - Science Animation)
   Dernière MAJ : 25/02/2019
*/
/**************************************************************************/

#include <Wire.h>
#include <Adafruit_PN532.h>
#include <Tune.h>
#include <SdFat.h>
#include <avr/wdt.h>

//#define DEBUGSERIE

Tune player;

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (3)
#define PN532_RESET (5)  // Not connected by default on the NFC Shield
// Use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

int TR = 0; //pour définir le numéro de la piste mp3 à jouer
int Q = 0; //pour connaitre le numéro de la quête initiée : 1-Odyssée ; 2-Héroïsme ; 3-Élégance
//variables permettant la lecture et la différenciation des tags RFID/NFC
int long id2;
int long id3;
int long valeur;

void setup() {
  wdt_enable(WDTO_2S);     // enable the watchdog

#ifdef DEBUGSERIE
  Serial.begin(115200);
  Serial.println("Hello!");
#endif

  //initialisation du shield Tune-mp3
  player.begin();
  player.setVolume(250);

  //initialisation lecteur RFID/NFC
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
#ifdef DEBUGSERIE
    Serial.print("Didn't find PN53x board");
#endif
    while (1); // halt
  }
  // Got ok data, print it out!
#ifdef DEBUGSERIE
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
#endif
  // configure board to read RFID tags
  nfc.SAMConfig();

#ifdef DEBUGSERIE
  Serial.println("Waiting for an ISO14443A Card ...");
#endif
}

void loop() {
  uint8_t success;
  uint8_t uid[4] = { 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  // Wait for an NTAG203 card.  When one is found 'uid' will be populated with
  // the UID, and uidLength will indicate the size of the UUID (normally 4 or 7)

  wdt_reset();
  delay(500);  //ce délai permet de laisser le temps au lecteur NFC de lire le tag NFC
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 500);
#ifdef DEBUGSERIE
  Serial.println(millis());
#endif

  //Serial.println(success);
  //nfc.PrintHex(uid, uidLength);

  if (success) {
    // Display some basic information about the card
    //Serial.println("Found an ISO14443A card");
    //Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    //Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    //Serial.println("");

    /* décalage de byte pour différencier les IDs des tags RFID/NFC :
       On extrait dans les variables idx les valeurs du tableau uid
       puis on créé une variable 'valeur' unique pour chaque carte
    */
    id2 = uid[2];
    id3 = uid[3];
    valeur = id2 * 1000 + id3;
#ifdef DEBUGSERIE
    Serial.println(valeur);
#endif

    switch (valeur) {
      case 116019: TR = 1; Q = 1;
#ifdef DEBUGSERIE
        Serial.println("Lancement quête Odyssée"); Serial.println("");
#endif
        break;
      case 46175: TR = 2; Q = 2;
#ifdef DEBUGSERIE
        Serial.println("Lancement quête Héroïsme"); Serial.println("");
#endif
        break;
      case 50147: TR = 3; Q = 3;
#ifdef DEBUGSERIE
        Serial.println("Lancement quête Élégance"); Serial.println("");
#endif
        break;
      case 97019:
        if (Q == 1) { //permet de ne lancer que l'audio de la quête initiée
          TR = 4;
#ifdef DEBUGSERIE
          Serial.println("Fin quête Odyssée"); Serial.println("");
#endif
        }
        break;
      case 37147:
        if (Q == 2) { //permet de ne lancer que l'audio de la quête initiée
          TR = 5;
#ifdef DEBUGSERIE
          Serial.println("Fin quête Héroïsme"); Serial.println("");
#endif
        }
        break;
      case 11245:
        if (Q == 3) { //permet de ne lancer que l'audio de la quête initiée
          TR = 6;
#ifdef DEBUGSERIE
          Serial.println("Fin quête Élégance"); Serial.println("");
#endif
        }
        break;
      default: break;
    }

    //lancement de la piste audio correspondante
    if (TR > 0 && TR < 7) {
      player.stopTrack(); //arrête la piste audio en cours avant de relancer la suivante
      player.playTrack(TR);
      TR = 0; //remise à zéro de TR pour éviter de relancer en boucle la piste audio
    }

  }
}
