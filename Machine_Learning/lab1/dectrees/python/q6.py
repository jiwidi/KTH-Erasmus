import random
import sys
import dtree as d
import monkdata as m
import numpy as np
import plotly
from statistics import pvariance
import plotly.graph_objs as go

plotly.tools.set_credentials_file(username='jiwidi', api_key='qMue368p0yeZMLP7rxmU')

reps=int(sys.argv[1])
def partition(data, fraction):
    ldata= list(data)
    random.shuffle(ldata)
    breakPoint = int(len(ldata)*fraction)
    return ldata[:breakPoint], ldata[breakPoint:]


fraction=[0.3,0.4,0.5,0.6,0.7,0.8]
valuesmonk1=[]
valuesmonk3=[]
variancemonk1=[]
variancemonk3=[]
monk1data=[]
monk3data=[]

monk1simple=[]
monk3simple=[]

for v in fraction:
    monk1train,monk1validation=partition(m.monk1,v)
    monk3train,monk3validation=partition(m.monk3,v)
    monk1tree=d.buildTree(monk1train,m.attributes)
    monk3tree=d.buildTree(monk3train,m.attributes)
    monk1simple.append(d.check(monk1tree,monk1validation))
    monk3simple.append(d.check(monk3tree,monk3validation))

#Monk1 evaluation
for v in fraction:
    value=0
    for _ in range(reps):
        monk1train,monk1validation=partition(m.monk1,v)
        monk1tree=d.buildTree(monk1train,m.attributes)
        topPerformance=0
        performance=0
        keepPruning=True
        while(keepPruning):
            keepPruning=False
            pruned=d.allPruned(monk1tree)
            for tree in pruned:
                performance=d.check(tree,monk1validation)
                if(performance>topPerformance):
                    keepPruning=True
                    topPerformance=performance
                    monk1tree=tree
        valuesmonk1.append(d.check(monk1tree,m.monk1test))
    print("Monk1 fraction "+ str(v))
    mean=np.mean(valuesmonk1)
    print("    Error: "+str(1-mean))
    monk1data.append(1-mean)
    variance=pvariance(valuesmonk1,mean)
    print("    Variance: "+str(variance))
    variancemonk1.append(variance)

print()
print()
print()
#Monk3 evaluation
for v in fraction:
    value=0
    for _ in range(reps):
        monk3train,monk3validation=partition(m.monk3,v)
        monk3tree=d.buildTree(monk3train,m.attributes)
        topPerformance=0
        performance=0
        keepPruning=True
        while(keepPruning):
            keepPruning=False
            pruned=d.allPruned(monk3tree)
            for tree in pruned:
                performance=d.check(tree,monk3validation)
                if(performance>topPerformance):
                    keepPruning=True
                    topPerformance=performance
                    monk3train=tree
        valuesmonk3.append(d.check(monk3tree,m.monk3test))
    print("Monk3 fraction "+ str(v))
    mean=np.mean(valuesmonk3)
    print("    Error: "+str(1-mean))
    monk3data.append(1-mean)
    variance=pvariance(valuesmonk3,mean)
    print("    Variance: "+str(variance))
    variancemonk3.append(variance)

trace1 = go.Scatter(
    x=fraction,
    y=monk1data,
    name = '<b>Monk1 </b>', # Style name/legend entry with html tags
    connectgaps=False
)
trace2 = go.Scatter(
    x=fraction,
    y=monk3data,
    name = '<b>Monk3 </b>', # Style name/legend entry with html tags
    connectgaps=False
)
trace1s = go.Scatter(
    x=fraction,
    y=monk1simple,
    name='<b>Monk1 error </b>'
)

trace3s = go.Scatter(
    x=fraction,
    y=monk3simple,
    name='<b>Monk3 error </b>'

)
trace4 = go.Scatter(
    x=fraction,
    y=variancemonk1,
    name = '<b>Monk1 variance</b>', # Style name/legend entry with html tags
    connectgaps=True
)
trace3 = go.Scatter(
    x=fraction,
    y=variancemonk3,
    name = '<b>Monk3 variance</b>', # Style name/legend entry with html tags
    fillcolor='rgba(0,100,80,0.2)',
    connectgaps=True
)
# data =[trace4,trace3] #Variance
data =[trace1,trace2] #Error
# Edit the layout

layout = dict(title = 'Effect of different prunings fractions in monk1 and monk3',
              xaxis = dict(title = 'Fraction between train and validation datasets'),
              yaxis = dict(title = 'Variance'),
              )

fig = dict(data=data, layout=layout)
plotly.plotly.iplot(fig, filename='Q7 ML lab1 Error test v3 ')

data =[trace4,trace3] #Variance
layout = dict(title = 'Effect of different prunings fractions in monk1 and monk3',
              xaxis = dict(title = 'Fraction between train and test'),
              yaxis = dict(title = 'Error on test dataset'),
              )

fig = dict(data=data, layout=layout)
plotly.plotly.iplot(fig, filename='Q7 ML lab1 variance v3 ')



