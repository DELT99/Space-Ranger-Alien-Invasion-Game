# Space-Ranger-Alien-Invasion-Game
Programming Fundamentals Lab Project

𝐒𝐩𝐚𝐜𝐞 𝐑𝐚𝐧𝐠𝐞𝐫: 𝐀𝐥𝐢𝐞𝐧 𝐈𝐧𝐯𝐚𝐬𝐢𝐨𝐧 𝐆𝐚𝐦𝐞 🚀 is a console-based game that challenges players' reflexes, strategy, and resourcefulness. The objective is to survive, destroy enemies, and collect resources while progressing through levels filled with obstacles and surprises.

🌟 Features
🗺️ Map Representation
The game is displayed as a 2D character grid (50x30).
Key entities represented using ASCII characters:
Spaceship: <^>
Asteroids: #
Enemy Ships: E
Stars: *
The spaceship operates in the bottom 10 rows, effectively dodging and engaging enemies.

🛸 Spaceship Abilities
Movement: Controlled using arrow keys (←, →).
Shooting: Press the spacebar to fire lasers (|) upward.
Boost: Pressing B temporarily increases speed, improving dodge ability.


⚠️ Obstacles
Asteroids:
Spawn randomly and move downward.
Collision reduces health and temporarily freezes the spaceship.
Enemy Ships:
Move horizontally and fire bullets (-) downward every 5 seconds.
Collision with enemy ships or bullets results in losing a life.
Stars: Collect for bonus points (+20) and power-ups.


📈 Game Progression
Levels:
Level 1: Normal speed, slower enemies.
Level 2: Increased speed for asteroids and enemies.
The transition occurs after destroying 10 enemy ships or covering 5 km (2500 steps).


🎁 Crates and Collectibles
Crates:
Destroy to gain lives (1 life after destroying 5 crates).
Stars:
Collect for points (+20) and temporary power-ups like invincibility or speed boosts.


👾 Alien Boss
A challenging boss appears after completing Level 2 and reaching a score of 400.
The boss moves unpredictably and shoots in all directions.
Defeating the boss grants bonus points (+50) and progression to advanced gameplay stages.

🏆 High Scores and Save System
Tracks the top 10 scores and stores them in a .txt file.
A leaderboard is available in the main menu to showcase achievements.

⏱️ Game Mechanics
Countdown Timer: Each level is limited to 2 minutes, adding urgency.
Distance Tracker: The spaceship's movement simulates forward travel, unlocking events after 5 km.
Bullet Interactions: Players and enemy bullets interact with objects to destroy or damage them.


📜 Menu System
Play Game: Start a new adventure.
Instructions: Learn controls and gameplay mechanics.
High Scores: View top players and personal rankings.
Exit: End the game.


🛠️ Programming Highlights
Modular Code Structure:
Summarized logic into reusable functions for clarity and efficiency.
Function Design:
Used descriptive names like drawMap() and handleCollisions() for easy understanding.
Each function assigned a single responsibility to enhance maintainability.
