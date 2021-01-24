/*
 * CLI.cpp
 *
 * Author: 322060187 Gali Seregin
 */
#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO *dio)
{
    this->dio = dio;
    one *first =new one(dio);
    comma.push_back(first);
    two *second =new two(dio);
    comma.push_back(second);
    three *third =new three(dio);
    comma.push_back(third);
    four *fourth =new four(dio);
    comma.push_back(fourth);
    five *fifth =new five(dio);
    comma.push_back(fifth);
}

void CLI::start()
{
    string menu = "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n1.upload a time series csv file\n2.algorithm settings\n3.detect anomalies\n4.display results\n5.upload anomalies and analyze results\n6.exit\n";
    
    //infomation info;
    float num = 0;
    dio->read(&num);
    
    while (num != 6){
        dio->write(menu);
        switch ((int)num)
        {
        case 1:{
            comma[0]->execute(infor);
            /*
            one first = *(new one(dio, &info));
           // first.setAnomalyDetec(this->detector);
            first.execute();
            dio->write(menu);
            */
            break;
        }
        case 2:{
            
             comma[1]->execute(infor);
            /*
            two second = *(new two(dio, &info));
            second.setAnomalyDetec(this->detector);
            second.execute();
            dio->write(menu);*/
            break;
        }
        case 3:{
             comma[2]->execute(infor);
            /*
            three third = *(new three(dio, &info));
            third.setAnomalyDetec(this->detector);
            third.execute();
            dio->write(menu);
            */
            break;
        }
        case 4:{
            comma[3]->execute(infor);
            /*
            four fourth = *(new four(dio, &info));
            fourth.setAnomalyDetec(this->detector);
            fourth.execute();
            dio->write(menu);
            */
            break;
        }
        case 5:{
            comma[4]->execute(infor);
            /*
            five fifth = *(new five(dio, &info));
            fifth.setAnomalyDetec(this->detector);
            fifth.execute();
            dio->write(menu);
            */
            break;
        }
        default:{
            break;
        }
        }
        dio->read(&num);
    }
     dio->write(menu);

}

CLI::~CLI()
{
}