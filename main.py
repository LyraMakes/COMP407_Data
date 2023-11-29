import serial
import sys
from typing import Dict
import requests


def init_serial(port_name: str, baud_rate: int) -> serial.Serial:
    serial_port = serial.Serial(port=port_name, baudrate=baud_rate)
    return serial_port


def get_read_from_serial(serial: serial.Serial) -> str:
    data = ""
    while not data.endswith("\n"):
        raw_data = serial.read()
        data += raw_data.decode()
    return data


# MM/DD/YYYY|HH:MM:SS|NAME|VALUE
def convert_data_to_dict(data: str) -> Dict[str, str]:
    ret_dict: Dict[str, str] = {}
    data_split = data.lower().strip().split("|")
    ret_dict["date"] = data_split[0]
    ret_dict["time"] = data_split[1]
    ret_dict["name"] = data_split[2]
    ret_dict["value"] = data_split[3]
    return ret_dict


def send_data(log_name: str, data: Dict[str, str]):
    query_params = f"?name={log_name}&date={data['date'].replace('/','_')}" + \
        f"&time={data['time'].replace(':', '_')}&{data['name']}={data['value']}"
    url = "http://riscy.info/dump.php"
    full_url = url + query_params
    print(full_url)
    requests.get(full_url)


def main(name: str):
    port = sys.argv[1]
    baud = int(sys.argv[2]) if len(sys.argv) == 3 else 9600
    serial = init_serial(port, baud)
    while serial.is_open:
        data = get_read_from_serial(serial)
        reading_dictionary = convert_data_to_dict(data)
        print(reading_dictionary)
        # continue
        send_data(name, reading_dictionary)


if __name__ == "__main__":
    logger_name = input("Enter your name: ")
    main(logger_name)
