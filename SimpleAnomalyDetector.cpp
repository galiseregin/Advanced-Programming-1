/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: 322060187 Gali Seregin
 */

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include "anomaly_detection_util.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	threshold = 0.9;
	hybridtreshhold=0.5;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

Point** SimpleAnomalyDetector::toPoints(vector<float> x, vector<float> y){
	Point** ps=new Point*[x.size()];
	for(size_t i=0;i<x.size();i++){
		ps[i]=new Point(x[i],y[i]);
	}
	return ps;
}

float SimpleAnomalyDetector::findThreshold(Point** ps,size_t len,Line rl){
	float max=0;
	for(size_t i=0;i<len;i++){
		float d=abs(ps[i]->y - rl.f(ps[i]->x));
		if(d>max)
			max=d;
	}
	return max;
}

size_t len_ptr;
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	vector<string> atts=ts.gettAttributes();
	size_t len=ts.getRowSize();
	len_ptr = ts.getRowSize();

	float vals[atts.size()][len];
	for(size_t i=0;i<atts.size();i++){
		for(size_t j=0;j<ts.getRowSize();j++){
			vals[i][j]=ts.getAttributeData(atts[i])[j];
		}
	}
	for(size_t i=0;i<atts.size();i++) {
		string f1=atts[i];
		float max=0;
		size_t jmax=0;

		for(size_t j=i+1;j<atts.size();j++) {
			float p=abs(pearson(vals[i],vals[j],len));
			if(p>max){
				max=p;
				jmax=j;
			}
		}
		string f2=atts[jmax];
		Point** ps=toPoints(ts.getAttributeData(f1),ts.getAttributeData(f2));
		//printf("max: %f, f1: %s, f2: %s\n", max, f1.c_str(), f2.c_str());

		learnHelper(ts,max,f1,f2,ps);

		// delete points
		for(size_t k=0;k<len;k++)
			delete ps[k];
		delete[] ps;
	}
}

void SimpleAnomalyDetector::learnHelper(const TimeSeries& ts,float p/*pearson*/,string f1, string f2,Point** ps){
	
	if(p>threshold){
		size_t len=ts.getRowSize();
		correlatedFeatures c;
		c.feature1=f1;
		c.feature2=f2;
		c.corrlation=p;
		c.lin_reg=linear_reg(ps,len);
		c.threshold=findThreshold(ps,len,c.lin_reg)*1.1; // 10% increase
		//printf("insert to cf\n");
		cf.push_back(c);
	}
	else if(p<threshold && p>hybridtreshhold){
		size_t len=ts.getRowSize();
		correlatedFeatures c;
		c.feature1=f1;
		c.feature2=f2;
		c.corrlation=p;
		Circle circle = findMinCircle(ps, len);
		c.x = circle.center.x;
		c.y= circle.center.y;
		c.radius=circle.radius;
		c.threshold=c.radius*1.1; // 10% increase
		//printf("insert to cf\n");
		cf.push_back(c);
	}
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> v;
	for_each(cf.begin(),cf.end(),[&v,&ts,this](correlatedFeatures c){
		vector<float> x=ts.getAttributeData(c.feature1);
		vector<float> y=ts.getAttributeData(c.feature2);
		for(size_t i=0;i<x.size();i++){
			Point p =Point(x[i],y[i]);
			if(c.corrlation>0.9){
				if(isAnomalous(x[i],y[i],c)){
					string d=c.feature1 + "-" + c.feature2;
					v.push_back(AnomalyReport(d,(i+1)));
				}
			}
			else{
				if(distance(p,Point(c.x,c.y))>c.threshold){
					string d=c.feature1 + "-" + c.feature2;
					v.push_back(AnomalyReport(d,(i+1)));
				}
			}
		}
	});
	return v;
}

bool SimpleAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
	return (abs(y - c.lin_reg.f(x))>c.threshold);
}
