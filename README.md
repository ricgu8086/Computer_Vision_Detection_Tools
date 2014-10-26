Computer_Vision_Detection_Tools
===============================

This is a set of useful functions for working with detections in C++.

In our computer vision research laboratory [1] we are specialized in object detection and pose estimation. Therefore, we deal almost everyday with thousands of files containing the coordinates of all objects detected in images. We have Matlab code to process, analyze and adapt those files, but because of the huge ammount of files and the slow speed of Matlab I have ported that tools (and added new ones) to C++ in order to improve the performance, so we don't need to wait much time to run the next experiment.

As expected, C++ is much faster than Matlab (C++ is a compiled language and Matlab is interpreted), but when we analyzed the results, it turned out in an amazing improvement: the Non-maximal Suppression algorithm [2] ran in **Matlab** took about **30 seconds** per image. Now in **C++** we have times like **5.6e-07 seconds** or **6.3775e-05 seconds**.


Requirements
============
OpenCV Library. As it's described in their project webpage: "The Open Source Computer Vision Library has >2500 algorithms, extensive documentation and sample code for real-time computer vision. It works on Windows, Linux, Mac OS X, Android and iOS."
It can be downloaded at [3]

Usage
=====



[1] http://agamenon.tsc.uah.es/Investigacion/gram/
[1] http://sourceforge.net/projects/opencvlibrary/
[3] 
