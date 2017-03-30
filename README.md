# RTOS-Vehicle-Simulation-Game

This game demonstrates the driving dynamics of a a vehicle on a linear track. For the simulation, the health of the vehicle should
be monitored (for example, if the car hits an obstacle, its health will decrease). Users
will need to avoid obstacles to complete each level with enough health remaining. There
will be multiple levels of increasing difficulty that will test the user's skill.

There are three KEIL peripherals that are implemented in this game: the INT0
button, LED, and joystick. The LEDS will be used to display the health of the vehicle.
The INT0 button will be needed to reset the game or to ensure that the user is ready to
move onto the next level. The joystick will be used to move the direction of the car. In
order to use these the peripherals, power must be enabled only to the ones being used.
These peripherals are connected to an interrupt request line that will stop the currently
executing program to signal a user's input.

There are three important tasks implemented in this game. Firstly, one task will be required to control the vehicle { it must take the user's input and display
visually on the KEIL board's GLCD. Another task will be responsible for the movement of obstacles inside each level. The number of obstacles will vary based on the level. Fi-
nally, a task will be responsible for the movement of power-ups inside each level. The number of obstacles will vary based on the level.
