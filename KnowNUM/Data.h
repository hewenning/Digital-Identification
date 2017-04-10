#include "NumberRec.hpp"

#define ImageHeight     128
#define ImageWidth      128
#define SavePath        "pic2"

VideoCapture capture(1);

//==================================================================
//=======================���ROI����================================
//==================================================================
Mat srcImage;  //��ʱ������Ŀ��ͼ�Ķ���
Mat midImage,dstImage,dstImage2;//��ʱ������Ŀ��ͼ�Ķ���

Mat ROIimage;
vector<vector<Point>> contours;//findContours�ҵ��������ļ���
vector<Vec4i> hierarchy;//

int threshold_g = 194;//194;//��ֵ ʵ��ֵ1.7
double threshold_gMax = 255;//��ֵ���ֵ ʵ��ֵ10

int adaptiveThreshold_g = 4;//��ֵ ʵ��ֵ1.7
double adaptiveThreshold_gMax = 100;//��ֵ���ֵ ʵ��ֵ10


void on_ContoursChange(int, void* ){}
//==================================================================
void CollectData(int DataNum,int DataTime,int NUM,int Coll)
{
	int After=0;
	char NumPath1[1024]={};//����ͼƬ·��
	char NumPath2[1024]={};//������������·��
	FILE *f;
	if(Coll)
	{
		//����ͼƬ·��
		sprintf(NumPath1,"%s\\%d",SavePath,NUM);
		//������������·��
		sprintf(NumPath2,"%s\\num.txt",NumPath1);
		////��ȡ��ǰ��������
		f=fopen(NumPath2,"r+");
		char str[20]={};
		fread(str,20,1,f);
		After=atoi(str);
		fclose(f);
	}
	//��ʼ��������
	int Now=1;
	int TimeOver=0;
	int over=1;
	capture >> srcImage;
	Mat CatchRect= Mat(ImageWidth,ImageHeight,srcImage.type());
	while(over)
	{
		capture >> srcImage;
		//��3��תΪ�Ҷ�ͼ������ͼ��ƽ��
		cvtColor(srcImage,midImage, CV_BGR2GRAY);//ת����Ե�����ͼΪ�Ҷ�ͼ
		//dilate(midImage,midImage,getStructuringElement(MORPH_RECT,Size(5,5)));
		erode(midImage,midImage,getStructuringElement(MORPH_ELLIPSE,Size(3,3)));	
		threshold( midImage, midImage,threshold_g,255,CV_THRESH_BINARY);
		//imshow("midle",midImage);
		//blur( midImage, dstImage2, Size(5,5) ); //��ֵ�˲�
		dilate(midImage,dstImage2,getStructuringElement(MORPH_RECT,Size(10,10)));
		//blur( dstImage2, dstImage2, Size(2,2) ); //��ֵ�˲�
		erode(dstImage2,dstImage2,getStructuringElement(MORPH_ELLIPSE,Size(10,10)));
		adaptiveThreshold( dstImage2, dstImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV ,7,adaptiveThreshold_g);
		//imshow("midle",dstImage);
		findContours( dstImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		vector<vector<Point> > contours_poly( contours.size() );//����αƽ������������
		vector<Rect> boundRect( contours.size() );//�����ı��ε������ļ���
		Rect cenRect;
		int j=0;
		int k=0;
		for(int i = 0; i < contours.size(); i++ )
		{
			approxPolyDP( Mat(contours[i]), contours_poly[i], 20, true );//��ָ�����ȱƽ���������� 
			boundRect[j] = boundingRect( Mat(contours_poly[i]) );
			if((boundRect[j].area()/contourArea(contours_poly[i]))<1.3)
			{
				if(boundRect[j].width<boundRect[j].height)//��������
					continue;
				for(k=0;k<j;k++)//�����ظ�λ�ü��
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
			imshow("��RPI��", CatchRect);
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
		//imshow("����ʴЧ��ͼ2��",midImage);
		//imshow("������Ч��ͼ3��",dstImage2);
		//imshow("������Ч��ͼ1��", dstImage);
		imshow("��ԭʼͼ��", srcImage);
		//createTrackbar( " threshold��ֵ:", "������Ч��ͼ1��", &threshold_g, threshold_gMax, on_ContoursChange );
		//createTrackbar( " adaptiveThreshold��ֵ:", "������Ч��ͼ1��", &adaptiveThreshold_g, adaptiveThreshold_gMax, on_ContoursChange );
		waitKey(10);  
	} 
}
