//#pragma once

#include "cstdlib"

enum class Direction {
    kUP,
    kDown,
    kRight,
    kLeft
};


class GameObject {
private:
    int x;
    int y;
    char texture; 
protected:
        void set_texture(char texture) { this->texture = texture; }
public:
    GameObject(int x, int y, char texture) : x(x), y(y), texture(texture) {}

    int get_x() const { return x; }
    int get_y() const { return y; }
    char get_texture() const { return texture; }

    void set_x(int x) { this->x = x; }
    void set_y(int y) { this->y = y; }

};

class Wall : public GameObject {
public:
    Wall(int x, int y) : GameObject(x, y, '#') {};
};

class Chest : public GameObject {
private:
    int coins;
    int bullets;
    int grenades;
protected:
    void set_coins(int new_coins) { this->coins = new_coins; }
    void set_bullets(int new_bullets) { this->bullets = new_bullets; }
    void set_grenades(int new_grenades) { this->grenades = new_grenades; }
public:
    Chest(int x, int y, int coins, int bullets, int grenades, char texture = '?' ) : GameObject(x, y, texture), coins(coins), bullets(bullets), grenades(grenades) {};
    int get_coins() const { return coins; }
    int get_bullets() const { return bullets; }
    int get_grenades() const { return grenades; }
};

class MedKit : public GameObject {
private:
    int health_points;
public:
    MedKit(int x, int y, int health_points) : GameObject(x, y, '+'), health_points(health_points) {}
    int get_health_points() const { return health_points; }
};


class Player : public Chest {
private:
    int health_points;
    Direction view;
public:
    Player(int x, int y, Direction view, int health_points, int bullets, int grenades) : Chest(x, y, 0, bullets, grenades, '>'), health_points(health_points), view(view) 
    {
        if (view == Direction::kDown)
            set_texture('v');
        else if (view == Direction::kLeft)
            set_texture('<');
        else if (view == Direction::kUP) set_texture('^');
    };
    
    int get_health_points() const { return health_points; }

    void move_up() 
    {
        int NewY = get_y() - 1;
        set_y(NewY);
    }
    void move_down() 
    {
        int NewY = get_y() + 1;
        set_y(NewY);
    }
    void move_right() 
    {
        int NewX = get_x() + 1;
        set_x(NewX);
    }
    void move_left() 
    {
        int NewX = get_x() - 1;
        set_x(NewX);
    }


    Direction get_view() const { return view; }

    void view_up() 
    { 
        set_texture('^'); 
        view = Direction::kUP;
    }
    void view_down() 
    {
         set_texture('v'); 
        view = Direction::kDown;
    }
    void view_left() 
    { 
        set_texture('<');
        view = Direction::kLeft;
    }
    void view_right() 
    { 
        set_texture('>'); 
        view = Direction::kRight;
    }


    void take_coins(int given_coins) 
    { 
        int NewCoins = get_coins() + given_coins; 
        set_coins(NewCoins);
    }
    void take_bullets(int given_bullets)
    {
        int NewBullets = get_bullets() + given_bullets;
        set_bullets(NewBullets);
    }
    void take_grenades(int given_grenades) 
    { 
        int NewGrenades = get_grenades() + given_grenades;
        set_grenades(NewGrenades);
    }
    void take_health_points(int given_health_points) 
    { 
        health_points += given_health_points; 
    }
     


    bool chest_check(Chest& chest) 
    {
        int ChestX = chest.get_x();
        int ChestY = chest.get_y();

        if (get_x() == ChestX && get_y() == ChestY)
            return true;
        return false;
    }

    bool med_kit_check(MedKit& med_kit)
    {
        int MedKitX = med_kit.get_x();
        int MedKitY = med_kit.get_y();

        if (get_x() == MedKitX && get_y() == MedKitY)
            return true;
        return false;
    }

};


class Enemy : public GameObject {
private:
	int health_points;
	int affected_area;
public:
    Enemy(int x, int y, char texture, int health_points, int affected_area) : GameObject(x, y, texture), health_points(health_points), affected_area(affected_area) {};

	void damage()
	{
		health_points--;
	}

	int get_health_points() const { return health_points; }
    bool player_check(Player& player) 
	{
		int EnemyX = get_x();
		int EnemyY = get_y();
		int PlayerX = player.get_x();
		int PlayerY = player.get_y();

        if ((abs(EnemyX - PlayerX) + abs(EnemyY - PlayerY)) <= affected_area)
            return true;
        return false;
    }
};


class Tower : public Enemy {
public:
    Tower(int x, int y, int health_points, int affected_area) : Enemy(x, y, 'T', health_points, affected_area) {};
};


class Monster : public Enemy {
public:
    Monster(int x, int y, int health_points, int affected_area) : Enemy(x, y, 'M', health_points, affected_area) {}
    void move_up() 
    {
        int NewY = get_y() + 1;
        set_y(NewY);
    }
    void move_down() 
    {
        int NewY = get_y() + 1;
        set_y(NewY);
    }
    // Method for random monster movement
    void move_random() 
    {
        // We will generate a random number from 1 to 3, and depending on the number
        // We will simulate the steps of the monster
        int MoveSeed = rand() % 3;
        if (MoveSeed == 0)
            move_up();
        if (MoveSeed == 1)
            move_down();
    }
};


class Projectile : public GameObject {
private:
    Direction direction;
public:
    Projectile(int x, int y, char texture, Direction direction) : GameObject(x, y, texture), direction(direction) //, isAliveV(true),
    {};
    void move()
    {
        int X = get_x();
        int Y = get_y();
        if (direction == Direction::kDown)
            set_y(Y + 1);
        else if (direction == Direction::kLeft)
            set_x(X - 1);
        else if (direction == Direction::kRight)
            set_x(X + 1);
        else
            set_y(Y - 1);
    }
    Direction get_direction() const { return direction; }

};

class Bullet : public Projectile {
public:
    Bullet(int x, int y, Direction direction) : Projectile(x, y, 'B', direction) {};

    // The method of checking for the proximity of the bullet to the monster
    bool hit_monster(Monster& monster) {
        int BulletX = get_x();
        int BulletY = get_y();
        int MonsterX = monster.get_x();
        int MonsterY = monster.get_y();

        // If the coordinates of the bullet and the monster match
        if (BulletX == MonsterX && BulletY == MonsterY) {
            return true;
        }

        return false;
    }

};

class Grenade : public Projectile {
private:
    int timer;
public:
    Grenade(int x, int y, Direction direction, int timer) : Projectile(x, y, 'G', direction), timer(timer) {};

    bool explosion()
    {
        if (timer == 0)
            return true;
        timer--;
        return false;
    }

    bool hit_enemy(Enemy& enemy) 
	{
        int GrenadeXl = get_x() - 1;
		int GrenadeXr = get_x() + 1;
		int GrenadeYd = get_y() - 1;
		int GrenadeYu = get_y() + 1;
        int EnemyX = enemy.get_x();
        int EnemyY = enemy.get_y();

        if (GrenadeXl <= EnemyX && EnemyX <= GrenadeXr && GrenadeYd && EnemyY <= GrenadeYu)
		{
            return true;
        }

        return false;
    }
};
