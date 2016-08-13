import cv2
import numpy as np
import unicodedata
import copy

class Image:
    def __init__(self,name,img):
        def calculate(self):
            detector = cv2.AKAZE_create()
            keypoints, descriptors = detector.detectAndCompute(self.image,None)
            return keypoints, descriptors

        self.name = name
        self.image = img
        self.kp,self.des = calculate(self)

    def show(self):
        cv2.imshow(self.name, self.image)
        cv2.waitKey(0)

    def resizeMat(self, div):
        height,width = (self.image).shape[0:2]
        d = [0,0]
        if div[0][0] < 0:
            d[0]=div[0][0]
            for i in range(-1 * int(div[0][0])):
                self.image = np.insert(self.image,0,[0,0,0],axis = 1)
        if div[0][1] > width:
            for i in range(int(div[0][1]) - width + 3):
                self.image = np.insert(self.image,-1,[0,0,0],axis = 1)
        if div[1][0] < 0:
            d[1]=div[1][0]
            for i in range(-1 * int(div[1][0])):
                self.image = np.insert(self.image, 0, np.array((0, 0, 0)), 0)
        if div[1][1] > height:
            for i in range(int(div[1][1]) - height + 3):
                self.image = np.insert(self.image, -1, np.array((0, 0, 0)), 0)
        print(d)
        return d

    def resizeMat2(self, div):
        height, width = self.image.shape[0:2]
        d = [0, 0, width, height]
        if div[0][0] < 0:
            d[0] = div[0][0]
        if div[0][1] > width:
            d[2] = div[0][1]
        if div[1][0] < 0:
            d[1] = div[1][0]
        if div[1][1] > height:
            d[3] = div[1][1]
        T = np.array([[1.0, 0.0, -d[0]], [0.0, 1.0, -d[1]], [0.0, 0.0, 1]])
        print(d)
        self.image = cv2.warpPerspective(self.image, T, (int(-d[0] + d[2]), int(-d[1] + d[3])))
        return d


def resize_image(img):
    img = cv2.resize(img,(int(img.shape[1]*600/3024),int(img.shape[0]*800/4032)))
    for i in img:
        for j in i:
            if not j.any():
                j[0] = 1
                j[1] = 1
                j[2] = 1
    return img

def calcDst4(H, size):
    x = []
    y = []
    x.append(((H[0][0]*0 + H[0][1]*0 + H[0][2])/(H[2][0]*0 + H[2][1]*0 + H[2][2])))
    y.append(((H[1][0]*0 + H[1][1]*0 + H[1][2])/(H[2][0]*0 + H[2][1]*0 + H[2][2])))
    x.append(((H[0][0]*0 + H[0][1]*size[0] + H[0][2])/(H[2][0]*0 + H[2][1]*size[0] + H[2][2])))
    y.append(((H[1][0]*0 + H[1][1]*size[0] + H[1][2])/(H[2][0]*0 + H[2][1]*size[0] + H[2][2])))
    x.append(((H[0][0]*size[1] + H[0][1]*0 + H[0][2])/(H[2][0]*size[1] + H[2][1]*0 + H[2][2])))
    y.append(((H[1][0]*size[1] + H[1][1]*0 + H[1][2])/(H[2][0]*size[1] + H[2][1]*0 + H[2][2])))
    x.append(((H[0][0]*size[1] + H[0][1]*size[0] + H[0][2])/(H[2][0]*size[1] + H[2][1]*size[0] + H[2][2])))
    y.append(((H[1][0]*size[1] + H[1][1]*size[0] + H[1][2])/(H[2][0]*size[1] + H[2][1]*size[0] + H[2][2])))

    min_x = min(x)
    min_y = min(y)
    max_x = max(x)
    max_y = max(y)
    div = ((min_x, max_x),(min_y, max_y))
    print(div)
    return div

def SquareCheck(Mat, i, j):
    flag = 0
    for col in range(-2,3):
        if Mat[i+col,j-2].any() and Mat[i+col,j-1].any() and Mat[i+col,j].any() and Mat[i+col,j+1].any() and Mat[i+col,j+2].any():
            flag+=1
    if flag == 5:
        return True
    else:
        return False

def Write(target, src):
    print('-----------Writing------------')
    mask = np.ndarray((target.shape[0], target.shape[1]), dtype=np.uint8)
    for i in range(src.image.shape[0]):
        for j in range(src.image.shape[1]):
            if i < 3 or i > src.image.shape[0]-3 or j < 3 or j > src.image.shape[1]-3:
                if src.image[i,j].all():
                    if target[i,j].all():
                        target[i,j] = src.image[i,j]/2+target[i,j]/2
                        mask[i,j] = 255
                    else:
                        target[i,j] = src.image[i,j]
            else:
                if SquareCheck(src.image, i, j):
                    if SquareCheck(target, i, j):
                        target[i,j] = src.image[i,j]/2+target[i,j]/2
                        mask[i,j] = 255
                    else:
                        target[i,j] = src.image[i,j]
    print('-----------Wrote------------')
    return target, mask

def Write2(target, source, SrcMask):
    mask = cv2.cvtColor(SrcMask,cv2.COLOR_GRAY2RGB)
    target[(mask != [0,0,0])] = source[(mask != [0,0,0])]
    return target


def MakeMask(target, src):
    CommonMaskRGB = np.zeros((target.shape[0], target.shape[1], target.shape[2]), dtype=np.uint8)
    SrcMaskRGB = np.zeros((target.shape[0], target.shape[1], target.shape[2]), dtype=np.uint8)
    CommonMaskRGB[(target!=0) * (src!=0)] = 255
    SrcMaskRGB[(src!=[0,0,0]) * (CommonMaskRGB==[0,0,0])] = 255
    CommonMask = cv2.cvtColor(CommonMaskRGB,cv2.COLOR_RGB2GRAY)
    SrcMask = cv2.cvtColor(SrcMaskRGB,cv2.COLOR_RGB2GRAY)
    #CommonMask = cv2.morphologyEx(CommonMask, cv2.MORPH_CLOSE, np.ones((5,5),np.uint8))
    #SrcMask = cv2.morphologyEx(SrcMask, cv2.MORPH_CLOSE, np.ones((5,5),np.uint8))
    CommonMask = cv2.dilate(CommonMask,np.ones((5,5),np.uint8),iterations = 1)
    CommonMask = cv2.erode(CommonMask,np.ones((5,5),np.uint8),iterations = 1)
    SrcMask = cv2.dilate(SrcMask,np.ones((5,5),np.uint8),iterations = 1)
    SrcMask = cv2.erode(SrcMask,np.ones((5,5),np.uint8),iterations = 2)
    SrcMask = cv2.dilate(SrcMask,np.ones((5,5),np.uint8),iterations = 2)
    cv2.imshow('common',CommonMask)
    cv2.imshow('src',SrcMask)
    cv2.imwrite('common.png',CommonMask)
    cv2.imwrite('src.png',SrcMask)
    cv2.waitKey(0)
    return CommonMask, SrcMask

def make_panorama(original1,original2):
    matcher = cv2.BFMatcher(cv2.NORM_L2,False)
    matches = matcher.knnMatch(original1.des,original2.des,2)
    goodmatches = []
    trainkeys = []
    querykeys = []
    maskArray = []

    for i in matches:
        if i[0].distance < 500:
            if i[0].distance/i[1].distance < 0.8:
                print("\U0001F37A", end=' ')
                goodmatches.append(i[0])
                querykeys.append((original1.kp[i[0].queryIdx].pt[0],original1.kp[i[0].queryIdx].pt[1]))
                trainkeys.append((original2.kp[i[0].trainIdx].pt[0],original2.kp[i[0].trainIdx].pt[1]))

    print("-----Calculating Homography-----")
    H, status = cv2.findHomography(np.array(trainkeys),np.array(querykeys),cv2.RANSAC, 5.0)
    print('-----finished to calculate-----')
    div = calcDst4(H, original2.image.shape)
    d = original1.resizeMat2(div)
    print(original1.image.shape)
    T_xy = [[1,0,-d[0]],[0,1,-d[1]],[0,0,1]]
    panorama = cv2.warpPerspective(original2.image,np.dot(T_xy,H),(original1.image.shape[1],original1.image.shape[0]))
    #panorama, mask = Write(panorama,original1)
    CommonMask, SrcMask= MakeMask(panorama, original1.image)
    label = cv2.connectedComponentsWithStats(CommonMask)
    center = np.delete(label[3], 0, 0)
    print(center[0])
    blending = cv2.seamlessClone(original1.image, panorama, CommonMask, (int(center[0][0]),int(center[0][1])), cv2.NORMAL_CLONE)
    blending = Write2(blending, original1.image, SrcMask)
    cv2.imshow('blend',blending)
    print("--next--")
    return blending
