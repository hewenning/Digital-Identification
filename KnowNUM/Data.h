#include "NumberRec.hpp"

#define ImageHeight     128
#define ImageWidth      128
#define SavePath        "pic2"

VideoCapture capture(1);

//==================================================================
//=======================检测ROI区域================================
//==================================================================
Mat srcImage;  //临时变量和目标图的定义
Mat midImage,dstImage,dstImage2;//临时变量和目标图的定义

Mat ROIimage;
vector<vector<Point>> contours;//findContours找到的轮廓的集合
vector<Vec4i> hierarchy;//

int threshold_g = 194;//194;//阈值 实际值1.7
double threshold_gMax = 255;//阈值最大值 实际值10

int adaptiveThreshold_g = 4;//阈值 实际值1.7
double adaptiveThreshold_gMax = 100;//阈值最大值 实际值10


void on_ContoursChange(int, void* ){}
//==================================================================
void CollectData(int DataNum,int DataTime,int NUM,int Coll)
{
	int After=0;
	char NumPath1[1024]={};//数字图片路径
	char NumPath2[1024]={};//数字样本个数路径
	FILE *f;
	if(Coll)
	{
		//数字图片路径
		sprintf(NumPath1,"%s\\%d",SavePath,NUM);
		//数字样本个数路径
		sprintf(NumPath2,"%s\\num.txt",NumPath1);
		////读取当前样本个数
		f=fopen(NumPath2,"r+");
		char str[20]={};
		fread(str,20,1,f);
		After=atoi(str);
		fclose(f);
	}
	//开始增加样本
	int Now=1;
	int TimeOver=0;
	int over=1;
	capture >> srcImage;
	Mat CatchRect= Mat(ImageWidth,ImageHeight,srcImage.type());
	while(over)
	{
		capture >> srcImage;
		//【3】转为灰度图并进行图像平滑
		cvtColor(srcImage,midImage, CV_BGR2GRAY);//转化边缘检测后的图为灰度图
		//dilate(midImage,midImage,getStructuringElement(MORPH_RECT,Size(5,5)));
		erode(midImage,midImage,getStructuringElement(MORPH_ELLIPSE,Size(3,3)));	
		threshold( midImage, midImage,threshold_g,255,CV_THRESH_BINARY);
		//imshow("midle",midImage);
		//blur( midImage, dstImage2, Size(5,5) ); //均值滤波
		dilate(midImage,dstImage2,getStructuringElement(MORPH_RECT,Size(10,10)));
		//blur( dstImage2, dstImage2, Size(2,2) ); //均值滤波
		erode(dstImage2,dstImage2,getStructuringElement(MORPH_ELLIPSE,Size(10,10)));
		adaptiveThreshold( dstImage2, dstImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV ,7,adaptiveThreshold_g);
		//imshow("midle",dstImage);
		findContours( dstImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		vector<vector<Point> > contours_poly( contours.size() );//多边形逼近后的轮廓集合
		vector<Rect> boundRect( contours.size() );//近似四边形的轮廓的集合
		Rect cenRect;
		int j=0;
		int k=0;
		for(int i = 0; i < contours.size(); i++ )
		{
			approxPolyDP( Mat(contours[i]), contours_poly[i], 20, true );//用指定精度逼近多边形曲线 
			boundRect[j] = boundingRect( Mat(contours_poly[i]) );
			if((boundRect[j].area()/contourArea(contours_poly[i]))<1.3)
			{
				if(boundRect[j].width<boundRect[j].height)//除掉噪音
					continue;
				for(k=0;k<j;k++)//避免重复位置检测
				{
					if(abs((boundRect[k].x+boundRect[k].width)-(boundRect[j].x+boundRect[j].width))<boundRect[j].width/2)
						if(abs((boundRect[k].y+boundRect[k].height)-(boundRect[j].y+boundRect[j].height))<boundRect[j].height/2)
							break;
				}
				if(k==j)
				{
					j++;
				}
			}
		}
		char ImaPath[20]={};

		for(k=0;k<j;k++)
		{
			boundRect[k].y+=boundRect[k].height/11.0;
			boundRect[k].x+=(boundRect[k].width-9*boundRect[k].height/11.0)/2;
			boundRect[k].height-=2*boundRect[k].height/11.0;
			boundRect[k].width=boundRect[k].height;
			
			getRectSubPix(srcImage,Size(boundRect[k].width,boundRect[k].height),Point2f(boundRect[k].x+boundRect[k].width/2,boundRect[k].y+boundRect[k].height/2),ROIimage);
			resize(ROIimage,CatchRect,CatchRect.size());
			cvtColor(CatchRect, CatchRect, CV_BGR2GRAY);
			threshold(CatchRect, CatchRect, 220, 255, CV_THRESH_BINARY_INV);
			imshow("【RPI】", CatchRect);
			//rectangle( srcImage, boundRect[k].tl(), boundRect[k].br(), Scalar(200,0,0), 2, 8, 0 );

			if(TimeOver==DataTime)
			{
				if(Coll)
				{
					if(Now>20)
					{
						sprintf(ImaPath,"%s\\%d.bmp",NumPath1,After+Now-20);
						imwrite(ImaPath, CatchRect);
						printf("Save num%d OK!\n",Now-20);
					}
					Now++;
				}
				else
				{
					int response=NumberRec::predicatImg(CatchRect);
					printf("result:%d\n",response);
				}
				TimeOver=0;
			}
			TimeOver++;

		}
		if(Coll&&Now==DataNum+21)
		{
			f=fopen(NumPath2,"w");
			char str[20]={};
			sprintf(str,"%d",After+Now-21);
			fwrite(&str,20,1,f);
			fclose(f);
			over=0;
		}
		//printf("%d\n",j);
		//imshow("【腐蚀效果图2】",midImage);
		//imshow("【膨胀效果图3】",dstImage2);
		//imshow("【处理效果图1】", dstImage);
		imshow("【原始图】", srcImage);
		//createTrackbar( " threshold阈值:", "【处理效果图1】", &threshold_g, threshold_gMax, on_ContoursChange );
		//createTrackbar( " adaptiveThreshold阈值:", "【处理效果图1】", &adaptiveThreshold_g, adaptiveThreshold_gMax, on_ContoursChange );
		waitKey(10);  
	} 
}
