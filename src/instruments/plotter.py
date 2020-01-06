from matplotlib import pyplot as plt 
import numpy as np 

x = read_txt("x.txt")
tbl = read_txt("table.txt")
plt.plot(x[0:2500], 'k.')
plt.plot(tbl[0:2500], 'b-')
plt.title("Vector generado al recorrer la tabla")
plt.legend(["Muestras Tomadas", "Tabla Original"], loc='upper right')
plt.autoscale(enable=True, axis='x', tight=True)
plt.show()

def read_txt(filename):
    text = open(filename, 'r')
    text = text.readlines()
    values = []
    for l in text:
        values.append(float(l))
    return np.array(values)