#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <ncurses.h>
#include "game_objects.h"

class Map {
private:
    int width;
    int height;
    std::vector<std::vector<char>> game_map;
    std::vector<Wall> walls;
    std::vector<Enemy> enemies;
    std::vector<Monster> monsters;
    std::vector<Tower> towers;
    std::vector<Bullet> bullets;
    std::vector<Grenade> grenades;
    std::vector<Chest> chests;
    std::vector<MedKit> med_kits;
    Player player;

public:
    Map(int width, int height, int x, int y, Direction view, int hp, int bul, int gren);
    
    void add_wall(int x, int y);
    void add_tower(int x, int y);
    void add_monster(int x, int y);
    void add_bullet(int x, int y, Direction direction);
    void add_grenade(int x, int y, Direction direction, int timer);
    void add_chest(int x, int y, int coins, int bullets, int grenades);
    void add_med_kit(int x, int y, int health_points);
    void get_enemies();
    void update_bullets();
    void update_grenades();
    void update_chest();
    void update_med_kits();
    void update_monsters();
	void move_check(int xOffset, int yOffset, void (Player::*moveFunction)());
    void player_input();
    void render_map();
    bool render_game();
    void start();
};
