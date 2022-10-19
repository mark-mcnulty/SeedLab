import picamera 
import time
import cv2
import numpy


'''
NOTES:  
    - The aruco marker size is 5cm x 5cm
    - The field of view of the camera x is 62.2 degrees
    - the field of view of the camera y is 48.8 degrees

TODO:
    - Speed up marker detection
    - Make the camera class a singleton
    - Make the camera class a thread
    - Make the camera class a thread that can be started and stopped
    - Make the camera class a thread that can be started and stopped and restarted
'''

# define the class for the camera
class arducam:
    '''
    About Camera: https://www.raspberrypi.com/documentation/accessories/camera.html
    '''
    # define a function to initalize the object 
    def __init__(self):
        self.camera = picamera.PiCamera()
        self.FOV_X = 53.50
        self.FOV_Y = 41.41 
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

        red_avg = red/N
        blue_avg = blue/N
        total_gains = (red_avg, blue_avg)

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
        cv2.waitKey(1000)
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
 
    # define a function that will calculate the angle between the camera and the marker
    def get_marker_location(self, name="image.jpg"):
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

                # find the center of the aruco marker
                centerObject = (min_x + max_x) / 2

                # if the center of the aruco marker is less than the center of the image
                # then the aruco marker is to the left of the center of the image making the angle negative
                if centerObject < center[0]:
                    angle = -1 * (self.FOV_X / 2) * ( (abs(centerObject - center[0])) / (abs(center[0] - size[0])) )
                else:
                    angle = (self.FOV_X / 2) * ( (abs(centerObject - center[0])) / (abs(center[0] - size[0])) )

                # return the angle
                return angle
            
        else:
            return None
    
    # define a function that detects the quadrant the aruco marker
    # is in the image
    # the image should be split into 4 quadrants
    # the function should return the quadrant number
    # 2 | 1
    # 3 | 4
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
                # 2 | 1
                # 3 | 4
                if centerObject_x < center[0] and centerObject_y < center[1]:
                    quadrant = 2
                elif centerObject_x > center[0] and centerObject_y < center[1]:
                    quadrant = 1
                elif centerObject_x < center[0] and centerObject_y > center[1]:
                    quadrant = 3
                elif centerObject_x > center[0] and centerObject_y > center[1]:
                    quadrant = 4

            # return the quadrant
            return quadrant

        else:
            return None

    # define a function that will calculate the camera matrix
    # this matrix is how the camera sees the world
    # the camera matrix is used to calculate the rotation and translation vectors
    def get_camera_matrix(self, name="image.jpg"):
        print("getting camera matrix")

    


if __name__ == "__main__":
    # make the object
    cam = arducam()

    # continuously capture images
    while True:
        try:
            # capture the image
            cam.capture()


            # detect the angle
            angle = cam.get_marker_location()

            # print the quadrant
            print(angle)


        # if there is a keyboard interrupt
        # exit the program
        # make sure to use ctrl + c to exit
        # ctrl + z will pause the program and not exit
        # this will result in the camera getting hung up
        except KeyboardInterrupt:
            print("Exiting")
            # close the camera
            cam.close()

            break

        # just for fun
        print()



