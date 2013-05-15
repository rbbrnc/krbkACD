import sys
import cv

#HAAR_CASCADE_PATH = "/opt/opencv/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml"
HAAR_CASCADE_PATH = "/opt/opencv/share/OpenCV/haarcascades/haarcascade_frontalface_alt_tree.xml"
HAAR_CASCADE_PATH2 = "/opt/opencv/share/OpenCV/haarcascades/haarcascade_profileface.xml"

def detectObjects(image, haarCascadePath, fast=True):
	objs = []

	"""Converts an image to grayscale and prints the locations of any object found"""
	grayscale = cv.CreateImage((image.width, image.height), 8, 1)
	cv.CvtColor(image, grayscale, cv.CV_BGR2GRAY)
	storage = cv.CreateMemStorage()
	cv.EqualizeHist(grayscale, grayscale)
	haar = cv.Load(haarCascadePath)

	if fast:
		detected = cv.HaarDetectObjects(image, haar, storage, 1.2, 2, cv.CV_HAAR_DO_CANNY_PRUNING)
	else:
		detected = cv.HaarDetectObjects(image, haar, storage)

	if detected:
		for (x,y,w,h),n in detected:
			'''x,y,w,h,neighbors '''
			objs.append((x,y,w,h))
	return objs


def paintRectangles(image, rects, thickness=1):
	if rects:
		for (x, y, w, h) in rects:
			cv.Rectangle(image, (x, y), (x + w, y + h), 255, thickness)

def printRectangles(image, rects):
	if rects:
		for (x, y, w, h) in rects:
			print 'x:%d y:%d w:%d h:%d' % (x, y, w, h)

def printXmpMwgRegionsTags(image, rects):
	if rects:
		print 'Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:w %d' % (image.width)
		print 'Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:h %d' % (image.height)
		print 'Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:unit pixel'
		print "Xmp.mwg-rs.Regions/mwg-rs:RegionList ''"

		#-- we want to use float division
		imgW = float(image.width)
		imgH = float(image.height)

		index = 0
		for (x, y, w, h) in rects:
			index += 1;
			print 'Xmp.mwg-rs.Regions/mwg-rs:RegionList[%d]/mwg-rs:Name Region%d' % (index, index)
			print 'Xmp.mwg-rs.Regions/mwg-rs:RegionList[%d]/mwg-rs:Type Face' % (index)
			print 'Xmp.mwg-rs.Regions/mwg-rs:RegionList[%d]/mwg-rs:Area/stArea:x %f' % (index, x / imgW)
			print 'Xmp.mwg-rs.Regions/mwg-rs:RegionList[%d]/mwg-rs:Area/stArea:y %f' % (index, y / imgH)
			print 'Xmp.mwg-rs.Regions/mwg-rs:RegionList[%d]/mwg-rs:Area/stArea:w %f' % (index, w / imgW)
			print 'Xmp.mwg-rs.Regions/mwg-rs:RegionList[%d]/mwg-rs:Area/stArea:h %f' % (index, h / imgH)
			print 'Xmp.mwg-rs.Regions/mwg-rs:RegionList[%d]/mwg-rs:Area/stArea:unit normalized' % (index)


def main(file):
	''' Load the image '''
	image = cv.LoadImage(sys.argv[1], cv.CV_LOAD_IMAGE_COLOR)

	faces = []
	faces = detectObjects(image, HAAR_CASCADE_PATH, True)

	#-- check if is not emty (if not faces: test emptyness)
	if faces:
		printRectangles(image, faces)
		printXmpMwgRegionsTags(image, faces)
		paintRectangles(image, faces, 3)
		cv.SaveImage("test_det.png", image)

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print 'usage: python ' + sys.argv[0] + ' <file>'
		raise SystemExit

	main(sys.argv[1])





''' Python exiv2 code
import pyexiv2
md = pyexiv2.ImageMetadata('BabyGnuTux-Big.jpg')
md.read()
md['Xmp.MP.RegionInfo/MPRI:Regions'] = ['']
md['Xmp.MP.RegionInfo/MPRI:Regions[1]/MPReg:Rectangle'] = '0.11, 0.22, 0.33, 0.44'
md['Xmp.MP.RegionInfo/MPRI:Regions[1]/MPReg:PersonDisplayName'] = 'Baby Gnu'

md['Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:w'] = '1600'
md['Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:h'] = '800'
md['Xmp.mwg-rs.Regions/mwg-rs:AppliedToDimensions/stDim:unit'] = 'pixel'
md['Xmp.mwg-rs.Regions/mwg-rs:RegionList'] = ['']
md['Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Name'] = 'Baby Gnu'
md['Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Type'] = 'Face'
md['Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:x'] = '0.275312'
md['Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:y'] = '0.3775'
md['Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:w'] = '0.164375'
md['Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:h'] = '0.28125'
md['Xmp.mwg-rs.Regions/mwg-rs:RegionList[1]/mwg-rs:Area/stArea:unit'] = 'normalized'

md.write()
'''
