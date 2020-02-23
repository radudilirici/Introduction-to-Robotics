# RoboPong

### Description
The project consists in a robotic finger that plays Pong as the second player.
Each player controls its bar with a joystick and the game is displayed on a 128px LCD Display.

### The Game
The game itself is a Pong with some new features to make it more entertaining.
After the ball touches a bar, there is a posibility that it will recieve one of these effects: smaller bar, bigger bar and slower bar. Each time you have one of these, the color of your bar changes acording to the effect (red, green or blue) in order to make it more obvious.
The trejectory of the ball can be horizontal or at 45 degrees (up or down) and is randomly given after each hit, but it can't go back exacly the way it came.
You can choose between two game modes in the options: AI and BT.

### The Finger
In the 'AI' mode the finger plays by itself to defeat you (and does a pretty good job at it).
When navigating the menu, the finger will not move, but when you select the 'Play' option the finger will bend its joints in order to touch the joystick.
Each time you hit the ball it calculates the exact position at which the ball will reach the side of the finger based on the ball position and the ball trajectory.
Because of this and because the finger is precise enough, it always gets to the ball. To be noted that the bar only moves because the joystick is moved by the robotic finger (you can remove his joystick in order to win :D).

### Bluetooth
If you select the 'BT' mode the game can be played in two. The first player will play as usual, with his joystick, but the second player will tell the finger to move left or right via Bluetooth.
In order to do this, the player must pair to the Bluetooth Module of the game and install the 'Arduino Bluetooth Controller' app from Google Play. In the controller section you have to customize the buttons with 'L' for left and 'R' for right.

### Technical information

The finger was 3D Printed by [Alex Tesileanu](https://github.com/TesileanuAlexandru). It is turned left or right by a Servo Motor.
There are also two Servo Motors attached to the finger that control the joints with nylon strings. On its back there is a resort so it gets to a straight position when the strings are released.

![Behind the scenes](https://user-images.githubusercontent.com/27884873/75118629-3bf7f880-5684-11ea-9012-d5142f4005ad.jpg)


The poject has the next components:
- Arduino UNO x2
- 1.44 inch TFT LCD Color Screen Module
- 3D Printed Robotic Finger
- MG995 Servo Motor
- SG90 Servo Motor x2
- HC-05 Serial Bluetooth Module
- 5V to 3.3V Logic Level Converter
- 5V Power Supply for Breadboard
- Charger x3
- 450 mAh Battery
- Joystick x2
- Breadboard x2
- Mini Breadboard
- Wires


This project was made for Unibuc Robotics, a course at University of Bucharest.
