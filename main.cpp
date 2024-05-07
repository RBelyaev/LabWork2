
#include "game_map.h"

int main()
{

	Map GAME(15, 15, 7, 2, Direction::kRight, 3, 10, 0);

	GAME.add_wall(3,1);
	GAME.add_wall(3,2);
	GAME.add_wall(3,3);
	GAME.add_wall(4,3);
	GAME.add_wall(5,3);
	GAME.add_wall(9,3);
	GAME.add_wall(10,3);
	GAME.add_wall(11,3);
	GAME.add_wall(11,2);
	GAME.add_wall(11,1);
	GAME.add_wall(6,5);

	GAME.add_monster(7,7);
   	GAME.add_monster(9, 7);
    GAME.add_tower(7, 12);
    
    GAME.add_chest(5, 2, 10, 10, 10);
    GAME.add_med_kit(4, 2, 3);
    
    GAME.start();
	

    return 0;
}