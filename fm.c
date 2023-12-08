#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char name[20];
    int skill;
    int energy;
    double value;
} Player;

// Function Declarations
void listPlayers(Player *players, int *numPlayers);
void printStats();
void sellPlayer(Player *players, int *numPlayers, int playerIndex);
void matchDay(Player *players, int *numPlayers);
void continueMatch(Player *players, int *numPlayers);
void buyRandomPlayer(Player *players, int *numPlayers, double *clubMoney);
void saveGame(Player *players, int numPlayers);
void loadGame(Player *players, int *numPlayers, double *clubMoney);

int managerLevel = 1; // Initial manager level
double clubMoney = 500.0; // Initial club money

int main() {
    // Initialize player data (example data)
    int numPlayers = 8;
    Player *players = (Player *)malloc(numPlayers * sizeof(Player));

    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < numPlayers; ++i) {
        sprintf(players[i].name, "Player%d", i + 1);
        players[i].skill = rand() % 5 + 1; // Random skill level from 1 to 5
        players[i].energy = rand() % 10 + 1; // Random energy level from 1 to 10
        players[i].value = players[i].skill * 100.0 + players[i].energy * 10.0; // Value based on player skill and energy levels
    }

    int choice;
    do {
        printf("\nMain Menu:\n");
        printf("1. List Players\n");
        printf("2. Print Stats\n");
        printf("3. Match Day\n");
        printf("4. Save Game\n");
        printf("5. Load Game\n");  // Added option to load a game
        printf("0. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                listPlayers(players, &numPlayers);
                break;
            case 2:
                printStats();
                break;
            case 3:
                matchDay(players, &numPlayers);
                break;
            case 4:
                saveGame(players, numPlayers);
                break;
            case 5:
                loadGame(players, &numPlayers, &clubMoney);
                break;
            default:
                if (choice >= 0) {
                    printf("Invalid choice. Please try again.\n");
                }
        }
    } while (choice >= 0);

    free(players); // Free allocated memory
    return 0;
}

void saveGame(Player *players, int numPlayers) {
    FILE *file = fopen("savegame.txt", "w");
    if (file == NULL) {
        printf("Error opening save file.\n");
        return;
    }

    // Save player data
    fprintf(file, "%d\n", numPlayers);
    for (int i = 0; i < numPlayers; ++i) {
        fprintf(file, "%s %d %d %.2f\n", players[i].name, players[i].skill, players[i].energy, players[i].value);
    }

    // Save manager level and club money
    fprintf(file, "%d %.2f\n", managerLevel, clubMoney);

    fclose(file);
    printf("Game saved successfully.\n");
}

void loadGame(Player *players, int *numPlayers, double *clubMoney) {
    FILE *file = fopen("savegame.txt", "r");
    if (file == NULL) {
        printf("No saved game found.\n");
        return;
    }

    // Load player data
    fscanf(file, "%d", numPlayers);
    for (int i = 0; i < *numPlayers; ++i) {
        fscanf(file, "%s %d %d %lf", players[i].name, &players[i].skill, &players[i].energy, &players[i].value);
    }

    // Load manager level and club money
    fscanf(file, "%d %lf", &managerLevel, clubMoney);

    fclose(file);
    printf("Game loaded successfully.\n");
}

void listPlayers(Player *players, int *numPlayers) {
    printf("\n%-5s%-15s%-15s%-15s%-15s\n", "No.", "Name", "Skill", "Energy", "Value (€)");
    printf("--------------------------------------------\n");
    int totalSkill = 0;
    int totalEnergy = 0;
    for (int i = 0; i < *numPlayers; ++i) {
        printf("%-5d%-15s%-15d%-15d%-15.2f\n", i + 1, players[i].name, players[i].skill, players[i].energy, players[i].value);
        totalSkill += players[i].skill;
        totalEnergy += players[i].energy;
    }

    if (*numPlayers > 0) {
        double avgSkill = (double)totalSkill / *numPlayers;
        double avgEnergy = (double)totalEnergy / *numPlayers;
        printf("\nAverage Skill Level: %.2f\n", avgSkill);
        printf("Average Energy Level: %.2f\n", avgEnergy);
    } else {
        printf("\nNo players available.\n");
    }

    printf("\nSell a player (Press 1-%d, 0 to go back): ", *numPlayers);
    int sellChoice;
    scanf("%d", &sellChoice);
    if (sellChoice >= 1 && sellChoice <= *numPlayers) {
        sellPlayer(players, numPlayers, sellChoice - 1);
    } else if (sellChoice != 0) {
        printf("Invalid choice. Returning to the main menu.\n");
    }
}

void printStats() {
    printf("\nManager Level: %d\n", managerLevel);
    printf("Club Money: %.2f€\n", clubMoney);
}

void sellPlayer(Player *players, int *numPlayers, int playerIndex) {
    if (playerIndex >= 0 && playerIndex < *numPlayers) {
        // Add player's value to clubMoney
        clubMoney += players[playerIndex].value;
        // Remove the player from the list
        for (int i = playerIndex; i < *numPlayers - 1; ++i) {
            players[i] = players[i + 1];
        }
        (*numPlayers)--;
        printf("Player %s has been sold. Club Money updated.\n", players[playerIndex].name);
    } else {
        printf("Invalid player index.\n");
    }
}

void matchDay(Player *players, int *numPlayers) {
    // Generate a random opponent
    Player opponent;
    sprintf(opponent.name, "Opponent");
    opponent.skill = rand() % 5 + 1; // Random skill level from 1 to 5
    opponent.energy = rand() % 10 + 1; // Random energy level from 1 to 10

    printf("\nOpponent's Statistics:\n");
    printf("Average Skill Level: %d\n", opponent.skill);
    printf("Average Energy Level: %d\n", opponent.energy);

    // User selects their team
    printf("\nYour Team:\n");
    for (int i = 0; i < *numPlayers; ++i) {
        printf("%d. %-15s%-15d%-15d\n", i + 1, players[i].name, players[i].skill, players[i].energy);
    }

    int selectedPlayers[5];
    printf("\nSelect 5 players by typing their numbers (1-%d): ", *numPlayers);
    for (int i = 0; i < 5; ++i) {
        scanf("%d", &selectedPlayers[i]);
    }

    // Adjust energy levels after the match
    for (int i = 0; i < *numPlayers; ++i) {
        if (i == selectedPlayers[0] - 1 || i == selectedPlayers[1] - 1 || i == selectedPlayers[2] - 1 || i == selectedPlayers[3] - 1 || i == selectedPlayers[4] - 1) {
            players[i].energy = (players[i].energy > 1) ? players[i].energy - 1 : 1;
        } else {
            players[i].energy = (players[i].energy < 10) ? players[i].energy + 2 : 10;
        }
        players[i].value = players[i].skill * 100.0 + players[i].energy * 10.0; // Update player value
    }

    // Calculate average skill and energy levels of selected players
    double avgUserSkill = 0.0;
    double avgUserEnergy = 0.0;
    for (int i = 0; i < 5; ++i) {
        int index = selectedPlayers[i] - 1;
        avgUserSkill += players[index].skill;
        avgUserEnergy += players[index].energy;
    }
    avgUserSkill /= 5;
    avgUserEnergy /= 5;

    printf("\nYour Team Statistics:\n");
    printf("Average Skill Level: %.2f\n", avgUserSkill);
    printf("Average Energy Level: %.2f\n", avgUserEnergy);

    // Determine the outcome of the match
    if (avgUserSkill >= opponent.skill || avgUserEnergy >= opponent.energy) {
        printf("\nCongratulations! You win the match!\n");
        clubMoney += 100.0;  // Award 100€ for winning
    } else {
        printf("\nSorry, you lose the match. Better luck next time!\n");
        clubMoney += 20.0;  // Award 20€ for losing
    }


    // Continue with buying a new player
    continueMatch(players, numPlayers);
}

void continueMatch(Player *players, int *numPlayers) {
    printf("\nPress Enter to continue...");
    getchar(); // Wait for Enter key
    // Buy a new player
    buyRandomPlayer(players, numPlayers, &clubMoney);
}

void buyRandomPlayer(Player *players, int *numPlayers, double *clubMoney) {
    // Generate a random player available for purchase
    Player randomPlayer;
    sprintf(randomPlayer.name, "RandomPlayer");
    randomPlayer.skill = rand() % 5 + 1; // Random skill level from 1 to 5
    randomPlayer.energy = rand() % 10 + 1; // Random energy level from 1 to 10
    randomPlayer.value = randomPlayer.skill * 100.0 + randomPlayer.energy * 10.0; // Value based on player skill and energy levels

    printf("\nRandom Player Available for Purchase (Club total: %.2f€):\n", *clubMoney);
    printf("Name: %-15sSkill: %-15dEnergy: %-15dValue (€): %.2f\n", randomPlayer.name, randomPlayer.skill, randomPlayer.energy, randomPlayer.value);

    printf("\nPress 1 to buy the player, or enter to continue: ");
    int buyChoice = getchar(); // Read user input

    if (buyChoice == '1') {
        // Buy the player
        if (*clubMoney >= randomPlayer.value) {
            *clubMoney -= randomPlayer.value;
            printf("\nCongratulations! You have successfully purchased %s!\n", randomPlayer.name);

            // Add the player to the user's team
            players[*numPlayers] = randomPlayer;
            (*numPlayers)++;

            // Display updated stats
            printStats();
        } else {
            printf("\nSorry, you do not have enough money to purchase %s.\n", randomPlayer.name);
        }
    } else {
        printf("Returning to the main menu.\n");
    }
}
