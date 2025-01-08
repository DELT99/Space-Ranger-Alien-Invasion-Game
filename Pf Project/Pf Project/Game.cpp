#include <iostream>
#include <conio.h>
#include <windows.h> /*for setcursorposition*/
#include <cstdlib>
#include <ctime>
#include<chrono> /*for timer*/
#include<fstream> /*for file handling*/

using namespace std;
using namespace std::chrono;

//all the constant values
const int Rows = 30;  // map height
const int Cols = 50;  // map width
const int Boostlane = Rows - 5;
const int max_bullets = 20;
const int max_asteroids = 2;
const int max_enemy_ships = 2;
const int max_enemy_bullets = 6;
const int max_crates = 2;
const int max_stars = 4;

char map[Rows][Cols];

//timer variables
steady_clock::time_point gameStartTime;
const int levelTimeLimit = 120; /*2 minutes per level*/
int remainingTime = levelTimeLimit;
//tracker variables
int spaceshipLives = 3;
int spaceshipX = Rows - 2, spaceshipY = Cols / 2 - 1;
int score = 0;
int distanceCovered = 0;
int enemyShipsDestroyed = 0;
int cratesDestroyed = 0;
bool levelTwo = false;  // Tracks current level

//bullets position arrays
int bulletX[max_bullets];
int bulletY[max_bullets];
bool bulletActive[max_bullets];

//asteroid position arrays
int asteroidX[max_asteroids];
int asteroidY[max_asteroids];
bool asteroidActive[max_asteroids];

//enemy ship arrays
int enemyShipX[max_enemy_ships];
int enemyShipY[max_enemy_ships];
int enemyShipSpeed[max_enemy_ships];
bool enemyShipActive[max_enemy_ships];

//enemy bullet arrays
int enemyBulletX[max_enemy_bullets];
int enemyBulletY[max_enemy_bullets];
bool enemyBulletActive[max_enemy_bullets];

//crate arrays
int crateX[max_crates];
int crateY[max_crates];
bool crateActive[max_crates];

//star arrays
int starX[max_stars];
int starY[max_stars];
bool starActive[max_stars];

//boss variables and arryays
int bossX = -1;
int bossY = -1;
bool bossActive = false;
int bossHealth = 20;
int bossBulletX[max_bullets];
int bossBulletY[max_bullets];
bool bossBulletActive[max_bullets];
int bossBulletCooldown = 0;

//Boost function variabels
int moveStep = 1;
bool boostActive = false;
int boostDuration = 100;
int boostTimer = 0;
int boostCount = 0;
const int maxBoosts = 3;

//Leaderboared variables
const int max_players = 10;
string filename = "highscore.txt";
string names[max_players];
int scores[max_players];
string Playername;

//cursor position fucntion to place it to the top

void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);
}
//Function to read leaderboard from file
void readLeaderboard() {
    ifstream file(filename);

    if (file.is_open()) {
        for (int i = 0; i < max_players; i++) {
            file >> names[i] >> scores[i];
        }
        file.close();
    }

}
//writng to leadernboard
void writeLeaderboard() {
    ofstream file(filename);

    if (file.is_open()) {
        for (int i = 0; i < max_players; i++) {
            file << names[i] << " " << scores[i] << endl;
        }
        file.close();
    }
}
//function for displaying leaderboard
void displayLeaderboard() {
    system("CLS");
    readLeaderboard();

    cout << "\033[34m==================================================\033[0m" << endl;
    cout << "\033[32m         Space Ranger: Alien Invasion     \033[0m" << endl;
    cout << "\033[34m==================================================\033[0m" << endl;
    for (int i = 1; i <= 5; i++) {
        cout << endl;
    }
    for (int i = 0; i < max_players; i++) {
        cout << i + 1 << ". " << "\033[35m" << names[i] << "\033[0m" << " - " << "\033[36m" << scores[i] << "\033[0m" << endl;
    }
    for (int i = 1; i <= 5; i++) {
        cout << endl;
    }
    cout << "PRESS ESC TO EXIT" << endl << endl;
    cout << "\033[34m==================================================\033[0m" << endl;

    char key = _getch();
    if (key == 27) {
        return;
    }
    else {
        displayLeaderboard();
    }
}

// Function to update leaderboard
void updateLeaderboard() {
    readLeaderboard(); /*calling it to read the stored leaderboard*/

    if (score > scores[max_players - 1]) {
        names[max_players - 1] = Playername;
        scores[max_players - 1] = score;

        for (int i = max_players - 1; i > 0; i--) {
            if (scores[i] > scores[i - 1]) {

                int tempScore = scores[i];
                scores[i] = scores[i - 1];
                scores[i - 1] = tempScore;

                string tempName = names[i];
                names[i] = names[i - 1];
                names[i - 1] = tempName;
            }
            else {
                break;
            }
        }

        // Write the updated leaderboard to the file
        writeLeaderboard();

        // Display the updated leaderboard
        displayLeaderboard();
    }
    else {
        cout << "Your score is not high enough to \nenter the Leaderboard." << endl;
        cout << "Press Any button to see the Leaderboard";
        if (_getch()) {
            displayLeaderboard();
        }
    }
}
//function for Endscreen 
void Endscreen() {
    system("CLS");
    cout << "\033[34m==================================================\033[0m" << endl;
    cout << "\033[32m         Space Ranger: Alien Invasion     \033[0m" << endl;
    cout << "\033[34m==================================================\033[0m" << endl;

    if (remainingTime <= 0) {
        cout << endl << endl << "Time's Up!" << endl << "Final Score: " << score << endl;
    }
    else {
        cout << endl << endl << "Game Over!" << endl << "Final Score: " << score << endl;
    }

    setCursorPosition(0, Rows);

    cout << "\033[34m==================================================\033[0m" << endl;
    setCursorPosition(0, Rows / 2);
    cout << "Enter Your name: ";
    cin >> Playername;

    updateLeaderboard();


}
//Starting the timer
void startTimer() {
    gameStartTime = steady_clock::now();
}

int updateRemainingTime() {

    auto currentTime = steady_clock::now();
    auto elapsedSeconds = duration_cast<seconds>(currentTime - gameStartTime).count();
    remainingTime = max(0, levelTimeLimit - static_cast<int>(elapsedSeconds));

    return remainingTime;
}

//function for displaying the menu 
void displayMenu(int selected) {
    string options[] = { "Play Game", "Instructions", "High Scores", "Exit" };

    cout << "\033[34m==================================================\033[0m" << endl;
    cout << "\033[32m         Space Ranger: Alien Invasion     \033[0m" << endl;
    cout << "\033[34m==================================================\033[0m" << endl;
    for (int i = 1; i <= 9; i++) {
        cout << endl;
    }
    for (int i = 0; i < 4; ++i) {
        if (i == selected) {
            cout << "\033[33m>> " << options[i] << " <<\033[0m" << endl;
        }
        else {
            cout << "   " << options[i] << endl;
        }
    }
    for (int i = 1; i <= 9; i++) {
        cout << endl;
    }
    cout << "\033[34m==================================================\033[0m" << endl;
}
void instructions() {
    system("CLS");
    cout << "\033[34m==================================================\033[0m" << endl;
    cout << "\033[32m         Space Ranger: Alien Invasion     \033[0m" << endl;
    cout << "\033[34m==================================================\033[0m" << endl;
    cout << "\033[33m Welcome, Space Ranger!\n Your mission is to defend your spaceship.\n Destroy asteroids, enemy ships, and\n crates to survive the invasion.\n Can you become the ultimate Space Ranger?\n\nControls:\n\n\Arrow Keys :\nMove left.\nMove right\nMove up\nMove down\n\nSpacebar:Fire bullets to destroy.\n\nJ:Perform a jump to avoid attacks.\n\nB:Activate Boost Mode for increased speed.\n\n\nPRESS ESC to go back to the main menu. \033[0m" << endl;



    cout << "\033[34m==================================================\033[0m" << endl;
    char key = _getch();
    if (key == 27) {
        return;
    }
    else {
        instructions();
    }
}

void menuChoice() {
    int selected = 0;
    const int optionsCount = 4;
    bool menubreak = false;
    while (!menubreak) {
        system("CLS");
        displayMenu(selected);

        char key = _getch();

        if (key == -32) {
            key = _getch();

            if (key == 72) {
                selected = (selected - 1 + optionsCount) % optionsCount;
            }
            else if (key == 80) {
                selected = (selected + 1) % optionsCount;
            }
        }
        else if (key == '\r') {
            if (selected + 1 == 1) {
                menubreak = true;
            }
            else if (selected + 1 == 2) {

                instructions();
            }
            else if (selected + 1 == 3) {
                displayLeaderboard();
            }
            else if (selected + 1 == 4) {
                exit(0);
            }
        }
    }
}

//intitializing the map
void initializeMap() {
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            if (i == 0 || i == Rows - 1 || j == 0 || j == Cols - 1) {
                map[i][j] = '%'; // Boundary
            }
            else if (i == Boostlane) {
                map[i][j] = '=';/*Boostlane*/
            }
            else {
                map[i][j] = ' '; // Empty space
            }
        }
    }
}

//displaying map with colors
void displayMap() {
    setCursorPosition(0, 0); /*gets the cursor to the top*/
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            if (map[i][j] == '%') {

                cout << "\033[44m \033[0m";
            }
            else if (map[i][j] == '*') {

                cout << "\033[33m*\033[0m";
            }
            else if (map[i][j] == 'E') {
                cout << "\033[31mE\033[0m";
            }
            else if (map[i][j] == '<') {
                cout << "\033[34m<\033[0m";
            }
            else if (map[i][j] == '>') {
                cout << "\033[34m>\033[0m";
            }
            else if (map[i][j] == '^') {
                cout << "\033[33m^\033[0m";
            }
            else if (map[i][j] == '$') {
                cout << "\033[32m$\033[0m";
            }
            else if (map[i][j] == '!') {
                cout << "\033[35m!\033[0m";
            }
            else {
                cout << map[i][j];
            }
        }
        cout << endl;
    }

    // Display timer below the game
    cout << "\n\033[91mTime Left: " << remainingTime / 60 << "m " << remainingTime % 60 << "s\033[0m" << endl;
    // Display other game stats
    cout << "Lives: " << spaceshipLives
        << "  Score: " << score
        << "  Distance: " << distanceCovered << " km" << endl;
    cout << "Crates Destroyed: " << cratesDestroyed << " Boost count: " << boostCount << endl;
    if (levelTwo) {
        cout << "Level: 2 (Increased Speed)" << endl;
    }
    else {
        cout << "Level: 1 (Normal Speed)" << endl;
    }
}

//functions for placing and removing the spaceship
void placeSpaceship(int x, int y) {
    map[x][y] = '<';
    map[x][y + 1] = '^';
    map[x][y + 2] = '>';
}

void clearSpaceship(int x, int y) {
    if (x != Boostlane) {
        map[x][y] = ' ';
        map[x][y + 1] = ' ';
        map[x][y + 2] = ' ';
    }
    else {
        map[x][y] = '=';
        map[x][y + 1] = '=';
        map[x][y + 2] = '=';
    }
}

//function for player bullets
void updateBullets() {
    for (int i = 0; i < max_bullets; i++) {
        if (bulletActive[i]) {

            if (bulletX[i] != Boostlane) {
                map[bulletX[i]][bulletY[i]] = ' ';
            }
            bulletX[i]--; // Move bullet upwards

            if (bulletX[i] == 0) {
                bulletActive[i] = false;
            }
            else {
                if (bulletX[i] != Boostlane) {
                    map[bulletX[i]][bulletY[i]] = '|';
                }
            }
        }
    }
}

//functions for spawning and updating the Asteroids
void spawnAsteroids() {
    for (int i = 0; i < max_asteroids; i++) {
        if (!asteroidActive[i]) {
            asteroidX[i] = rand() % (Rows / 2) + 2; // Spawn below boundary
            asteroidY[i] = rand() % (Cols - 2) + 1; // Random column
            asteroidActive[i] = true;
        }
    }
}

void updateAsteroids(int frameCounter) {
    for (int i = 0; i < max_asteroids; i++) {
        if (asteroidActive[i]) {
            int speed;
            if (levelTwo) {
                speed = 2;
            }
            else {
                speed = 4;
            }
            if (frameCounter % speed == 0) {
                if (asteroidX[i] != Boostlane) {
                    map[asteroidX[i]][asteroidY[i]] = ' ';
                }
                asteroidX[i]++; // Move asteroid down

                if (asteroidX[i] == spaceshipX &&
                    (asteroidY[i] == spaceshipY || asteroidY[i] == spaceshipY + 1 || asteroidY[i] == spaceshipY + 2)) {
                    spaceshipLives--;
                    asteroidActive[i] = false;
                }
                else if (asteroidX[i] >= Rows - 1) {
                    asteroidActive[i] = false; // Deactivate if it reaches the bottom
                }
                else {
                    if (asteroidX[i] != Boostlane) {
                        map[asteroidX[i]][asteroidY[i]] = '#';
                    }
                }
            }
        }
    }
    spawnAsteroids();
}

//fuctions for spawning and updating the enemyships
void spawnEnemyShips() {
    for (int i = 0; i < max_enemy_ships; i++) {
        if (!enemyShipActive[i]) {
            enemyShipX[i] = rand() % (Rows / 2) + 2;
            enemyShipY[i] = rand() % (Cols - 2) + 1;
            enemyShipActive[i] = true;
        }
    }
}

void updateEnemyShips(int frameCounter) {
    for (int i = 0; i < max_enemy_ships; i++) {
        if (enemyShipActive[i]) {
            int speedFactor = 3;
            if (levelTwo) {
                speedFactor = 2;
            }
            if (frameCounter % speedFactor == 0) {
                map[enemyShipX[i]][enemyShipY[i]] = ' ';
                enemyShipY[i]++;

                if (enemyShipY[i] >= Cols - 1) {
                    enemyShipY[i] = 1;
                }
                map[enemyShipX[i]][enemyShipY[i]] = 'E';
            }
        }
    }
    spawnEnemyShips();
}
//fuctions for spawning and updating the enemybullets
void fireEnemyBullets() {
    static int fireCounter = 0;
    fireCounter++;
    if (fireCounter % 100 == 0) {
        for (int i = 0; i < max_enemy_ships; i++) {
            if (enemyShipActive[i]) {
                for (int j = 0; j < max_enemy_bullets; j++) {
                    if (!enemyBulletActive[j]) {
                        enemyBulletX[j] = enemyShipX[i] + 1;
                        enemyBulletY[j] = enemyShipY[i];
                        enemyBulletActive[j] = true;
                        break;
                    }
                }
            }
        }
    }
}

void updateEnemyBullets() {
    for (int i = 0; i < max_enemy_bullets; i++) {
        if (enemyBulletActive[i]) {
            if (enemyBulletX[i] != Boostlane) {
                map[enemyBulletX[i]][enemyBulletY[i]] = ' ';
            }

            enemyBulletX[i]++;


            if (enemyBulletX[i] >= spaceshipX &&
                (enemyBulletY[i] == spaceshipY || enemyBulletY[i] == spaceshipY + 1 || enemyBulletY[i] == spaceshipY + 2)) {
                spaceshipLives--;
                enemyBulletActive[i] = false;
            }
            else if (enemyBulletX[i] >= Rows - 1) {
                enemyBulletActive[i] = false;
            }
            else {
                if (enemyBulletX[i] != Boostlane) {
                    map[enemyBulletX[i]][enemyBulletY[i]] = '!';
                }
            }
        }
    }
}
//spawn crates and update its movement
void spawnCrates() {
    for (int i = 0; i < max_crates; i++) {
        if (!crateActive[i]) {
            crateX[i] = rand() % (Rows / 2) + 2;
            crateY[i] = rand() % (Cols - 2) + 1;
            crateActive[i] = true;
        }
    }
}

void updateCrates(int frameCounter) {
    for (int i = 0; i < max_crates; i++) {
        if (crateActive[i]) {
            int speed = 4;
            if (levelTwo) {
                speed = 3;
            }
            if (frameCounter % speed == 0) {
                if (crateX[i] != Boostlane) {
                    map[crateX[i]][crateY[i]] = ' ';
                }
                crateX[i]++; // Move crate down

                if (crateX[i] == spaceshipX &&
                    (crateY[i] == spaceshipY || crateY[i] == spaceshipY + 1 || crateY[i] == spaceshipY + 2)) {
                    spaceshipLives--;
                    crateActive[i] = false;
                }
                else if (crateX[i] >= Rows - 1) {
                    crateActive[i] = false;
                }
                else {
                    if (crateX[i] != Boostlane) {
                        map[crateX[i]][crateY[i]] = '$';/*symbol for crates*/
                    }
                }
            }
        }
    }
    spawnCrates();
}
//functions for spawning and updating stars
void spawnStars() {
    for (int i = 0; i < max_stars; i++) {
        if (!starActive[i]) {
            starX[i] = rand() % (Rows / 2) + 2;
            starY[i] = rand() % (Cols - 2) + 1;
            starActive[i] = true;
        }
    }
}

void updateStars(int frameCounter) {
    for (int i = 0; i < max_stars; i++) {
        if (starActive[i]) {
            int speed = 5;
            if (levelTwo) {
                speed = 3;
            }
            if (frameCounter % speed == 0) {
                if (starX[i] != Boostlane) {
                    map[starX[i]][starY[i]] = ' ';
                }
                starX[i]++; // Move star down

                if (starX[i] >= Rows - 2) {
                    starX[i] = 1; // Reset to top
                    starY[i] = rand() % (Cols - 2) + 1;
                }
                if (starX[i] != Boostlane) {
                    map[starX[i]][starY[i]] = '*'; // '*' represents stars
                }
                if (starX[i] + 1 == spaceshipX && (starY[i] == spaceshipY || starY[i] == spaceshipY + 1 || starY[i] == spaceshipY + 2)) {
                    if (starX[i] == Boostlane) {
                        map[starX[i]][starY[i]] = '=';
                    }
                    else {
                        map[starX[i]][starY[i]] = ' ';
                    }

                    score += 30; // Add 20 to the total score
                    starActive[i] = false; // Deactivate the star
                    spawnStars();  // Spawn a new star
                }
            }
        }
    }
}
//function for handling collision between different obstacles
void handleCollisions() {
    // Bullet-Asteroid Collision
    for (int i = 0; i < max_bullets; i++) {
        if (bulletActive[i]) {
            for (int j = 0; j < max_asteroids; j++) {
                if (asteroidActive[j] && bulletX[i] == asteroidX[j] && bulletY[i] == asteroidY[j]) {
                    // Deactivate both bullet and asteroid
                    asteroidActive[j] = false;
                    bulletActive[i] = false;
                    if (asteroidX[j] == Boostlane) {
                        map[asteroidX[j]][asteroidY[j]] = '='; // boost lane
                    }
                    else {
                        map[asteroidX[j]][asteroidY[j]] = ' ';
                    }

                    // Increase score
                    score += 20;
                }
            }
        }
    }

    // Bullet-Enemy Ship Collision
    for (int i = 0; i < max_bullets; i++) {
        if (bulletActive[i]) {
            for (int j = 0; j < max_enemy_ships; j++) {
                if (enemyShipActive[j] && bulletX[i] == enemyShipX[j] && bulletY[i] == enemyShipY[j]) {
                    // Deactivate both bullet and enemy ship
                    enemyShipActive[j] = false;
                    map[enemyShipX[j]][enemyShipY[j]] = ' ';
                    bulletActive[i] = false;
                    map[bulletX[i]][bulletY[i]] = ' ';

                    // Increase score and destroyed count
                    score += 50;
                    enemyShipsDestroyed++;
                }
            }
        }
    }

    // Bullet-Crate Collision
    for (int i = 0; i < max_bullets; i++) {
        if (bulletActive[i]) {
            for (int j = 0; j < max_crates; j++) {
                if (crateActive[j] && bulletX[i] == crateX[j] && bulletY[i] == crateY[j]) {
                    // Deactivate both bullet and crate
                    crateActive[j] = false;
                    bulletActive[i] = false;
                    if (crateX[j] == Boostlane) {
                        map[crateX[j]][crateY[j]] = '='; // boost lane
                    }
                    else {
                        map[crateX[j]][crateY[j]] = ' ';
                    }

                    // Update crates destroyed and lives
                    cratesDestroyed++;
                    if (cratesDestroyed % 3 == 0) {
                        spaceshipLives++;  // Grant a life after every 5 crates
                    }
                }
            }
        }
    }
}
//function for spawning boss
void spawnBoss() {
    if (!bossActive && score >= 400) {
        bossX = 2;
        bossY = Cols / 2 - 3;
        bossActive = true;// Set the boss as active
        bossHealth = 15; //   health
        cout << "Boss Battle! Defeat the Boss to continue!" << endl;
    }
}

//function for displaying boss

void displayBoss() {
    if (bossActive) {
        map[bossX][bossY] = '<';
        map[bossX][bossY + 1] = 'B';
        map[bossX][bossY + 2] = '>';
    }
}

//update boss movement
void updateBossMovement() {
    if (bossActive) {
        // Clear the previous position before moving the boss
        map[bossX][bossY] = ' ';
        map[bossX][bossY + 1] = ' ';
        map[bossX][bossY + 2] = ' ';

        // Random movement pattern (move boss left, right, up or down)
        int direction = rand() % 4;

        switch (direction) {
        case 0: // Move left
            if (bossY > 1) bossY--;
            break;
        case 1: // Move right
            if (bossY < Cols - 4)
                bossY++;
            break;
        case 2: // Move up
            if (bossX > 1) bossX--;
            break;
        case 3: // Move down
            if (bossX < Rows - 7) bossX++;
            break;
        }

        // place the boss at the new position
        map[bossX][bossY] = '<';
        map[bossX][bossY + 1] = 'B';
        map[bossX][bossY + 2] = '>';
    }
}


//funciton for boss bullet spawning
void fireBossBullets() {
    if (!bossActive || bossBulletCooldown > 0) {
        bossBulletCooldown--; // Decrease cooldown every frame
        return; // Skip firing if cooldown is active
    }

    for (int i = 0; i < max_bullets; i++) {
        if (!bossBulletActive[i]) {
            // Firing in all directions
            bossBulletX[i] = bossX;
            bossBulletY[i] = bossY;

            int direction = rand() % 4;
            switch (direction) {

            case 1:
                bossBulletX[i]++;
                break;

            }

            // Ensure bullets remain within bounds
            if (bossBulletX[i] >= 1 && bossBulletX[i] < Rows - 2 && bossBulletY[i] >= 1 && bossBulletY[i] < Cols - 1) {
                bossBulletActive[i] = true;
                map[bossBulletX[i]][bossBulletY[i]] = '|'; // Display bullet
            }

            bossBulletCooldown = 20; // Reset cooldown after firing
            break;
        }
    }
}


//collisions related with boss
void handleBossCollisions() {
    for (int i = 0; i < max_bullets; i++) {
        if (bulletActive[i] && bossActive) {
            // Check if bullet hits the boss
            if (bulletX[i] == bossX && (bulletY[i] == bossY || bulletY[i] == bossY + 1 || bulletY[i] == bossY + 2)) {
                // Boss is hit by a bullet
                bossHealth--;
                bulletActive[i] = false; // Deactivate bullet
                map[bulletX[i]][bulletY[i]] = ' '; // Clear bullet

                if (bossHealth <= 0) {
                    bossActive = false; // Boss is defeated
                    map[bossX][bossY] = ' ';
                    map[bossX][bossY + 1] = ' ';
                    map[bossX][bossY + 2] = ' ';
                    score += 100; // Add bonus points
                    cout << "Boss Defeated! 100 Bonus Points!" << endl;
                }
            }
        }
    }
}
//update boss bullets
void updateBossBullets() {
    for (int i = 0; i < max_bullets; i++) {
        if (bossBulletActive[i]) {
            if (bossBulletX[i] != Boostlane) {
                map[bossBulletX[i]][bossBulletY[i]] = ' ';
            }

            if (bossBulletX[i] < Rows - 2) {
                bossBulletX[i]++;
                if (bossBulletX[i] != Boostlane) {
                    map[bossBulletX[i]][bossBulletY[i]] = '|';
                } // Display the bullet
            }
            else {
                bossBulletActive[i] = false; // Deactivate bullet if it goes out of bounds
            }

            if (bossBulletX[i] + 1 == spaceshipX &&
                (bossBulletY[i] == spaceshipY || bossBulletY[i] == spaceshipY + 1 || bossBulletY[i] == spaceshipY + 2)) {
                spaceshipLives--;
                bossBulletActive[i] = false; // Deactivate bullet
                map[bossBulletX[i]][bossBulletY[i]] = ' '; // Clear bullet
            }
        }
    }
}


//checking if level changes
void checkLevelTransition() {
    if (!levelTwo && (enemyShipsDestroyed >= 10 || distanceCovered >= 5)) {
        levelTwo = true;
        remainingTime = levelTimeLimit;
        startTimer();
    }
}


// Jump Variables
bool jumpActive = false;

void handleJump() {
    clearSpaceship(spaceshipX, spaceshipY);

    // Move spaceship up 3 rows
    spaceshipX -= 3;

    // Ensure spaceship doesn't go out of bounds
    if (spaceshipX < Rows - 10) {
        spaceshipX = Rows - 10;
    }
    placeSpaceship(spaceshipX, spaceshipY);


}






int main() {
    /* calling all the functions*/
    menuChoice();
    srand(time(0));
    initializeMap();
    placeSpaceship(spaceshipX, spaceshipY);
    spawnAsteroids();
    spawnEnemyShips();
    spawnStars();
    startTimer();
    int frameCounter = 0;
    int sleepTime = 50;

    while (spaceshipLives > 0) {


        updateRemainingTime();
        if (remainingTime <= 0) {
            break;
        }

        displayMap();
        displayBoss();

        if (score >= 400 && !bossActive) {
            spawnBoss();
        }


        checkLevelTransition();
        updateStars(frameCounter);
        handleBossCollisions();
        updateBossMovement();
        fireBossBullets();
        updateBossBullets();

        if (_kbhit()) {
            char input = _getch();
            clearSpaceship(spaceshipX, spaceshipY);

            // Boost activation
            if ((input == 'b' || input == 'B') && boostCount < maxBoosts && !boostActive) {
                boostActive = true;
                boostTimer = boostDuration; // Reset boost timer
                boostCount++; // Increment boost count
            }
            /*    speed for boost*/
            if (boostActive) {
                moveStep = 3;
            }
            else {
                moveStep = 1;
            }
            /* for boostlane movement*/
            if (spaceshipX == Boostlane) {
                /*left movement*/
                if (input == 75 && spaceshipY > 1) {

                    spaceshipY -= 3;
                    if (spaceshipY < 1) {
                        spaceshipY = 1;
                    }
                }/*right movement*/
                else if (input == 77 && spaceshipY < Cols - 4) {

                    spaceshipY += 3;
                    if (spaceshipY > Cols - 4) {
                        spaceshipY = Cols - 4; \
                    }
                }
            }
            /*  for normal movement*/
            else {
                /* left movement*/
                if (input == 75 && spaceshipY > 1) {
                    spaceshipY -= moveStep;
                    if (spaceshipY < 1) spaceshipY = 1;
                }/*right movement*/
                else if (input == 77 && spaceshipY < Cols - 4) {
                    spaceshipY += moveStep;
                    if (spaceshipY > Cols - 4) spaceshipY = Cols - 4;
                }
            }
            if (input == 72 && spaceshipX > (Rows - 10)) {/* up movement*/
                spaceshipX -= moveStep;
                if (spaceshipX < Rows - 10)
                    spaceshipX = Rows - 10;
            }
            else if (input == 80 && spaceshipX < Rows - 2) { /*down movement*/
                spaceshipX += moveStep;
                if (spaceshipX > Rows - 2)
                    spaceshipX = Rows - 2;
            }
            else if (input == ' ') {
                for (int i = 0; i < max_bullets; i++) {
                    if (!bulletActive[i]) {
                        bulletX[i] = spaceshipX - 1;
                        bulletY[i] = spaceshipY + 1;
                        bulletActive[i] = true;
                        break;
                    }
                }


            } /*if user presses ESC*/
            else if (input == 27) {
                menuChoice();
            } /*if user Presses J for jump*/
            else if (input == 'j' || input == 'J') { // Activate jump
                handleJump();


            }
            placeSpaceship(spaceshipX, spaceshipY);
        }
        if (boostActive) {
            boostTimer--;
            if (boostTimer <= 0) {
                boostActive = false; // Disable boost when timer runs out
            }
        }
        updateBullets();
        updateAsteroids(frameCounter);
        updateEnemyShips(frameCounter);
        fireEnemyBullets();
        updateEnemyBullets();
        updateCrates(frameCounter);
        updateStars(frameCounter);
        handleCollisions();
        handleBossCollisions();
        /* for counting the frames*/
        frameCounter++;
        distanceCovered = frameCounter / 100;
        /*Sleep time change for different levels*/
        if (levelTwo) {
            sleepTime = 25;
        }
        else {
            sleepTime = 40;
        }
        Sleep(sleepTime);
    }
    /* show endscreen*/
    Endscreen();
    system("Pause");
    return 0;
}
