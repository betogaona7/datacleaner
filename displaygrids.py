
import os
import subprocess 
import matplotlib.pyplot as plt
import cv2
import gc

def exportPaths(directory, file_name, append=True):
	mode = "a+"
	if not append:
		mode = "w"
	file = open(file_name, mode)
	imgs = os.listdir(directory)
	for img in imgs:
		file.write(directory+img)
		file.write("\n")
	file.close()

def imageSelected(event, figure):
	global bad_imgs
	axlist = figure.axes
	for i in range(len(axlist)):
		if axlist[i] == event.inaxes:
			try:
				print("Image: ", sample[i], " deleted.")
				os.remove(sample[i])
			except:
				print("The image had already been deleted")

def plotGrid(figures, nrows=1, ncols=1, fullsize=False):
	fig, axeslist = plt.subplots(ncols=ncols, nrows=nrows)
	for ind, title in zip(range(len(figures)), figures):
		axeslist.ravel()[ind].imshow(figures[title])
		axeslist.ravel()[ind].set_axis_off()
	fig.subplots_adjust(left=0, bottom=0, right=1, top=1, wspace=0, hspace=0.02)
	fig.canvas.mpl_connect("button_press_event", lambda event: imageSelected(event, fig))
	if fullsize:
		mng = plt.get_current_fig_manager()
		mng.resize(*mng.window.maxsize())
	plt.show()


def startCleaningData(file, columns=1, rows=1, fullsize=False):
	global sample
	with open(file) as f: 
		imgs = f.readlines()
	imgs = [x.strip() for x in imgs] 
	nimgs = columns*rows
	completed = True 
	# Return 33 grids of 100 images each 
	for i in range(0, len(imgs), nimgs):
		grid = {}
		sample = []
		for img in imgs[i:i+nimgs]:
			image = cv2.imread(img)
			image = cv2.resize(image, (300, 300)) #To speed up
			grid[img] = image
			sample.append(img)
			image=None
		plotGrid(grid, columns, rows, fullsize)
		del grid
		del sample
		answer = input("Next slot? [n/y]: ")
		if answer == "n" or answer =="N":
			print("No checkpoint saved.")
			completed = False
			break
	if completed: print("Data is now clean.")

directory1 = './../Desktop/Rover/opportunity/mer1om_0xxx/browse/navcam/'
directory2 = './../Desktop/Rover/spirit/mer2om_0xxx/browse/navcam/'

dst_pol = './../Desktop/Rover/ground-pol/'
dst_vrt = './../Desktop/Rover/ground-vrt/'


gc.collect()
# Separate POL and VRT projections 
#pol_o, vrt_o = separateProjections(directory1, dst_pol, dst_vrt)
#pol_s, vrt_s = separateProjections(directory2, dst_pol, dst_vrt)

#print("There are ", pol_o, " POL and ", vrt_o, " VRT projections for Opportunity") # 1151, 1316
#print("There are ", pol_s, " POL and ", vrt_s, " VRT porjections for Spirit") #383, 429
#print("Total: ", pol_s+pol_o, " POL", vrt_o+vrt_s, " VRT") # 1534, 1745 ... total 3279

# Save its paths on a file, so in the cleaning step you don't start over from zero each time
file = "unclean_data.txt"
#exportPaths(dst_vrt, file, False)
#exportPaths(dst_pol, file, False)

# Remove all the incomplete projections manually by displaying a grid of images and clicking on them to delete faster
sample = []
startCleaningData(file, 10, 10)
#print("Total: ", len(os.listdir(dst_pol)), " POL", len(os.listdir(dst_vrt)), " VRT") # 661, 676 ... total 1337




