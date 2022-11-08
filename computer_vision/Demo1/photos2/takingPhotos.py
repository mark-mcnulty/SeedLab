import picamera
import cv2


if __name__ == '__main__':
    # Create an instance of the camera
    camera = picamera.PiCamera()

    try:
        while True:
            name = input("Enter the name of the file: ")

            
            # Take a picture
            camera.capture(name + '.jpg')
            
            img = cv2.imread(name + '.jpg')
            cv2.imshow("image", img)
            cv2.waitKey(0)
            cv2.destroyAllWindows()

    except: 
        print("Goodbye")

