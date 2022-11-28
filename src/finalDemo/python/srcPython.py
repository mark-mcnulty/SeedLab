from enum import Enum, auto
import camera
import smbus
import time


class States(Enum):
    START = auto()
    IS_DONE = auto()
    LOOK = auto()
    SEND = auto()
    INC_ID = auto()

# making the fsm
class fsm:
    def __init__(self):
        self.__state = States.START

        # these will be all the flags for the fsm
        self.__is_done = False
        self.__marker_found = False

    def get_state(self):
        return self.__state

    def set_state(self, state):
        self.__state = state

# making the robot
class robot_object(fsm):
    def __init__(self):
        super().__init__()
        self.__camera = camera.arducam()
        self.__bus = smbus.SMBus(1)
        self.__arduino_address = 0x04

        # this is for the marker detection
        self.__current_marker_id = 1
        self.__marker_id_index = 0

        # picture information
        self.__ids = None
        self.__corners = None
        self.__h = None
        self.__w = None

        # marker information
        self.__marker_distance = None
        self.__marker_angle = None

    def take_picture(self):
        self.__camera.capture()

    def calculate_angle_distance(self):
        self.__marker_distance, self.__marker_angle =  self.__camera.get_specific_marker_distance_and_angle(self.__corners, self.__ids, self.__h, self.__w, self.__marker_id_index)

    def is_the_marker_there(self):
        # detect the markers
        self.__marker_found = False
        self.__ids, self.__corners, self.__h, self.__w = self.__camera.detect_markers_undistorted()

        # check if the correct marker is there
        if self.ids is not None:
            for i in range(self.ids.size):
                if self.ids[i] == self.current_marker_id:
                    self.__marker_found = True
                    self.__marker_id_index = i
                    break


    def send_data_arduino(self, angle, distance):
        print("send data to arduino")
        try:
            distance = round(distance, 2)
            angle = round(angle, 2)
            temp = str(distance) + " " + str(angle)
            byteValue = self.StringToBytes(temp)
            self.__bus.write_i2c_block_data(0x04, 0, byteValue)
            print(str(angle) + " " + str(distance))
            time.sleep(0.5)
        except:
            print("Error")

    def StringToBytes(val):
        retVal = []
        for c in val:
            retVal.append(ord(c))
        return retVal

    def ask_arduino_if_done(self):
        print("ask arduino if done")
        number = self.__bus.read_byte_data(self.arduino_address)
        if number == 1:
            self.__is_done = True
    


if __name__ == "__main__":
    Satan = robot_object()


    while True:
        if Satan.get_state() == States.START:
            print("start")
            Satan.set_state(States.LOOK)

        elif Satan.get_state() == States.IS_DONE:
            print("is the movement done?")
            # ask the arduino if the movement is done 
            Satan.ask_arduino_if_done()
            if Satan.__is_done:
                Satan.set_state(States.INC_ID)
            else:
                Satan.set_state(States.LOOK)
            

        elif Satan.get_state() == States.LOOK:
            print("look")
            Satan.take_picture()
            Satan.is_the_marker_there()

            # change the state if the correct marker is there
            if Satan.__marker_found:
                Satan.set_state(States.SEND)
            else:
                Satan.set_state(States.IS_DONE)
            
        elif Satan.get_state() == States.SEND:
            print("send")
            # first calculate the distance and angle
            Satan.calculate_angle_distance()

            # send the data to the arduino
            Satan.send_data_arduino(Satan.marker_angle, Satan.marker_distance)

            # go to the next state
            Satan.set_state(States.IS_DONE)

        elif Satan.get_state() == States.INC_ID:
            print("inc id")
            Satan.__current_marker_id += 1
            Satan.set_state(States.LOOK)

        else:
            print("error")
    