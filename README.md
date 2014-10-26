Computer_Vision_Detection_Tools
===============================

This is a set of useful functions for working with detections in C++.

In our computer vision research laboratory [1] we are specialized in object detection and pose estimation. Therefore, we deal almost everyday with thousands of files containing the coordinates of all objects detected in images. We have Matlab code to process, analyze and adapt those files, but because of the huge ammount of files and the slow speed of Matlab I have ported that tools (and added new ones) to C++ in order to improve the performance, so we don't need to wait much time to run the next experiment.

As expected, C++ is much faster than Matlab (C++ is a compiled language and Matlab is interpreted), but when we analyzed the results, it turned out in an amazing improvement: the Non-maximal Suppression algorithm (hereinafter referred as NMS) [2] ran in **Matlab** took about **40 seconds** per image. Now in **C++** we have times like **5.6e-07 seconds** or **6.3775e-05 seconds**.

There is a folder called `test` that contains some typical files used by us which contain in each line the coordinates, score and object's class of every detection found in the image with the same filename.

Requirements
============

* OpenCV Library. 
    As it's described in their project webpage: "The Open Source Computer Vision Library has >2500 algorithms,            extensive documentation and sample code for real-time computer vision. It works on Windows, Linux, Mac OS X,          Android and iOS."

    It can be downloaded at [3]

Usage
=====

Just compile the code using the `Makefile` given and run the executable `main` (or `main.exe` in Windows). This executable contains a demo that runs NMS algorithm in the selected file contained in the test folder.
In some cases, you should need to edit the `Makefile` to update the paths used in your system .

Notes
=====
The NMS implemented is a C++ - ported version from Tomasz Malisiewicz's Matlab-version of NMS algorithm based on Pedro Felzenszwalb's.

(The default parameter for overlap is 0.5, the same used in Pascal Voc Challenge database)

References
==========

[1] http://agamenon.tsc.uah.es/Investigacion/gram/

[2] http://quantombone.blogspot.com.es/2011/08/blazing-fast-nmsm-from-exemplar-svm.html

[3] http://sourceforge.net/projects/opencvlibrary/


