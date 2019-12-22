# Grapp Remastered

![IMG_20191219_124948](https://user-images.githubusercontent.com/27884873/71325777-77e2d600-24fa-11ea-8b67-2c4e6983cca5.jpg)
Game presentation: https://youtu.be/y43-t0pTpAc

### The game
The game is an endless runner. While moving forward, the player will encounter various obstacles generated on the run.
Colliding with them will result in losing a life (up to 3). Touching an obstacle from sideways won't result in a collision.

### How to play
The player position is controlled by using the joystick X axis. Specifying a direction will make the player move to that side all the way to the border of the screen, or until the opposite direction is chosen.

If the player dies, the game will slow and eventually stop moving until pressing the joystick button, thus reviving the player.
While in the "dead" state, the joystick X axis is used to move the player just one position at a time.
After reviving, the player will not move sideways until it's told to.
If the player loses their last life, the game is over.

### Technical requierments
For the hardware part I used:
- Arduino UNO
- two 8x8 matrices
- two MAX7219 Drivers
- LCD
- Potentiometer
- Joystick
- one normal sized Breadboard and 4 mini Breadboards
- Wires

The game is displayed on the two 8x8 matrices (16 rows, 8 columns).
For the control it is used a joystick.
While playing, some info is displayed on the LCD (the current number of lifes, score and the difficulty level). The LCD contrast can be changed with a potentiometer. The LCD only refreshes when something has changed on the screen, so it won't flicker.

The difficulty level increases with time and that includes spawning more advanced obstacles and speeding up the forward movement. Some of the obstacles have animations and change their states over time. The score will increase constantly depending on the difficulty level.
The best three scores are saved in EEPROM.

### The menu
The menu has four options. While navigating through it, a animation will play on the matrices.
#### 1. Play
The first one is to play the game. After the option is selected, the player is announced that the game is starting and after a brief period of time the game will start.
#### 2. Settings
In this seciton the player can choose the starting difficulty of the game and the player name.
Here, the player can also see the 'How to play?' section and have a quick reminder of how the game works.
#### 3. Score
Here, the best three scores are shown alongside with the names of the players who did the scores.
#### 4. About
This section contanis info about the game, like: the game name, the author of the game, the github link and that it was made for the Unibuc Robotics course.
