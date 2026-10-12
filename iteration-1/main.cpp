#include <iostream>

class Fighter
{
private:
    const char* name;
    int health;
    int maxHealth;
    int damage;

public:
    Fighter(const char* name, int health, int damage)
        : name(name), health(health), maxHealth(health), damage(damage)
    {
    }

    void TakeDamage(int amount)
    {
        health -= amount;

        if (health < 0)
        {
            health = 0;
        }
    }

    void Attack(Fighter& target) const
    {
        target.TakeDamage(damage);
    }

    bool IsAlive() const
    {
        return health > 0;
    }

    void PrintStatus() const
    {
        std::cout << name << ": " << health << '/' << maxHealth << " HP, damage " << damage << '\n';
    }
};

void PrintFighter(const Fighter& fighter)
{
    fighter.PrintStatus();
}

class Game
{
private:
    Fighter player;
    Fighter enemy;

public:
    Game()
        : player("Hero", 30, 8),
          enemy("Enemy", 12, 3)
    {
    }

    void Run()
    {
        while (player.IsAlive() && enemy.IsAlive())
        {
            std::cout << "\n--- Arena ---\n";
            PrintFighter(player);
            PrintFighter(enemy);
            std::cout << "1 - attack\n";
            std::cout << "0 - exit\n";
            std::cout << "Command: ";

            int command = 0;

            if (!(std::cin >> command))
            {
                std::cout << "Input ended.\n";
                return;
            }

            if (command == 0)
            {
                std::cout << "Game closed.\n";
                return;
            }

            if (command != 1)
            {
                std::cout << "Unknown command.\n";
                continue;
            }

            player.Attack(enemy);
            std::cout << "Hero attacks Enemy.\n";

            if (!enemy.IsAlive())
            {
                std::cout << "Enemy is defeated. You win!\n";
                break;
            }

            enemy.Attack(player);
            std::cout << "Enemy attacks Hero.\n";

            if (!player.IsAlive())
            {
                std::cout << "Hero is defeated. You lose.\n";
            }
        }

        std::cout << "\n--- Final status ---\n";
        PrintFighter(player);
        PrintFighter(enemy);
    }
};

int main()
{
    Game game;
    game.Run();

    return 0;
}
