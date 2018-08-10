/*
  Le code ci-dessous ainsi que les fichiers sounddata.h et speaker_pcm.h doivent être présents lors du transfert sur votre
  Arduino. Ce fichier gère le matériel qui est branché au Arduino. sounddata.h contient les données audio à jouer et
  speaker_pcm.h s'occupe de transformer ces données audio en signal pour le speaker.

  Sophie Déziel <courrier@sophiedeziel.com>
  twitch.tv/sophiedeziel
*/

#define SWITCH_PIN 2          // Sur le Arduino nano, seule la pin 2 ou 3 peuvent servir avec les interupts de ce code
#define LED_PIN 7             // Peut seulement n'importe quelle sauf la 2 ou celle choisie pour le speaker
#define SPEAKER_PIN 11        // Peut seulement être 3 ou 11, selon les possibilités de la puce

#include "speaker_pcm.h"

// Variables et constances utilisées pour gérer le bouton
const int debounceTime = 200;
long last_play;

/*
  Les fonctions setup et loop sont toujours présentes dans les programmes Arduino.
  Setup est exécuté une seule fois au démarage et sert surtout pour la configuration des
  pins, initialiser des composantes et autres opérations du genre.

  Ici, on détermine les modes des trois pins qu'on va utiliser. Ensuite, on s'assure
  qu'aucun voltage n'est appliqué par la puce à la pin du bouton. Finalement, on déclare que
  lorsque le bouton ferme le circuit, la fonction switchon doit etre exécuté.

*/
void setup()
{
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(LED_PIN,     OUTPUT);
  pinMode(SWITCH_PIN,  INPUT);

  digitalWrite(SWITCH_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchon, RISING);
}

// Nous n'avons rien à exécuter dans loop, puisque le 'attachInterrupt' s'occupe de tout
void loop()
{
}

void switchon()  {
  if (millis() - last_play > debounceTime) {    // Pour s'assurer que le bouton n'enregistre qu'un clic à la fois
    last_play = millis();

    // on allume les LED
    digitalWrite(LED_PIN, HIGH);

    // Le fichier sonore commence a jouer
    startPlayback();

    // Nous prévoyons changer le comportement du bouton pour attendre qu'il soit relaché
    // Lorsque le bouton est relaché, switchoff doit etre exécuté
    detachInterrupt(digitalPinToInterrupt(SWITCH_PIN));
    attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchoff, FALLING);
  }
}

void switchoff() {
  if (millis() - last_play > debounceTime) {
    last_play = millis();

    // On éteint les LED
    digitalWrite(LED_PIN, LOW); 

    // Le fichier sonore arrête de jouer
    stopPlayback();             

    // On recommence à attendre que le bouton soit pressé
    detachInterrupt(digitalPinToInterrupt(SWITCH_PIN));
    attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchon, RISING);
  }
}
