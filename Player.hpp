//
// Created by timti on 07/04/2025.
//

#ifndef PLAYER_H
#define PLAYER_H

#endif //PLAYER_H

struct position
{
    int x, y;
};

class Player{

    private : 
        position pos;
        float orientation;
        int pv;
    
    public :

        void setPosition(position NewPos){};
        void setPosition(position NewPos, float NewOrientation){};
        void setPv(int NewPv){};

        position getPosition(){};
        float getOrientation(){};
        int getPv(){};



};
