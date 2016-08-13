import cv2
import numpy as np
import sys

from pano_sub import *

if __name__=="__main__":
    image_names = sys.argv
    if len(image_names) == 1:
        print("usage: python3 panorama.py image1 image2 image3 ..")
        exit()

    if len(image_names) == 2:
        print("usage: ２枚以上画像のパスを渡してください")
        exit()

    print("--start--")
    images = []
    panorama = []
    for i in range(1,len(image_names)):
        print(image_names[i])
        img = resize_image(cv2.imread(image_names[i],cv2.IMREAD_COLOR))
        images.append(Image(str(i), img))

    panorama.append(Image(images[0].name, images[0].image))

    for i in range(0,len(images)-1):
        print(i)
        panorama.append(Image(str(i+1),make_panorama(panorama[i],images[i+1])))

    cv2.imwrite("panorama.png",panorama[-1].image)
    cv2.imshow("panorama.png",panorama[-1].image)

    #for i in range(0,len(panorama)):
    #   cv2.imshow("result"+str(i),panorama[i].image)

    print("--FIN--")
    cv2.waitKey(0)
