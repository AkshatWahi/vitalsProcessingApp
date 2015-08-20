/* Code for data collection at Kumbh Nasik
It will be interfaced with eHealth shield by cooking Hacks.

Uses ControlP5 2.2.5 library for GUI elements

Author:- Akshat Wahi
Date Created:- 15 August 2015
Date Modified:- 16 August 2015
*/

/*
Description of various states-
1- Select Com port
2- Select file name
3- read temperatture
4- save temperature
5- read skin response
6- save skin response
7- read pulse OX
8- save pulse OX
9- read BP
10- save BP
*/

import controlP5.*;
import processing.serial.*;
import java.util.*;

Serial myport;
ControlP5 cp5;
Chart mychart_temp,mychart_skin;
String log_str="Select COM port ...",file_path="D:/Kumbh_data/",concat_path="", patient_id = "";
String[] srl;
int prev_state = 0, current_state = 1,droplist_no=0;
String instring, save_data;
char inbyte;
boolean save_gsr = false;
long current_time, collection_time = 10000;
PFont font1,font2;
PrintWriter output_file;

void setup()
{
  size(1024,600);
  noStroke();
  cp5 = new ControlP5(this);
  font1 = createFont("arial",13);
  font2 = createFont("arial",20);
  
  srl = Serial.list();
  initiate_allcontrols();
}

void draw()
{
  background(100);
  fill(0);
  rect(0,height-30,width,30);
  textFont(font1);
  fill(255);
  text(log_str,10,height-10);
  text("Patient ID:- " + patient_id , 800,height-10);
  if(save_gsr)
  {
    if((long)(millis()-current_time) >= collection_time)
    {
      save_gsr = false;
      mychart_skin.hide();
      current_state = 7;
      log_str = str(collection_time/1000) + " seconds data saved. Read Blood Oxygen Saturation";
      output_file.flush();
      output_file.close();
    }
  }
  switch(current_state)
  {
    case 1:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state1_controls();
            }
    break;

    case 2:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state2_controls();
            }
    break;
    
    case 3:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state3_controls();
            }
    break;

    case 4:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state4_controls();
            }
            if(instring != null)
            {
              instring = trim(instring);
              float indata = float(instring);
              if(indata >0)
              {
              println(instring);
              mychart_temp.push("temp",indata);
              log_str = "Temperature = " + instring;
              
              save_data = instring;
             // save_data += ",";
              }
              instring = "";
              
            }
           
    break;

    case 5:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state5_controls();
            }
    break;

    case 6:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state6_controls();
            }

            if(instring != null)
            {
              instring = trim(instring);
              float[] indata = float(split(instring,','));
              if(indata.length == 4)
              {
             
              println(indata.length);
              println(instring);
              mychart_skin.push("skin",indata[1]);
              log_str = "Time = "+ str(indata[0]) + " Skin Conductance = " + str(indata[1])+" Resistance = " + str(indata[2])+ " Voltage = "+str(indata[3]);
              
              save_data = instring;
              if(save_gsr)
              {
                log_str += "  saving...";
                output_file.println(save_data);
              }
             // save_data += ",";
             
              }
              instring = "";
            }

    break;

    case 7:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state7_controls();
            }
    break;

    case 8:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state8_controls();
            }
    break;

    case 9:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state9_controls();
            }
    break;

    case 10:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state10_controls();
            }
    break;
  }
}

void state1_controls()
{
  cp5.getController("dropdown").show();
  cp5.getController("Connect").show();
}

void state2_controls()
{
  cp5.getController("Input_Filename").show();
  cp5.getController("Create").show();
  
  log_str = "Connected. Enter the file name to begin data collection";      
}

void state3_controls()
{
  cp5.getController("Read_temp").show();
  cp5.getController("New").show(); 
}

void state4_controls()
{
  mychart_temp.show();
  cp5.getController("Save").show();
 
  myport.write('t');
}

void state5_controls()
{
  cp5.getController("Read_Skin").show();
 
}

void state6_controls()
{
  mychart_skin.show();
  cp5.getController("Save").show();
 
  myport.write('s');
}

void state7_controls()
{}

void state8_controls()
{}

void state9_controls()
{}

void state10_controls()
{}

public void dropdown(int n)
{
  droplist_no = n;
  log_str = Serial.list()[n] + " is selected";
}

public void Connect(int val)
{
  try{
    myport = new Serial(this,Serial.list()[droplist_no], 9600);
    myport.bufferUntil(10); // ASCII for linefeed
  }
  catch(Exception e)
  {
    println("Cannot connect to port");
    log_str = "Unable to connect. Check the connections and Restart application.";
  }
  cp5.getController("Connect").hide();
  cp5.getController("dropdown").hide();
  if(myport != null)
  {
    current_state = 2;
  }
  
}

public void Create(int val)
{
  patient_id = day() + "_K_" + (cp5.get(Textfield.class,"Input_Filename").getText()) + "_VT";
  concat_path = file_path + patient_id + "/" + patient_id + "_temp.txt";
  output_file = createWriter(concat_path);
  println(concat_path);
  log_str = "File created at "+ concat_path + " Read Temperature";
  current_state = 3;
  cp5.getController("Create").hide();
}

public void Read_temp(int val)
{
  log_str = "Reading Temperature...";
  cp5.getController("Read_temp").hide();
  current_state = 4;
}

public void Save()
{
  output_file.println(save_data);
  switch(current_state)
  {
    case 4:
          mychart_temp.hide();
          current_state = 5;
          log_str = save_data + " saved. Read Skin Conductance";
          output_file.flush();
          output_file.close();
          cp5.getController("Save").hide();
          myport.write('d');
    break;

    case 6:
          current_time = millis();
          save_gsr = true;
          cp5.getController("Save").hide();
          log_str = save_data + " saving...";
          /*
          mychart_skin.hide();
          current_state = 7;
          log_str = save_data + " saved. Read Blood Oxygen Saturation";
          output_file.flush();
          output_file.close();
          */
    break;
  }
  
  
  
  
  //output_file.flush();
  //output_file.close();

}

public void Read_Skin()
{
  log_str = "Reading Skin conductance...";
  cp5.getController("Read_Skin").hide();
  concat_path = file_path + patient_id + "/" + patient_id + "_gsr.txt";
  output_file = createWriter(concat_path);
  println(concat_path);
  current_state = 6;
}

void serialEvent(Serial p)
{
  instring = p.readString();
}

public void initiate_allcontrols()
{
  cp5.addScrollableList("dropdown")
     .setBroadcast(false)
     .setPosition(400,40)
     .setSize(200,500)
     .setBarHeight(40)
     .setItemHeight(30)
     .addItems(srl)
     .setBroadcast(true)
     ;

  cp5.getController("dropdown").hide();   

  cp5.addButton("Connect")
     .setBroadcast(false)
     .setPosition(630,40)
     .setSize(100,40)
     .setBroadcast(true)
     .getCaptionLabel().setFont(font1).align(CENTER,CENTER)
     ;
  
  cp5.getController("Connect").hide();

  cp5.addTextfield("Input_Filename")
     .setBroadcast(false)
     .setPosition(80,40)
     .setSize(750,40)
     .setFont(font2)
     .setFocus(true)
     .setColor(color(200,0,0))
     .setText("")
     .setAutoClear(false)
     .setBroadcast(true)
     ;

  cp5.getController("Input_Filename").hide();   

  cp5.addButton("Create")
     .setBroadcast(false)
     .setPosition(860,40)
     .setSize(100,40)
     .setBroadcast(true)
     .getCaptionLabel().setFont(font1).align(CENTER,CENTER)
     ;

  cp5.getController("Create").hide();   

  cp5.addButton("Read_temp")
     .setBroadcast(false)
     .setPosition(450,250)
     .setSize(150,50)
     .setBroadcast(true)
     .getCaptionLabel().setText("Read Temperature").setFont(font1).align(CENTER,CENTER)
     ;
  
  cp5.getController("Read_temp").hide();


  cp5.addButton("New")
     .setBroadcast(false)
     .setPosition(860,40)
     .setSize(100,40)
     .setBroadcast(true)
     .getCaptionLabel().setFont(font1).align(CENTER,CENTER)
     ;

  cp5.getController("New").hide();   

  mychart_temp = cp5.addChart("Temperature")
                    .setPosition(120,150)
                    .setSize(750,350)
                    .setRange(-2,80)
                    .setView(Chart.LINE)
                    .setStrokeWeight(1.5)
                   // .setColorCaptionLabel(color(40))
                   // .getCaptionLabel().setFont(font1)
                    ;

  mychart_temp.addDataSet("temp");
  mychart_temp.setData("temp",new float[150]);
  mychart_temp.hide();

  cp5.addButton("Save")
     .setBroadcast(false)
     .setPosition(900,300)
     .setSize(100,40)
     .setBroadcast(true)
     .getCaptionLabel().setFont(font1).align(CENTER,CENTER)
     ;
  
  cp5.getController("Save").hide(); 
  
  cp5.addButton("Read_Skin")
     .setBroadcast(false)
     .setPosition(450,250)
     .setSize(150,50)
     .setBroadcast(true)
     .getCaptionLabel().setText("Read GSR").setFont(font1).align(CENTER,CENTER)
     ;
  
  cp5.getController("Read_Skin").hide();

  mychart_skin = cp5.addChart("Skin_Conductance")
                    .setPosition(120,150)
                    .setSize(750,350)
                    .setRange(-2,15)
                    .setView(Chart.LINE)
                    .setStrokeWeight(1.5)
                   // .setColorCaptionLabel(color(40))
                   // .getCaptionLabel().setFont(font1)
                    ;
  
  mychart_skin.addDataSet("skin");
  mychart_skin.setData("skin",new float[150]);
  mychart_skin.hide();                  

}