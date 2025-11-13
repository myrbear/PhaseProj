#ifndef GAME_OBJECT

#include "../Headers/collider.h"


#define GAME_OBJECT

class GameObject
{
    public:
        GameObject(int id);
        Collider collider;

        int GetId();
        
    private:
        int id;

};



#endif // GAME_OBJECT