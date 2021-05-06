# -*- coding: utf-8 -*-
"""
Created on Thu Oct 22 10:29 2020

@author: Elkin G.

Codigo ejemplo organizado 
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
    time.sleep(1.0) # Esperamos un tiempo para empezar a recibir los datos 
    serialConnection.reset_input_buffer() # Reseteamos el buffer de entrada de los datos

    while (isRun): # Leemos los datos 
        global isReceiving
        global value
        value = float(serialConnection.readline().strip()) # Leemos los datos del sensor
        isReceiving = True


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
    data.append(value) # Guardamos el datos de la lectura en la ultima posicion del vetor 
    lines.set_data(range(Samples),data) # Ploteamos una nueva linea
    rpm.set('RPM: '+str(value))


def motorControl(int):
    pwmValue = str(slider.get())
    pwm.set("PWM: "+pwmValue)
    serialConnection.write((pwmValue+'\n').encode())


# Variables 
serialPort = 'COM3' # Definimos el puerto serial del arduino 
baudRate = 9600 # Baudios 
isReceiving = False # Bandera de inicio de recepcion de los datos 
isRun = True # Bandera para recibir datos 
value = 0.0 # Datos del sensor 

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
data = collections.deque([0]*samples,maxlen=samples) # Vector de muestras 
sampleTime = 100 # Periodo de muestreo 

#Limites de los ejes de la grafica
xmin = 0
xmax = samples
ymin = 0
ymax = 200

fig = plt.figure(facecolor='0.94') # Creamos una nueva figura 
ax = plt.axes(xlim=(xmin,xmax),ylim=(ymin,ymax))
plt.title('Real-Time RPM reading') # Titulo de la grafica
ax.set_xlabel('Samples')
ax.set_ylabel('RPM')

lines = ax.plot([],[])[0] # Grafica de los datos iniciales

# Comienza la interfaz grafica 
root = tk.Tk()
root.protocol('WM_DELETE_WINDOW',askQuit)

canvas = FigureCanvasTkAgg(fig,master=root)
canvas._tkcanvas.grid(row=0)

frame = tk.Frame(root,width=400,height=200)
frame.grid(row=0,column=1,padx=10,pady=5)
frame.grid_propagate(False) # Congela el tamaño del Frame

pwm = tk.StringVar(root,'PWM: 0.00')

labelPwm = tk.Label(frame,textvariable=pwm)
labelPwm.grid(row=0,column=0,padx=20,pady=20)

rpm = tk.StringVar(root,'RPM: 0.00')

labelRpm = tk.Label(frame,textvariable=rpm)
labelRpm.grid(row=0,column=1,padx=20,pady=20)

slider = tk.Scale(frame,from_=0, to=255,orient=tk.HORIZONTAL,command=motorControl,length=200)
slider.grid(row=1,padx=20,pady=20)

anim = animation.FuncAnimation(fig,plotData,fargs=(samples,lines),interval=sampleTime)

root.geometry('1100x600')
root.mainloop()

