#code for SeaSweepersROV GUI 'BRUCE' written by Brian Ishii. 2016
# for python 3.x use 'tkinter' rather than 'Tkinter'
import Tkinter as tk
import time
import math
from serial import *

#Setting up Serial port
#for raspberry pi use serialPort = "/dev/ttyACM0"
#serialPort = "/dev/cu.usbmodemFD121"
serialPort = "/dev/cu.usbmodem106"
baudRate = 115200
ser = Serial(serialPort , baudRate, timeout=0, writeTimeout=0) #ensure non-blocking, code will not run if the port is not connected

#assigned variables
dataList = [] #empty dataList for receiving data
serBuffer = ""
tempBuffer= ""
depthBuffer = ""
tickerForDepth = 0
probeTempBuffer = ""
#not in use yet
joyStickOneBuffer = ""
joyStickTwoBuffer = ""
joyStickThreeBuffer = ""
joyStickFourBuffer = ""
joyStickFiveBuffer = ""
joyStickSixBuffer = ""
servoBuffer = ""
lightBuffer = ""
xAccelBuffer = ""
yAccelBuffer = ""
zAccelBuffer = ""
#
angleBuffer = 0
dataArray=[]
previousAngle = ""
motorColor = "white"
timeInWater = "00:00"
topDepthNumber = 0 #saved values for depthValues in dataTwo
middleDepthNumber = 0
bottomDepthNumber = 0
l=""#char for top middle bottom usage in dataTwo
z=0 #value for depth canvas movement horizontal in data Two
lineCoordsX=0
lineCoordsY=0
r=""
e = ""
g = ""
b = ""
coords=0
looops = 1
color = "white"
w=0
altitudeBuffer = ""
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
		self.aTitle = tk.Label(text="X", bg ="gray")
		self.aData = tk.Label(text="TBD",relief=tk.SUNKEN,width=20,height=2)
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
		self.topDepthButton = tk.Button(text="top",width=7,highlightbackground="gray",command= self.topDepthValue)
		self.middleDepthButton = tk.Button(text="middle",width=7,highlightbackground="gray", command=self.middleDepthValue)
		self.bottomDepthButton = tk.Button(text="bottom",width=7,highlightbackground="gray", command=self.bottomDepthValue)
		
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
		self.probeButton = tk.Button(text="top",width=7,highlightbackground="gray",command=self.probeTempValue)
				
		#top right stuff
		self.timerTitle = tk.Label(text="Timer", bg="gray",width=15,height=2)
		self.timerButton = tk.Button(text= "Start", bg="gray", width=12,height=2,highlightbackground="gray", command=self.getTime)
		self.timerData = tk.Label(text="00:00", relief=tk.SUNKEN, width=7,height=1,font=("Rockwell", 100),bg="green")
		self.dataButton = tk.Button(text="compile data", bg="gray", width=12,height=2,highlightbackground="gray", command=self.getData)
		
		#depthCanvas for depth
		self.depthCanvas = tk.Canvas(self.root, width=800, height = 500, background= "blue",bd=0,highlightthickness=1)
		self.rov2 = self.depthCanvas.create_polygon(0, 0, 40, 0, 40,5, 30,5, 30,15, 40,15, 40,20, 0,20, 0,15, 10,15, 10,5, 0,5, 0,0,outline='black', fill='black')
		self.light = self.depthCanvas.create_arc(0, -10, 90, 30,start=0,outline='blue', fill='white',extent=0)		
		self.topDepthLine = self.depthCanvas.create_line(0,0,800,0, fill = "orange",width=3, dash=(8, 8))
		self.middleDepthLine = self.depthCanvas.create_line(0,0,800,0, fill = "red",width=3, dash=(8, 8))
		self.bottomDepthLine = self.depthCanvas.create_line(0,0,800,0, fill = "yellow",width=3, dash=(8, 8))
		self.finishLineWhite = self.depthCanvas.create_line(720, 0, 720, 500, fill = "white",width=8, dash=(20, 20))
		self.finishLineBlack = self.depthCanvas.create_line(720, 20, 720, 500, fill = "black",width=8, dash=(20, 20))
		
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
		self.motorControl = tk.Canvas(self.root, width=200, height = 200, background= "gray")
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
		self.aTitle.grid(				 column=6,	row=13)
		self.aData.grid(				 column=6,	row=14)
		self.bTitle.grid(				 column=6,	row=15)
		self.bData.grid(				 column=6,	row=16)
		self.cTitle.grid(				 column=9,	row=15)
		self.cData.grid(				 column=9,	row=16)	  
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

		self.update_data()
		self.root.mainloop()
		
	#functions
	def getData(self):
		global dataList
		newList = dataList[:]
		file = open('dataSheet.txt','a')
		file.truncate()
		for i in newList:
			file.write(i)
		print "done"
		#file.close()

	def topDepthValue(self):
		global depthBuffer
		global topDepthNumber
		tb = depthBuffer
		length = len(tb)
		length = length - 2
		labelDepth = tb[:length] + "." + tb[length:]
		self.topDepthData.configure(text=labelDepth)
		topDepthNumber = float(tb)
		self.depthCanvas.update() 
	def middleDepthValue(self):
		global depthBuffer
		global middleDepthNumber
		length = len(depthBuffer)
		length = length - 2
		labelDepth = depthBuffer[:length] + "." + depthBuffer[length:]
		self.middleDepthData.configure(text=labelDepth)
		middleDepthNumber = float(depthBuffer)
		self.depthCanvas.update()
	def bottomDepthValue(self):
		global depthBuffer
		global bottomDepthNumber
		length = len(depthBuffer)
		length = length - 2
		labelDepth = depthBuffer[:length] + "." + depthBuffer[length:]
		self.bottomDepthData.configure(text=labelDepth)
		bottomDepthNumber = float(depthBuffer)
		self.depthCanvas.update()
	def probeTempValue(self):
		global probeTempBuffer
		try:
			convertedTemp = self.tempConversion("p")
			self.probeDataF.configure(text=convertedTemp)
			self.probeData.configure(text=probeTempBuffer)
		except:
			self.probeDataF.configure(text="ERR")
			self.probeData.configure(text="ERR")
	
	def updateClock(self):
		now = time.time()
		global startTime
		global timeInWater
		timeElapsed = int(now) - int(startTime)
		minutes= int(timeElapsed / 60)
		if minutes >13:
			self.timerData.configure(bg = "red")
		elif minutes >12:
			self.timerData.configure(bg = "yellow")
		if minutes < 10:
			minutes = "0" + str(minutes)
		seconds= timeElapsed % 60
		if seconds < 10:
			seconds = "0" +str(seconds)
		timeElapsed = str(minutes)+":"+str(seconds)
		timeInWater = timeElapsed
		self.timerData.configure(text=timeElapsed)
		self.root.after(1000, self.updateClock)		   
		#timer function
	
	def getTime(self):
		global z
		z=0
		self.depthCanvas.delete()
		start = time.time()
		global startTime
		startTime = int(start)
		self.updateClock()
	
	def update_data(self):
		global dataList
		global timeInWater
		global w
		ser.open
		serr=""
		c= ""
		first = 0
		global dataArray
		dataArray = []
		data = ser.readline()
		dataList.append(timeInWater)
		dataList.append(data)
		#print dataList
		for i in data:
			dataArray.append(i)
		#print dataArray
		w = 0
		ser.close
		for i in range(19):
			self.dataOne(i)
		self.dataTwo()
		self.root.after(100, self.update_data)

	def dataOne(self,c):
		head = ['A','B','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n']
		limits = ['30','35','10000','200000','3000', '3500','1000','1000','1000','1000','1000','1000','1000',
		'1000','1000','1000','1000','1000','1000','1000','1000','1000','10','13','1000','1000','100','100',
		'100','100','100','100','1000','1000','50','100','50','100']
		global color 
		global motorColor
		global w
		global coords
	   # buffers = ['tempBuffer','pressureBuffer', 'probeTempBuffer','joyStickOneBuffer',
		 #'joyStickTwoBuffer','joyStickThreeBuffer','joyStickFourBuffer','joyStickFiveBuffer','joyStickSixBuffer','hThreeBuffer',
		 #'hFourBuffer','totalVoltBuffer','totalAmpBuffer','xAccelBuffer','yAccelBuffer',
		 #'zAccelBuffer','angleBuffer','waterOne','waterTwo']
		global buf
		#buf = buffers[c]
		first = 0
		buf = ""
		for item in range(len(dataArray)):
			if first == 0:
				if dataArray[item] == head[2*c]:
					first +=1
					#print item
					a = 1
					try:
						while dataArray[int(item)+a] != head[(2*c)+1]: 
							buf += dataArray[int(item)+a]
							a +=1
						#print int(buf) data points
						if int(buf) >= int(limits[(2*c)+1]):
							color = "red"
							self.stopTitle.configure(bg = color)
							w+=1
							#ser.open
							#ser.write(b'2')
							#ser.close
						elif int(buf)>= int(limits[(2*c)]):
							color = "yellow"
							self.warningTitle.configure(bg = color)
							w+=1
							#ser.open
							#ser.write(b'1')
							#ser.close
						else: 
							color = "white"
							if w == 0:
								self.warningTitle.configure(bg = "gray")
								self.stopTitle.configure(bg = "gray")
								#ser.open
								#ser.write(b'0')
								#ser.close
					except: 
						print "bad data" + str(c)
					if c == 0:
						try:
							self.temperatureData.configure(text=buf,bg = color)
							global tempBuffer
							tempBuffer = buf				
							f = self.tempConversion("i")
							self.insideTempF.configure(text=f, bg = color)
						except:
							print "bad temp"
					elif c == 1:
						try:
							self.pressureData.configure(text=buf, bg = color)
						except:
							print "bad pressure"
					elif c == 2:
						try:
							length = len(buf)
							length = length - 2
							buf = buf[:length] + "." + buf[length:]
							self.probeTemperatureDataCelcius.configure(text=buf, bg = color)
							global probeTempBuffer
							probeTempBuffer = buf
						except:
							print"bad probe temp"
					elif c == 3:
						global joyStickOneBuffer
						joyStickOneBuffer = buf
						#self.joyStickConversion()

					elif c == 4:
						global joyStickTwoBuffer
						joyStickTwoBuffer = buf
						self.joyStickConversion()
					elif c == 5:
						global joyStickThreeBuffer
						joyStickThreeBuffer = buf
					elif c == 6:
						global joyStickFourBuffer
						joyStickFourBuffer = buf
					elif c == 7:
						global joyStickFiveBuffer
						joyStickFiveBuffer = buf
					elif c == 8:
						global joyStickSixBuffer
						joyStickSixBuffer = buf
						self.joyStickConversion()
					elif c == 9:
						global lightBuffer
						lightBuffer = buf
					elif c == 10:
						try:
							print"hi"
							#motorColor = self.motorCanvasColor(buf)
							#self.motorControl.itemconfigure(self.H4, fill=motorColor)
							#self.motorControl.update()
						except:
							print "bad motor eight data"
					elif c == 11:
						try:
							self.voltData.configure(text="12", bg = color)
						except:
							print "bad volt data"
					elif c == 12:
						try:
							self.ampData.configure(text="1.0", bg = color)
						except:
							print "bad amp data"
					elif c == 13:
						try:
							adf=1
							#self.aData.configure(text=buf, bg = color)
						except:
							print "bad a data"
					elif c == 14:
						try:
							self.bData.configure(text=buf, bg = color)
						except:
							print "bad b data"
					elif c == 15:
						try:
							self.cData.configure(text=buf, bg = color)
						except:
							print "bad c data"
					elif c == 16:
						try:
							self.angle.configure(text=buf, bg = color)
							self.compassData(buf)
						except:
							print "bad compass data"
					elif c == 17:
						try:
							self.waterSensorDataOne.configure(text=buf, bg = color)
						except:
							print "bad water sensor data"
					elif c == 18:
						try:
							self.waterSensorDataTwo.configure(text=buf, bg = color)
						except:
							print "bad water sensor data"
	def dataTwo(self):
		global topDepthNumber
		global middleDepthNumber
		global bottomDepthNumber
		global z #value for depth canvas movement horizontal
		global tickerForDepth #will replace z
		global coords
		global color 
		global depthBuffer 
		global timeInWater
		global lightBuffer
		first = 0
		for item in range(len(dataArray)):
			if first == 0:
				if dataArray[item] == 'C':
					first +=1
					depthBuffer = ''
					#print item
					a = 1
					try:
						while dataArray[int(item)+a] != 'D': 
							depthBuffer += dataArray[int(item)+a]
							a +=1
					except: 
						print "bad depthData"
					try:
						coords = int(depthBuffer)
						if timeInWater != ("00:00"):
							second = timeInWater[4:]
							if (second != tickerForDepth):
								self.depthCanvas.coords(self.rov2, 0+z, 0+(coords), 40+z, 0+(coords), 40+z,5+(coords), 30+z,5+(coords), 30+z,15+(coords), 40+z,15+(coords), 40+z,20+(coords), 0+z,20+(coords), 0+z,15+(coords), 10+z,15+(coords), 10+z,5+(coords), 0+z,5+(coords), 0+z,0+(coords))
								lightBuffer = int(lightBuffer)
								if (lightBuffer == 0):
									self.light = self.depthCanvas.create_arc(0+z, -10+(coords), 90+z, 30+(coords),start=0,outline='blue', fill='white',extent=0)		
								else:
									lightValue = int(lightBuffer)/100
									self.light = self.depthCanvas.create_arc(0+z, -10+(coords), 90+z, 30+(coords),start=-30-lightValue,outline='blue', fill='white',extent=60+(2*lightValue))		
								global lineCoordsX
								global lineCoordsY #coords for line
								item = self.depthCanvas.create_line(lineCoordsX, lineCoordsY, z, (coords), fill = "white",width=1)
								lineCoordsX=z
								lineCoordsY=(coords)			
								tickerForDepth = second		
								z+=1	
							
							self.depthCanvas.coords(self.topDepthLine,0,topDepthNumber,800,topDepthNumber)
							self.depthCanvas.coords(self.middleDepthLine,0,middleDepthNumber,800,middleDepthNumber)
							self.depthCanvas.coords(self.bottomDepthLine,0,bottomDepthNumber,800,bottomDepthNumber)
							minute = timeInWater[:2]+timeInWater[3:]
							if (int(minute) % 100) == 0:
								item = self.depthCanvas.create_line(z, 450, z, 500, fill = "white",width=1)
						ice = (topDepthNumber - middleDepthNumber)/100
						ocean = (middleDepthNumber - bottomDepthNumber)/100
						self.iceData.configure(text=ice)
						self.oceanData.configure(text=ocean)
					except:
						print"bad depth"
						
					length = len(depthBuffer)
					length = length - 2
					labelDepth = depthBuffer[:length] + "." + depthBuffer[length:]
					self.currentDepthData.configure(text=labelDepth,bg = color)
					
	def compassData(self,angle):
		global previousAngle
		global looops
		try:
			if int(angle) >=0 and int(angle) < 20:
				if int(previousAngle) <=360 and int(previousAngle) >340:
					looops +=1
			if int(angle) <=360 and int(angle) >340:
				if int(previousAngle) >=0 and int(previousAngle) < 20:
					looops -=1
			#print looops
		except:
			print "bad compassData"
		try: 
			pi = int(angle)
			previousAngle = ""
			previousAngle += angle
		except:
			angle = previousAngle
		angleBuffer = "" # empty the buffer
		r = int(angle)
		num = math.radians(pi)
		num2 = math.radians(pi+180)
		x =(100-math.sin(num)*70)
		y= (100-math.cos(num)*70)
		x2=(100-math.sin(num2)*50)
		y2= (100-math.cos(num2)*50)
		self.compassCanvas.coords(self.compassLineOne, 100,100,x,y)
		self.compassCanvas.coords(self.compassLineTwo, 100,100,x2,y2)
		if looops == 1:
			self.compassCanvas.itemconfigure(self.compassArc, extent=r)
			self.compassCanvas.itemconfigure(self.compassArcTwo, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcThree, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcFour, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcNegativeOne, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcNegativeTwo, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcNegativeThree, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcNegativeFour, extent=0)
		if looops == 2:
			self.compassCanvas.itemconfigure(self.compassArc, extent=359)
			self.compassCanvas.itemconfigure(self.compassArcTwo, extent=r)
			self.compassCanvas.itemconfigure(self.compassArcThree, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcFour, extent=0)
		if looops == 3:
			self.compassCanvas.itemconfigure(self.compassArcTwo, extent=359)
			self.compassCanvas.itemconfigure(self.compassArcThree, extent=r)
			self.compassCanvas.itemconfigure(self.compassArcFour, extent=0)
		if looops == 4:
			self.compassCanvas.itemconfigure(self.compassArcTwo, extent=359)
			self.compassCanvas.itemconfigure(self.compassArcFour, extent=r)		
		if pi <0 and pi>-360:
			self.compassCanvas.itemconfigure(self.compassArc, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcNegativeFour, extent=359)
			self.compassCanvas.itemconfigure(self.compassArcNegativeThree, extent=359)
			self.compassCanvas.itemconfigure(self.compassArcNegativeTwo, extent=359)
			self.compassCanvas.itemconfigure(self.compassArcNegativeOne, extent=r)
		if pi <=-360 and pi>-720:
			self.compassCanvas.itemconfigure(self.compassArcNegativeFour, extent=359)
			self.compassCanvas.itemconfigure(self.compassArcNegativeThree, extent=359)
			self.compassCanvas.itemconfigure(self.compassArcNegativeTwo, extent=r)
			self.compassCanvas.itemconfigure(self.compassArcNegativeOne, extent=0)
		if pi <=-720 and pi>-1080:
			self.compassCanvas.itemconfigure(self.compassArcNegativeFour, extent=359)
			self.compassCanvas.itemconfigure(self.compassArcNegativeThree, extent=r)
			self.compassCanvas.itemconfigure(self.compassArcNegativeTwo, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcNegativeOne, extent=0)
		if pi <=-1080 and pi>-1440:
			self.compassCanvas.itemconfigure(self.compassArcNegativeFour, extent=r)
			self.compassCanvas.itemconfigure(self.compassArcNegativeThree, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcNegativeTwo, extent=0)
			self.compassCanvas.itemconfigure(self.compassArcNegativeOne, extent=0)
		self.compassCanvas.update()
	def motorCanvasColor(self, buf):
		try:
			if int(buf)>500:
				return "orange"		
			elif int(buf)>300:
				return "yellow"
			elif int(buf)>100:
				return "green"
			else:
				return "white"
		except:
			print"bad motorCanvasColor"
	def tempConversion(self,t):
		if t == "p":
			global probeTempBuffer
			try:
				fahreinheit = ((float(probeTempBuffer)*1.8000)+32.00)
			except:
				print"bad probe temp data"
		if t == "i":
			global tempBuffer
			try:
				fahreinheit = ((float(tempBuffer)*1.8000)+32.00)
			except:
				print"bad inside temp"
		try:
			fahreinheit = float(fahreinheit * 100)
			fahreinheit = float(int(fahreinheit) / 100)
			return fahreinheit
		except:
			return 0000
	def joyStickConversion(self):
		global joyStickOneBuffer
		global joyStickTwoBuffer
		global joyStickThreeBuffer
		global joyStickFourBuffer
		global joyStickFiveBuffer
		global joyStickSixBuffer
		vOne = 0
		vTwo = 0
		vThree = 0
		vFour = 0
		hOne = 0
		hTwo = 0
		hThree = 0
		hFour = 0
		try:
			joyStickOneBuffer = self.joyStickMap(int(joyStickOneBuffer))
			joyStickTwoBuffer = self.joyStickMap(int(joyStickTwoBuffer))
			joyStickThreeBuffer = self.joyStickMap(int(joyStickThreeBuffer))
			joyStickFourBuffer = self.joyStickMap(int(joyStickFourBuffer))
			joyStickFiveBuffer = self.joyStickMap(int(joyStickFiveBuffer))
			joyStickSixBuffer = self.joyStickMap(int(joyStickSixBuffer))
		except:
			print"bad joystick conversion"
		try:
			if (joyStickOneBuffer > 50) or (joyStickOneBuffer < -50):
				vOne = joyStickOneBuffer
				vTwo = joyStickOneBuffer
				vThree = joyStickOneBuffer
				vFour = joyStickOneBuffer
			elif (joyStickThreeBuffer > 50) or (joyStickThreeBuffer < -50):
				vOne = joyStickThreeBuffer
				vTwo = joyStickThreeBuffer
				vThree = joyStickThreeBuffer
				vFour = joyStickThreeBuffer
			elif (joyStickFourBuffer > 15) or (joyStickFourBuffer < -15):
				vOne = joyStickFourBuffer
				vTwo = joyStickFourBuffer
				vThree = -int(joyStickFourBuffer)
				vFour = -int(joyStickFourBuffer)	
			if (joyStickTwoBuffer > 50) or (joyStickTwoBuffer < -50):
				hOne = joyStickTwoBuffer
				hTwo = joyStickTwoBuffer
				hThree = joyStickTwoBuffer
				hFour = joyStickTwoBuffer
			elif (joyStickFiveBuffer > 50) or (joyStickFiveBuffer < -50):
				hOne = int(joyStickFiveBuffer)
				hTwo = -joyStickFiveBuffer
				hThree = -int(joyStickFiveBuffer)
				hFour = joyStickFiveBuffer
			elif (joyStickSixBuffer > 15) or (joyStickSixBuffer < -15):
				hOne = joyStickSixBuffer
				hTwo = joyStickSixBuffer
				hThree = -int(joyStickSixBuffer)
				hFour = -int(joyStickSixBuffer)
		except:
			print"bad joystick read"
		try:
			self.motorOneData.configure(text=vOne)
			self.motorTwoData.configure(text=vTwo)
			self.motorThreeData.configure(text=vThree)
			self.motorFourData.configure(text=vFour)
			
			self.motorFiveData.configure(text=hOne)
			self.motorSixData.configure(text=hTwo)
			self.motorSevenData.configure(text=hThree)
			self.motorEightData.configure(text=hFour)
		except:
			print "bad labeling motors"
		try:
			hOne = int(hOne)/17
			hTwo = int(hTwo)/17
			hThree = int(hThree)/17
			hFour = int(hFour)/17

			self.motorControl.coords(self.H1R, 65,65,65+hOne,65-hOne,75+hOne,75-hOne,75,75,65,65)
			self.motorControl.coords(self.H2R, 135,65,135+hTwo,65+hTwo,125+hTwo,75+hTwo,125,75,135,65)
			self.motorControl.coords(self.H3R, 65,135,65+hThree,135+hThree,75+hThree,125+hThree,75,125,65,135)
			self.motorControl.coords(self.H4R, 135,135,135+hFour,135-hFour,125+hFour,125-hFour,125,125,135,135)

			self.motorControl.itemconfigure(self.V1R, extent=vOne)
			self.motorControl.itemconfigure(self.V2R, extent=vTwo)
			self.motorControl.itemconfigure(self.V3R, extent=vThree)
			self.motorControl.itemconfigure(self.V4R, extent=vFour)
		except:
			print "bad joystick canvas"
		#motorColor = self.motorCanvasColor(vOne)
		#self.motorControl.itemconfigure(self.V1, fill=motorColor)
		#self.motorControl.itemconfigure(self.V2, fill=motorColor)
		#self.motorControl.itemconfigure(self.V3, fill=motorColor)
		#self.motorControl.itemconfigure(self.V4, fill=motorColor)
		self.motorControl.update()

	def map(self, value, leftMin, leftMax, rightMin, rightMax):
		leftSpan = leftMax - leftMin      # Figure out how 'wide' each range is
		rightSpan = rightMax - rightMin
		valueScaled = float(value - leftMin) / float(leftSpan)    # Convert the left range into a 0-1 range (float)
		return int(rightMin + (valueScaled * rightSpan))    # Convert the 0-1 range into a value in the right range.

	def joyStickMap(self, joyStick):
		if joyStick >= 512:		#512-1023 down
			joyStick = self.map(joyStick, 512, 1023, 0, 255)
		elif joyStick < 512:	#0-512 up
			joyStick = self.map(joyStick, 512, 0, 0, 255)
			joyStick = (joyStick * -1)
		return joyStick


		

app=App()
