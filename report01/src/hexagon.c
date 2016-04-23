#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>

// Bresenham's algorithm
void myLine(IplImage* img, CvPoint pt1, CvPoint pt2)
{
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
            
        }
    }
    
}

void myHexagon(IplImage* img, int cx, int cy, double radius, double angle, CvScalar color)
{
    for (int i=0; i<6; i++) {
        double theta1 = (angle + i*60) / 180 * M_PI;
        double theta2 = (angle + (i+1)%6*60) / 180 * M_PI;
        CvPoint pt1 = cvPoint(cx + cos(theta1)*radius, cy + sin(theta1)*radius);
        CvPoint pt2 = cvPoint(cx + cos(theta2)*radius, cy + sin(theta2)*radius);
        myLine(img, pt1, pt2);
    }
}

int main (int argc, char **argv)
{
    int width=960, height=640;
    IplImage *img=0;
    double c, f;
    f = cvGetTickFrequency()*1000;
    
    int cx = width/2;
    int cy = height/2;
    double radius = 100;
    double angle = 0;
    CvScalar color = cvScalarAll(255);
    
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 1.0, 1.0, 1, CV_AA);
    
    cvNamedWindow ("hexagon", CV_WINDOW_AUTOSIZE);
    while (1) {
        
        // (1)allocate and initialize an image
        img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        if(img == 0) return -1;
        cvZero(img);
        
        // (2) draw hexagon
        c = cvGetTickCount();
        myHexagon(img, cx, cy, radius, angle, color);
        printf("%fms\n", (cvGetTickCount()-c)/f);
        
        // (3)show the iamge, and press some key
        cvPutText(img, "Coordinate Right(D) Left(A) Up(W) Down(X)", cvPoint(10, 20), &font, cvScalarAll(255));
        cvPutText(img, "Rotate Right(R) Left(E)", cvPoint(10, 40), &font, cvScalarAll(255));
        cvPutText(img, "Radius Big(V) Small(C)", cvPoint(10, 60), &font, cvScalarAll(255));
        cvPutText(img, "Quit(Q, esc)", cvPoint(10, 80), &font, cvScalarAll(255));
        char s[64];
        sprintf(s, "cx:%d cy:%d radius:%f angle:%f", cx, cy, radius, angle);
        cvPutText(img, s, cvPoint(10, 110), &font, cvScalarAll(255));
        
        cvShowImage ("hexagon", img);
        char key = cvWaitKey (0);
        if (key == 27 || key == 'q') break;
        else if (key == 'r') angle += 5;
        else if (key == 'e') angle -= 5;
        else if (key == 'a') cx -= 5;
        else if (key == 'd') cx += 5;
        else if (key == 'w') cy -= 5;
        else if (key == 'x') cy += 5;
        else if (key == 'v') radius += 5;
        else if (key == 'c') radius -= 5;
    }
    
    cvDestroyWindow("hexagon");
    cvReleaseImage(&img);
    
    return 0;
}




