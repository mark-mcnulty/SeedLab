import camera
import smbus
import time
ids = []


address = 0x04

def correct_angle_distortion(angle):
    # get the sign
    sign = 0
    if angle < 0:
        sign = -1
    else:
        sign = 1

    print("correcting angle")
    # correct the angle
    print("angle before correction: " + str(angle))
    

    # find what to add to the measured angle
    if abs(angle) >= 5 and abs(angle) < 9:
        angle = (abs(angle) + 0.75) * sign
    elif abs(angle) >= 9 and abs(angle) < 12:
        angle = (abs(angle) + 1.5) * sign
    elif abs(angle) >= 12 and abs(angle) < 15:
        angle = (abs(angle) + 2) * sign
    elif abs(angle) >= 15 and abs(angle) < 20:
        angle = (abs(angle) + 3) * sign
    elif abs(angle) >= 20 and abs(angle) < 25:
        angle = (abs(angle) + 2) * sign

    print("angle after correction: " + str(angle))
    return angle
    


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

        # angle correction
        angle = correct_angle_distortion(angle)

        distance = cam.get_marker_distance_func("image.jpg", corners, ids, h, w)

        return detected, angle, distance
    else:
        return detected, angle, distance


def send(bus, angle, distance):
    try:
        distance = round(distance, 2)
        angle = round(angle, 2)
        temp = str(distance) + " " + str(angle)
        print(temp)
        byteValue = StringToBytes(temp)
        bus.write_i2c_block_data(0x04, 0, byteValue)
        time.sleep(0.75)
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
                state = "look_and_calc"


    except KeyboardInterrupt:
        print("Exiting...")

