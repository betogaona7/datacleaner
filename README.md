# datacleaner

**Cleaner.cpp**: It is a C++ program that helps me to quickly confirm that all the images have the same size, crop all of them by selecting a ROI, resize them, and rename all of them according to pattern.

**displaygrids.py**: It is a python program that uses matplot to display a grid of nxn images. It helps me to quickly remove undesired images just by clicking them. 

____

Cleaner commands: 
- `cleaner checksizes -s [srcpath] -e [img_ext]` 

- `cleaner resize -s [srcpath] -d [dstpath] -e [img_ext] -w [width] -h [height]` 

- `cleaner crop -s [srcpath] -d [dstpath] -e [img_ext]` 

- `cleaner rename -s [srcpath] -d [dstpath] -e [img_ext] -p [pattern]` 

___

NOTE: The program was made for my specific necessities, so it is no too flexible yet. e.g. the order and number of args matter. It will be modified in the future.