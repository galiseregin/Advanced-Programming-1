/*
 * commands.h
 *
 * Author: 322060187 Gali Seregin
 */
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include "timeseries.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
	// you may add additional methods here
};
struct information{
vector<AnomalyReport> v;
int step;
HybridAnomalyDetector *detector=new HybridAnomalyDetector();
TimeSeries *train=new TimeSeries();
TimeSeries *test=new TimeSeries();
};
class inf{
public:
information infi;
};


class Command{
protected:
   // HybridAnomalyDetector *detector;
    string description;
	DefaultIO* dio;
    information* info;
public:
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute(inf *i)=0;
	virtual ~Command(){}
    void print(){
        dio->write(description);
    }/*
    void setAnomalyDetec(HybridAnomalyDetector *dt){
        this->detector=dt;
    }
    HybridAnomalyDetector* getAnomalyDetec(){
        return this->detector;
    }*/
    void upload(string str1,string str2){
        ofstream newFile(str1);
        dio->write(str2);
        string line ="0";
        do{
            line =dio->read();
            if(line=="done"){
                break;
            }
            newFile << line << endl;
        }
        while(line !="done");
        dio->write("Upload complete.\n");
        newFile.close();
    }
    bool c(long one ,long two,long first,long second){
        if(first<=one&& one<=second && first<=two && two<=second){
            return true;
        }
        else if(one<=first && two>=second){
            return true;
        }
        else if(first<=one&& one<=second && two>=second){
            return true;
        }
        else if(one<=first && first<=two&& two<=second){
            return true;
        }
        else{
            return false;
        }
    }

};

class one:public Command{
public:
    one(DefaultIO* dio) : Command(dio) {
        this->description="1.upload a time series csv file";
    }
    virtual void execute(inf * i) {
        dio->write("Please upload your local train CSV file.\n");
        ofstream out;
        const char* train ="anomalyTrain.csv";
        const char* test ="anomalyTest.csv";
        out.open(train);
        string line = dio->read();
        while(line.compare("done")){
            out << (line + "\n");
            line = dio->read();
        }
        out.close();
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        out.open(test);
        line = " ";
        line = dio->read();
        while(line.compare("done")){
            out << (line + "\n");
            line = dio->read();
        }
        out.close();
        dio->write("Upload complete.\n");
        i->infi.train->setName(train);
        i->infi.test->setName(test);
    }
};

class two:public Command{
public:
    two(DefaultIO* dio):Command(dio){
    this->description="2.algorithm settings";
    }
    virtual void execute(inf * i) {
        dio->write("The current correlation threshold is ");
        float tresh = i->infi.detector->treshholdGetter();
        dio->write(tresh);
        dio->write("\n");
        dio->write("Type a new threshold\n");
        string temp=dio->read();
        float tempnum=stof(temp);
        i->infi.detector->setCorrelationThreshold(tempnum);
    }
};

class three:public Command{
public:
    three(DefaultIO* dio):Command(dio){
    this->description="3.detect anomalies";
    }
    virtual void execute(inf* i){

        i->infi.train->setName("anomalyTrain.csv");
        i->infi.test->setName("anomalyTest.csv");
        i->infi.step = i->infi.train->getRowSize();
        i->infi.detector->learnNormal(*i->infi.train);
        i->infi.v = i->infi.detector->detect(*i->infi.test);
        dio->write("anomaly detection complete.\n");
    }
};

class four:public Command{
public:
    four(DefaultIO* dio):Command(dio){
    this->description="4.display results";
    }
    virtual void execute(inf* i){
        int size = i->infi.v.size();
        for(int j=0;j<size;j++){
            dio->write(i->infi.v[j].timeStep);
            dio->write("\t"+i->infi.v[j].description+"\n");
        }
        dio->write("Done.\n");
    }
};

class five:public Command{
public:
    bool done_on = false;
    five(DefaultIO* dio):Command(dio){
    this->description="5.upload anomalies and analyze results";
    }
    virtual void execute(inf* i){
        dio->write("Please upload your local anomalies file.\n");
        float P=0;
        float N=0;
		vector<pair<long,long>> inputLine ;
		string line = dio->read();
        stringstream ss;
        ss = stringstream (line);
        string first, second;
        while (line != "done"){
        	getline(ss,first,',');
            getline(ss,second,',');
            pair<long,long> pair (stoi(first),stoi(second));
            inputLine.push_back(pair);
            line= dio->read();
            ss = stringstream (line);
        }
       dio->write("Upload complete.\n");
        
        vector<pair<long, long>> five;
        if (i->infi.v.size() == 1){
            five.push_back(make_pair(i->infi.v[0].timeStep, i->infi.v[0].timeStep));
        }

        int k = 1;
        int one = k-1;
        unsigned long end = i->infi.v[k-1].timeStep;
        for(k;k < i->infi.v.size();k++){
            while(i->infi.v[k-1].description == i->infi.v[k].description 
            && i->infi.v[k-1].timeStep == i->infi.v[k].timeStep-1){
                end = i->infi.v[k].timeStep;
                k++;
            }
            
            five.push_back(make_pair(i->infi.v[one].timeStep, end));
            one = k;
        }
    
		P = inputLine.size();
		N = i->infi.step;
		for (int j = 0; j < inputLine.size(); j++){
			N -= (inputLine[j].second - inputLine[j].first + 1);
		}

		int c=0;
        float FP=0;
		for (int j = 0; j < five.size(); j++){
			for (int k = 0; k < inputLine.size(); k++){
				if((inputLine[k].second < five[j].first) 
                || (inputLine[k].first > five[j].second)){
					c++;
				}
			}
			if(c == inputLine.size()){
				FP++;
			}
			c=0;
		}
        float TP=0;
        int x=0;
		for (int j = 0; j < inputLine.size(); j++){
			for (int k = 0; k < five.size(); k++){
				if((inputLine[j].second >= five[k].first) 
                && (inputLine[j].first <= five[k].second)){
					x=1;
				}
			}
			if(x==1){
				TP++;
			}		
		}
		float TPR = floor((TP / P)*1000)/1000;
        dio->write("True Positive Rate: ");
		dio->write(TPR);
		dio->write("\n");
		float FAR = floor((FP / N)*1000)/1000;
		dio->write("False Positive Rate: ");
		dio->write(FAR);
		dio->write("\n");
 	}

};



// implement here your command classes
#endif 