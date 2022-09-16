import cv2

# define a function that takes the name of an image file
# and then resizes it to half the size of the original
def resize_image(name):
    # read the image
    orig = cv2.imread(name)

    h, w, c = orig.shape

    # resize the image
    img = cv2.resize(orig, (0,0), fx=0.5, fy=0.5)
    h, w, c = img.shape
    
    # save the image
    cv2.imwrite("newSize"+name, img)
    
if __name__ == "__main__":
    print()
    name = input("image name to resize: ")
    if name[-4:] != ".jpg":
        name = name + ".jpg"
    resize_image(name)
    
    # display the picture
    img = cv2.imread(name)
    cv2.imshow("image", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()