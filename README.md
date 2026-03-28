# IRC Server

## Description du Projet

Ce projet implémente un serveur IRC (Internet Relay Chat) en C++. Il permet aux utilisateurs de se connecter, de rejoindre des canaux, d'envoyer des messages privés et de gérer des commandes IRC de base. Le serveur gère également un bot intégré pour des fonctionnalités supplémentaires.

## Notions Abordées

- Programmation orientée objet en C++
- Programmation réseau avec sockets TCP
- Protocole IRC (RFC 1459)
- Gestion des connexions multiples et non-bloquantes
- Gestion des utilisateurs et des canaux
- Implémentation de commandes IRC (NICK, USER, JOIN, PART, PRIVMSG, etc.)
- Gestion des signaux pour l'arrêt propre du serveur
- Utilisation de bibliothèques externes (libcurl pour les requêtes HTTP potentielles, OpenSSL pour la sécurité)

## Prérequis

Avant de lancer le programme, assurez-vous d'avoir installé les éléments suivants :

- Un compilateur C++ supportant le standard C++98 (comme g++)
- Les bibliothèques suivantes :
  - libcurl (pour les requêtes HTTP)
  - OpenSSL (pour la cryptographie)
  - zlib (pour la compression)

Les bibliothèques libcurl et OpenSSL sont fournies pré-compilées dans les dossiers `libcurl/` et `openssl-built/` du projet.

## Installation et Compilation

1. Clonez ou téléchargez le projet dans un répertoire local.
2. Assurez-vous que les bibliothèques pré-compilées sont présentes dans `libcurl/libcurl-built/` et `openssl-built/`.
3. Ouvrez un terminal dans le répertoire racine du projet.
4. Exécutez la commande `make` pour compiler le projet :
   ```
   make
   ```
   Cette commande génère l'exécutable `ircserv`.

## Utilisation

Après la compilation, lancez le serveur avec la commande suivante :

```
./ircserv <port> <mot_de_passe>
```

- `<port>` : Le numéro de port sur lequel le serveur écoutera les connexions (doit être entre 1 et 65535).
- `<mot_de_passe>` : Le mot de passe requis pour les connexions administrateur ou spéciales.

Exemple :
```
./ircserv 6667 monmotdepasse
```

Le serveur démarrera et écoutera les connexions entrantes sur le port spécifié. Les clients IRC peuvent alors se connecter en utilisant un client IRC standard.

### Commandes de Nettoyage

- `make clean` : Supprime les fichiers objets générés lors de la compilation.
- `make fclean` : Supprime les fichiers objets et l'exécutable.

## Commandes IRC Disponibles

Le serveur supporte les commandes IRC suivantes :

- **PASS** : Authentifie l'utilisateur avec un mot de passe.
- **NICK** : Change le pseudonyme de l'utilisateur.
- **USER** : Définit les informations de l'utilisateur (nom d'utilisateur, nom réel).
- **JOIN** : Rejoint un canal. Supporte les canaux avec mot de passe, limites d'utilisateurs et mode invitation seulement.
- **PART** : Quitte un canal.
- **PRIVMSG** : Envoie un message privé à un utilisateur ou un canal.
- **TOPIC** : Définit ou affiche le sujet d'un canal.
- **KICK** : Expulse un utilisateur d'un canal (réservé aux opérateurs).
- **INVITE** : Invite un utilisateur dans un canal en mode invitation seulement.
- **MODE** : Change les modes d'un canal ou d'un utilisateur (opérateur, etc.).
- **PING** : Vérifie la connectivité avec le serveur.

Ces commandes suivent le protocole IRC standard et peuvent être utilisées via un client IRC connecté au serveur.

## Exemples d'Utilisation

Voici quelques exemples d'utilisation des commandes IRC dans un client IRC connecté au serveur :

### Connexion et Authentification
```
/PASS monmotdepasse
/NICK MonPseudo
/USER MonUser 0 * :Mon Nom Réel
```

### Rejoindre un Canal
```
/JOIN #general
/JOIN #secret motdepasse
```

### Envoyer des Messages
```
/PRIVMSG #general Salut tout le monde !
/PRIVMSG utilisateur Salut, message privé !
```

### Gérer un Canal
```
/TOPIC #general Nouveau sujet du canal
/KICK #general utilisateur Raison de l'expulsion
/INVITE utilisateur #prive
/MODE #general +o utilisateur  (donne le statut opérateur)
/MODE #general +k motdepasse  (protège le canal par mot de passe)
```

### Quitter un Canal
```
/PART #general Au revoir !
```

### Vérifier la Connexion
```
/PING serveur
```

Ces exemples montrent comment interagir avec le serveur IRC. Utilisez un client IRC comme irssi, HexChat ou mIRC pour vous connecter au serveur sur le port spécifié.

## Contributeurs

- Léo Ye
- Rémi Grangeon
- Maxime Grodin