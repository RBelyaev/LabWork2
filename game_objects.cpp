#include "game_objects.h"

GameObject::GameObject(int x, int y, char texture) : x(x), y(y), texture(texture) {}

int GameObject::get_x() const {
    return x;
}

int GameObject::get_y() const {
    return y;
}

char GameObject::get_texture() const {
    return texture;
}

void GameObject::set_x(int x) {
    this->x = x;
}

void GameObject::set_y(int y) {
    this->y = y;
}

void GameObject::set_texture(char texture) {
    this->texture = texture;
}




Chest::Chest(int x, int y, int coins, int bullets, int grenades, char texture) :
    GameObject(x, y, texture), coins(coins), bullets(bullets), grenades(grenades) {}

int Chest::get_coins() const {
    return coins;
}

int Chest::get_bullets() const {
    return bullets;
}

int Chest::get_grenades() const {
    return grenades;
}

void Chest::set_coins(int new_coins) {
    this->coins = new_coins;
}

void Chest::set_bullets(int new_bullets) {
    this->bullets = new_bullets;
}

void Chest::set_grenades(int new_grenades) {
    this->grenades = new_grenades;
}



MedKit::MedKit(int x, int y, int health_points) : 
    GameObject(x, y, '+'), health_points(health_points) {}

int MedKit::get_health_points() const { return health_points; }



Player::Player(int x, int y, Direction view, int health_points, int bullets, int grenades) 
    : Chest(x, y, 0, bullets, grenades), health_points(health_points), view(view) {
    switch (view) {
        case Direction::kDown:
            set_texture('v');
            break;
        case Direction::kLeft:
            set_texture('<');
            break;
        case Direction::kUP:
            set_texture('^');
            break;
        case Direction::kRight:
            set_texture('>');
            break;
    }
}

int Player::get_health_points() const { 
    return health_points; 
}

void Player::move_up() {
    set_y(get_y() - 1);
}

void Player::move_down() {
    set_y(get_y() + 1);
}

void Player::move_right() {
    set_x(get_x() + 1);
}

void Player::move_left() {
    set_x(get_x() - 1);
}

Direction Player::get_view() const { 
    return view; 
}

void Player::view_up() {
    set_texture('^'); 
    view = Direction::kUP;
}

void Player::view_down() {
    set_texture('v'); 
    view = Direction::kDown;
}

void Player::view_left() {
    set_texture('<');
    view = Direction::kLeft;
}

void Player::view_right() {
    set_texture('>');
    view = Direction::kRight;
}

void Player::take_coins(int given_coins) {
    set_coins(get_coins() + given_coins);
}

void Player::take_bullets(int given_bullets) {
    set_bullets(get_bullets() + given_bullets);
}

void Player::take_grenades(int given_grenades) {
    set_grenades(get_grenades() + given_grenades);
}

void Player::take_health_points(int given_health_points) {
    health_points += given_health_points;
}

bool Player::chest_check(Chest& chest) {
    return (get_x() == chest.get_x() && get_y() == chest.get_y());
}

bool Player::med_kit_check(MedKit& med_kit) {
    return (get_x() == med_kit.get_x() && get_y() == med_kit.get_y());
}



Enemy::Enemy(int x, int y, char texture, int health_points, int affected_area) 
    : GameObject(x, y, texture), health_points(health_points), affected_area(affected_area) {}

void Enemy::damage() {
	health_points--;
}

int Enemy::get_health_points() const { 
	return health_points; 
}

bool Enemy::player_check(Player& player) {
	int EnemyX = get_x();
	int EnemyY = get_y();
	int PlayerX = player.get_x();
	int PlayerY = player.get_y();

    return (std::abs(EnemyX - PlayerX) + std::abs(EnemyY - PlayerY)) <= affected_area;
}



Tower::Tower(int x, int y, int health_points, int affected_area) : Enemy(x, y, 'T', health_points, affected_area) {}



Monster::Monster(int x, int y, int health_points, int affected_area) 
    : Enemy(x, y, 'M', health_points, affected_area) {}

void Monster::move_up() {
    set_y(get_y() - 1);
}

void Monster::move_down() {
    set_y(get_y() + 1);
}

void Monster::move_random() {
    int MoveSeed = rand() % 3;
    switch (MoveSeed) {
        case 0:
            move_up();
            break;
        case 1:
            move_down();
            break;
        case 2:
            // В случае необходимости добавьте здесь дополнительное действие
            break;
    }
}



Projectile::Projectile(int x, int y, char texture, Direction direction) 
    : GameObject(x, y, texture), direction(direction) {}

void Projectile::move() {
    int X = get_x();
    int Y = get_y();
    switch (direction) {
        case Direction::kDown:
            set_y(Y + 1);
            break;
        case Direction::kLeft:
            set_x(X - 1);
            break;
        case Direction::kRight:
            set_x(X + 1);
            break;
        case Direction::kUP:
            set_y(Y - 1);
            break;
    }
}

Direction Projectile::get_direction() const {
    return direction;
}



Bullet::Bullet(int x, int y, Direction direction) 
    : Projectile(x, y, 'B', direction) {}

bool Bullet::hit_monster(Monster& monster) {
    int BulletX = get_x();
    int BulletY = get_y();
    int MonsterX = monster.get_x();
    int MonsterY = monster.get_y();

    return (BulletX == MonsterX && BulletY == MonsterY);
}



Grenade::Grenade(int x, int y, Direction direction, int timer) 
    : Projectile(x, y, 'G', direction), timer(timer) {}

bool Grenade::explosion() {
    if (timer == 0)
        return true;
    timer--;
    return false;
}

bool Grenade::hit_enemy(Enemy& enemy) {
    int GrenadeXl = get_x() - 1;
    int GrenadeXr = get_x() + 1;
    int GrenadeYd = get_y() - 1;
    int GrenadeYu = get_y() + 1;
    int EnemyX = enemy.get_x();
    int EnemyY = enemy.get_y();

    return (GrenadeXl <= EnemyX && EnemyX <= GrenadeXr && GrenadeYd <= EnemyY && EnemyY <= GrenadeYu);
}
