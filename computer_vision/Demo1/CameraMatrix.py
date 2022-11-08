# get the srcPythonClass folder avaliable
#import the stuff
# import camera
import sys
import cv2 
import numpy as np
import glob 
import pickle

# camera matrix is defined as a 3x3 matrix
#
# [ fx  0  cx ]
# [  0 fy  cy ]
# [  0  0   1 ]
# 
# 
# This link below is the best resource I have found for camera calibration
# https://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
# 
# https://docs.opencv.org/4.x/dc/dbb/tutorial_py_calibration.html

if __name__ == "__main__":
    # Define the dimensions of checkerboard
    CHECKERBOARD = (7, 10)
    
    
    # stop the iteration when specified
    # accuracy, epsilon, is reached or
    # specified number of iterations are completed.
    criteria = (cv2.TERM_CRITERIA_EPS +
                cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
    
    
    # Vector for 3D points
    threedpoints = []
    
    # Vector for 2D points
    twodpoints = []
    
    
    #  3D points real world coordinates
    objectp3d = np.zeros((1, CHECKERBOARD[0]
                        * CHECKERBOARD[1],
                        3), np.float32)
    objectp3d[0, :, :2] = np.mgrid[0:CHECKERBOARD[0],
                                0:CHECKERBOARD[1]].T.reshape(-1, 2)
    prev_img_shape = None
    
    
    # Extracting path of individual image stored
    # in a given directory. Since no path is
    # specified, it will take current directory
    # jpg files alone
    images = glob.glob('*.jpg')
    
    for filename in images:
        image = cv2.imread(filename)
        grayColor = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
        # Find the chess board corners
        # If desired number of corners are
        # found in the image then ret = true
        ret, corners = cv2.findChessboardCorners(
                        grayColor, CHECKERBOARD,
                        cv2.CALIB_CB_ADAPTIVE_THRESH
                        + cv2.CALIB_CB_FAST_CHECK +
                        cv2.CALIB_CB_NORMALIZE_IMAGE)
    
        # If desired number of corners can be detected then,
        # refine the pixel coordinates and display
        # them on the images of checker board
        if ret == True:
            threedpoints.append(objectp3d)
    
            # Refining pixel coordinates
            # for given 2d points.
            corners2 = cv2.cornerSubPix(
                grayColor, corners, (11, 11), (-1, -1), criteria)
    
            twodpoints.append(corners2)
    
            # Draw and display the corners
            image = cv2.drawChessboardCorners(image,
                                            CHECKERBOARD,
                                            corners2, ret)
    
        cv2.imshow('img', image)
        cv2.waitKey(500)
    
    cv2.destroyAllWindows()
    
    h, w = image.shape[:2]
    
    
    # Perform camera calibration by
    # passing the value of above found out 3D points (threedpoints)
    # and its corresponding pixel coordinates of the
    # detected corners (twodpoints)
    ret, matrix, distortion, r_vecs, t_vecs = cv2.calibrateCamera(
    threedpoints, twodpoints, grayColor.shape[::-1], None, None)
    # save the camera matrix and distortion coefficients
    cameraMatrix = [ret, matrix, distortion, r_vecs, t_vecs]

    # save the camera matrix and distortion coefficients
    with open('cameraMatrix.pkl', 'wb') as f:
        pickle.dump(cameraMatrix, f)

    # mean_error = 0
    # for i in range(len(objpoints)):
    #     imgpoints2, _ = cv2.projectPoints(threedpoints[i], rvecs[i], tvecs[i], mtx, dist)
    #     error = cv.norm(imgpoints[i], imgpoints2, cv.NORM_L2)/len(imgpoints2)
    #     mean_error += error
    # print( "total error: {}".format(mean_error/len(objpoints)) )

    # # print the parameters above 
    # print("ret: ", ret)
    # print("mtx: ", mtx)
    # print("dist: ", dist)
    # print("rvecs: ", rvecs)
    # print("tvecs: ", tvecs)

    # # print the types
    # print()
    # print("ret: ", type(ret))
    # print("mtx: ", type(mtx))
    # print("dist: ", type(dist))
    # print("rvecs: ", type(rvecs))
    # print("tvecs: ", type(tvecs))

    # img = cv.imread('1.jpg')
    # h,  w = img.shape[:2]
    # newcameramtx, roi = cv.getOptimalNewCameraMatrix(mtx, dist, (w,h), 1, (w,h))