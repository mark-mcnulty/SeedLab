import pickle
import cv2 as cv

if __name__ == "__main__":
    print("main")
    
    # open the pickle file
    with open('cameraMatrix.pkl', 'rb') as f:
        cameraMatrix = pickle.load(f)

    # assign the values
    ret = cameraMatrix[0]
    mtx = cameraMatrix[1]
    dist = cameraMatrix[2]
    rvecs = cameraMatrix[3]
    tvecs = cameraMatrix[4]

    # undistort the image
    img = cv.imread('1.jpg')

    h,  w = img.shape[:2]
    newcameramtx, roi = cv.getOptimalNewCameraMatrix(mtx, dist, (w,h), 1, (w,h))

    # undistort
    dst = cv.undistort(img, mtx, dist, None, newcameramtx)

    # crop the image
    x, y, w, h = roi
    dst = dst[y:y+h, x:x+w]
    cv.imwrite('calibresult.jpg', dst)