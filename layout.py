#code for SeaSweepersROV GUI 'BRUCE' written by Brian Ishii. 2016
# for python 3.x use 'tkinter' rather than 'Tkinter'
import Tkinter as tk
import time
import math
from serial import *

class App():
	def __init__(self):
		self.root = tk.Tk()
		self.root.title("SeaSweepers BRUCE the RILF")
		self.root.option_add("*Font", "Rockwell 20") #Use with MACBOOK
		#self.root.option_add("*Font", "Rockwell 10") what we used in comp
		self.root.minsize(width=1440, height=880)
		self.root.maxsize(width=1440, height=880)
		self.root.configure(bg ="gray")
		dataLabel = ['Volt (V)','Amp (A)','Inside Temp (C)','Inside Temp (F)','Probe Temperature','Pressure', 
		'V1','V2','V3','V4','H5','H6','H7','H8'] #set some labels
		x=1
		c=2
		r=13
		for l in dataLabel:
			if (x > 12):
				self.l = tk.Label(text=l, bg ="gray", width=5).grid(column=c,row=r)
				if c < 5:
					c+=1
				else:
					c=2
					r=15
				x+=1
				continue
			self.l = tk.Label(text=l, bg ="gray").grid(column=0,row=x,columnspan=2)
			x+=2
			
		self.warningTitle = tk.Label(text="WARNING", bg="yellow", width=10,height=2)
		self.stopTitle = tk.Label(text="STOP", bg="red", width=10,height=2)
		
		#LabelsData
		self.voltData = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2) 
		self.ampData = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)
		self.temperatureData = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)
		self.insideTempF = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)
		self.probeTemperatureDataCelcius = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)				 
		self.pressureData = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)
		self.waterLeak = tk.Label(text="Water Leak", bg ="gray", width=10)
		self.waterSensorDataOne = tk.Label(text="TBD", relief=tk.SUNKEN, width=20,height=2)
		self.waterSensorDataTwo = tk.Label(text="TBD", relief=tk.SUNKEN, width=20,height=2)
		self.angle = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)

		#motorData labels
		self.motorOneData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5,height=2)
		self.motorTwoData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5,height=2)
		self.motorThreeData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5,height=2)
		self.motorFourData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5,height=2)
		self.motorFiveData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5,height=2)
		self.motorSixData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5,height=2)
		self.motorSevenData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5,height=2)
		self.motorEightData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5,height=2)
		#extra data points 
		self.servoTitle = tk.Label(text="Servo", bg ="gray")
		self.servoData = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)
		self.bTitle = tk.Label(text="Y", bg ="gray")
		self.bData = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)	   
		self.cTitle = tk.Label(text="Z", bg ="gray")
		self.cData = tk.Label(text="TBD",relief=tk.SUNKEN,width=5,height=2)
		
		#depth Datas and Labels	mission 1
		self.currentDepthTitle = tk.Label(text="Current Depth (m)", bg ="gray")
		self.currentDepthData = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)
		self.topDepthTitle = tk.Label(text="Starting Depth", bg ="orange")
		self.topDepthData = tk.Label(text="TBD",relief=tk.SUNKEN,width=10,height=2)
		self.middleDepthTitle = tk.Label(text="Middle Depth", bg ="red")
		self.middleDepthData = tk.Label(text="TBD",relief=tk.SUNKEN,width=10,height=2)		  
		self.bottomDepthTitle = tk.Label(text="Bottom Depth", bg ="yellow")
		self.bottomDepthData = tk.Label(text="TBD",relief=tk.SUNKEN,width=10,height=2)
		
		#depth buttons
		self.topDepthButton = tk.Button(text="top",width=7,highlightbackground="gray")
		self.middleDepthButton = tk.Button(text="middle",width=7,highlightbackground="gray")
		self.bottomDepthButton = tk.Button(text="bottom",width=7,highlightbackground="gray")
		
		#difference in depths
		self.iceDepth = tk.Label(text="Ice Depth", bg ="gray")
		self.oceanDepth = tk.Label(text="Ocean Depth", bg ="gray")
		self.iceData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5)
		self.oceanData = tk.Label(text="TBD", relief=tk.SUNKEN,width=5)
		
		#temp datas and labels mission 2
		self.probeTempTitle = tk.Label(text="Probe Temp", bg ="gray")
		self.probeData = tk.Label(text="TBD",relief=tk.SUNKEN,width=10,height=2)
		self.probeDataF = tk.Label(text="TBD",relief=tk.SUNKEN,width=10,height=2)
		self.C = tk.Label(text="Celcius", bg ="gray",width=10,height=2)
		self.F = tk.Label(text="Fahrenheit", bg ="gray",width=10,height=2)
		self.probeButton = tk.Button(text="top",width=7,highlightbackground="gray")
				
		#top right stuff
		self.timerTitle = tk.Label(text="Timer", bg="gray",width=15,height=2)
		self.timerButton = tk.Button(text= "Start", bg="gray", width=12,height=2,highlightbackground="gray")
		self.timerData = tk.Label(text="00:00", relief=tk.SUNKEN, width=7,height=1,font=("Rockwell", 100),bg="green")
		self.dataButton = tk.Button(text="compile data", bg="gray", width=12,height=2,highlightbackground="gray")
		
		#depthCanvas for depth
		self.depthCanvas = tk.Canvas(self.root, width=800, height = 500, background= "blue",bd=0,highlightthickness=1)
		self.rov2 = self.depthCanvas.create_polygon(0, 0, 40, 0, 40,5, 30,5, 30,15, 40,15, 40,20, 0,20, 0,15, 10,15, 10,5, 0,5, 0,0,outline='black', fill='black')
		bucket = 630
		self.bucketWhite = self.depthCanvas.create_polygon(bucket,440, bucket+22,440, bucket+22,410, bucket+28,410, bucket+28,440, bucket+50,440, bucket+50,500, bucket,500,fill="white",outline="black")
		self.bucketLid = self.depthCanvas.create_rectangle(bucket-2,438, bucket+52,442,fill="orange",outline="black")
		cap = 100
		self.capWhite = self.depthCanvas.create_polygon(cap,480, cap+15,480, cap+15,410, cap+35,410, cap+35,480, cap+50,480, cap+50,500, cap,500,fill="white",outline="black")
		self.capLid = self.depthCanvas.create_rectangle(cap-2,480, cap+52,480,fill="white",outline="black")
		self.flange = self.depthCanvas.create_rectangle(cap+10,410, cap+40,440,fill="black",outline="black")						
		self.bolt = self.depthCanvas.create_polygon(cap+18,410, cap+12,410, cap+12,407, cap+25,407, cap+25,402, cap+5,402, cap+5,407, cap+18,407,fill="white", outline="black")
		self.bolt = self.depthCanvas.create_polygon(cap+38,410, cap+32,410, cap+32,407, cap+45,407, cap+45,402, cap+25,402, cap+25,407, cap+38,407,fill="white", outline="black")		
		cube = 500
		self.cubeSateOutside = self.depthCanvas.create_rectangle(cube,460, cube+70,500,width=1,fill="white",outline="black")		
		self.cubeSateMiddle = self.depthCanvas.create_rectangle(cube+5,465, cube+65,495,width=1,fill="blue",outline="white")
		self.cubeSateInside = self.depthCanvas.create_rectangle(cube+10,470, cube+60,490,fill="white",outline="white")
		self.cubeSateLine = self.depthCanvas.create_line(cube+5, 480, cube+65,480, fill="white",width=5)		
		self.cubeSatNumber = self.depthCanvas.create_text(cube+35, 480, text="H139D")
		coral =550
		self.coralBranchOne = self.depthCanvas.create_line(coral+3, 475, coral+15, 490, fill = "red",width=2)
		self.coralBranchTwo = self.depthCanvas.create_line(coral+10, 470, coral+15, 490, fill = "yellow",width=2)
		self.coralBranchThree = self.depthCanvas.create_line(coral+30, 465, coral+15, 490, fill = "orange",width=2)
		self.coralBranchFour = self.depthCanvas.create_line(coral, 460, coral+15, 490, fill = "purple",width=2)
		self.coralBranchFive = self.depthCanvas.create_line(coral+35, 470, coral+15, 490, fill = "white")
		self.coralBase = self.depthCanvas.create_polygon(coral+18,490, coral+12,490, coral+12,492, coral+25,492, coral+25,497, coral+5,497, coral+5,492, coral+18,492,fill="white")
		oilT=0
		self.oilTOne = self.depthCanvas.create_polygon(oilT,475, oilT+35,475, oilT+35,485, oilT+23,485, oilT+23,496, oilT+12,496, oilT+12,485, oilT,485,fill="brown",outline="black")
		self.oilTTwo = self.depthCanvas.create_polygon(oilT+40,475, oilT+75,475, oilT+75,485, oilT+63,485, oilT+63,496, oilT+52,496, oilT+52,485, oilT+40,485,fill="brown",outline="black")
		self.oilTBase = self.depthCanvas.create_rectangle(oilT+10,496, oilT+64,500, fill="brown",outline="black")
		cCoral=600
		self.aBase = self.depthCanvas.create_line(cCoral,500, cCoral+50,500,fill="#daa520",width=5)
		self.aBranch = self.depthCanvas.create_line(cCoral+25,498, cCoral+25,430, fill="#daa520",width=5)
		self.aBranchTwo = self.depthCanvas.create_line(cCoral,480, cCoral+50,480,fill="#daa520",width=5)
		self.aBranchThree = self.depthCanvas.create_line(cCoral,483, cCoral,460,fill="#daa520",width=5)
		self.aBranchFour = self.depthCanvas.create_line(cCoral+50,483, cCoral+50,450,fill="#daa520",width=5)
		self.aBranchDead = self.depthCanvas.create_line(cCoral+15,445, cCoral+23,445, fill="black",width=5)
		self.aBranchDeadTwo = self.depthCanvas.create_line(cCoral+15,445, cCoral+15,430, fill="black",width=5)
		self.aSign = self.depthCanvas.create_rectangle(cCoral+15,480, cCoral+35,498, fill="white")
		self.aLetter = self.depthCanvas.create_text(cCoral+25,490, text="A")
		ESP = 200
		self.box = self.depthCanvas.create_polygon(ESP+50,450, ESP+100,450, ESP+100,500, ESP+50,500,fill="red",outline="black")
		self.box = self.depthCanvas.create_polygon(ESP+50,450, ESP+100,450, ESP+100,500, ESP+50,500,fill="red",outline="black")
		self.connector = self.depthCanvas.create_polygon(ESP,480, ESP+20,480, ESP+20,470, ESP+30,470, ESP+30,480, ESP+60,480, ESP+60,490, ESP+30,490, ESP+30,500, ESP+20,500, ESP+20,490, ESP,490, fill="white",outline="black")
		self.connectorLine = self.depthCanvas.create_line(ESP,485, ESP-10,480, fill="orange")
		self.connectorLineT = self.depthCanvas.create_line(ESP-10,480, ESP-40,490, fill="orange")
		self.light = self.depthCanvas.create_arc(0, -10, 90, 30,start=-30,outline='blue', fill='white',extent=60)		
		self.finishLineWhite = self.depthCanvas.create_line(760, 0, 760, 500, fill = "white",width=8, dash=(20, 20))
		self.finishLineBlack = self.depthCanvas.create_line(760, 20, 760, 500, fill = "black",width=8, dash=(20, 20))
		
		#servoCanvas
		self.servoCanvas = tk.Canvas(self.root, width=200, height = 150, background= "white")
		self.servoClawRight = self.servoCanvas.create_polygon(0,0, 20,0, 20,10, 30,10, 30,30, 20,30, 20,100, 0,100, outline='black', fill='black')
		self.servoClawLeft = self.servoCanvas.create_polygon(200,0, 180,0, 180,10, 170,10, 170,30, 180,30, 180,100, 200,100, outline='black', fill='black')
		self.rovBase = self.servoCanvas.create_polygon(0,100,200,100,200,150,0,150,outline='black', fill='black')
		#compassCanvas
		self.compassCanvas = tk.Canvas(self.root, width=200, height = 200, background= "gray")
		self.compass = self.compassCanvas.create_oval(10, 10, 190, 190, outline='black', fill='white')
		self.compassArcNegativeFour = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='red',extent=0)
		self.compassArcNegativeThree = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='orange',extent=0)
		self.compassArcNegativeTwo = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='yellow',extent=0)
		self.compassArcNegativeOne = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='green',extent=0)
		self.compassArc = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='green',extent=0)
		self.compassArcTwo = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='yellow',extent=0)
		self.compassArcThree = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='orange',extent=0)
		self.compassArcFour = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='red',extent=0)
		self.compassLineOne = self.compassCanvas.create_line(100,100,10,60, fill="red",arrow=tk.LAST, arrowshape=(70,75,3))
		self.compassLineTwo = self.compassCanvas.create_line(100,100,10,60,arrow=tk.LAST, arrowshape=(50,55,3))
		self.middle = self.compassCanvas.create_oval(95,95,105,105, outline='black', fill='white')		

		#motorControl canvas
		self.motorControl = tk.Canvas(self.root, width=200, height = 200, background= "blue")
		self.hexagon = self.motorControl.create_polygon(25,75,75,25,125,25,175,75,175,135,125,185,75,185,25,135, outline='black', fill='black')
		self.V1 = self.motorControl.create_oval(40,40,60,60, outline='black', fill='white')
		self.V2 = self.motorControl.create_oval(140,40,160,60, outline='black', fill='white')		
		self.V3 = self.motorControl.create_oval(40,150,60,170, outline='black', fill='white')		
		self.V4 = self.motorControl.create_oval(140,150,160,170, outline='black', fill='white')		
		self.H1 = self.motorControl.create_polygon(50,80,80,50,90,60,60,90,50,80, outline='black', fill='white')
		self.H1R = self.motorControl.create_polygon(65,65,80,50,90,60,75,75,65,65,outline='black',fill='green')
		self.H2 = self.motorControl.create_polygon(150,80,120,50,110,60,140,90,150,80, outline='black', fill='white')
		self.H2R = self.motorControl.create_polygon(135,65,120,50,110,60,125,75,135,65,outline='black',fill='green')
		self.H3 = self.motorControl.create_polygon(50,120,80,150,90,140,60,110,50,120, outline='black', fill='white')
		self.H3R = self.motorControl.create_polygon(65,135,80,150,90,140,75,125,65,135,outline='black',fill='green')
		self.H4 = self.motorControl.create_polygon(150,120,120,150,110,140,140,110,150,120, outline='black', fill='white')	
		self.H4R = self.motorControl.create_polygon(135,135,120,150,110,140,125,125,135,135,outline='black',fill='green')
		
		#error display
		self.errorLog = tk.Text(self.root, width=45, height=4)
		self.messageLog = tk.Text(self.root, width=45, height=4)
		#grid layout
		#left column
		self.warningTitle.grid(			 column=0,	row=0)
		self.stopTitle.grid(			 column=1,	row=0)
		self.voltData.grid(				 column=0,	row=2,	columnspan=2)
		self.ampData.grid(				 column=0,	row=4,	columnspan=2)
		self.temperatureData.grid(		 column=0,	row=6,	columnspan=2)
		self.angle.grid(				 column=2,	row=6,	columnspan=4)		 
		self.insideTempF.grid(			 column=0,	row=8,	columnspan=2)
		self.probeTemperatureDataCelcius.grid(column=0,	 row=10, columnspan=2)
		self.pressureData.grid(			 column=0,	row=12, columnspan=2)
		self.waterLeak.grid(			 column=6,	row=0) 
		self.waterSensorDataOne.grid(	 column=2,	row=0,	columnspan=4)
		self.waterSensorDataTwo.grid(	 column=7,	row=0,	columnspan=2)
		#motor grid
		self.motorOneData.grid(			 column=2,	row=14)
		self.motorTwoData.grid(			 column=3,	row=14)
		self.motorThreeData.grid(		 column=4,	row=14)
		self.motorFourData.grid(		 column=5,	row=14)
		self.motorFiveData.grid(		 column=2,	row=16)
		self.motorSixData.grid(			 column=3,	row=16)
		self.motorSevenData.grid(		 column=4,	row=16)
		self.motorEightData.grid(		 column=5,	row=16)
		#extras
		self.servoTitle.grid(			 column=6,	row=13)
		self.servoCanvas.grid(           column=6,  row=14, rowspan=3)
		#self.servoData.grid(				 column=6,	row=14)
		#self.bTitle.grid(				 column=6,	row=15)
		#self.bData.grid(				 column=6,	row=16)
		#self.cTitle.grid(				 column=9,	row=15)
		#self.cData.grid(				 column=9,	row=16)	  
		#right side
		self.timerTitle.grid(			column=10,	row=2,	columnspan= 2)
		self.timerButton.grid(			column=12,	row=2,	columnspan= 3)
		self.dataButton.grid(			column=12,	row=3,	columnspan= 3)
		self.timerData.grid(			column=10,	row=0,	columnspan= 5, rowspan=2) 
		self.currentDepthTitle.grid(	column=10,	row=3,	columnspan= 2)
		self.currentDepthData.grid(		column=10,	row=4,	columnspan= 2)
		self.topDepthTitle.grid(		column=10,	row=5)
		self.topDepthButton.grid(		column=11,	row=5)
		self.topDepthData.grid(			column=10,	row=6)
		self.middleDepthTitle.grid(		column=10,	row=7)
		self.middleDepthButton.grid(	column=11,	row=7)
		self.middleDepthData.grid(		column=10,	row=8)
		self.bottomDepthTitle.grid(		column=10,	row=9)
		self.bottomDepthButton.grid(	column=11,	row=9)
		self.bottomDepthData.grid(		column=10,	row=10)
		self.iceDepth.grid(				column=12,	row=6)
		self.iceData.grid(				column=12,	row=7)
		self.oceanDepth.grid(			column=12,	row=8)
		self.oceanData.grid(			column=12,	row=9)
		#probe right side
		self.probeTempTitle.grid(		column=10,	row=11)
		self.probeButton.grid(			column=11,	row=11)		   
		self.probeData.grid(			column=10,	row=12)
		self.probeDataF.grid(			column=11,	row=12)
		self.C.grid(					column=10,	row=13)
		self.F.grid(					column=11,	row=13)
		#canvases
		self.depthCanvas.grid(			column=2,	row=2, columnspan=8,  rowspan=11)
		self.compassCanvas.grid(		column=7,	row=13, columnspan=1,  rowspan=4)
		self.motorControl.grid(			column=0,	row=13, columnspan=2,  rowspan=4)
		self.errorLog.grid(             column=9,   row=13, columnspan=4,  rowspan=2)
		self.messageLog.grid(           column=9,   row=15, columnspan=4,  rowspan=2)
		self.root.mainloop()

		

app=App()
