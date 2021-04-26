#include "dialog.h"
#include "ui_dialog.h"
#include "fstream"
#include "iostream"
#include "string"
#include "sstream"
//using namespace std;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
        ui->setupUi(this);
        ui-> lcdNumberTemp ->display("---------");

        std::ifstream temp,person,hours;
        temp.open("tempdata.txt");
        person.open("personsdata.txt");
        hours.open("hours.txt");


//     while(1)
     {
        if(temp.fail())
        {
          std::cout << "Error: File failed to open"<<std::endl;

        }
        else
        {


            std::string text;

            while(getline (temp, text))
            {



            //temp>>text;
            //text ="123";
            //temp.close();

            QString qstr = QString::fromStdString(text);

           // std::cout<<text<<std::endl;

            Dialog::updateTemperature(qstr);
            text.clear();
            }

        }
        if(person.fail())
         {
           std::cout << "Error: File failed to open"<<std::endl;

         }
         else
         {
              std::string text1;
              while(getline (person, text1))
              {
              QString qstr1 = QString::fromStdString(text1);
              Dialog::updatePersons(qstr1);
              }
         }


        if(hours.fail())
         {
           std::cout << "Error: File failed to open"<<std::endl;

         }
         else
         {
              std::string text2;
              while(getline (hours, text2))
              {
              QString qstr2 = QString::fromStdString(text2);
              Dialog::updateHours(qstr2);
              }
         }




     }
        temp.close();
        person.close();
        hours.close();


}

Dialog::~Dialog()
    {
         delete ui;
    }

void Dialog::updateTemperature(QString temp_sensor)
    {
          ui->lcdNumberTemp->display(temp_sensor);
    }

void Dialog::updatePersons(QString person_sensor)
    {
           ui->lcdNumberPersons->display(person_sensor);
    }

void Dialog::updateHours(QString time_hours)
    {
           ui->lcdNumberHour->display(time_hours);
    }


