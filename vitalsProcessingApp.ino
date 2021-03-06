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

Serial myport,myprinterport;
ControlP5 cp5;
Chart mychart_temp,mychart_skin;
String log_str="Select COM port ...",file_path="D:/Kumbh_data/",concat_path="", patient_id = "";
String[] srl;
int prev_state = 0, current_state = 1,droplist_no=0, g_bpm = 0;
String instring, save_data, pr_data;
char inbyte;
boolean save_gsr = false,invalid_data = true;
long current_time, collection_time = 10000;
PFont font1,font2;
PrintWriter output_file;

void setup()
{
  size(1024,600);
  noStroke();
  cp5 = new ControlP5(this);
  font1 = createFont("arial",13);
  font2 = createFont("arial",35);
  
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
      myport.write('d');
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

             if(instring != null)
            {
              instring = trim(instring);
              float[] indata = float(split(instring,','));
              if(indata.length == 2)
              {
             
              println(indata.length);
              println(instring);
              cp5.getController("bpm").setValue(indata[0]);
              cp5.getController("SpO2%").setValue(indata[1]);
             
              log_str = "beats per minute = "+ str(indata[0]) + " SpO2% = " + str(indata[1]);
              
              save_data = instring;
              g_bpm = int(indata[0]);             
              }
              instring = "";
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
            float[] indata = new float[2];
            instring = "";

            indata[0] = float(cp5.get(Textfield.class,"Input_Sys").getText());
            indata[1] = float(cp5.get(Textfield.class,"Input_Dia").getText());
            
            save_data = str(indata[0]) +","+ str(indata[1]);
            log_str = "Systolic Pressure = " + str(indata[0])+ "  Diastolic Pressure = " +str(indata[1]);
            if(indata[0] >33 && indata[0] < 210 && indata[1] >33 && indata[1] <210)
            {
              invalid_data = false;
              log_str += " Valid data";
            }          
            else
            {
              invalid_data = true;
              log_str += " Invalid data. Enter valid values.";
            }
    break;

    case 11:
            if(prev_state != current_state)
            {
              prev_state = current_state;
              state11_controls();
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
 // cp5.getController("New").show(); 
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
{
   cp5.getController("Read_Pulseox").show();
}

void state8_controls()
{
   cp5.getController("bpm").show();
   cp5.getController("SpO2%").show();
   cp5.getController("Save").show();

   myport.write('p');
}

void state9_controls()
{
  cp5.getController("Read_BP").show();
}

void state10_controls()
{
  cp5.getController("Input_Sys").show();
  cp5.getController("Input_Dia").show();
  cp5.getController("Save").show();
  //myport.write()
}

void state11_controls()
{
  cp5.getController("Print").show();
  cp5.getController("New").show();
}

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

  try{
    myprinterport = new Serial(this,Serial.list()[1],9600);
  }
  catch(Exception e)
  {
    println("Cannot connect to printer");
    log_str = "Unable to connect to printer.";
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
 // patient_id = day() + "_K_" + (cp5.get(Textfield.class,"Input_Filename").getText()) + "_VT";
  patient_id = (cp5.get(Textfield.class,"Input_Filename").getText()) + "_VT";
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

public void Read_Pulseox(int val)
{
  log_str = "Reading BPM and SpO2%.";
  cp5.getController("Read_Pulseox").hide();
  concat_path = file_path + patient_id + "/" + patient_id + "_pox.txt";
  output_file = createWriter(concat_path);
  println(concat_path);
  current_state = 8;
}

public void Save()
{
 // output_file.println(save_data);
  switch(current_state)
  {
    case 4:
          output_file.println(save_data);
          mychart_temp.hide();
          current_state = 5;
          log_str = save_data + " saved. Read Skin Conductance";
          output_file.flush();
          output_file.close();
          cp5.getController("Save").hide();
          myport.write('d');
    break;

    case 6:
          output_file.println(save_data);
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

    case 8:
          output_file.println(save_data);
          cp5.getController("bpm").hide();
          cp5.getController("SpO2%").hide();
          cp5.getController("Save").hide();
          log_str = save_data + " saved. Read Blood Pressure";
          output_file.flush();
          output_file.close();
          current_state = 9;
          myport.write('d');
    break;

    case 10:
           if(!invalid_data)
           {
            output_file.println(save_data);
           cp5.getController("Input_Sys").hide();
           cp5.getController("Input_Dia").hide();
           cp5.getController("Save").hide();
           log_str = save_data + " saved. Print ";
           pr_data = save_data;
           output_file.flush();
           output_file.close();
           current_state = 11;
           }
           else
           {
            log_str = "Not a valid data. Enter valid data";
           }

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

public void Print()
{
  pr_data += "," + str(g_bpm)+ ","+"o";
  myprinterport.write(pr_data);
  //myprinterport.write('o');
  myprinterport.write('\n');
  println(pr_data);
}

public void New()
{
  log_str = "Enter New Patient ID";
  current_state = 2;
  patient_id = "";
  cp5.getController("New").hide();
  cp5.getController("Print").hide();
}

public void Read_BP()
{
  log_str = "Reading Blood Pressure...";
  cp5.getController("Read_BP").hide();
  concat_path = file_path + patient_id + "/" + patient_id + "_bp.txt";
  output_file = createWriter(concat_path);
  println(concat_path);
  current_state = 10;
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
                    .setRange(-2,50)
                    .setView(Chart.LINE)
                    .setStrokeWeight(1.5)
                   // .setColorCaptionLabel(color(40))
                   // .getCaptionLabel().setFont(font1)
                    ;

  mychart_temp.addDataSet("temp");
  mychart_temp.setData("temp",new float[100]);
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

  cp5.addButton("Read_Pulseox")
     .setBroadcast(false)
     .setPosition(450,250)
     .setSize(150,50)
     .setBroadcast(true)
     .getCaptionLabel().setText("Read Pulse OX").setFont(font1).align(CENTER,CENTER)
     ;
  
  cp5.getController("Read_Pulseox").hide();

  cp5.addSlider("bpm")
     .setPosition(300,150)
     .setSize(80,350) 
     .setRange(0,150)
     .setValue(0)
     ;     
  
  cp5.getController("bpm").getValueLabel().setFont(font1);
  cp5.getController("bpm").getCaptionLabel().setFont(font2);
  cp5.getController("bpm").hide();

  cp5.addSlider("SpO2%")
     .setPosition(600,150)
     .setSize(80,350) 
     .setRange(0,100)
     .setValue(0)
     ;     
  
  cp5.getController("SpO2%").getValueLabel().setFont(font1);
  cp5.getController("SpO2%").getCaptionLabel().setFont(font2);
  cp5.getController("SpO2%").hide();            

 // cp5.getController("BPM").getValueLabel().align(ControlP5.LEFT, ControlP5.BOTTOM_OUTSIDE).setPaddingX(0);
 // cp5.getController("BPM").getCaptionLabel().align(ControlP5.RIGHT, ControlP5.BOTTOM_OUTSIDE).setPaddingX(0);

  cp5.addTextfield("Input_Sys")
     .setBroadcast(false)
     .setPosition(250,300)
     .setSize(150,60)
     .setFont(font2)
     .setFocus(true)
     .setColor(color(200,0,0))
     .setText("")
     .setAutoClear(false)
     .setBroadcast(true)
     .getCaptionLabel().setText("Systolic").setFont(font1)
     ;

  cp5.getController("Input_Sys").hide();   


  cp5.addTextfield("Input_Dia")
     .setBroadcast(false)
     .setPosition(550,300)
     .setSize(150,60)
     .setFont(font2)
     .setFocus(true)
     .setColor(color(200,0,0))
     .setText("")
     .setAutoClear(false)
     .setBroadcast(true)
     .getCaptionLabel().setText("Diastolic").setFont(font1)
     ;

  cp5.getController("Input_Dia").hide();   

  cp5.addButton("Read_BP")
     .setBroadcast(false)
     .setPosition(450,250)
     .setSize(150,50)
     .setBroadcast(true)
     .getCaptionLabel().setText("Read BP").setFont(font1).align(CENTER,CENTER)
     ;
  
  cp5.getController("Read_BP").hide();
  
  cp5.addButton("Print")
     .setBroadcast(false)
     .setPosition(450,250)
     .setSize(150,50)
     .setBroadcast(true)
     .getCaptionLabel().setFont(font1).align(CENTER,CENTER)
     ;
  
  cp5.getController("Print").hide();

}