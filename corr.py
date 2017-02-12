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
SUBPLOT_x=SUBPLOT_y=69 ## 5 for xeon and 60 mic
label=[10000,1000,15000,30000,5000]
#label=[10000,1000,15000,30000,35000,40000,5000]
lw=1.2 #line weight
ms=6.5 #markersize
ty='ParPHI_' ### ParXEON,ForXEON,ParPHI_,ForPHI_
selector_s='serPHI_' ### serXEON,serPHI_

def import_data(filename):
    fo = open(filename, "r")
    data=[]
    for line in fo:
        data.append(ast.literal_eval(str(line[:-1])))
    fo.close()
    return data

data_array=[]
data_ser=[]
for filename in os.listdir('ris/'):
    if(filename[:7] == ty):
        print filename
        data_array.append(import_data('ris/'+filename))
    elif(filename[:7] == selector_s):
        print filename
        data_ser.append(import_data('ris/'+filename))

plt.rc('text', usetex=True)
plt.rc('font', family='Times-Roman')
sns.set_style(style='white')
fig = plt.figure(figsize=(13,10))

ax1 = fig.add_subplot(1,1, 1)
inset1 = inset_axes(ax1, width="60%", height="60%")
marker = ['*','o', 'v', 's', 'D', '8', '^','*','o', 'v', 's', 'D', '8', '^']
i=0
for val in data_array:
    x=[]
    y=[]
    z=[]
    #del val[0]
    for e in val:
        x.append(e['thread_num'])
        avg= reduce(lambda x, y: float(x) + float(y),e['Tc']) / float(len(e['Tc']))
        y.append(((avg/50)-(e['Tnorm']))-float(e['Tnorm']))

    ax1.plot(x,y,label=str(label[i]), linewidth=lw, alpha=0.8, marker=marker[i], markersize=ms)
    inset1.plot(x, y, marker=marker[i],linewidth=lw, alpha=0.8, markersize=ms)
    inset1.set_ylim([0, 1])
    inset1.set_xlim([0, 40])
    ax1.set_xlabel(r'$\textit{Threads}$')
    ax1.set_ylabel(r'$\frac{T_{jac}}{p}-(T_{norm}+T_{swap})$')
    i+=1
ax1.set_title(r'Threads version')
#ax1.set_title(r'FastFlow version')
ax1.legend(loc='lower center', bbox_to_anchor=(0.2, 0.6),
               bbox_transform=plt.gcf().transFigure)
plt.savefig(ty+'Corr.eps', format='eps', dpi=300)
plt.show()
