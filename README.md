# PES_Final_Project
PES Final Project - Position Tracking System using onboard mmA8451 accelerometer
Created by: Shashank Chandrasekaran

The main code can be found in the folder: https://github.com/shashankchandrasekaran/PES_Final_Project/tree/main/Code

The documentation can be found in folder: https://github.com/shashankchandrasekaran/PES_Final_Project/tree/main/Documentation
The documents consist of:
1) Project Demo - Video of the working of project
2) Test Plan and Results - Manual tests to be conducted consisting of happy, corner and error cases. Automated test cases are provided in the code.
3) Command_Processor_snap1 - Contains the interactive terminal session with user with different commands and results
4) Command_Processor_snap2 - Contains the help command

Overview of the project:
The user can move object in roll angle (Horizontal) from (0 to 180 - Reference zero angle) degrees. The reference angle by default is 0 degrees but user can calibrate the reference to any angle by entering the command and touching TSI at angle desired.
The user can enter roll angle setpoint and when the object reaches that angle, green LED will blink indicating the object is at desired angle.
The user can also enter pitch angle (vertical) setpoint from (0 to 85) degrees and when object reaches that angle, blue LED will blink.
The calibration is kept for roll angle only as the main motive was to show the working of calibration.
The major details can be found in the test plan and demo.
Note: Set Baud rate to 9600, 1 start and 1 stop bit while connecting to terminal manager

Learnings:
1) I2C and acclerometer integration
2) Integrating all modules together to get proper functionality
3) Developing mixture of automated and manual test cases

Challenge: To get good amount of accuracy. This was established by constant testing and modifying ranges, delays and conditions in the code.

Submitted Project Matches Functionality:
1) I2C, Comamnd Processing, PWM, Interrupts, GPIO and UART was committed in the project proposal. All these functionalities were implemented. In addition, SysTick was also used in the project.
2) Project proposal mentioned calibration of zero degree by either UART, external button or TSI.The option was kept open to decide in future. In my implementation, have used TSI. 
3) Proposal mentioned the operation in only one plane. But in implementation apart from roll angle, have also added pitch angle functionality.
4) Project proposal mentioned a mixture of automated and manual test cases. Automated cases for testing few of the individual modules and manual test cases for overall working of the project. The same was executed in the implementation.


