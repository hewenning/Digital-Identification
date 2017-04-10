#include <iostream>
#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ml.hpp>

using namespace  std;
using namespace cv;
using namespace ml;


#define ImageHeight     128
#define ImageWidth      128
#define SavePath        "pic2"
namespace ImageTool {




	vector<float> getHog(string path){



		Mat imageMat;
		imageMat = imread(path, 1);//


		HOGDescriptor *hog = new HOGDescriptor(cvSize(ImageWidth, ImageHeight), cvSize(64, 64), cvSize(32, 32), cvSize(32, 32), 9);
		vector<float> descriptors;
		hog->compute(imageMat, descriptors);
		return descriptors;
	}


	Mat getHotMat(string path){

		vector<float> mats=getHog(path);

		Mat ret(1, (int)mats.size(), CV_32F);


		for(int i=0,isize=(int)mats.size();i<isize;i++){

			ret.at<float>(0, i)=mats.at(i);
		}

		return ret;
	}


	Mat getHogFromMat(Mat inputMat){



		HOGDescriptor *hog = new HOGDescriptor(cvSize(ImageWidth, ImageHeight), cvSize(64, 64), cvSize(32, 32), cvSize(32, 32), 9);
		vector<float> descriptors;
		hog->compute(inputMat, descriptors);

		Mat ret(1, (int)descriptors.size(), CV_32F);


		for(int i=0,isize=(int)descriptors.size();i<isize;i++){

			ret.at<float>(0, i)=descriptors.at(i);
		}
		return ret;
	}
}


namespace NumberRec {

	Ptr<SVM> svm = StatModel::load<SVM>("save.txt"); 
	
	Mat dataMat;//所有的样本数据
	Mat labelMat;//样本所代表的数字
	//Ptr<KNearest> knn;

	//Ptr<SVM> svm;

	void init()
	{
		//=======================第一步开始=================================
		// printf("注意你的样本文件保存路径,当前路路经视作者的电脑路经\n");
		//printf("Step 1:Catch Data===========================\n");

		char fn[1024]={};
		char NumPatch[100]={};
		for(int num=0;num<10;num++)
		{
		sprintf(NumPatch,"%s\\%d\\num.txt",SavePath,(int)num);
		FILE *f=fopen(NumPatch,"r+");
		char str[20]={};
		fread(str,20,1,f);
		fclose(f);
		int N=atoi(str);
		for(int j=1;j<=N;j++)
		{
			sprintf(fn, "%s\\%d\\%d.bmp",SavePath,(int)num,(int)j);
			vector<float> hogs=ImageTool::getHog(fn);
			Mat tempMat(1,(int)hogs.size(),CV_32F);
			for(int i=0,isize=(int)hogs.size();i<isize;i++)
			{
				tempMat.at<float>(0, i)=hogs.at(i);
			}
			labelMat.push_back(num);
			dataMat.push_back(tempMat);
		}
		printf("NUM:%d Finished!             ",num);
		}
		//=======================第一步结束=================================
		//=======================第二步开始=================================
		//=========================SVM=====================================
		Ptr<SVM> svm1;
		svm1 = SVM::create();
		svm1->setType(SVM::C_SVC);
		svm1->setKernel(SVM::RBF);
		svm1->setGamma(0.01);
		svm1->setC(10.0);
		svm1->setTermCriteria(TermCriteria(CV_TERMCRIT_EPS,10000,FLT_EPSILON));
		Ptr<TrainData> tData = TrainData::create(dataMat, ROW_SAMPLE, labelMat);
		svm1->train(tData);
		svm1->save("save.txt");
			//========================KNN========================================
			//printf_s("Step 2:Opencv API ===========================\n");
		//	knn = KNearest::create();  //创建knn分类器
			//knn->getDefaultName("save.dat");
		//	knn->setDefaultK(5);//设定k值
		//	knn->setIsClassifier(true);
		//	Ptr<TrainData> tData = TrainData::create(dataMat, ROW_SAMPLE, labelMat);
			//tData->loadFromCSV("save.csv",10000);
		//	knn->train(tData);
			//knn->save("knn_data.npz");
			//knn->save("save.csv");
			//=======================第二步结束=================================
		
		//=======================第三步开始=================================
	}

	int predicatImg(Mat inputMat)
	{
		/*if(dataMat.rows==0)
		{
			printf_s("Please Init Before Stare\n");
			return -1;
		}*/
		Mat mat= ImageTool::getHogFromMat(inputMat);
		float response = svm->predict(mat);
		return response;
	}
}