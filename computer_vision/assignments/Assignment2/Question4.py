import cv2

def display(name="image.jpg"):
    if name[-4:] != ".jpg":
        name = name + ".jpg"

    img = cv2.imread(name)
    cv2.imshow("image", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def resize_image(name="image.jpg"):
    if name[-4:] != ".jpg":
        name = name + ".jpg"

    orig = cv2.imread(name)

    h, w, c = orig.shape

    img = cv2.resize(orig, (0,0), fx=0.5, fy=0.5)

    cv2.imwrite(name, img)



def convert_to_grayscale(name="image.jpg"):
    # check that the name has the correct extension
    if name[-4:] != ".jpg":
        name = name + ".jpg"

    # read the image
    img = cv2.imread(name)

    # convert the image to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # save the image
    cv2.imwrite(name, gray)


def detect_markers(name):
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

    # lists of ids and the corners beloning to each id
    corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

    # draw the markers
    if ids is not None:
        img = cv2.aruco.drawDetectedMarkers(img, corners, ids)
    else:
        print(f"No markers found in image {name}")

    # save the image
    cv2.imwrite("new"+name, img)

if __name__ == "__main__":
    for i in range(1, 5+1):
        convert_to_grayscale(f"{i}.jpg")
    
    for i in range(1, 5+1):
        detect_markers(f"{i}.jpg")


