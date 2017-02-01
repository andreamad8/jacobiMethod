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
import re

PROC=80 ## 17 for xeon and 90 mic
SUBPLOT=30 ## 7 for xeon and 40 mic
lw=1.2 #line weight
ms=3.5 #markersize
ty='ForPHI_' ### ParXEON,ForXEON,ParPHI_,ForPHI_
selector_s='serXEON' ### serXEON,serPHI_

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
        data_ser.append(import_data('ris/'+filename))


ser =[e[0]['Tc'][0] for e in data_ser]

print len(data_array),len(data_ser)

label=[10000,1000,100,5000,500]


plt.rc('text', usetex=True)
plt.rc('font', family='Times-Roman')
sns.set_style(style='white')
fig = plt.figure()

marker = ['o', 'v', 's', 'D', '8', '^','o', 'v', 's', 'D', '8', '^']
ax1 = fig.add_subplot(2, 2, 1)
ax2 = fig.add_subplot(2, 2, 2)
ax3 = fig.add_subplot(2, 2, 3)
ax4 = fig.add_subplot(2, 2, 4)
inset1 = inset_axes(ax1, width="60%", height="60%")
inset2 = inset_axes(ax2, width="40%", height="40%", loc=2)
inset3 = inset_axes(ax3, width="40%", height="40%", loc=2)
x_ideal = np.linspace(1, PROC, 100)
'''
Completion Time
'''
seq_time=[]
i = 0
for val in data_array:
    x=[]
    y=[]
    for e in val:
        x.append(e['thread_num'])
        avg= reduce(lambda x, y: float(x) + float(y),e['Tc']) / float(len(e['Tc']))
        y.append(avg)
    ax1.plot(x,y, label=str(label[i]), linewidth=lw, alpha=0.8, marker=marker[i], markersize=ms)
    inset1.plot(x, y, marker=marker[i],linewidth=lw, alpha=0.8, markersize=ms)
    inset1.set_ylim([0, 1])
    inset1.set_xlim([0, SUBPLOT])
    inset1.yaxis.grid(True)
    # inset_axes.set_yscale('log')
    plt.legend(fontsize = 'large',loc='opt')
    #ax1.set_xticks(np.arange(min(x) - 1, max(x) + 1, 5.0))
    ax1.set_xlabel(r'$\textit{Threads}$')
    ax1.set_ylabel(r'$\textit{Seconds}$')
    # ax1.xaxis.grid(True)
    ax1.yaxis.grid(True)
    i += 1

'''
Scalability Plot
'''
i = 0
for val in data_array:
    x=[]
    y1=[]
    for e in val:
        x.append(e['thread_num'])
        avg= reduce(lambda x, y: float(x) + float(y),e['Tc']) / float(len(e['Tc']))
        y1.append(avg)

    y = [y1[0] / float(e) for e in y1]
    ax2.plot(x,y, label=str(label[i]), linewidth=lw,alpha=0.8, marker=marker[i], markersize=ms)
    ax2.plot(x_ideal, x_ideal, label='Opt', linewidth=1.95, alpha=0.2, color='gray', linestyle='dashed')
    inset2.plot(x,y, marker=marker[i],linewidth=lw, alpha=0.8, markersize=ms)
    inset2.plot(x_ideal, x_ideal, linewidth=1.95, alpha=0.4, color='gray', linestyle='dashed')
    inset2.set_ylim([0, 30])
    inset2.set_xlim([0, SUBPLOT])
    inset2.yaxis.grid(True)
    inset2.yaxis.tick_right()
    #plt.legend(loc='left center', bbox_to_anchor=(0.46, -0.08),
    #            fancybox=True,
    #            shadow=True, ncol=5)
    #ax2.set_xticks(np.arange(min(x) - 1, max(x) + 1, 5.0))
    ax2.set_xlabel(r'$\textit{Threads}$')
    ax2.set_ylabel(r'$\textit{Scalability}$')
    # ax2.xaxis.grid(True)
    ax2.yaxis.grid(True)
    i += 1
'''
Speed-up Plot
'''

i = 0
for val in data_array:
    x=[]
    y1=[]
    for e in val:
        x.append(e['thread_num'])
        avg= reduce(lambda x, y: float(x) + float(y),e['Tc']) / float(len(e['Tc']))
        y1.append(avg)

    #y=[(float(ser[i])/e)for e in y1]
    y = [y1[0] / float(e) for e in y1]

    ax3.plot(x, y, label=str(label[i]), linewidth=lw,alpha=0.8, marker=marker[i], markersize=ms)
    ax3.plot(x_ideal, x_ideal, linewidth=1.95, alpha=0.2,color='gray', linestyle='dashed')
    inset3.plot(x, y, marker=marker[i],linewidth=lw, alpha=0.8, markersize=ms)
    inset3.plot(x_ideal, x_ideal, linewidth=1.95, alpha=0.4,color='gray', linestyle='dashed')
    inset3.set_ylim([0, 30])
    inset3.set_xlim([0, SUBPLOT])
    inset3.yaxis.grid(True)
    inset3.yaxis.tick_right()
    # plt.legend(fontsize = 'large',loc='opt')
    #ax3.set_xticks(np.arange(min(x) - 1, max(x) + 1, 5.0))
    ax3.set_xlabel(r'$\textit{Threads}$')
    ax3.set_ylabel(r'$\textit{Speedup}$')
    # ax3.xaxis.grid(True)
    ax3.yaxis.grid(True)
    i += 1


'''
#Efficienty Plot
#'''
i = 0
for val in data_array:
    x=[]
    y1=[]
    for e in val:
        x.append(e['thread_num'])
        avg= reduce(lambda x, y: float(x) + float(y),e['Tc']) / float(len(e['Tc']))
        y1.append(avg)

    #y2=[(float(ser[i])/e)for e in y1]
    y2=[y1[0] / float(e) for e in y1]

    thnume=[e['thread_num'] for e in val]
    y=[]
    for j in range(0,len(y2)):
        y.append(y2[j]/float(thnume[j]))

    ax4.plot(x, y, label=str(label[i]), linewidth=lw, alpha=0.8, marker=marker[i], markersize=ms)
    x_new = np.linspace(0, PROC, 100)
    y_new = [1 for val in x_new]
    #plt.xticks(np.arange(min(x) - 1, max(x) + 1, 5.0))
    ax4.set_xlabel(r'$\textit{Threads}$')
    ax4.set_ylabel(r'$\textit{Efficiency}$')
    # ax4.xaxis.grid(True)
    ax4.yaxis.grid(True)
    ax4.set_ylim([0, 1.1])
    i += 1
ax4.plot(x_new, y_new, label='ideal', linewidth=1.95,  alpha=0.5, color='gray', linestyle='dashed')

plt.subplots_adjust(left=0.05, bottom=0.05, right=0.99, top=0.98, wspace=0.25, hspace=0.25)
plt.legend(loc='lower center', bbox_to_anchor=(0.52, 0.43), bbox_transform=plt.gcf().transFigure)
#if len(label) > 2:
#
#else:
#    plt.legend(loc='lower center', bbox_to_anchor=(0.52, 0.459),
#               bbox_transform=plt.gcf().transFigure)
plt.show()
