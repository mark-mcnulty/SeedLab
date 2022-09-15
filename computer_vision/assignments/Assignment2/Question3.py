import cv2

# define a function that will take an image and convert it to grayscale
def convert_to_grayscale(name):
    # read the image
    img = cv2.imread(name)

    # convert the image to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # save the image
    cv2.imwrite(name, gray)
    
if __name__ == "__main__":
    name = input("name of the file: ")
    if name[-4:] != ".jpg":
        name = name + ".jpg"
        
    convert_to_grayscale(name)
    
    # display the picture
    img = cv2.imread(name)
    cv2.imshow("image", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()