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
String log_str="Select COM port ...",file_path="D:/Kumbh_data/",concat_path="";
String[] srl;
int prev_state = 0, current_state = 1,droplist_no=0;
String instring, save_data;
char inbyte;
PFont font1,font2,font3;
PrintWriter output_file;

void setup()
{
  size(1024,600);
  noStroke();
  cp5 = new ControlP5(this);
  font1 = createFont("arial",13);
  font2 = createFont("arial",20);
  font3 = createFont("arial",23);
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
              save_data += ",";
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
              if(indata.length == 3)
              {
             
              println(indata.length);
              println(instring);
              mychart_skin.push("skin",indata[0]);
              log_str = "Skin Conductance = " + str(indata[0])+" Resistance = " + str(indata[1])+ " Voltage = "+str(indata[2]);
              
              save_data = instring;
              save_data += ",";
             
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
  /*
  cp5.addScrollableList("dropdown")
     .setBroadcast(false)
     .setPosition(400,40)
     .setSize(200,500)
     .setBarHeight(40)
     .setItemHeight(30)
     .addItems(srl)
     .setBroadcast(true)
     ;

  cp5.addButton("Connect")
     .setBroadcast(false)
     .setPosition(630,40)
     .setSize(100,40)
     .setBroadcast(true)
     .getCaptionLabel().setFont(font1).align(CENTER,CENTER)
     ;
  */
}

void state2_controls()
{
  cp5.getController("Input_Filename").show();
  cp5.getController("Create").show();
  /*
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

  cp5.addButton("Create")
     .setBroadcast(false)
     .setPosition(860,40)
     .setSize(100,40)
     .setBroadcast(true)
     .getCaptionLabel().setFont(font1).align(CENTER,CENTER)
     ;
  */
  log_str = "Connected. Enter the file name to begin data collection";      
}

void state3_controls()
{
  cp5.getController("Read_temp").show();
  cp5.getController("New").show();
  /*
  cp5.addButton("Read_temp")
     .setBroadcast(false)
     .setPosition(450,250)
     .setSize(150,50)
     .setBroadcast(true)
     .getCaptionLabel().setText("Read Tempearture").setFont(font1).align(CENTER,CENTER)
     ;

  cp5.addButton("New")
     .setBroadcast(false)
     .setPosition(860,40)
     .setSize(100,40)
     .setBroadcast(true)
     .getCaptionLabel().setFont(font1).align(CENTER,CENTER)
     ;
  */   
}

void state4_controls()
{
  mychart_temp.show();
  cp5.getController("Save").show();
  /*
  mychart_temp = cp5.addChart("Temperature")
                    .setPosition(120,150)
                    .setSize(750,350)
                    .setRange(-2,80)
                    .setView(Chart.LINE)
                    .setStrokeWeight(1.5)
                   // .setColorCaptionLabel(color(40))
                   // .getCaptionLabel().setFont(font1)
                    ;

   cp5.addButton("Save")
     .setBroadcast(false)
     .setPosition(900,300)
     .setSize(100,40)
     .setBroadcast(true)
     .getCaptionLabel().setFont(font1).align(CENTER,CENTER)
     ;                 

  mychart_temp.addDataSet("temp");
  mychart_temp.setData("temp",new float[150]);
  */
  myport.write('t');

}

void state5_controls()
{
  cp5.getController("Read_Skin").show();
  /*
  cp5.addButton("Read_Skin")
     .setBroadcast(false)
     .setPosition(450,250)
     .setSize(150,50)
     .setBroadcast(true)
     .getCaptionLabel().setText("Read Skin Conductance").setFont(font1).align(CENTER,CENTER)
     ;
  */   
}

void state6_controls()
{
  mychart_skin.show();
  cp5.getController("Save").show();
  /*
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
  */
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
  concat_path = file_path + (cp5.get(Textfield.class,"Input_Filename").getText()) + "_vital.txt";
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
  output_file.print(save_data);
  switch(current_state)
  {
    case 4:
          mychart_temp.hide();
          current_state = 5;
          log_str = save_data + " saved. Read Skin Conductance";
    break;

    case 6:
          mychart_skin.hide();
          current_state = 7;
          log_str = save_data + " saved. Read Blood Oxygen Saturation";
          output_file.flush();
            output_file.close();
    break;
  }
  
  cp5.getController("Save").hide();
  myport.write('d');
  
  
  //output_file.flush();
  //output_file.close();

}

public void Read_Skin()
{
  log_str = "Reading Skin conductance...";
  cp5.getController("Read_Skin").hide();
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
     .getCaptionLabel().setText("Read Tempearture").setFont(font1).align(CENTER,CENTER)
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
     .getCaptionLabel().setText("Read Skin Conductance").setFont(font1).align(CENTER,CENTER)
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