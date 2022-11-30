import camera
import smbus
import time


ids = []
address = 0x04

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
        print("found")
        detected = True
        angle = cam.get_marker_angle("image.jpg", corners, ids, h, w)
        distance = cam.get_marker_distance_func("image.jpg", corners, ids, h, w)

        return detected, angle, distance
    else:
        return detected, angle, distance


def send(bus, angle, distance):
    try:
        distance = round(distance, 2)
        angle = round(angle, 2)
        temp = str(distance) + " " + str(angle)
        byteValue = StringToBytes(temp)
        bus.write_i2c_block_data(0x04, 0, byteValue)
        print(str(angle) + " " + str(distance))
        time.sleep(0.5)
    except:
        print("Error")

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
    # create the object
    bus = smbus.SMBus(1)
    cam = camera.arducam()
    print("activated camera")
    
    # create variables
    state = "look_and_calc"
    angle = 0
    distance = 0

    sendTolerance = 0.5


    detected = False
    angleOld = None

    try: 
        while True:
            if state == "look_and_calc":
                detected, angle, distance = look_and_calc(cam)

                # switch to send state
                if detected:
                    if angleOld is None:
                        angleOld = angle
                        break
                    if angleOld - angle < sendTolerance:
                        state = "send"
                    angleOld = angle

            elif state == "send":
                print("sending", angle, distance)
                send(bus, angle, distance)
                detected = False

                # switch to look_and_calc
                state = "look_and_calc"


    except KeyboardInterrupt:
        print("Exiting...")

