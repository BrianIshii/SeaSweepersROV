# for python 3.x use 'tkinter' rather than 'Tkinter'
import Tkinter as tk
import time
import math
from serial import *

serialPort = "/dev/cu.usbmodemFD121"
baudRate = 4800
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
        self.warningTitle = tk.Label(text="WARNING", bg="gray", width=10)
        self.stopTitle = tk.Label(text="STOP", bg="gray", width=10)#needgrid
        self.title = tk.Label(text="Sea Sweepers", bg="gray")#needgrid
        
        self.voltData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20, height=2)
        
        self.ampData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
        
        self.temperatureData = tk.Label(text="TBA",relief=tk.SUNKEN,width=20,height=2)
        
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


        #grid layout
        #left column
        self.warningTitle.grid(         column=0,  row=0)
        self.stopTitle.grid(            column=1,  row=0)
        self.voltData.grid(             column=0,  row=2,  columnspan=2)
        self.ampData.grid(              column=0,  row=4,  columnspan=2)
        self.temperatureData.grid(      column=0,  row=6,  columnspan=2)
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
        self.bottomDepthTitle.grid(     column=9, row=9)
        self.bottomDepthButton.grid(    column=10, row=9)
        self.bottomDepthData.grid(      column=9, row=10)
        
    	self.depthCanvas.grid(          column=7,   row=1, columnspan=2,  rowspan=10)
    	self.horizonCanvas.grid(        column=6,   row=1,                rowspan=10)
    	self.compassCanvas.grid(        column=2,   row=1, columnspan=4,  rowspan=5)
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
		ser.close
		ser.open
		c = ""
		serr = ""
		first = 0
		dataArray = []
		global dataArray
		data = ser.readline()
		for i in data:
			dataArray.append(i)
		print dataArray
		
		self.dataOne(dataArray)
		self.dataTwo(dataArray)
		self.root.after(400, self.update_data)

    def dataOne(self,dataArray):
    	global color
    	global tempBuffer
    	first = 0
    	for item in range(len(dataArray)):
			if first == 0:
				if dataArray[item] == 'A':
					first +=1
					print item
					a = 1
					try:
						while dataArray[int(item)+a] != 'B': 
							tempBuffer += dataArray[int(item)+a]
							a +=1
						print int(tempBuffer)
						if int(tempBuffer) >= 73:
							color = "red"
							self.stopTitle.configure(bg = color)
						elif int(tempBuffer)>=71:
							color = "yellow"
							self.warningTitle.configure(bg = color)
						else: 
							color = "white"
							self.warningTitle.configure(bg ="gray")
							self.stopTitle.configure(bg = "gray")
					except: 
						print "bad AB"
					self.temperatureData.configure(text=tempBuffer,bg = color)
					tempBuffer = ""
    def dataTwo(self,dataArray):
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
							self.warningTitle.configure(bg ="gray")
							self.stopTitle.configure(bg = "gray")
					except: 
						print "bad CD"
					self.currentDepthData.configure(text=depthBuffer,bg = color)
    def data(self,c):
		global serBuffer
		global tempBuffer
		global depthBuffer 
		global pressureBuffer 
		global probeTempBuffer 
		global vOneBuffer
		global vTwoBuffer 
		global vThreeBuffer
		global vFourBuffer
		global hOneBuffer 
		global hTwoBuffer
		global hThreeBuffer 
		global hFourBuffer
		global totalVoltBuffer 
		global totalAmpBuffer
		global xAccelBuffer
		global yAccelBuffer
		global zAccelBuffer 
		global angleBuffer
		global dataArray
		global r
		global e
		global g
		global b
		global waterOne
		global waterTwo
		if c.find('A')!=-1:
			head = c.find('A')
			tail = c.find('B')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				tempBuffer += c[head+i+1]
			if tempBuffer != "":
				self.temperatureData.configure(text=tempBuffer)
				tempBuffer = ""		
			else:
				tempBuffer = ""
		if c.find('C') !=-1:
			head = c.find('C')
			tail = c.find('D')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				depthBuffer += c[head+i+1]
			if depthBuffer !="":
				self.currentDepthData.configure(text=depthBuffer)
				depthBuffer = ""
			else:
				depthBuffer = ""	
		if c.find('E')!=-1:
			head = c.find('E')
			tail = c.find('F')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				pressureBuffer += c[head+i+1]
			if pressureBuffer !="":
				self.pressureData.configure(text=pressureBuffer)
				pressureBuffer = ""
			else:
				pressurebuffer = ""
		if c.find('G')!=-1:
			head = c.find('G')
			tail = c.find('H')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				probeTempBuffer += c[head+i+1]
			if probeTempBuffer !="":
				self.temperatureDataCelcius.configure(text=probeTempBuffer)
				probeTempBuffer = ""	
			else:
				probetempBuffer = ""	
		if c.find('I')!=-1:
			head = c.find('I')
			tail = c.find('J')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				vOneBuffer += c[head+i+1]
			if vOneBuffer != "":
				self.motorOneData.configure(text=vOneBuffer)
				vOneBuffer = ""	
			else:
				vOneBuffer = ""
		if c.find('K')!=-1:
			head = c.find('K')
			tail = c.find('L')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				vTwoBuffer += c[head+i+1]
			if vTwoBuffer !="":
				self.motorTwoData.configure(text=vTwoBuffer)
				vTwoBuffer = ""	
			else:
				vTwoBuffer = ""
		if c.find('M')!=-1:
			head = c.find('M')
			tail = c.find('N')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				vThreeBuffer += c[head+i+1]
			if vThreeBuffer !="":
				self.motorThreeData.configure(text=vThreeBuffer)
				vThreeBuffer = ""	
			else:
				vThreeBuffer = ""
		if c.find('O')!=-1:
			head = c.find('O')
			tail = c.find('P')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				vFourBuffer += c[head+i+1]
			if vFourBuffer !="":
				self.motorFourData.configure(text=vFourBuffer)
				vFourBuffer = ""
			else:
				vFourBuffer = ""	
		if c.find('Q')!=-1:
			head = c.find('Q')
			tail = c.find('R')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				hOneBuffer += c[head+i+1]
			if hOneBuffer !="":
				self.motorFiveData.configure(text=hOneBuffer)
				hOneBuffer = ""
			else:
				hOneBuffer = ""
		if c.find('S')!=-1:
			head = c.find('S')
			tail = c.find('T')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				hTwoBuffer += c[head+i+1]
			if hTwoBuffer !="":
				self.motorSixData.configure(text=hTwoBuffer)
				hTwoBuffer = ""
			else:
				hTwoBuffer = ""
		if c.find('U')!=-1:
			head = c.find('U')
			tail = c.find('V')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				hThreeBuffer += c[head+i+1]
			if hThreeBuffer!="":
				self.motorSevenData.configure(text=hThreeBuffer)
				hThreeBuffer = ""
			else:
				hThreeBuffer = ""
		if c.find('W')!=-1:
			head = c.find('W')
			tail = c.find('X')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				hFourBuffer += c[head+i+1]
			if hFourBuffer !="":
				self.motorEightData.configure(text=hFourBuffer)
				hFourBuffer = ""
			else:
				hFourBuffer = ""
		if c.find('Y')!=-1:
			head = c.find('Y')
			tail = c.find('Z')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				totalVoltBuffer += c[head+i+1]
			if	totalVoltBuffer !="":
				self.voltData.configure(text=totalVoltBuffer)
				totalVoltBuffer = ""
			else:
				totalVoltBuffer = ""
		if c.find('a')!=-1:
			head = c.find('a')
			tail = c.find('b')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				totalAmpBuffer += c[head+i+1]
			if totalAmpBuffer !="":
				self.ampData.configure(text=totalAmpBuffer)
				totalAmpBuffer = ""
			else:
				totalAmpbuffer = ""
		if c.find('c')!=-1:
			head = c.find('c')
			tail = c.find('d')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				xAccelBuffer += c[head+i+1]
			if xAccelBuffer != "":
				self.aData.configure(text=xAccelBuffer)
				xAccelBuffer = ""
			else:
				xAccelBuffer = ""
		if c.find('e')!=-1:
			head = c.find('e')
			tail = c.find('f')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				yAccelBuffer += c[head+i+1]
			if yAccelBuffer !="":
				self.bData.configure(text=yAccelBuffer)
				yAccelBuffer = ""
			else:
				yAccelBuffer = ""
		if c.find('g')!=-1:
			head = c.find('g')
			tail = c.find('h')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				zAccelBuffer += c[head+i+1]
			if zAccelBuffer !="":
				self.humidityData.configure(text=zAccelBuffer)
				zAccelBuffer = ""
			else:
				zAccelBuffer = ""
		if c.find('i')!=-1:
			pi = 0
			head = c.find('i')
			tail = c.find('j')
			diff = tail - head
			#print diff
			ggg = ""
			#for i in range(diff-1):
				#gleBuffer += c[head+i+1].rstrip('\r\n')
			#print angleBuffer
			#pi = angleBuffer
			pi = float(30.0) + float(9.0)
			pi = int(pi)
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
		if c.find('k')!=-1:
			head = c.find('k')
			tail = c.find('l')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				waterOne += c[head+i+1]
			if waterOne !="":
				self.waterSensorDataOne.configure(text=waterOne)
				waterOne = ""
			else:
				waterOne= ""
		if c.find('m')!=-1:
			head = c.find('m')
			tail = c.find('n')
			diff = tail - head
			#print diff
			for i in range(diff-1):
				waterTwo += c[head+i+1]
			if waterTwo !="":
				self.waterSensorDataTwo.configure(text=waterTwo)
				waterTwo = ""
			else:
				waterTwo = ""
		self.root.after(400, self.update_data)

app=App()