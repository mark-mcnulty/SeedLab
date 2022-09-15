from picamera import PiCamera
from time import sleep
import sys 
import cv2 

# write a function that gets a file name from the user 
# and takes a picture with the camera 
# and saves it to the file name given by the user
def take_picture(name):
    # make the object
    camera = PiCamera()
    sleep(2)

    # take the picture
    camera.capture(name)

if __name__ == "__main__":
    print("hello world")
    # get the file name from the user
    name = input("Enter a file name: ")
    
    if name[-4:] != ".jpg":
        name = name + ".jpg"

    # take the picture
    take_picture(name)

    # display the picture
    img = cv2.imread(name)
    cv2.imshow("image", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
