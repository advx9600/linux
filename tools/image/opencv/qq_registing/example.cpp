#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{

 Mat src1, src2,src3,src4;

 src1 = imread("/mnt/hgfs/Desktop/temp/one.jpg");
 src2 = imread("/mnt/hgfs/Desktop/temp/unregisted_phone.jpg");

 if( !src1.data ) { printf("Error loading src1 \n"); return -1; }
 if( !src2.data ) { printf("Error loading src2 \n"); return -1; }


 imwrite("/mnt/hgfs/Desktop/registing_0.jpg",src2);

 int N = 4;
 for (int i=0;i< N; i++)
 {
  int w = src1.rows;
  int h = src1.cols;

  Range R1; 
  R1.start=h-h/N*(i+1);
  R1.end =h;

  Range R2; 
  R2.start=0;
  R2.end =w; 

  src3=Mat(src1,R1,R2); // cut picture

  src4=src2(cv::Rect(0,h-h/N*(i+1),w,h/N*(i+1)));

  src3.copyTo(src4);
  imshow( "Linear Blend", src2 );
  
  char buf[100] ;
  sprintf(buf,"/mnt/hgfs/Desktop/registing_%d.jpg",i+1);
  imwrite(buf,src2);
//  waitKey(0);
 }


 return 0;
}
