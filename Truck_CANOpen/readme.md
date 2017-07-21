### Etape 1 :

- Creer une structure avec les 3 infos suivantes :
  - arbitrage
  - DLC
  - donnees
  
 - Creer un sketch qui permet d'envoyer un message en fct de ces 3 données.
 
 - Recevoir le message et l'afficher
 
##### A faire !
 
 Tester les différentes tailles de messages, ce qu'il se passe en cas de conteneur trop grand ou trop petit.
 Attention aux types de variables et décalages de bytes.
 
### Etape 2 :
 
 - Rajouter le CRC et le ACK
 
### Etape 3 :
 
 - Traiter un défaut de CRC
 - Renvoyer une réponse
 
### Etape 4 (aider par moi) :
 
 - Separer le code par fonction
 - Si beaucoup de temps libre : transformer la structure en objet


### Explications bonus :

 - Il faut voir les structures comme des petits objets.
 - Le plus simple pour savoir si ta stucture fonctionne bien c'est de créer une fonction dans le main qui te permet d'afficher ta structure.
 - Pour afficher du binaire tu peux utiliser Serial.println(***variable***,BIN)
 - L'avantage étant que quand cette fonction sera prête elle pourra t'aider à lire le message du côté du receveur
