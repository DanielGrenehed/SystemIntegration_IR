#!/usr/bin/python
import json
from websocket import create_connection

def _on_sig(signal):
    print(signal)

class IR_API_WS:

    def __init__(self, url, sensor_id):
        self.ws = create_connection(url)
        self.ws.send(json.dumps({"sensor_id":sensor_id}))

    def run(self, signal_responder):
        while True:
            result = self.ws.recv()
            print("Received '" + result + "'")
            json_object = json.loads(result)
            signal_responder(json_object["signal"])
        self.ws.close()

def IR_WS(sensor_id):
    url = "ws://localhost:8081/ir-sensor"
    return IR_API_WS(url, sensor_id)

if __name__ == "__main__" :
    con = IR_WS(5)
    con.run(_on_sig)