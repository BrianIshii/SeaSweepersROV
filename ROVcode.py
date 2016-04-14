# for python 3.x use 'tkinter' rather than 'Tkinter'
import Tkinter as tk
import time
import math
from serial import *

serialPort = "/dev/cu.usbmodemFD121"
baudRate = 115200
ser = Serial(serialPort , baudRate, timeout=0, writeTimeout=0) #ensure non-blocking
serBuffer = ""
tempBuffer= ""
depthBuffer = ""
pressureBuffer = ""
probeTempBuffer = ""
vOneBuffer = ""
vTwoBuffer = ""
vThreeBuffer = ""
vFourBuffer = ""
hOneBuffer = ""
hTwoBuffer = ""
hThreeBuffer = ""
hFourBuffer = ""
totalVoltBuffer = ""
totalAmpBuffer = ""
xAccelBuffer = ""
yAccelBuffer = ""
zAccelBuffer = ""
angleBuffer = 0
dataArray=[]
waterOne = ""
waterTwo = ""
r=""
e = ""
g = ""
b = ""
color = "white"
w=0

altitudeBuffer = ""
class App():
    def __init__(self):
    	self.root = tk.Tk()
        self.root.title("SeaSweepers GANG BANG BANG")
        self.root.configure(bg ="gray")
        dataLabel = ['Volt (V)','Amp (A)','Temperature (F)','Humidity','Temperature (C)','Pressure','Front Leak','Back Leak', 'M1','M2','M3','M4','M5','M6','M7','M8']
        x=1
        c=2
        r=13
        for l in dataLabel:
        	if (x > 16):
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
        self.warningTitle = tk.Label(text="WARNING", bg="yellow", width=10)
        self.stopTitle = tk.Label(text="STOP", bg="red", width=10)#needgrid
        self.title = tk.Label(text="Sea Sweepers", bg="gray")#needgrid
        
        self.voltData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20, height=2) 
        self.ampData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
        
        self.temperatureData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
        self.angle = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
        self.humidityData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
        
        self.temperatureDataCelcius = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
    	                
        self.pressureData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
        
        self.waterSensorDataOne = tk.Label(text="TBA", relief=tk.SUNKEN, width=20,height=2)
        
        self.waterSensorDataTwo = tk.Label(text="TBA", relief=tk.SUNKEN, width=20,height=2)

        
        self.motorOneData = tk.Label(text="TBA", relief=tk.SUNKEN,width=5,height=2)
        
        self.motorTwoData = tk.Label(text="TBA", relief=tk.SUNKEN,width=5,height=2)
        
        self.motorThreeData = tk.Label(text="TBA", relief=tk.SUNKEN,width=5,height=2)
        
        self.motorFourData = tk.Label(text="TBA", relief=tk.SUNKEN,width=5,height=2)
        
        self.motorFiveData = tk.Label(text="TBA", relief=tk.SUNKEN,width=5,height=2)
        
        self.motorSixData = tk.Label(text="TBA", relief=tk.SUNKEN,width=5,height=2)
        
        self.motorSevenData = tk.Label(text="TBA", relief=tk.SUNKEN,width=5,height=2)
        
        self.motorEightData = tk.Label(text="TBA", relief=tk.SUNKEN,width=5,height=2)

        self.motorEightData = tk.Label(text="TBA", relief=tk.SUNKEN,width=5,height=2)

        self.aTitle = tk.Label(text="TBA", bg ="gray")
        self.aData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
        
        self.bTitle = tk.Label(text="TBA", bg ="gray")
        self.bData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
                
        self.currentDepthTitle = tk.Label(text="Current Depth", bg ="gray")
        self.currentDepthData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
        
        self.topDepthTitle = tk.Label(text="Starting Depth", bg ="gray")
        self.topDepthData = tk.Label(text="TBA",relief=tk.SUNKEN,width=10,height=2)
                
        self.middleDepthTitle = tk.Label(text="Bottom Depth", bg ="gray")
        self.middleDepthData = tk.Label(text="TBA",relief=tk.SUNKEN,width=10,height=2)        
        
        self.bottomDepthTitle = tk.Label(text="Bottom Depth", bg ="gray")
        self.bottomDepthData = tk.Label(text="TBA",relief=tk.SUNKEN,width=10,height=2)

        self.probeTempTitle = tk.Label(text="Probe Temperature", bg ="gray")
        self.probeData = tk.Label(text="TBA",relief=tk.SUNKEN,width=10,height=2)
        self.probeButton = tk.Button(text="top",width=7,highlightbackground="gray", background='black',command=self.probeTempValue)
                
        self.timerTitle = tk.Label(text="Timer", bg="gray",width=10)
        self.timerButton = tk.Button(text= "Start", bg="gray", width=7,highlightbackground="gray", command=self.getTime)
        self.timerData = tk.Label(text="00:00", relief=tk.SUNKEN, width=20,height=2)
        
        #depth buttons
        self.topDepthButton = tk.Button(text="top",width=7,highlightbackground="gray", background='black',command=self.topDepthValue)
        self.middleDepthButton = tk.Button(text="middle",width=7,highlightbackground="gray", command=self.middleDepthValue)
        self.bottomDepthButton = tk.Button(text="bottom",width=7,highlightbackground="gray", command=self.bottomDepthValue)
        
        #depthCanvas for depth
        self.depthCanvas = tk.Canvas(self.root, width=200, height = 400, background= "blue",bd=0,highlightthickness=1)
        self.rov = self.depthCanvas.create_rectangle(40, 20, 0, 0, outline='red', fill='black')
        self.lineTenFeet = self.depthCanvas.create_line(0,100,200,100)
    	self.lineTwentyFeet = self.depthCanvas.create_line(0,200,200,200)
        self.lineThirtyFeet = self.depthCanvas.create_line(0,300,200,300)
        self.textTenFeet = self.depthCanvas.create_text(10,110 ,text= "10")
        self.textTwentyFeet = self.depthCanvas.create_text(10,210 ,text= "20")
        self.textThirtyFeet = self.depthCanvas.create_text(10,310 ,text= "30")
        
        
        #compassCanvas
        self.compassCanvas = tk.Canvas(self.root, width=200, height = 200, background= "gray")
        self.compass = self.compassCanvas.create_oval(10, 10, 190, 190, outline='black', fill='white')
        self.compassArcNegativeTwo = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='green',extent=0)
        self.compassArcNegative = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='blue',extent=0)
        self.compassArc = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='blue',extent=0)
        self.compassArcTwo = self.compassCanvas.create_arc(10, 10, 190, 190,start=90, fill='green',extent=0)
        self.compassLineOne = self.compassCanvas.create_line(100,100,10,60, fill="red",arrow=tk.LAST, arrowshape=(70,75,3))
        self.compassLineTwo = self.compassCanvas.create_line(100,100,10,60,arrow=tk.LAST, arrowshape=(50,55,3))
        self.middle = self.compassCanvas.create_oval(95,95,105,105, outline='black', fill='white')		


        #horizonCanvas
        self.horizonCanvas = tk.Canvas(self.root, width=200, height = 400, background="gray",highlightthickness=1, bd=0)
        self.horizonLine = self.horizonCanvas.create_line(0,100,200,200, activefill="white")#delete
        self.sky = self.horizonCanvas.create_rectangle(0,0,202,200, fill="blue", outline='black')

        #motorControl
        self.motorControl = tk.Canvas(self.root, width=200, height = 200, background= "gray")
        self.hexagon = self.motorControl.create_polygon(25,75,75,25,125,25,175,75,175,135,125,185,75,185,25,135, outline='black', fill='black')
        self.V1 = self.motorControl.create_oval(40,40,60,60, outline='black', fill='white')
        self.V2 = self.motorControl.create_oval(140,40,160,60, outline='black', fill='white')		
        self.V3 = self.motorControl.create_oval(40,150,60,170, outline='black', fill='white')		
        self.V4 = self.motorControl.create_oval(140,150,160,170, outline='black', fill='white')		
        self.H1 = self.motorControl.create_polygon(50,80,80,50,90,60,60,90,50,80, outline='black', fill='white')
        self.H2 = self.motorControl.create_polygon(150,80,120,50,110,60,140,90,150,80, outline='black', fill='white')		
        self.H3 = self.motorControl.create_polygon(50,120,80,150,90,140,60,110,50,120, outline='black', fill='white')		
        self.H4 = self.motorControl.create_polygon(150,120,120,150,110,140,140,110,150,120, outline='black', fill='white')	

        #grid layout
        #left column
        self.warningTitle.grid(         column=0,  row=0)
        self.stopTitle.grid(            column=1,  row=0)
        self.voltData.grid(             column=0,  row=2,  columnspan=2)
        self.ampData.grid(              column=0,  row=4,  columnspan=2)
        self.temperatureData.grid(      column=0,  row=6,  columnspan=2)
        self.angle.grid(                column=2,  row=6,  columnspan=4)        
        self.humidityData.grid(         column=0,  row=8,  columnspan=2)
        self.temperatureDataCelcius.grid(column=0, row=10, columnspan=2)
        self.pressureData.grid(         column=0,  row=12, columnspan=2)
        self.waterSensorDataOne.grid(    column=0, row=14, columnspan=2)
        self.waterSensorDataTwo.grid(    column=0, row=16, columnspan=2)
        #motor grid
        self.motorOneData.grid(         column=2,  row=14)
        self.motorTwoData.grid(         column=3,  row=14)
        self.motorThreeData.grid(       column=4,  row=14)
        self.motorFourData.grid(        column=5,  row=14)
        self.motorFiveData.grid(        column=2,  row=16)
        self.motorSixData.grid(         column=3,  row=16)
        self.motorSevenData.grid(       column=4,  row=16)
        self.motorEightData.grid(       column=5,  row=16)

        self.aTitle.grid(               column=6,  row=13)
        self.aData.grid(                column=6,  row=14)
        self.bTitle.grid(               column=6,  row=15)
        self.bData.grid(                column=6,  row=16)
        
        self.timerTitle.grid(           column=9,   row=1)
        self.timerButton.grid(          column=10,  row=1)
        self.timerData.grid(            column=9,   row=0, columnspan=2) 
        
        self.currentDepthTitle.grid(    column=9,  row=3,  columnspan= 2)
        self.currentDepthData.grid(     column=9,  row=4,  columnspan= 2)
        self.topDepthTitle.grid(        column=9,  row=5)
        self.topDepthButton.grid(       column=10, row=5)
        self.topDepthData.grid(         column=9,  row=6)
        self.middleDepthTitle.grid(     column=9,  row=7)
        self.middleDepthButton.grid(    column=10, row=7)
        self.middleDepthData.grid(      column=9,  row=8)
        self.bottomDepthTitle.grid(     column=9,  row=9)
        self.bottomDepthButton.grid(    column=10, row=9)
        self.bottomDepthData.grid(      column=9,  row=10)
        
        #probe right side
        self.probeTempTitle.grid(       column=9,  row=11)
        self.probeButton.grid(          column=10, row=11)        
        self.probeData.grid(            column=9,  row=12)
                
    	self.depthCanvas.grid(          column=7,   row=1, columnspan=2,  rowspan=10)
    	self.horizonCanvas.grid(        column=6,   row=1,                rowspan=10)
    	self.compassCanvas.grid(        column=2,   row=1, columnspan=4,  rowspan=5)
    	self.motorControl.grid(         column=2,   row=8, columnspan=4,  rowspan=5)

    	self.update_data()
        self.root.mainloop()
    def topDepthValue(self):
    	global depthBuffer
    	self.topDepthData.configure(text=depthBuffer)
    def middleDepthValue(self):
    	global depthBuffer
    	self.middleDepthData.configure(text=depthBuffer)
    def bottomDepthValue(self):
    	global depthBuffer
    	self.bottomDepthData.configure(text=depthBuffer)
    def probeTempValue(self):
    	global probeTempBuffer
    	self.probeData.configure(text=probeTempBuffer)
    def updateClock(self):
    	now = time.time()
        global startTime
        timeElapsed = int(now) - int(startTime)
        minutes= int(timeElapsed / 60)
        if minutes < 10:
        	minutes = "0" + str(minutes)
        seconds= timeElapsed % 60
        if seconds < 10:
        	seconds = "0" +str(seconds)
        timeElapsed = str(minutes)+":"+str(seconds)
        self.timerData.configure(text=timeElapsed)
        self.root.after(1000, self.updateClock)        
    	#timer function
    def getTime(self):
    	start = time.time()
    	global startTime
    	startTime = int(start)
    	self.updateClock()
    def update_data(self):
		global w
		ser.close
		ser.open
		serr=""
		c= ""
		first = 0
		dataArray = []
		global dataArray
		data = ser.readline()
		for i in data:
			dataArray.append(i)
		print dataArray
		w = 0
		for i in range(19):
			self.dataOne(i)
		self.dataTwo()
		self.root.after(400, self.update_data)

    def dataOne(self,c):
    	head = ['A','B','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n']
    	limits = ['23','29','109','110','20','20','20','20','20','20','20','20','20','20','20','20','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2','2']
    	global color 
    	global w
    	buffers = ['tempBuffer','pressureBuffer', 'probeTempBuffer','vOneBuffer',
    	 'vTwoBuffer','vThreeBuffer','vFourBuffer','hOneBuffer','hTwoBuffer','hThreeBuffer',
		 'hFourBuffer','totalVoltBuffer','totalAmpBuffer','xAccelBuffer','yAccelBuffer',
		 'zAccelBuffer','angleBuffer','waterOne','waterTwo']
    	buf = buffers[c]
    	global buf
    	first = 0
    	buf = ""
    	for item in range(len(dataArray)):
			if first == 0:
				if dataArray[item] == head[2*c]:
					first +=1
					print item
					a = 1
					try:
						while dataArray[int(item)+a] != head[(2*c)+1]: 
							buf += dataArray[int(item)+a]
							a +=1
						print int(buf)
						if int(buf) >= int(limits[(2*c)+1]):
							color = "red"
							self.stopTitle.configure(bg = color)
							w+=1
						elif int(buf)>= int(limits[(2*c)]):
							color = "yellow"
							self.warningTitle.configure(bg = color)
							w+=1
						else: 
							color = "white"
							if w == 0:
								self.warningTitle.configure(bg = "gray")
								self.stopTitle.configure(bg = "gray")
					except: 
						print "bad AB"
					if c == 0:
						self.temperatureData.configure(text=buf,bg = color)
					elif c == 1:
						self.pressureData.configure(text=buf, bg = color)
					elif c == 2:
						self.temperatureDataCelcius.configure(text=buf, bg = color)
						global probeTempBuffer
						probeTempBuffer = buf
					elif c == 3:
						self.motorOneData.configure(text=buf, bg = color)
						if int(buf)>10:
							self.motorControl.itemconfigure(self.V1, fill='green')
							self.compassCanvas.update()
					elif c == 4:
						self.motorTwoData.configure(text=buf, bg = color)
						if int(buf)>10:
							self.motorControl.itemconfigure(self.V2, fill='green')
							self.compassCanvas.update()
					elif c == 5:
						self.motorThreeData.configure(text=buf, bg = color)
						if int(buf)>10:
							self.motorControl.itemconfigure(self.V3, fill='green')
							self.compassCanvas.update()
					elif c == 6:
						self.motorFourData.configure(text=buf, bg = color)
						if int(buf)>10:
							self.motorControl.itemconfigure(self.V4, fill='green')
							self.compassCanvas.update()
					elif c == 7:
						self.motorFiveData.configure(text=buf, bg = color)
						if int(buf)>10:
							self.motorControl.itemconfigure(self.H1, fill='green')
							self.compassCanvas.update()
					elif c == 8:
						self.motorSixData.configure(text=buf, bg = color)
						if int(buf)>10:
							self.motorControl.itemconfigure(self.H2, fill='green')
							self.compassCanvas.update()
					elif c == 9:
						self.motorSevenData.configure(text=buf, bg = color)
						if int(buf)>10:
							self.motorControl.itemconfigure(self.H3, fill='green')
							self.compassCanvas.update()
					elif c == 10:
						self.motorEightData.configure(text=buf, bg = color)
						if int(buf)>10:
							self.motorControl.itemconfigure(self.H4, fill='green')
							self.compassCanvas.update()
					elif c == 11:
						self.voltData.configure(text=buf, bg = color)
					elif c == 12:
						self.ampData.configure(text=buf, bg = color)
					elif c == 13:
						self.aData.configure(text=buf, bg = color)
					elif c == 14:
						self.bData.configure(text=buf, bg = color)
					elif c == 15:
						self.humidityData.configure(text=buf, bg = color)
					elif c == 16:
						self.angle.configure(text=buf, bg = color)
						self.compassData(buf)
					elif c == 17:
						self.waterSensorDataOne.configure(text=buf, bg = color)
					elif c == 18:
						self.waterSensorDataTwo.configure(text=buf, bg = color)
    def dataTwo(self):
    	global color 
    	global depthBuffer 
    	first = 0
    	depthBuffer = ""		
    	for item in range(len(dataArray)):
			if first == 0:
				if dataArray[item] == 'C':
					first +=1
					print item
					a = 1
					try:
						while dataArray[int(item)+a] != 'D': 
							depthBuffer += dataArray[int(item)+a]
							a +=1
						print int(depthBuffer)
						if int(depthBuffer) >= 71:
							color = "red"
							self.stopTitle.configure(bg = color)
						elif int(depthBuffer)>=71:
							color = "yellow"
							self.warningTitle.configure(bg = color)
						else: 
							color = "white"
							#self.warningStop()
					except: 
						print "bad CD"
					self.currentDepthData.configure(text=depthBuffer,bg = color)
    def dataThree(self,dataArray):
    	global color 
    	global pressureBuffer 
    	first = 0
    	pressureBuffer = ""		
    	for item in range(len(dataArray)):
			if first == 0:
				if dataArray[item] == 'E':
					first +=1
					print item
					a = 1
					try:
						while dataArray[int(item)+a] != 'F': 
							pressureBuffer += dataArray[int(item)+a]
							a +=1
						print int(pressureBuffer)
						if int(pressureBuffer) >= 71:
							color = "red"
							self.stopTitle.configure(bg = color)
						elif int(pressureBuffer)>=71:
							color = "yellow"
							self.warningTitle.configure(bg = color)
						else: 
							color = "white"
							#self.warningStop()
					except: 
						print "bad EF"
					self.pressureData.configure(text=pressureBuffer,bg = color)
    def dataThree(self,dataArray):
    	global color 
    	global vOneBuffer 
    	first = 0
    	vOneBuffer = ""		
    	for item in range(len(dataArray)):
			if first == 0:
				if dataArray[item] == 'G':
					first +=1
					print item
					a = 1
					try:
						while dataArray[int(item)+a] != 'H': 
							vOneBuffer += dataArray[int(item)+a]
							a +=1
						print int(vOneBuffer)
						if int(vOneBuffer) >= 71:
							color = "red"
							self.stopTitle.configure(bg = color)
						elif int(vOneBuffer)>=71:
							color = "yellow"
							self.warningTitle.configure(bg = color)
						else: 
							color = "white"
							#self.warningStop()
					except: 
						print "bad EF"
					self.motorOneData.configure(text=vOneBuffer,bg = color)
    def compassData(self,angle):
			print angle
			pi = int(angle)
			angleBuffer = "" # empty the buffer
			if pi <360 and pi>0:
				r = pi % 360
				num = math.radians(pi)
				num2 = math.radians(pi+180)
				x =(100-math.sin(num)*70)
				y= (100-math.cos(num)*70)
				x2=(100-math.sin(num2)*50)
				y2= (100-math.cos(num2)*50)
				self.compassCanvas.coords(self.compassLineOne, 100,100,x,y)
				self.compassCanvas.coords(self.compassLineTwo, 100,100,x2,y2)
				self.compassCanvas.itemconfigure(self.compassArc, extent=r)
				#self.compassCanvas.itemconfigure(self.compassArcTwo, extent=e)
				#self.compassCanvas.itemconfigure(self.compassArcNegative, extent=b)
				#self.compassCanvas.itemconfigure(self.compassArcNegativeTwo, extent=g)
				self.compassCanvas.update()
			if pi == 0:
				e = 0
			elif pi >= 360:
				e = pi % 360
			elif pi >= -360 and pi<0:
				b = 360- (-pi % 360)
			elif pi < -360:
				g = 360 - (-pi % 360)


		

app=App()