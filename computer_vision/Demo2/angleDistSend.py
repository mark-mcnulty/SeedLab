import camera

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


def send(angle, distance):
    # 
    print(str(angle) + " " + str(distance))


state_dictionary = {
    look_and_calc: "looking",
    send: "sending"
}


# create main loop
if __name__ == "__main__":

    cam = camera.arducam()
    print("activated camera")
    
    state = "look_and_calc"
    angle = 0
    distance = 0
    detected = False

    try: 
        while True:
            if state == "look_and_calc":
                detected, angle, distance = look_and_calc(cam)

                # switch to send state
                if detected:
                    state = "send"

            elif state == "send":
                send(angle, distance)
                detected = False

                # switch to look_and_calc
                state = "look_and_calc"

    except KeyboardInterrupt:
        print("Exiting...")



    # try:
    #     while True:
    #         print("main")
    #         new_state= state(cam)
    #         state = new_state
    # except KeyboardInterrupt:
    #     print("KeyboardInterrupt")
    #     print("Exiting")