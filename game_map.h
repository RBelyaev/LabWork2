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
	Map(int width, int height, int x, int y, Direction view, int hp, int bul, int gren) : width(width), height(height), walls(), enemies(),
		towers(), monsters(), bullets(), grenades(), chests(), med_kits(), player(Player(x, y, view, hp, bul, gren))
	{
		game_map = std::vector<std::vector<char>>(height, std::vector<char>(width, '.'));
	}
	
	void add_wall(int x, int y) { walls.push_back(Wall(x, y)); }

	void add_tower(int x, int y) { towers.push_back(Tower(x, y, 1, 1)); }

	void add_monster(int x, int y) { monsters.push_back(Monster(x, y, 1, 1)); }

	void add_bullet(int x, int y, Direction direction) 
	{
		Bullet bullet(x, y, direction);
		bullets.push_back(bullet);
	}

	void add_grenade(int x, int y, Direction direction, int timer) 
	{
		Grenade grenade(x, y, direction, timer);
		grenades.push_back(grenade);
	}

	void add_chest(int x, int y, int coins, int bullets, int grenades) 
	{
		chests.push_back(Chest(x, y, coins, bullets, grenades));
	}

	void add_med_kit(int x, int y, int health_points)
	{
		med_kits.push_back(MedKit(x, y, health_points));
	}

	void get_enemies() 
	{
		enemies.clear();
		enemies.insert(enemies.end(), towers.begin(), towers.end());
		enemies.insert(enemies.end(), monsters.begin(), monsters.end());
	}

	
	void update_bullets() 
	{
		bool Destroyed = false;

		for (auto bullet = bullets.begin(); bullet != bullets.end();) 
		{
			Destroyed = false;
			bullet->move();
			
			for (auto monster = monsters.begin(); monster != monsters.end();)
				{
					if (bullet->hit_monster(*monster))
					{
						monster->damage();
						Destroyed = true;
						if (monster->get_health_points() == 0) monster = monsters.erase(monster);
						break;
					}
					else monster++;

				}
	
			if(bullet->get_x() < 0 || bullet->get_x() >= width || bullet->get_y() < 0 || bullet->get_y() >= height || game_map[bullet->get_y()][bullet->get_x()] != '.')
				Destroyed = true;
			
			if(Destroyed) bullet = bullets.erase(bullet);
			else bullet++;
		}
			
	}
	

	void update_grenades()
	{
		bool Destroyed = false;

		for (auto grenade = grenades.begin(); grenade != grenades.end();)
		{
			Destroyed = false;
			grenade->move();

			if (grenade->explosion())
			{
				Destroyed = true;
				for (auto monster = monsters.begin(); monster != monsters.end();)
				{
					if (grenade->hit_enemy(*monster))
					{
						monster->damage();
						if (monster->get_health_points() == 0) monster = monsters.erase(monster);
					}
					else monster++;
				}
				for (auto tower = towers.begin(); tower != towers.end();)
				{
					if (grenade->hit_enemy(*tower))
					{
						tower->damage();
						if (tower->get_health_points() == 0) tower = towers.erase(tower);
					}
					else tower++;
				}
				for(int i = grenade->get_x() - 1; i <= grenade->get_x() + 1; i++)
				{
					for(int j = grenade->get_y() - 1; j <= grenade->get_y() + 1; j++)
					{
						if(i>=0 && i < width && j >= 0 && j < height)
							game_map[j][i] = '@';
					}
				}
			}
		
			if(grenade->get_x() < 0 || grenade->get_x() >= width || grenade->get_y() < 0 || grenade->get_y() >= height || game_map[grenade->get_y()][grenade->get_x()] != '.')
				Destroyed = true;
			if(Destroyed) grenade = grenades.erase(grenade);
			else grenade++;
		}
		
	}

	void update_chest()
	{
		for (auto chest = chests.begin(); chest != chests.end(); chest++)
		{
			if (player.chest_check(*chest))
			{
				player.take_bullets(chest->get_bullets());
				player.take_coins(chest->get_coins());
				player.take_grenades(chest->get_grenades());
				chest = chests.erase(chest);
				break;
			}
		}
	}

	void update_med_kits()
	{
		for (auto med_kit = med_kits.begin(); med_kit != med_kits.end(); med_kit++)
		{
			if (player.med_kit_check(*med_kit))
			{
				player.take_health_points(med_kit->get_health_points());
				med_kit = med_kits.erase(med_kit);
				break;
			}
		}
	}

	
	void move_check(int xOffset, int yOffset, void (Player::*moveFunction)()) 
	{
    	int next_y = player.get_y() + yOffset;
    	int next_x = player.get_x() + xOffset;
    
		if(next_y < height && next_y >= 0 && next_x < width && next_x >= 0 && game_map[next_y][next_x] != '#'){
			(player.*moveFunction)();
		}
	}
	

	
	void player_input() 
	{
		int ch = 0;
		initscr();
		keypad(stdscr, 1);
		noecho();
		curs_set(0);
		ch = getch();

		if (ch == int('w') || ch == int('W')) 
		{
       	 	move_check(0, -1, &Player::move_up);
   		}
   		if (ch == int('s') || ch == int('S')) 
		{
       		move_check(0, 1, &Player::move_down);
  		}
    	if (ch == int('a') || ch == int('A')) 
		{
        	move_check(-1, 0, &Player::move_left);
    	}
    	if (ch == int('d') || ch == int('D')) 
		{
        	move_check(1, 0, &Player::move_right);
    	}
    	if (ch == 'f' || ch == 'F') 
		{
        	this->add_bullet(player.get_x(), player.get_y(), player.get_view());
    	}
    	if (ch == 'g' || ch == 'G') 
		{
        	this->add_grenade(player.get_x(), player.get_y(), player.get_view(), 4);
    	}


		if (ch == 'i' || ch == 'I') 
		{
     		player.view_up();
    	}
		if (ch == 'k' || ch == 'K') 
		{
        	player.view_down();
   		}
		if (ch == 'j' || ch == 'J') 
		{
        	player.view_left();
    	}
		if (ch == 'l' || ch == 'L') 
		{
        	player.view_right();
    	}
		endwin();

	}
	

	void render_map() 
	{
		for (int y = 0; y < height; y++) 
		{
			for (int x = 0; x < width; x++) 
			{
				game_map[y][x] = '.';
			}
		}
		for (const auto& wall : walls) 
			game_map[wall.get_y()][wall.get_x()] = wall.get_texture();


		for (Enemy enemy : enemies) 
			game_map[enemy.get_y()][enemy.get_x()] = enemy.get_texture();


		for (const auto& bullet : bullets) 
			game_map[bullet.get_y()][bullet.get_x()] = bullet.get_texture();


		for (const auto& grenade : grenades) 
			game_map[grenade.get_y()][grenade.get_x()] = grenade.get_texture();


		for (const auto& chest : chests) 
			game_map[chest.get_y()][chest.get_x()] = chest.get_texture();


		for (const auto& mk : med_kits) 
			game_map[mk.get_y()][mk.get_x()] = mk.get_texture();


		game_map[player.get_y()][player.get_x()] = player.get_texture();
	}


    bool render_game() 
	{
		render_map();
		get_enemies();
        for (Enemy enemy : enemies) 
		{
            if (enemy.player_check(player)) 
			{
                game_map[player.get_y()][player.get_x()] = 'X';
                std::cout << "Game Over!" << std::endl;
                return false; 
            }
        }
        
        update_bullets();
        update_grenades();


        update_chest();
		update_med_kits();


		int x = 0, y = 0;
        for (auto& monster : monsters) 
		{
			y = monster.get_y();
			x = monster.get_x();
            monster.move_random();
			if(monster.get_x() < 0 || monster.get_x() >= width || monster.get_y() < 0 || monster.get_y() >= height)
			{
				monster.set_x(x);
				monster.set_y(y);
			}
        }


		std::cout << std::endl;
        for (int y = 0; y < height; y++) 
		{
            for (int x = 0; x < width; x++) 
			{
                std::cout << game_map[y][x];
            }
            std::cout << std::endl;
        }
		
        std::cout << "Bullets: " << player.get_bullets() << std::endl;
		std::cout << "Grenades: " << player.get_grenades() << std::endl;
		std::cout << "Health Points: " << player.get_health_points() << std::endl;
		std::cout << "Coins: " << player.get_coins() << std::endl;
		
		
		
        player_input();
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
		clear();
		
		return true;
    }


    void start(){
		srand(time(0));
		bool game = true;
        while (game){
            game = render_game();
        }
    }
};
