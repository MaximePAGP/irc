#!/bin/bash

# Configuration du serveur IRC
HOST="localhost"
PORT="6667"

# Fonction pour envoyer une commande au serveur
send_command() {
    local SOCKET=$1
    local COMMAND=$2
    echo -e "$COMMAND\r\n" | nc $HOST $PORT > $SOCKET &
    sleep 1
}

# Fonction pour lire les réponses du serveur
read_response() {
    local SOCKET=$1
    echo "=== Réponse du serveur ==="
    cat $SOCKET
    echo "==========================="
}

# Test du serveur IRC
test_irc_server() {
    echo "Démarrage des tests IRC..."

    # Création de sockets temporaires pour les utilisateurs
    SOCKET_USER1=$(mktemp)
    SOCKET_USER2=$(mktemp)

    # Connexion du premier utilisateur
    echo "Connexion de User1..."
    send_command $SOCKET_USER1 "NICK User1"
    send_command $SOCKET_USER1 "USER user1 0 * :User One"
    read_response $SOCKET_USER1

    # Connexion du deuxième utilisateur
    echo "Connexion de User2..."
    send_command $SOCKET_USER2 "NICK User2"
    send_command $SOCKET_USER2 "USER user2 0 * :User Two"
    read_response $SOCKET_USER2

    # User1 crée et rejoint un canal
    echo "User1 rejoint le canal #testchannel..."
    send_command $SOCKET_USER1 "JOIN #testchannel"
    read_response $SOCKET_USER1

    # User2 rejoint le même canal
    echo "User2 rejoint le canal #testchannel..."
    send_command $SOCKET_USER2 "JOIN #testchannel"
    read_response $SOCKET_USER2

    # User1 envoie un message dans le canal
    echo "User1 envoie un message dans le canal..."
    send_command $SOCKET_USER1 "PRIVMSG #testchannel :Hello, this is User1!"
    read_response $SOCKET_USER1
    read_response $SOCKET_USER2

    # User2 envoie un message dans le canal
    echo "User2 envoie un message dans le canal..."
    send_command $SOCKET_USER2 "PRIVMSG #testchannel :Hello, this is User2!"
    read_response $SOCKET_USER1
    read_response $SOCKET_USER2

    # User1 quitte le canal
    echo "User1 quitte le canal..."
    send_command $SOCKET_USER1 "PART #testchannel"
    read_response $SOCKET_USER1
    read_response $SOCKET_USER2

    # Nettoyage des sockets temporaires
    rm -f $SOCKET_USER1 $SOCKET_USER2

    echo "Tests terminés."
}

# Exécution des tests
test_irc_server
