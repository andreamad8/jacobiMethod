import json
import os
import numpy as np
import scipy as sp
import pandas as pd
from pandas.tools.plotting import scatter_matrix
import seaborn as sns
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid.inset_locator import inset_axes
import ast

def import_data(filename):
    fo = open(filename, "r")
    data=[]
    for line in fo:
        data.append(ast.literal_eval(str(line[:-1])))
    fo.close()
    return data
data_array=[]
names=[]
for filename in os.listdir('ris/'):
    if filename[0] != '.' and filename[0] != 'f' and filename[0] != 'S':
        names.append(filename)
        data_array.append(import_data('ris/'+filename))

a=[]
c=[]
for i in range(1,8):
    print names[-i]
    a.append(data_array[-i][0]['Ax-b'])
    c.append(data_array[-i][0]['Conv'])

a.reverse()
c.reverse()
j=0
for name,val in zip(names,data_array):
    if len(val)>1:
        print name
        for e in val:
            print e['Ax-b'],a[j],e['Conv'],c[j]
        break
        j+=1
