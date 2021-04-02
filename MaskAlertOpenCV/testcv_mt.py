# Name: Yumeng Jiang
# Couse_Number: EECE4354
# Assignment#: 03
# Date: 03/25/2021

"""
Example code for live video processing
Also multithreaded video processing sample using opencv 3.4

Usage:
   python testcv_mt.py {<video device number>|<video file name>}

   Use this code as a template for live video processing

   Also shows how python threading capabilities can be used
   to organize parallel captured frame processing pipeline
   for smoother playback.

Keyboard shortcuts: (video display window must be selected

   ESC - exit
   space - switch between multi and single threaded processing
   a - enable feature matching
   c - displays harris corner detector
   d - running difference of current and previous imagesmveeve
   e - displays canny edges
   f - displays raw frames
   g - displays gaussian filtering
   l - displays laplacian gradient
   m - displays median filtering
   n - turns on NMS
   s - displays sobel gradiant
   o - displays ORB feature detector
   p - capture image
   v - write video output frames to file "vid_out.avi"
"""

from collections import deque
from multiprocessing.pool import ThreadPool
from random import shuffle
from ssc import ssc

# import the necessary packages
import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

import video
from common import draw_str, StatValue
from time import perf_counter, sleep


# used to execute process_frame when in non threaded mode
class DummyTask:
    def __init__(self, data):
        self.data = data

    @staticmethod
    def ready():
        return True

    def get(self):
        return self.data


# initialize global variables
frame_counter = 0
show_frames = True
diff_frames = False
show_edges = False
vid_frames = False
gaussian_filter = False
median_filter = False
sobel_gradient = False
laplacian_grad = False
show_corners = False
orb = None
orb_init = False
do_orb = False
feature_match = False
capped = 0
SE_diameter = 7
SE = cv.getStructuringElement(cv.MORPH_ELLIPSE, (SE_diameter, SE_diameter), None)

do_nms = False
frame1 = None
frame2 = None
frame_match = None
MATCH_NUM = 50
bf = None
matched = False

# trackbars
threshold = 128
threshold_slider_max = 255

# canny edge detector
lower = 100
upper = 200
upper_slider_max = 255
lower_slider_max = 255

# gaussian filter
int_sigma = 0
gauss_slider_max = 13
sqrt2ov2 = np.sqrt(2) / 2

corner_tol = 0.1


# this routine is run each time a new video frame is captured
def process_frame(_frame, _prevFrame, _currCount):
    global lower, upper
    global gaussian_filter, show_frames, show_edges, diff_frames, show_corners, median_filter
    global sobel_gradient, laplacian_grad, do_orb
    global frame1, frame2, MATCH_NUM, bf, frame_match

    # gaussian_filter
    if gaussian_filter:
        _frame = GaussianFilter(_frame)
        if _frame.dtype != np.dtype(np.uint8):
            imin = _frame.min()
            imax = _frame.max()
            if imax > imin:
                _frame = np.uint8(255 * (_frame - imin) / (imax - imin))

    if not show_frames and show_edges:  # edges alone
        if lower == 0 and upper == 0:
            upper = 1
        if lower > upper:
            lower = upper - 1
            cv.setTrackbarPos('lower', 'video', lower)
        edges = cv.Canny(_frame, upper, lower)
        _frame = cv.cvtColor(edges, cv.COLOR_GRAY2RGB, edges)
    elif show_frames and show_edges:  # edges and frames
        if lower > upper:
            lower = upper - 1
            cv.setTrackbarPos('lower', 'video', lower)
        edges = cv.Canny(_frame, 100, 200)
        edges = cv.cvtColor(edges, cv.COLOR_GRAY2RGB, edges)
        _frame = cv.add(_frame, edges)
    else:  # current frame
        _frame = _frame.copy()

    if diff_frames:
        # compute absolute difference between the current and previous frame
        difframe = cv.absdiff(_frame, _prevFrame)
        # save current frame as previous
        _prevFrame = _frame.copy()
        # set the current frame to the difference image
        _frame = difframe.copy()
    else:
        # save current frame as previous
        _prevFrame = _frame.copy()

    # feature matching if we have two frames
    if feature_match and (frame1 is not None) and (frame2 is not None):
        # initialize ORB if needed
        if not orb_init:
            init_orb()

        # initialize bf matcher if needed
        if bf is None:
            bf = cv.BFMatcher_create(normType=cv.NORM_HAMMING, crossCheck=True)

        # assign frame, kp, and des for each frame after getting orb features
        # False means we don't mark the keypoint locations on the images
        frame1, kp1, des1 = get_orb_features(frame1, False)
        frame2, kp2, des2 = get_orb_features(frame2, False)

        # match descriptors
        matches = bf.match(des1, des2)

        # sort the match in the order of distance
        matches = sorted(matches, key=lambda x: x.distance)

        # make the matched frame with the best (MATCH_NUM) matches
        frame_match = cv.drawMatches(frame1, kp1, frame2, kp2, matches[:MATCH_NUM], None,
                                     flags=cv.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

    # median_filter
    if median_filter:
        _frame = cv.medianBlur(_frame, 5)

    # sobel_gradient
    if sobel_gradient:
        _frame = cv.Sobel(_frame, cv.CV_8U, 1, 0, ksize=5)

    if laplacian_grad:
        _frame = cv.Laplacian(_frame, cv.CV_8U)

    if show_corners:
        _frame = get_harris_corners(_frame)

    if do_orb:
        if not orb_init:
            init_orb()
        _frame = get_orb_features(_frame)

    return _frame, _prevFrame, _currCount


def GaussianFilter(_frame):
    global int_sigma, sqrt2ov2

    # if int_sigma == 0 the Gaussian is an identity, so do nothing
    if int_sigma != 0:
        sigma = int_sigma * sqrt2ov2

        # compute the width of the Gaussian kernel as a function of sigma
        ksize = int(np.rint(((((sigma - 0.8) / 0.3) + 1) / 0.5) + 1))
        # k has to be an odd number
        if np.mod(ksize, 2) == 0:
            ksize += 1

        gkern = cv.getGaussianKernel(ksize, sigma)
        _frame = cv.sepFilter2D(_frame, cv.CV_32F, gkern, gkern, borderType=cv.BORDER_REFLECT101)

    return _frame


def get_harris_corners(_frame):
    global do_nms, corner_count, SE, SE_diameter
    blocksize = 5
    ksize = 5
    ktrace = 0.05
    nstart = np.int(SE_diameter / 2)
    nstop = np.int((SE_diameter + 1) / 2)

    gray_image = np.float32(cv.cvtColor(_frame, cv.COLOR_BGR2GRAY))
    img_dims = gray_image.shape
    corner_strength = np.clip(cv.cornerHarris(gray_image, blocksize, ksize, ktrace), 0, None)
    retval, not_weak_corners = cv.threshold(src=corner_strength,
                                            thresh=0.02 * corner_strength.max(None),
                                            maxval=0,
                                            type=cv.THRESH_TOZERO)
    if do_nms:
        corner_dilated = cv.dilate(not_weak_corners, SE, iterations=2)
        corner_mask = np.uint8(np.multiply(np.equal(not_weak_corners, corner_dilated), not_weak_corners) > 0)
    else:
        corner_mask = np.uint8(not_weak_corners > 0)

    corner_count, corner_labels, corner_stats, corner_centroids = cv.connectedComponentsWithStats(corner_mask)
    criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_COUNT, 30, 0.1)
    corners = cv.cornerSubPix(gray_image, np.float32(corner_centroids), (5, 5), (-1, -1), criteria)
    corners = corners[:, np.newaxis, :]

    keypoints = []
    descriptors = []
    for i in range(corners.shape[0]):
        loc = np.int32(corners[i])
        x, y = loc[0]
        if SE_diameter <= x <= (img_dims[1] - SE_diameter) and SE_diameter <= y <= (img_dims[0] - SE_diameter):
            nbhd = gray_image[y - nstart: y + nstop, x - nstart: x + nstop]
            if np.any(nbhd):
                strength = corner_strength[y, x]
                keypoints.append(cv.KeyPoint(np.float(x), np.float(y), _size=SE_diameter, _response=strength))
                nbhd = nbhd - np.mean(nbhd)
                descriptors.append(nbhd.flatten())
    _frame = cv.drawKeypoints(_frame, keypoints, None, color=(0, 0, 255))

    return _frame


def init_orb():
    global orb, orb_init
    orb = cv.ORB_create(nfeatures=5000,
                        scaleFactor=1.2,
                        nlevels=8,
                        edgeThreshold=31,
                        firstLevel=0,
                        WTA_K=2,
                        scoreType=cv.ORB_HARRIS_SCORE,
                        patchSize=31,
                        fastThreshold=10)
    orb_init = True


def get_orb_features(_frame, orb_mark):
    global orb, do_nms, corner_tol
    keypoint_img = np.empty((_frame.shape[0], _frame.shape[1], 3), dtype=np.uint8)

    kp, des = orb.detectAndCompute(_frame, None)

    if do_nms:
        shuffle(kp)
        kp = ssc(keypoints=kp,
                 num_ret_points=len(kp),
                 tolerance=corner_tol,
                 cols=_frame.shape[1],
                 rows=_frame.shape[0])
        kp, des = orb.compute(_frame, kp)
    cv.drawKeypoints(_frame, kp, keypoint_img, color=(0, 255, 0), flags=0)

    if not orb_mark:
        return _frame, kp, des

    return keypoint_img, kp, des


def on_upper_trackbar(val):
    global upper
    upper = val


def on_lower_trackbar(val):
    global lower
    lower = val


def on_gauss_trackbar(val):
    global int_sigma
    int_sigma = val


def create_canny_trackbars():
    global upper, upper_slider_max, lower, lower_slider_max
    cv.createTrackbar("upper", "video", upper, upper_slider_max, on_upper_trackbar)
    cv.createTrackbar("lower", "video", lower, lower_slider_max, on_lower_trackbar)


def create_gaussian_trackbar():
    global int_sigma, gauss_slider_max
    cv.createTrackbar("rt2/2 * ", 'video', int_sigma, gauss_slider_max, on_gauss_trackbar)


# create a video capture object
# noinspection DuplicatedCode
def create_capture(source=0):
    # parse source name (defaults to 0 which is the first USB camera attached)

    source = str(source).strip()
    chunks = source.split(':')
    # handle drive letter ('c:', ...)
    if len(chunks) > 1 and len(chunks[0]) == 1 and chunks[0].isalpha():
        chunks[1] = chunks[0] + ':' + chunks[1]
        del chunks[0]
    source = chunks[0]
    try:
        source = int(source)
    except ValueError:
        pass

    params = dict(s.split('=') for s in chunks[1:])

    # video capture object defined on source

    timeout = 100
    _iter = 0
    _cap = cv.VideoCapture(source)
    while (_cap is None or not _cap.isOpened()) & (_iter < timeout):
        sleep(0.1)
        _iter = _iter + 1
        _cap = cv.VideoCapture(source)

    if _iter == timeout:
        print('camera timed out')
        return None
    else:
        print(_iter)

    if 'size' in params:
        w, h = map(int, params['size'].split('x'))
        _cap.set(cv.CAP_PROP_FRAME_WIDTH, w)
        _cap.set(cv.CAP_PROP_FRAME_HEIGHT, h)

    if _cap is None or not _cap.isOpened():
        print('Warning: unable to open video source: ', source)
        return None

    return _cap


# main program
if __name__ == '__main__':
    import sys

    # print in the program shell window the text at the beginning of the file
    print(__doc__)

    # if there is no argument in the program invocation default to camera 0
    # noinspection PyBroadException
    # try:
    #     fn = sys.argv[1]
    # except:
    #     fn = 0
    if len(sys.argv) < 2:
        fn = 0
    else:
        fn = sys.argv[1]

    # grab initial frame, create window
    cv.waitKey(1) & 0xFF
    cap = video.create_capture(fn)
    ret, frame = cap.read()
    frame_counter += 1
    height, width, channels = frame.shape
    prevFrame = frame.copy()
    cv.namedWindow("video")

    # Create video of Frame sequence -- define the codec and create VideoWriter object
    fourcc = cv.VideoWriter_fourcc(*'XVID')
    cols = np.int(cap.get(cv.CAP_PROP_FRAME_WIDTH))
    rows = np.int(cap.get(cv.CAP_PROP_FRAME_HEIGHT))
    vid_out = cv.VideoWriter('vid_out.avi', fourcc, 20.0, (cols, rows))

    # Set up multiprocessing
    threadn = cv.getNumberOfCPUs()
    pool = ThreadPool(processes=threadn)
    pending = deque()

    threaded_mode = True
    onOff = False

    # initialize time variables
    latency = StatValue()
    frame_interval = StatValue()
    last_frame_time = perf_counter()

    # main program loop
    while True:
        while len(pending) > 0 and pending[0].ready():  # there are frames in the queue
            res, prevFrame, t0 = pending.popleft().get()
            latency.update(perf_counter() - t0)
            # plot info on threading and timing on the current image
            # comment out the next 3 lines to skip the plotting
            draw_str(res, (20, 20), "threaded      :  " + str(threaded_mode))
            draw_str(res, (20, 40), "latency        :  %.1f ms" % (latency.value * 1000))
            draw_str(res, (20, 60), "frame interval :  %.1f ms" % (frame_interval.value * 1000))
            if vid_frames:
                vid_out.write(res)
            # show the current image
            cv.imshow('video', res)

        if len(pending) < threadn:  # fewer frames than thresds ==> get another frame
            # get frame
            ret, frame = cap.read()
            frame_counter += 1
            t = perf_counter()
            frame_interval.update(t - last_frame_time)
            last_frame_time = t
            if threaded_mode:
                task = pool.apply_async(process_frame, (frame.copy(), prevFrame.copy(), t))
            else:
                task = DummyTask(process_frame(frame, prevFrame, t))
            pending.append(task)

        # check for a keypress
        key = cv.waitKey(1) & 0xFF

        # threaded or non threaded mode
        if key == ord(' '):
            threaded_mode = not threaded_mode

        # feature matching
        if key == ord('a'):
            feature_match = not feature_match
            if feature_match:
                matched = False

        # showing matched feature and saving it after feature matching
        if feature_match and (frame_match is not None) and (not matched):
            cv.namedWindow('Matched points')
            cv.imshow('Matched points', frame_match)
            cv.waitKey(1) & 0xFF
            cap_out = cv.imwrite('match.jpg', frame_match)
            if cap_out:
                print("Capture is being output to image")
            else:
                print("Capture is not being output to video")
            matched = True

        # toggle Harris corner detection
        if key == ord('c'):
            show_corners = not show_corners
            if show_corners:
                do_orb = False
            # else:
            #     do_nms = True

        # image difference mode
        if key == ord('d'):
            diff_frames = not diff_frames

        # toggle edges
        if key == ord('e'):
            show_edges = not show_edges
            if show_edges:
                create_canny_trackbars()
            if not show_edges:
                cv.destroyWindow('video')
                cv.namedWindow('video')
                cv.imshow('video', frame)
            if not show_edges and not show_frames:
                show_frames = True

        # toggle frames
        if key == ord('f'):
            show_frames = not show_frames
            if not show_frames and not show_edges:
                show_frames = True

        # gaussian mode
        if key == ord('g'):
            gaussian_filter = not gaussian_filter
            if gaussian_filter:
                create_gaussian_trackbar()

        # laplacian gradient
        if key == ord('l'):
            laplacian_grad = not laplacian_grad

        # median mode
        if key == ord('m'):
            median_filter = not median_filter

        # nms
        if key == ord('n'):
            do_nms = not do_nms

        # orb feature detector
        if key == ord('o'):
            do_orb = not do_orb

        # screenshot the video to frame 1 and frame2.
        # If we have more screenshots, we will keep the previous frame as frame 1 and the new one as frame2
        if key == ord('p'):
            if capped == 0:
                frame1 = frame.copy()
                cv.namedWindow('capture')
                cv.imshow('capture', frame1)
                cv.waitKey(1) & 0xFF
                cap_out = cv.imwrite('cap_out.jpg', frame1)
                if cap_out:
                    print("Capture is being output to image")
                else:
                    print("Capture is not being output to video")
                capped += 1;
            elif capped == 1:
                frame2 = frame.copy()
                cv.imshow('capture', cv.hconcat([frame1, frame2]))
                cv.waitKey(1) & 0xFF
                cap_out = cv.imwrite('cap_out2.jpg', frame2)
                if cap_out:
                    print("Capture is being output to image")
                else:
                    print("Capture is not being output to video")

                capped += 1
            else:
                frame1 = frame2
                frame2 = frame.copy()
                cv.imshow('capture', cv.hconcat([frame1, frame2]))
                cap_out1 = cv.imwrite('cap_out.jpg', frame1)
                cap_out2 = cv.imwrite('cap_out2.jpg', frame2)
                if cap_out1 and cap_out2:
                    print("Capture is being output to image")
                else:
                    print("Capture is not being output to video")

        # sobel gradient
        if key == ord('s'):
            sobel_gradient = not sobel_gradient

        # output video
        if key == ord('v'):
            vid_frames = not vid_frames
            if vid_frames:
                print("Frames are being output to video")
            else:
                print("Frames are not being output to video")

        # ESC terminates the program
        if key == 27:
            # release video capture object
            cap.release()
            # release video output object
            vid_out.release()
            cv.destroyAllWindows()
            break
