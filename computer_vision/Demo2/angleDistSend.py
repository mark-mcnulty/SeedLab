import camera

# define gloabl variables
ANGLE = 0
DISTANCE = 0

def look(cam):
    # setup
    ANGLE = 0
    DISTANCE = 0

    # take picture
    cam.capture()

    # check for object
    # detect the markers
    corners, ids, h, w = cam.detect_markers_undistorted()

    # check if the markers are found
    if ids is not None:
        return calc
    else:
        return look



def calc(cam):
    # calculate the angle and distance of the aruco marker
    corners, ids, h, w = cam.detect_markers_undistorted()

    # calculate the angle
    DISTANCE = cam.get_marker_distance_func("image.jpg", corners, ids, h, w)
    ANGLE = cam.get_marker_angle("image.jpg", corners, ids, h, w)

    return send

def send(cam):
    # 
    print(DISTANCE)
    print(ANGLE)

    return look

state_dictionary = {
    look: "looking",
    calc: "calc",
    send: "sending"
}


# create main loop
if __name__ == "__main__":
    state = look

    cam = camera.arducam()

    try:
        while True:
            new_state = state(cam)
            state = new_state
    except KeyboardInterrupt:
        print("KeyboardInterrupt")
        print("Exiting")