# get the srcPythonClass folder avaliable
import sys
cur_path = sys.path[0]
path_src_python = cur_path.replace("computer_vision\Demo1", "src\srcPythonClass")
path_src_python = path_src_python.replace("\\","/")
sys.path.append(path_src_python )

#import the stuff
import camera
import cv2
import numpy as np
import glob 

if __name__ == "__main__":
    # define the dimensions of checkerboard
    CHECKERBOARD = (6,9)

    # stop the iteration when specified accuracy, epsilon, is reached or
    # specified number of iterations are completed.
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

    # vector for 3d points
    threedpoints = []

    # vector for 2d points
    twodpoints = []

    # 3d points real world coordinates
    objectp3d = np.zeros((1, CHECKERBOARD[0] * CHECKERBOARD[1], 3), np.float32)
    objectp3d[0, :, :2] = np.mgrid[0:CHECKERBOARD[0], 0:CHECKERBOARD[1]].T.reshape(-1, 2)
    prev_img_shape = None

    #extracting path of individual image stored in a given directory
    images = glob.glob('*.jpg')

    for filename in images:
        img = cv2.imread(filename)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # find the chess board corners
        ret, corners = cv2.findChessboardCorners(gray, CHECKERBOARD, cv2.CALIB_CB_ADAPTIVE_THRESH + cv2.CALIB_CB_FAST_CHECK + cv2.CALIB_CB_NORMALIZE_IMAGE)

        if ret == True:
            threedpoints.append(objectp3d)

            # refining pixel coordinates for given 2d points.
            corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)

            twodpoints.append(corners2)

            # Draw and display the corners
            img = cv2.drawChessboardCorners(img, CHECKERBOARD, corners2, ret)
        cv2.imshow('img', img)
        cv2.waitKey(0)

cv2.destroyAllWindows()

h,w = img.shape[:2]

#performing camera calibration by
#passing the value of known 3D points (threedpoints)
#and corresponding pixel coordinates of the
#detected corners (twodpoints)
ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(threedpoints, twodpoints, gray.shape[::-1], None, None)

# Displaying required output
print(" Camera matrix : ")
print(mtx)

print(" dist : ")
print(dist)

print(" rvecs : ")
print(rvecs)

print(" tvecs : ")
print(tvecs)