from ir_ws import IR_WS
from pynput.mouse import Button, Controller

mouse = Controller()

def right_click():
    mouse.click(Button.right, 1)

def left_click():
    mouse.click(Button.left, 1)
    
def scroll_up():
    mouse.scroll(0, 2)

def scroll_down():
    mouse.scroll(0, -2)

def invalid_signal():
    print("Invalid signal!")


signal_map = {  "1C5C87EE"  :   left_click,
                "1C0687EE"  :   right_click,
                "1C0A87EE"  :   scroll_up,
                "1C0C87EE"  :   scroll_down,
                ""          :   invalid_signal}

def onSignal(signal): 
    if signal in signal_map :
        signal_map[signal]()

if __name__ == "__main__":
    con = IR_WS(5)
    con.run(onSignal)