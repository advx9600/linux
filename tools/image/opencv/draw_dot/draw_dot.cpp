#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace cv;
using namespace std;

int main()
{
    int i;
    int len,radio;

    IplImage* img = cvLoadImage("ic_launcher.png",CV_LOAD_IMAGE_UNCHANGED);
    if (img == NULL){
	printf ("load image failed!\n");
	return -1;
    }

    len=32;
    radio=3;
    int dots=14;
    int center_x=img->width/2-1;
    int center_y=img->height/2-1;
    double pi=3.1415926;
    for( i = 0; i < dots; i++ )
    {
        CvPoint pt;
        pt.x = center_x+cvRound(len*cos(2*pi/dots*i));
        pt.y = center_y+cvRound(len*sin(2*pi/dots*i));
        cvCircle( img, pt, radio, CV_RGB(255,0,0), CV_FILLED );
    }

    cvSaveImage("ic_launcher_light.png",img);
    cvShowImage("img",img);
    cvWaitKey(0);

    return 0;
}
