from camera import arducam


if __name__ == "__main__":
    # create an object
    cam = arducam()

    # capture images continuously
    # till the user presses the q key
    while True:
        name = "temp.jpg"
        # capture an image 
        cam.capture(name)

        # see if the image contains an aruco marker
        id = cam.detect_markers(name)

        # if the image contains an aruco marker
        if id == []:
            print("No marker found")
        else:
            print("Marker found: ", id)

        # check if the user pressed the q key
