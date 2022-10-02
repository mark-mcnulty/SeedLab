import picamera 
import time
import cv2
import numpy
import keyboard


# define the class for the camera
class arducam:
    '''
    About Camera: https://www.raspberrypi.com/documentation/accessories/camera.html
    '''
    # define a function to initalize the object 
    def __init__(self):
        self.camera = picamera.PiCamera()
        self.FV = 53.50
        time.sleep(2)

    # define a function to capture images with the camera
    def capture(self, name="image.jpg"):
        if name[-4:] != ".jpg":
            name = name + ".jpg"
        self.camera.capture(name)

    # define a function to close the camera
    def close(self):
        self.camera.close()

    # make a setup function
    def setup(self):
        # self.camera.iso = 300
        # time.sleep(2)

        # self.camera.exposure_mode = 'off'
        # self.camera.awb_mode = 'off'
        # self.camera.drc_strength = 'off'

        # self.camera.shutter_speed = self.camera.exposure_speed
        # self.camera.exposure_mode = 'off'
        N = 10
        red = 0
        blue = 0
        for i in range(N):
            self.capture()
            g = self.camera.awb_gains
            red += g[0]
            blue += g[1]

            print(type(g))
            print(len(g))
            print(g[0])

        red_avg = red/N
        blue_avg = blue/N
        total_gains = (red_avg, blue_avg)
        print()
        print(total_gains)

        print("red: ", red_avg)
        print("blue: ", blue_avg)


        self.camera.awb_mode = 'off'
        self.camera.awb_gains = total_gains

    # define a function to display the image
    def display(self, name="image.jpg"):
        if name[-4:] != ".jpg":
            name = name + ".jpg"

        # display the image
        img = cv2.imread(name)
        cv2.imshow("image", img)
        time.sleep(1)
        cv2.destroyAllWindows()

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
        aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_6X6_50)
        parameters =  cv2.aruco.DetectorParameters_create()
        corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

        # return the ids of the markers
        # understanding corners:
        #                        corners[0][0][0][0] -> x
        #                        corners[0][0][0][1] -> y
        #                        corners[0][0][1][0] -> 2nd corner of the id marker 0
        return ids, corners

    def get_marker_location(name="image.jpg"):
        FOV = 53.50
        # check that the name has the correct extension
        if name[-4:] != ".jpg":
            name = name + ".jpg"

        # read the image
        img = cv2.imread(name)

        # get the size of the image
        h, w, c = img.shape
        size = (w, h)
        center = (w/2, h/2)

        # check if grey scale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # detect the markers
        aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_50)
        parameters =  cv2.aruco.DetectorParameters_create()
        corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

        # check if markers were found
        if ids is not None:
            # loop through if there are multiple markers

            for x in range(len(ids)):
                # find the mix right side and min left side
                min_x = 100000
                max_x = 0
                for i in range(len(corners[0][0])):
                    if corners[x][0][i][0] < min_x:
                        min_x = corners[0][0][i][0]
                    if corners[x][0][i][0] > max_x:
                        max_x = corners[0][0][i][0]

                # find the center of the image
                centerObject = (min_x + max_x) / 2
                print(centerObject)

                # find the angle 
                # feild of view divided by two multiplied by the ratio:
                # (center of the image - center of the screen) / (center of the screen - right side of the screen)
                angle = (FOV / 2) * ( (abs(centerObject - center[0])) / (abs(center[0] - size[0])) )
                print("marker", str(x), angle)
            
            
        else:
            print("No markers detected")
            return None
    
    # define a function that detects the quadrant the aruco marker
    # is in the image
    # the image should be split into 4 quadrants
    # the function should return the quadrant number
    # 1 | 0
    # 2 | 3
    def detect_quadrant(self, name="image.jpg"):
        FOV_X = 53.50
        FOV_Y = 41.41
        quadrant = None
        # check that the name has the correct extension
        if name[-4:] != ".jpg":
            name = name + ".jpg"

        # read the image
        img = cv2.imread(name)

        # get the size of the image
        h, w, c = img.shape
        size = (w, h)
        center = (w/2, h/2)

        # check if grey scale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # detect the markers
        aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_6X6_50)
        parameters =  cv2.aruco.DetectorParameters_create()
        corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

        # check if markers were found
        if ids is not None:
            # loop through if there are multiple markers
            for x in range(len(ids)):
                # find the mix right side and min left side
                min_x = 100000
                max_x = 0

                min_y = 100000
                max_y = 0
                for i in range(len(corners[0][0])):
                    # detect max_x and min_x
                    if corners[x][0][i][0] < min_x:
                        min_x = corners[0][0][i][0]
                    if corners[x][0][i][0] > max_x:
                        max_x = corners[0][0][i][0]

                    # detect max_y and min_y
                    if corners[x][0][i][1] < min_y:
                        min_y = corners[0][0][i][1]
                    if corners[x][0][i][1] > max_y:
                        max_y = corners[0][0][i][1]

                # find the center of the image
                centerObject_x = (min_x + max_x) / 2
                centerObject_y = (min_y + max_y) / 2
                
                # find what quadrant the marker is in
                # 1 | 0
                # 2 | 3
                if centerObject_x < center[0] and centerObject_y < center[1]:
                    quadrant = 1
                elif centerObject_x > center[0] and centerObject_y < center[1]:
                    quadrant = 0
                elif centerObject_x < center[0] and centerObject_y > center[1]:
                    quadrant = 2
                elif centerObject_x > center[0] and centerObject_y > center[1]:
                    quadrant = 3

            # return the quadrant
            return quadrant

        else:
            return None


if __name__ == "__main__":
    # make the object
    cam = arducam()

    # setup the camera
    cam.setup()

    # continuously capture images
    while True:
        # capture the image
        cam.capture()

        # display the image
        cam.display()

        # detect the quadrant
        quadrant = cam.detect_quadrant()

        # print the quadrant
        print(quadrant)

        # check if the user wants to quit
        if keyboard.is_pressed('q'):
            break



    # close the camera
    cam.close()

