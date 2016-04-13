#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>

// Bresenham's algorithm
void myLine(IplImage* img, CvPoint pt1, CvPoint pt2) {
    //cvNamedWindow("draw", CV_WINDOW_AUTOSIZE);
    
    // ww >= hh
    int x, y, xx, yy, ww, hh;
    int j = 1;
    int i = 1;
    xx = 0; yy = 0;
    ww = pt2.x - pt1.x;
    hh = pt2.y - pt1.y;
    if (ww < 0) j = -1;
    if (hh < 0) i = -1;
    
    double e = 0;
    if (ww*j > hh*i) {
        while(xx*j < ww*j){
            xx += j;
            int hs = 2*hh*i;
            int ws = 2*ww*j;
            e += hs;
            if (e >= ws){
                e -= ws;
                yy += i;
            }
            
            x = pt1.x + xx;
            y = pt1.y + yy;
            if (x < 0 || y < 0 || x > img->width || y > img->height) {
                continue;
            }
            int a = img->widthStep * y + (x*3);
            img->imageData[a+0] = 255;
            img->imageData[a+1] = 255;
            img->imageData[a+2] = 255;
            
//            cvShowImage("draw", img);
//            cvWaitKey(1);
        }
    } else {
        while(yy*i < hh*i){
            yy += i;
            int hs = 2*hh*i;
            int ws = 2*ww*j;
            e += ws;
            if (e >= hs){
                e -= hs;
                xx += j;
            }
            
            x = pt1.x + xx;
            y = pt1.y + yy;
            if (x < 0 || y < 0 || x > img->width || y > img->height) {
                continue;
            }
            int a = img->widthStep * y + (x*3);
            img->imageData[a+0] = 255;
            img->imageData[a+1] = 255;
            img->imageData[a+2] = 255;
            
//            cvShowImage("draw", img);
//            cvWaitKey(1);
        }
    }
    
}

int main (int argc, char **argv)
{
    int x, y;
    int width=1025, height=978;
    IplImage *img=0, *hsv_img=0;
    double c, f;
    
    // (1)allocate and initialize an image
    img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
    if(img==0) return -1;
    cvZero(img);
    hsv_img = cvCloneImage(img);
    cvCvtColor(img, hsv_img, CV_BGR2HSV);
    f = cvGetTickFrequency()*1000;
    
    // (2) draw hexagone
    int cx = 500;
    int cy = 500;
    double radius = 100;
    double angle = 0;
    CvScalar color = cvScalarAll(255);
    
    for (int i=0; i<6; i++) {
        double theta1 = (angle + i*60) / 180 * M_PI;
        double theta2 = (angle + (i+1)%6*60) / 180 * M_PI;
        CvPoint pt1 = cvPoint(cx + cos(theta1)*radius, cy + sin(theta1)*radius);
        CvPoint pt2 = cvPoint(cx + cos(theta2)*radius, cy + sin(theta2)*radius);
        // cvLine(img, pt1, pt2, color, 1, 8, 0);
        myLine(img, pt1, pt2);
    }
    
    // (3)show the iamge, and quit when any key pressed
    cvNamedWindow ("hexagon", CV_WINDOW_AUTOSIZE);
    cvShowImage ("hexagon", img);
    cvWaitKey (0);
    
    cvDestroyWindow("hexagon");
    cvReleaseImage(&img);
    
    return 0;
}