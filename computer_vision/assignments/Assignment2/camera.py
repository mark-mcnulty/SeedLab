import picamera 
import time
import cv2
import numpy

class arducam:
    # define a function to initalize the object 
    def __init__(self):
        self.camera = picamera.PiCamera()
        time.sleep(2)

    # define a function to capture images with the camera
    def capture(self, name="image.jpg"):
        if name[-4:] != ".jpg":
            name = name + ".jpg"
        self.camera.capture(name)

    # define a function to close the camera
    def close(self):
        self.camera.close()

    # define a function to display the image
    def display(self, name="image.jpg"):
        if name[-4:] != ".jpg":
            name = name + ".jpg"

        img = cv2.imread(name)
        cv2.imshow("image", img)

    # define a function to resize the image
    # the resized image should be half the original size
    def resize_image(self, name="image.jpg"):
        if name[-4:] != ".jpg":
            name = name + ".jpg"

        orig = cv2.imread(name)

        h, w, c = orig.shape

        img = cv2.resize(orig, (0,0), fx=0.5, fy=0.5)

        cv2.imwrite(name, img)

    # define a function to convert the image to grayscale
    # the grayscale image should be saved with the same name
    def convert_to_grayscale(self, name="image.jpg"):
        # check that the name has the correct extension
        if name[-4:] != ".jpg":
            name = name + ".jpg"

        # read the image
        img = cv2.imread(name)

        # convert the image to grayscale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # save the image
        cv2.imwrite(name, gray)

    # define a function that detects aruco markers in the image
    def detect_markers(self, name):
        # check that the name has the correct extension
        if name[-4:] != ".jpg":
            name = name + ".jpg"

        # read the image
        img = cv2.imread(name)

        # convert the image to grayscale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # detect the markers
        aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_50)
        parameters =  cv2.aruco.DetectorParameters_create()
        corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

        # return the ids of the markers
        return ids

        # # draw the markers
        # img = cv2.aruco.drawDetectedMarkers(img, corners, ids)

        # # save the image
        # cv2.imwrite("new"+name, img)

if __name__ == "__main__":
    print("main")
