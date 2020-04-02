[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0c2b3215b77a4a6f82d977986bca7842)](https://www.codacy.com/app/michelpromonet_2643/v4l2tools?utm_source=github.com&utm_medium=referral&utm_content=mpromonet/v4l2tools&utm_campaign=badger)
[![Build status](https://travis-ci.org/mpromonet/v4l2tools.png)](https://travis-ci.org/mpromonet/v4l2tools)
[![CircleCI](https://circleci.com/gh/mpromonet/v4l2tools.svg?style=shield)](https://circleci.com/gh/mpromonet/v4l2tools)
[![Snap Status](https://build.snapcraft.io/badge/mpromonet/v4l2tools.svg)](https://build.snapcraft.io/user/mpromonet/v4l2tools)


v4l2tools
====================

This is simple V4L2 tools based on libv4l2cpp.
v4l2reduceFPS_jpeg program which reduces FPS to a given value is added in this fork.

Dependencies
------------
 - liblog4cpp5-dev (optional)
 - libvpx-dev      (for v4l2compress_vpx)
 - libx264-dev     (for v4l2compress_h264)
 - libx265-dev     (for v4l2compress_x265)
 - libjpeg-dev     (for v4l2compress_jpeg & v4l2uncompress_jpeg)
 
Tools
-------

 - v4l2copy          : 

>	read from a V4L2 capture device and write to a V4L2 output device

 - v4l2convert_YUV          : 

>	read an YUV format from a V4L2 capture device, convert to an other YUV format and write to a V4L2 output device

 - v4l2compress_vpx  : 

>	read YUV from a V4L2 capture device, compress in VP8/VP9 format using libvpx and write to a V4L2 output device

 - v4l2compress_h264 : 

>	read YUV from a V4L2 capture device, compress in H264 format using libx264 and write to a V4L2 output device

 - v4l2compress_x265 : 

>	read YUV from a V4L2 capture device, compress in HEVC format using libx265 and write to a V4L2 output device

 - v4l2compress_jpeg : 

>	read YUYV from a V4L2 capture device, compress in JPEG format using libjpeg and write to a V4L2 output device

 - v4l2uncompress_jpeg : 

>	read JPEG format from a V4L2 capture device, uncompress in JPEG format using libjpeg and write to a V4L2 output device

 - v4l2dump          : 

>	read from a V4L2 capture device and print to output frame information (work with H264 & HEVC)

 - v4l2source_yuv :
 
>	generate YUYV frames and write to a V4L2 output device

- v4l2reduceFPS_jpeg :
 
>	read from a V4L2 capture device, reduce framerate and write to a V4L2 output device

Tools for Raspberry
-------------------

 - v4l2grab_h264     : 

>	grab raspberry pi screen, compress in H264 format using OMX and write to a V4L2 output device

 - v4l2display_h264     : 

>	read H264 from V4L2 capture device, uncompress and display using OMX

 - v4l2compress_omx : 

>	read YUV from a V4L2 capture device, compress in H264 format using OMX and write to a V4L2 output device

Build
-----

     make

Install
-------

     make install

