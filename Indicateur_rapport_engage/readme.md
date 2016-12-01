# Projet de l'indicateur de rapport engagé sur une moto

Liste des personnes associées au projet :

Description du projet:

Ce projet consiste à fabriquer un afficheur de rapport engagé pour moto avec Arduino.
L'afficheur (7 segments) doit pouvoir afficher 0 pour le neutre (point mort) et 1,2,3,4,5 pour les vitesses (ou rapports engagés).
Pour cela, je dois utiliser plusieurs capteurs, dont un qui est déjà sur la moto, c'est celui du neutre. En position neutre, un voyant vert sur le tableau de bord s'allume. La condition donc pour afficher 0 (pour le neutre) sera quand ce capteur est à l'état haut (12 volts) plus d'une seconde. Pour afficher 1,2,3,4,5 (il y a 5 vitesses sur cette moto), on installera 2 capteurs magnétique proche du selecteur de vitesse et 1 aimant sur le selecteur.

Fonctionnement de passage des vitesses sur une moto: 
Initialement la moto est en neutre.
Pour passer la 1ère vitesse, il suffit d'appuyer vers le bas sur le selecteur.
Pour repasser le neutre, il suffit alors de lever le selecteur, mais pas à fond, sinon la 2ème vitesse est passée (le selecteur peut soit monter, soit descendre, mais il revient toujours en position initiale).
Pour passer la 3ème, il suffit de le lever encore une fois. Et ainsi de suite pour la 4ème, et la 5ème.
En 5ème si le selecteur est encore levé, il ne se passe rien. A l'inverse, pour rétrograder, il suffit d'apuyer sur le sélecteur, autant de fois qu'il faut pour rétrograder toutes les vitesses.
Le neutre se situant entre la 1ère et la 2ème, si le 2ème rapport en engagé, en appuyant pas à fond mais légèrement, le neutre est engagée. Mais on ne le fait jamais, on passe toujours par la 1ère pour passer ensuite le neutre. En fait il y a un "cran" intermédiaire entre la 1ère et la 2ème.

Remarque: Il faut savoir qu'on ne peut pas sauter une vitesse, elle se passe une par une et dans l'ordre sauf dans le cas de la 2ème où l'on peut directement passer au neutre sans passer par la 1ère.

Rappel du fonctionnement moto : Mouvement du levier de vitesse haut/bas, 1ère en bas les autres en haut, point mort entre la 1ère et la 2ème à détecter par 2 capteurs magnétique. Dans l’ordre : 102345, 0=Point mort, Info point mort disponible par contacteur d’origine.

Liste des fonctionnalités:
-2 Interruptions rising ;
-Detection point mort ;
-7 Seg update.

Matériel électronique:
-Arduino Uno ou Nano ;
-2 Capteurs magnétique normalement ouvert ;
-Afficheur 7 Segments ;
-2 Convertisseur de tension 12V-5V.

