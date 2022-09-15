from camera import arducam


if __name__ == "__main__":
    # create an object
    cam = arducam()

    # capture images continuously
    # till the user presses the q key
    while True:
        # capture an image 
        cam.capture("temp.jpg")

        # see if the image contains an aruco marker
        id = cam.detect_aruco()

        # if the image contains an aruco marker
        if id == None:
            print("No marker found")
        else:
            print("Marker found: ", id)