'''
    https://www.raspberrypi.com/documentation/accessories/camera.html
    horizontal FOV: 53.50 +/- 0.13 degrees 
'''
import cv2
import math



def get_marker_location(name):
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
        # find the mix right side and min left side
        min_x = 100000
        max_x = 0
        for i in range(len(corners[0][0])):
            if corners[0][0][i][0] < min_x:
                min_x = corners[0][0][i][0]
            if corners[0][0][i][0] > max_x:
                max_x = corners[0][0][i][0]

        # find the center of the image
        centerObject = (min_x + max_x) / 2

        # find the angle 
        # feild of view divided by two multiplied by the ratio:
        # (center of the image - center of the screen) / (center of the screen - right side of the screen)
        angle = (FOV / 2) * ( (abs(centerObject - center[0])) / (abs(center[0] - size[0])) )
        return angle
        
        
    else:
        print("No markers detected")
        return None


    


# in this function, you should detect the aruco marker
# and return the id of the marker
if __name__ == "__main__":
    print(get_marker_location("1"))

    