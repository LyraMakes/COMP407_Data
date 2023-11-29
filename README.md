# Data Logger Project

## Completed by Alexander M. Dasneves & Lyra Brooke Brown

### Steps Completed

We completed steps 1,2,3,5 & 7 (Only steps graded)

### Wiring

To wire these devices to the arduino, please follow these steps:

* RTC Device: SDA -> A4, SCL -> A5
* Gyroscope & Accelerometer: SDA -> A0, SCL -> A1
* SD Card Interface: CS -> 10, MISO -> 11, MOSI -> 12, SCK/CLK -> 13

We also implemented an LED connected on Pin 3 to act as a "debug" LED (Since outputs to the serial port would be interpreted by the Python Script).

### Running Code

To run this code, create and source a venv by running the following code:


```bash
python3.10 -m venv venv
source ./venv/bin/activate
```

Then, install the requirements by using the "requirements.txt"


```bash
python3.10 -m pip install -r requirements.txt
```

Then, run the code using this command:


```bash
python3.10 main.py <PORT> <BAUD>
```

The baudrate of this program is __***9600***__

Please note: this project uses Requests rather than direct urllib, as Requests has become the more ubiquitous library for HTML Requests.

### Results

Our code produces a "readings.txt" file on the flash card connected to the SD Card Interface.
It also will send the line wrote to the SD Card down the Serial Connection to the attached computer.

The Python program provided will interpret the data sent by the Arduino, and perform an HTTP GET request to (riscy.info/dump.php)[http://riscy.info/dump.php], where the readings will be saved.

To view all readings collected, visit (this)[http://riscy.info/data.txt] link.

