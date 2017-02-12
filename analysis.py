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

#sns.palplot()
sns.color_palette("Set1", n_colors=8, desat=.5)
PROC=255 ## 17 for xeon and 255 mic
SUBPLOT_x=SUBPLOT_y=60 ## 5 for xeon and 60 mic
label=[10000,1000,15000,30000,5000]
#label=[10000,1000,15000,30000,35000,40000,5000]
lw=1.2 #line weight
ms=6.5 #markersize
ty='ForPHI_' ### ParXEON,ForXEON,ParPHI_,ForPHI_
tP='ParPHI_' ### ParXEON,ForXEON,ParPHI_,ForPHI_

selector_s='serPHI_' ### serXEON,serPHI_

def import_data(filename):
    fo = open(filename, "r")
    data=[]
    for line in fo:
        data.append(ast.literal_eval(str(line[:-1])))
    fo.close()
    return data

data_par=[]
data_For=[]
data_ser=[]
for filename in os.listdir('ris/'):
    if(filename[:7] == tP):
        print filename
        data_par.append(import_data('ris/'+filename))
    if(filename[:7] == ty):
        print filename
        data_For.append(import_data('ris/'+filename))
    elif(filename[:7] == selector_s):
        print filename
        data_ser.append(import_data('ris/'+filename))


ser =[sum(e[0]['Tc'])/float(len(e[0]['Tc'])) for e in data_ser]
serNorm =[e[0]['Tnorm'] for e in data_ser]
print ser
#print len(data_array),len(data_ser)

seq_time=[]
i = 0
for val in data_par:
    x=[]
    Tc=[]
    y=[]
    for e in val:
        x.append(e['thread_num'])
        avg= reduce(lambda x, y: float(x) + float(y),e['Tc']) / float(len(e['Tc']))
        Tc.append(avg)
    print Tc[np.argmin(Tc)],x[np.argmin(Tc)]

    #sprint ser[i],Tc[0],((ser[i]/50)-val[0]['Tnorm'])/val[0]['Tnorm']
    i += 1
