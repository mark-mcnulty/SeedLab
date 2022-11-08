import camera

def look(cam, angle, distance):
    # take picture
    cam.capture()
    angle = 0
    distance = 0

    # check for object
    # detect the markers
    corners, ids, h, w = cam.detect_markers_undistorted()

    # check if the markers are found
    if ids is not None:
        # calculate the angle
        distance = cam.get_marker_distance_func("image.jpg", corners, ids, h, w)
        angle = cam.get_marker_angle("image.jpg", corners, ids, h, w)

        return send, angle, distance
    else:
        return look, angle, distance


def send(cam, angle, distance):
    # 
    print(angle + " " + distance)

    return look

state_dictionary = {
    look: "looking",
    send: "sending"
}


# create main loop
if __name__ == "__main__":
    state = look

    cam = camera.arducam()
    
    angle = 0
    distance = 0

    try:
        while True:
            print("main")
            new_state, angle, distance = state(cam, angle, distance)
            state = new_state
    except KeyboardInterrupt:
        print("KeyboardInterrupt")
        print("Exiting")