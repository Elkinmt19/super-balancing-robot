# -*- coding: utf-8 -*-
"""
Created on Thu Oct 22 10:50 2020

@author: Elkin G.

Plantilla try 1 
"""

import serial 
import time 
import collections
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.animation as animation 
import tkinter as tk
from threading import Thread

def getData():
    dataFile = open(namef,'a')
    time.sleep(1.0) # Esperamos un tiempo para empezar a recibir los datos 
    serialConnection.reset_input_buffer() # Reseteamos el buffer de entrada de los datos

    while (isRun): # Leemos los datos 
        global isReceiving
        global value
        global value_setPoint
        try:
            value = float(serialConnection.readline().strip()) # Leemos los datos del sensor
        except:
            print('Hubo un error')
        isReceiving = True
        if isRecording:
            dataFile.write('['+ str(value_setPoint)+','+str(value)+']\n')
    if (isRecording):
        dataFile.close()
                
def askQuit():
    global isRun
    isRun = False
    thread.join()
    serialConnection.write(('0\n').encode())
    serialConnection.close() # Cerramos el puerto serial 
    root.quit()
    root.destroy()

def plotData(self,Samples,lines):
    global value
    global value_setPoint
    data.append(value) # Guardamos el datos de la lectura en la ultima posicion del vetor 
    lines.set_data(range(Samples),data) # Ploteamos una nueva linea
    data_setPoint.append(float(value_setPoint)) # Guardamos el datos del setpoint en la ultima posicion del vetor 
    lines_setPoint.set_data(range(Samples),data_setPoint) # Ploteamos una nueva linea
    rpm.set('VARIABLE: '+str(value))

def recordData():
    global isRecording
    global namef
    namef = name_file.get()
    if reco.get() == 'Not Recording':
        isRecording = True
        reco.set('Recording')
    else:
        isRecording = False
        reco.set('Not Recording')
    

def motorControl():
    global value_setPoint
    try:
        value_setPoint = setPoint.get()
        pwmValue = str(int((float(setPoint.get())*255)/100))
        print(pwmValue)
        pwm.set("SETPOINT: "+setPoint.get())
        serialConnection.write((pwmValue+'\n').encode())
    except:
        print('Valor de setpoint no valido')


# Variables 
serialPort = 'COM3' # Definimos el puerto serial del arduino 
baudRate = 115200 # Baudios 
isReceiving = False # Bandera de inicio de recepcion de los datos 
isRun = True # Bandera para recibir datos 
isRecording = False # Bandera para guardar los datos
value = 0.0 # Datos del sensor 
value_setPoint = 0.0 # Datos del setpoint
namef = 'MotorPIDFULL.txt' # Nombre del archivo para guardar los datos


try:
    serialConnection = serial.Serial(serialPort,baudRate) # Creamos instancia para la comunicacion serial 
except:
    print('No se ha podido encontrar el puerto de comunicacion')

thread = Thread(target=getData) # Creamos primera thread asociada a la adquisicion de los datos
thread.start() # Iniciamos la thread anteriormente creada

while isReceiving != True:
    print('Iniciando recepcion de los datos')
    time.sleep(0.1)

samples = 100 # Muestras
data = collections.deque([0]*samples,maxlen=samples) # Vector de muestras para la variable medida
data_setPoint = collections.deque([0]*samples,maxlen=samples) # Vector de muestras para el setpoint
sampleTime = 100 # Periodo de muestreo 

#Limites de los ejes de la grafica
xmin = 0
xmax = samples
ymin = -7
ymax = 7

fig = plt.figure(facecolor='0.94') # Creamos una nueva figura 
ax = plt.axes(xlim=(xmin,xmax),ylim=(ymin,ymax))
plt.title('Real-Time Pot Voltage reading') # Titulo de la grafica
ax.set_xlabel('Samples')
ax.set_ylabel('Variable')

lines = ax.plot([],[])[0] # Grafica de los datos iniciales
lines_setPoint = ax.plot([],[])[0] # Grafica de los datos iniciales

# Comienza la interfaz grafica 
root = tk.Tk()
root.protocol('WM_DELETE_WINDOW',askQuit) 
stop = tk.Button(root,text='STOP',command=askQuit)
stop.grid(row=0,column=1,padx=5,pady=5)

canvas = FigureCanvasTkAgg(fig,master=root)
canvas._tkcanvas.grid(row=0)

frame = tk.Frame(root,width=400,height=200)
frame.grid(row=0,column=2,padx=10,pady=5)
frame.grid_propagate(False) # Congela el tamaño del Frame

# Label asociada al setpoint
pwm = tk.StringVar(root,'SETPOINT: 0.00')

labelPwm = tk.Label(frame,textvariable=pwm)
labelPwm.grid(row=0,column=0,padx=5,pady=5)

# Label asociada a la variable medida
rpm = tk.StringVar(root,'VARIABLE: 0.00')

labelRpm = tk.Label(frame,textvariable=rpm)
labelRpm.grid(row=0,column=1,padx=5,pady=5)

# Input asociada a la asignacion del setpoint por parte del usuario 
setPoint = tk.Entry(root)
setPoint.insert(0,'0')
setPoint.grid(row=1,column=1,padx=5,pady=5)
acep_setpoint = tk.Button(root,text='OK',command=motorControl)
acep_setpoint.grid(row=0,column=2,padx=5,pady=5)

# Input asociada al nombre del archivo de texto para guardar los datos 
name_file = tk.Entry(root)
name_file.insert(0,'data.txt')
name_file.grid(row=2,column=1,padx=5,pady=5)
entry_nameFile = tk.Button(root,text='RECORD',command=recordData)
entry_nameFile.grid(row=2,column=2,padx=5,pady=5)

# Label asociada al guardado de los datos 
reco = tk.StringVar(root,'Not Recording')
labelReco = tk.Label(frame,textvariable=reco)
labelReco.grid(row=2,column=3,padx=5,pady=5)

anim = animation.FuncAnimation(fig,plotData,fargs=(samples,lines),interval=sampleTime)

root.geometry('1100x600')
root.mainloop()

