# Door_locker_security_system_using_ATmega32

The project is designed and implemented based on the layered architecture model as follow:

ℳc1 → HMI_ECU (Human Machine Interface) with 2x16 LCD and 4x4 keypad.

![f](https://user-images.githubusercontent.com/101186125/204103887-1a1961d8-9415-47a9-a353-e8b15b98ab2f.PNG)

ℳc2 → Control_ECU with EEPROM, Buzzer, and Dc-Motor.

![s](https://user-images.githubusercontent.com/101186125/204103934-08ac25a5-cb2e-4d2d-ae37-93ea03bbb9f4.PNG)

HMI_ECU is just responsible interaction with the user just take inputs through keypad and display messages on the LCD.
CONTROL_ECU is responsible for all the processing and decisions in the system like password checking, open the door and activate the system alarm.

## System Sequence:
### Step1 – Create a System Password
- The LCD displays “Please Enter Password” like that:
![Capture](https://user-images.githubusercontent.com/101186125/204104646-c1daa750-7ac8-4f35-b248-bf9ddc59518d.PNG)

- Enter a password consists of 5 numbers, Display * in the screen for each number.
![Capture1](https://user-images.githubusercontent.com/101186125/204104648-7eef47d7-d98f-4469-ac16-18185a165505.PNG)

- Press enter button(ON/C).
- HMI_ECU asks the user to renter the same password for confirmation by display this message “Please re-enter the same Pass:”.
![Capture2](https://user-images.githubusercontent.com/101186125/204104649-74db5033-3d67-4120-8250-6aee34bab3d1.PNG)

- HMI_ECU sends the two passwords to the Control_ECU through the UART.
- If the two passwords are matched then the system has a password now and save it inside the EEPORM and go to Step 2.
- If the two passwords are unmatched then repeat step 1 again.

### Step2 - Main Options
- The LCD will always display the main system option:
![Capture4](https://user-images.githubusercontent.com/101186125/204105189-7879db09-1c77-43c1-b324-a67374fdb7f6.PNG)

### Step3 - Open Door +

- The LCD displays “Please Enter Password” like that:
![Capture5](https://user-images.githubusercontent.com/101186125/204105190-6b4d953b-11da-434a-81cf-7ac62640c4a9.PNG)

- Enter the password then press enter button (ON/C).
- HMI_ECU sends the Password to the Control_ECU and it compares it with the one saved in the EEPROM.
- If two passwords are matched:
• Motor rotates for 15-seconds CW and “Door is Unlocking” is displayed on the screen.
• Hold the motor for 3-seconds.
• Motor rotates for 15-seconds A-CW and “Door is locking” is displayed on the screen.

### Step 4 - Change Password -
- The LCD displays “Please Enter Password” like that:
![Capture6](https://user-images.githubusercontent.com/101186125/204105606-2a708425-5487-4eb3-ac8e-dd84c7867448.PNG)

- Enter the password then press enter button (ON/C).
- HMI_ECU sends the Password to the Control_ECU and it compares it with the one saved in the EEPROM.
- If two passwords are matched:
• Repeat Step 1.

### Step 5
- If the two passwords are unmatched at step 3 (+ : Open Door) or step 4 (- : Change Password)
- HMI_ECU asks the user one more time for the password.
- The LCD displays “Please Enter Password” like that:
![Capture6](https://user-images.githubusercontent.com/101186125/204105606-2a708425-5487-4eb3-ac8e-dd84c7867448.PNG)

- Enter the password then press enter button (ON/C).
- HMI_ECU sends the password to the Control_ECU and it compares it with the one saved in the EEPROM.
- If two passwords are matched then open the door or change the password in steps 3 and 4.
- If the two passwords are not matched again then ask the user one last time for the password.
- If two passwords are matched then open the door or change the password in steps 3 and 4.
- If the two passwords are not matched for the third consecutive time, then:
• Buzzer is activated for 1-minute.
• error message is displayed on LCD for 1 minute.
• System is locked and no inputs from Keypad will be accepted during this time period.
• Go to Step 2 the main options again.

## Drivers used:
GPIO, Keypad, LCD, Timer, UART, I2C, EEPROM, Buzzer and DC-Motor.


## proteus simulation.
![final](https://user-images.githubusercontent.com/101186125/204105748-e0343e2f-2529-4424-af5d-409cef8c4210.PNG)
