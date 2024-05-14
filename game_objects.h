#pragma once

#include <cstdlib>

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
    void set_texture(char texture);
public:
    GameObject(int x, int y, char texture);
    int get_x() const;
    int get_y() const;
    char get_texture() const;
    void set_x(int x);
    void set_y(int y);
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
    void set_coins(int new_coins);
    void set_bullets(int new_bullets);
    void set_grenades(int new_grenades);
public:
    Chest(int x, int y, int coins, int bullets, int grenades, char texture = '$' );
    int get_coins() const;
    int get_bullets() const;
    int get_grenades() const;
};



class MedKit : public GameObject {
private:
    int health_points;
public:
    MedKit(int x, int y, int health_points);
    int get_health_points() const;
};



class Player : public Chest {
private:
    int health_points;
    Direction view;

public:
    Player(int x, int y, Direction view, int health_points, int bullets, int grenades);
    int get_health_points() const;
    void move_up();
    void move_down();
    void move_right();
    void move_left();
    Direction get_view() const;
    void view_up();
    void view_down();
    void view_left();
    void view_right();
    void take_coins(int given_coins);
    void take_bullets(int given_bullets);
    void take_grenades(int given_grenades);
    void take_health_points(int given_health_points);
    bool chest_check(Chest& chest);
    bool med_kit_check(MedKit& med_kit);
};



class Enemy : public GameObject {
private:
	int health_points;
	int affected_area;

public:
    Enemy(int x, int y, char texture, int health_points, int affected_area);
	void damage();
	int get_health_points() const;
    bool player_check(Player& player);
};



class Tower : public Enemy {
public:
    Tower(int x, int y, int health_points, int affected_area);
};



class Monster : public Enemy {
public:
    Monster(int x, int y, int health_points, int affected_area);
    void move_up();
    void move_down();
    void move_random();
};



class Projectile : public GameObject {
private:
    Direction direction;

public:
    Projectile(int x, int y, char texture, Direction direction);
    void move();
    Direction get_direction() const;
};



class Bullet : public Projectile {
public:
    Bullet(int x, int y, Direction direction);
    bool hit_monster(Monster& monster);
};



class Grenade : public Projectile {
private:
    int timer;

public:
    Grenade(int x, int y, Direction direction, int timer);
    bool explosion();
    bool hit_enemy(Enemy& enemy);
};
