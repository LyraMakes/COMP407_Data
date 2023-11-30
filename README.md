# Data Logger Project

## Completed by Alexander M. Dasneves & Lyra Brooke Brown

### Steps Completed

We completed steps **1,2,3,5 & 7**

### Wiring

To wire these devices to the arduino, please connect the wires as such

| Device | SDA | SCL |
|--------|-----|-----|
| GY521  | A4  | A5  |
| DS1307 | A0  | A1  |

| Device | CS | MISO | MOSI | SCK/CLK |
|--------|----|------|------|---------|
| SD Card Interface | 10 | 11 | 12 | 13 |

We also added a Debug LED on Pin 3 to help diagnose SD Init Failures.

### Running Code

To run this code, create and source a venv by running the following code:


```bash
python3.10 -m venv venv
source ./venv/bin/activate
```

Then, install the requirements by using the *requirements.txt* file


```bash
python3.10 -m pip install -r requirements.txt
```

Then, run the code using this command:


```bash
python3.10 main.py <PORT> <BAUD>
```

The baudrate of this program is <u>***9600***</u>

Please note: this project uses **Requests** rather than direct **urllib**, as **Requests** has become the more ubiquitous library for HTML Requests.

### Results

Our code produces a *readings.txt* file on the SD Card connected to the arduino. The file will contain the following information:\\

* Date of reading
* Time of reading
* Reading Name
* Reading value

This information is also sent down the line through the Serial Connection to the attached computer.

The Python program provided will interpret the data sent by the Arduino, and perform an HTTP GET request to [riscy.info/dump.php](http://riscy.info/dump.php), where the readings will be saved.

To view all readings collected by the COMP-407, visit [this link](http://riscy.info/data.txt).

