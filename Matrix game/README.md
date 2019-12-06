# Grapp
# Infinite scroller with obstacles

### The game
The game is an infinite scroller. While moving forward, the player will encounter various obstacles generated on the run.
Colliding with them will result in losing a life (up to 3). Touching an obstacle from sideways won't result in a collision.
The difficulty level increases with time and that includes spawning more advanced obstacles and speeding up the forward movement.

### How to play
The player position is controlled by using the joystick X axis. Specifying a direction will make the player to move to that side all the way to the border of the screen.
Changing the direction can also be made wile being "in mid-air" (in the process of moving).

If the player dies, the game will slow and eventually stop moving until pressing the joystick button, thus reviving the player.
While in the "dead" state, the joystick X axis can be used to move the player just ONE position.
After reviving, the player will not move sideways until it's told to.
If the player lost their last life, the game is over.

### Technical requierments
The game is displayed on two 8x8 matrixes (16 rows, 8 columns) using two MAX7219 drivers that are connected.
For the control it is used a joystick.
While playing, some info is displayed on the LCD.

The score will increase constantly depending on the difficulty level.
The best score will be saved on EEPROM.
