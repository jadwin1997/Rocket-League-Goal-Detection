import socket
import json
import serial
import time
PORT = "COM4"
BAUD_RATE = 9600
arduino = serial.Serial(PORT, BAUD_RATE, timeout=1)
time.sleep(2)
HOST = "127.0.0.1"
PORT = 49123

sock = socket.create_connection((HOST, PORT), timeout=10)
sock.settimeout(None)

print("CONNECTED to Rocket League Stats API")

buffer = ""
decoder = json.JSONDecoder()
def send_to_arduino(command):
    message = json.dumps(command) + "\n"
    arduino.write(message.encode())
    print("Sent:", message.strip())
while True:
    chunk = sock.recv(8192)

    if not chunk:
        print("Connection closed")
        break

    buffer += chunk.decode("utf-8", errors="ignore")

    while buffer.strip():
        buffer = buffer.lstrip()

        try:
            msg, index = decoder.raw_decode(buffer)
        except json.JSONDecodeError:
            # Wait for more data
            break

        buffer = buffer[index:]

        event = msg.get("Event")
        data = msg.get("Data", {})

        #print("EVENT:", event)

    if event == "GoalScored":
        if isinstance(data, str):
            data = json.loads(data)

        scorer_data = data.get("Scorer", {})

        if isinstance(scorer_data, str):
            scorer_data = json.loads(scorer_data)

        scorer = scorer_data.get("Name", "Unknown")
        team_num = scorer_data.get("TeamNum", "Unknown")
        speed = data.get("GoalSpeed", "Unknown")
        if(team_num == 0):
            team = "Blue"
            send_to_arduino({
                "mode": "blink",
                "r": 0,
                "g": 0,
                "b": 255,
                "blinkSpeed": 150,
                "blinkCount": 8,
                "brightness": 150
            })
        else:
            team = "Orange"
            send_to_arduino({
                "mode": "blink",
                "r": 255,
                "g": 170,
                "b": 0,
                "blinkSpeed": 150,
                "blinkCount": 8,
                "brightness": 150
            })
        #team = "Blue" if team_num == 0 else "Orange" if team_num == 1 else "Unknown"

        print(f"GOAL SCORED by {scorer} on {team} | Speed: {speed}")
