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

    void Heal(int amount)
    {
        health += amount;

        if (health > maxHealth)
        {
            health = maxHealth;
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
        std::cout << name << ": " << health << '/' << maxHealth
                  << " HP, damage " << damage << '\n';
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
    Fighter enemies[3];
    Fighter* currentTarget = nullptr;
    int medkits = 1;

    bool TrySelectTarget(int number, Fighter*& outTarget)
    {
        outTarget = nullptr;

        if (number < 1 || number > 3)
        {
            return false;
        }

        Fighter& selectedEnemy = enemies[number - 1];

        if (!selectedEnemy.IsAlive())
        {
            return false;
        }

        outTarget = &selectedEnemy;
        return true;
    }

    bool AllEnemiesDefeated() const
    {
        for (int i = 0; i < 3; ++i)
        {
            if (enemies[i].IsAlive())
            {
                return false;
            }
        }

        return true;
    }

    void PrintState() const
    {
        std::cout << "\n--- Arena ---\n";
        PrintFighter(player);

        for (int i = 0; i < 3; ++i)
        {
            PrintFighter(enemies[i]);
        }

        std::cout << "Medkits: " << medkits << '\n';
        std::cout << "Current target: ";

        if (currentTarget == nullptr)
        {
            std::cout << "none\n";
        }
        else
        {
            currentTarget->PrintStatus();
        }
    }

public:
    Game()
        : player("Hero", 20, 8),
          enemies{
              {"Enemy 1", 12, 3},
              {"Enemy 2", 18, 4},
              {"Enemy 3", 24, 5}}
    {
    }

    void Run()
    {
        while (player.IsAlive() && !AllEnemiesDefeated())
        {
            PrintState();
            std::cout << "1 - select target\n";
            std::cout << "2 - attack selected target\n";
            std::cout << "3 - use medkit\n";
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

            if (command == 1)
            {
                std::cout << "Target number (1-3): ";
                int number = 0;

                if (!(std::cin >> number))
                {
                    std::cout << "Input ended.\n";
                    return;
                }

                if (TrySelectTarget(number, currentTarget))
                {
                    std::cout << "Target selected.\n";
                }
                else
                {
                    std::cout << "Invalid target. Current target was cleared.\n";
                }

                continue;
            }

            if (command == 2)
            {
                if (currentTarget == nullptr)
                {
                    std::cout << "Select a living target first.\n";
                    continue;
                }

                player.Attack(*currentTarget);
                std::cout << "Hero attacks the selected enemy.\n";

                if (!currentTarget->IsAlive())
                {
                    std::cout << "The selected enemy is defeated.\n";
                    currentTarget = nullptr;

                    if (AllEnemiesDefeated())
                    {
                        std::cout << "All enemies are defeated. You win!\n";
                        break;
                    }

                    continue;
                }

                currentTarget->Attack(player);
                std::cout << "The selected enemy attacks Hero.\n";

                if (!player.IsAlive())
                {
                    std::cout << "Hero is defeated. You lose.\n";
                    break;
                }

                continue;
            }

            if (command == 3)
            {
                if (currentTarget == nullptr || !currentTarget->IsAlive())
                {
                    std::cout << "Select a living target before using a medkit.\n";
                    continue;
                }

                if (medkits == 0)
                {
                    std::cout << "No medkits left.\n";
                    continue;
                }

                player.Heal(8);
                --medkits;
                std::cout << "Hero uses a medkit.\n";

                currentTarget->Attack(player);
                std::cout << "The selected enemy attacks Hero.\n";

                if (!player.IsAlive())
                {
                    std::cout << "Hero is defeated. You lose.\n";
                    break;
                }

                continue;
            }

            std::cout << "Unknown command.\n";
        }

        PrintState();
    }
};

int main()
{
    Game game;
    game.Run();

    return 0;
}
