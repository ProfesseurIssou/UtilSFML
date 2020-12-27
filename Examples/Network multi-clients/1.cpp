#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "UtilSFML.h"


void serverTest(){
  Server server;        //On cree le serveur
  bool done = false;
  int port = 2000;
  server.start(port);   //On demarre le serveur
  while(!done){
    if(server.newClientRequest()){  //Si un nouveau client tente de se connecter
      int newClientPos;               //Position du nouveau client
      newClientPos = server.acceptClient();//On accepte le nouveau client
      sf::Packet receivePacket;       //On prepare le packet a recevoir
      receivePacket = server.receivePacket(newClientPos);//On recupere le packet du nouveau client
      std::string pseudo;             //Le pseudo du nouveau client
      receivePacket >> pseudo;        //On extrait le pseudo du packet
      std::cout << pseudo << " connected\n";
    }else{//Si il n'y a pas de nouveau client, alors
      int posClient;                            //Position du client qui nous envoie une données
      posClient = server.getPosClientRequests();//On recupere la position du client qui nous envoie une données
      if(posClient != -1){                      //Si on a bien un client qui nous envoie une données
        sf::Packet receivePacket;                 //On prepare le packet a recevoir
        std::string pseudo;                       //Le pseudo du client qui nous envoie une données
        int x,y;                                  //Position du client qui nous envioe une données
        receivePacket = server.receivePacket(posClient);//On recupere le packet que le client nous a envoyer
        receivePacket >> pseudo >> x >> y;        //On extrait le packet
        std::cout << pseudo << " " << x << " " << y << std::endl;
        sf::Packet sendPacket;                    //On prepare un packet a envoyer
        sendPacket << pseudo << x << y;           //On met les données dans le packet
        for(int i=0;i<server.clientsList.size();i++){//Pour chaque client de la liste
          if(i!=posClient){                         //Si ce n'est pas le client de base
            server.sendPacket(i,sendPacket);          //On envoie le packet au client
          }
        }
      }
    }
  }
}

int clientTest(){
  Client client;                  //On cree le client
  bool done = false;              //Pour la boucle
  int port = 2000;                //Le port du server
  std::string ip = "192.168.1.7"; //L'ip du server
  std::string pseudo;             //Pseudo du client

  bool good = client.connect(ip,port);//Connexion au server
  std::cout << good;
  if(!good){  //Si on arrive pas a se connecter
    return -1;  //Erreur
  }

  std::cout << "pseudo :";
  std::cin >> pseudo;

  sf::Packet sendPacket;
  sendPacket << pseudo;
  client.sendPacket(sendPacket);

  int x = 0,
      y = 0;

  sf::Clock timer;

  while(!done){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) y--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) y++;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x++;

    if(timer.getElapsedTime().asMilliseconds()>30){
      sf::Packet sendPacket;
      sendPacket << pseudo << x << y;
      client.sendPacket(sendPacket);
      timer.restart();
    }
    std::cout << x << " " << y << std::endl;
    if(client.haveReceivePacket()){ //Si le client a recus un packet
      std::string otherPseudo;
      int otherX,
          otherY;
      client.receivePacket >> otherPseudo >> otherX >> otherY;//On extrait le packet
      std::cout << otherPseudo << " > " << otherX << " " << otherY << std::endl;
    }
  }
  return 0;
}


int main(){
  char who;
	std::cout << "Do you want to be a server (s) or a client (c) ? ";
	std::cin  >> who;

	if (who == 's'){//SERVEUR
    serverTest();
	}else{//CLIENT
	  clientTest();
  }
  return 0;
}

