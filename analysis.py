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
#print ser
#print len(data_array),len(data_ser)

final=[]
i = 0
for val,valff in zip(data_par,data_For):
    x=[]
    Tc=[]
    y=[]
    for e in val:
        x.append(e['thread_num'])
        avg= reduce(lambda x, y: float(x) + float(y),e['Tc']) / float(len(e['Tc']))
        Tc.append(avg)
    ffx=[]
    ffTc=[]
    ffy=[]
    for e in valff:
        ffx.append(e['thread_num'])
        avg= reduce(lambda x, y: float(x) + float(y),e['Tc']) / float(len(e['Tc']))
        ffTc.append(avg)

    final.append([label[i],ser[i],Tc[0],x[np.argmin(Tc)],Tc[np.argmin(Tc)],ffTc[0],ffx[np.argmin(ffTc)],ffTc[np.argmin(ffTc)]])
    #print 'optThread '+str(((((ser[i]/51)-serNorm[i])/label[i])/serNorm[i])*100)
    #print str(label[i])+' optThread '+str(((Tc[0]/51-val[0]['Tnorm'])/label[i])/val[0]['Tnorm'])

    i += 1
for e in sorted(final):
    print '%d\t%.4f\t%.4f\t%d\t%.4f\t%.2f\t%.4f\t%d\t%.4f\t%.2f'%(e[0],e[1],e[2],e[3],e[4],e[1]/e[4],e[5],e[6],e[7],e[1]/e[7])
    #print e[0],'\t',e[1],'\t',e[2],'\t',e[3],'\t',e[4],'\t',e[1]/e[4],'\t',e[5],'\t',e[6],'\t',e[7],'\t',e[1]/e[7]
