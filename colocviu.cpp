//
// Created by marac on 4/29/2025.
//
#include<iostream>
using namespace std;
 class Item{
   protected:
     const int Id;
     static int counterId;
   public:
       };
   int Item::counterId=1;
   class Zid: public Item{
     private:
       double lungime;
       double inaltime;
       double grosime;
     public:
       Zid();
       };
      Zid::Zid():Id(counterId++){
        lungime=0;
        inaltime=0;
        grosime=0;
      }
      class Turn: public Item{
        private:
          double putereLaser;
        public:
          Turn();
      };
     Turn::Turn():Item(){
       putereLaser=0;
     }
     class Robot: public Item{
       protected:
         int damage;
         int viata;
         int nivel;
     }