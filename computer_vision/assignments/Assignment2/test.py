import cv2

if __name__ == "__main__":
    name = "two.jpg"
    # Reading the image
    img = cv2.imread(name)

    h, w, c = img.shape

    # convert the image to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # detect the markers
    aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_50)
    parameters =  cv2.aruco.DetectorParameters_create()
    print(help(cv2.aruco.DetectorParameters_create()))
    corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

