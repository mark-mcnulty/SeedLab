import cv2

if __name__ == "__main__":
    name = "6.jpg"
    # Reading the image
    img = cv2.imread(name)

    h, w, c = img.shape

    print(type(img.shape))

    # convert the image to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # detect the markers
    aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_50)
    parameters =  cv2.aruco.DetectorParameters_create()
    parameters.minMarkerPerimeterRate = 0.2
    corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

    # draw the markers
    img = cv2.aruco.drawDetectedMarkers(img, corners, ids)

    # display the image
    cv2.imshow("image", img)
    cv2.waitKey(0)


