import camera
import smbus
import time
ids = []

turnDriveDone = False
address = 0x04

def wait():
    # number = bus.read_byte_data(address)
    number = bus.read_byte_data(address, 0x01)
    time.sleep(2)
    print(number)
    if number == 1:
        turnDriveDone = True

def look_and_calc(cam):
    # take picture
    cam.capture()


    detected = False
    angle = 0
    distance = 0

    # check for object
    # detect the markers
    corners, ids, h, w = cam.detect_markers_undistorted()

    # check if the markers are found
    if ids is not None:
        # calculate the angle
        detected = True
        angle = cam.get_marker_angle("image.jpg", corners, ids, h, w)
        distance = cam.get_marker_distance_func("image.jpg", corners, ids, h, w)

        return detected, angle, distance
    else:
        return detected, angle, distance


def send(bus, angle, distance):
    distance = round(distance, 2)
    angle = round(angle, 2)
    temp = str(distance) + " " + str(angle)
    byteValue = StringToBytes(temp)
    bus.write_i2c_block_data(0x04, 0, byteValue)
    print(str(angle) + " " + str(distance))

def StringToBytes(val):
    retVal = []
    for c in val:
        retVal.append(ord(c))
    return retVal


state_dictionary = {
    look_and_calc: "looking",
    send: "sending"
}


# create main loop
if __name__ == "__main__":
    bus = smbus.SMBus(1)
    cam = camera.arducam()
    print("activated camera")
    
    state = "look_and_calc"
    angle = 0
    distance = 0
    detected = False

    try: 
        while True:
            if state == "look_and_calc":
                turnDriveDone = False
                detected, angle, distance = look_and_calc(cam)

                # switch to send state
                if detected:
                    state = "send"

            elif state == "send":
                send(bus, angle, distance)
                detected = False

                # switch to look_and_calc
                state = "wait"

            elif state == "wait":
                wait()
                if turnDriveDone:
                    # reset turnDriveDone
                    turnDriveDone = False
                    # go to next state
                    state = "look_and_calc"

    except KeyboardInterrupt:
        print("Exiting...")

