function GetNewPos (x,y,mode)
if mode==-1 then
return x..","..y
elseif mode==0 then--模式0为直线向下移动
if x<=0 then
x=100
end
if x>=600 then
x=560
end
y=y+2
return x..","..y
elseif mode==1 then--模式1为斜线向左下运动
if y <=100 then
x=x+1
y=y+4
elseif y>100 and y<200 then
x=x-1
y=y+1
elseif y>=200 and y<300 then
x=x+1
y=y+1
elseif y>=300 then
y=y+4
end
return x..","..y
elseif mode==2 then--模式2为斜线向右下运动
x=x+1
y=y+1
return x..","..y
elseif mode==3 then--模式3为斜线向右下抛物线运动
x=x+y*0.01
y=y+1
return x..","..y
elseif mode==4 then--模式4为运输直升机的移动方式
if y <=100 then
x=x+1
y=y+2
elseif y>100 and y<200 then
x=x-1
y=y+2
elseif y>=200 and y<230 then
x=x+1
y=y+2
elseif y>=230 then
x=x-1
y=y+2
end
return x..","..y
elseif mode==5 then--模式5为带奖励单位的移动方式(比一般单位要快)
if y <=100 then
x=x+2
y=y+4
elseif y>100 and y<200 then
x=x-2
y=y+2
elseif y>=200 and y<300 then
x=x+2
y=y+2
elseif y>=300 then
y=y+4
end
return x..","..y
elseif mode==6 then--模式6为直线向下移动
if x<=0 then
x=100
end
if x>=600 then
x=560
end
y=y+8
return x..","..y
elseif mode==7 then--模式7为直线向左移动（for简单移动状态机）
x=x-2
y=y+2
return x..","..y
elseif mode==8 then--模式8为直线向右移动（for简单移动状态机）
x=x+2
y=y+2
return x..","..y
elseif mode==9 then--模式9为直线向右移动（for简单移动状态机）
x=x+1
y=y+3
return x..","..y
elseif mode==10 then--模式10为直线向右移动（for简单移动状态机）
x=x-1
y=y+3
return x..","..y
elseif mode==11 then--模式11为boss直线向右移动（for简单移动状态机）
x=x-4
return x..","..y
elseif mode==12 then--模式12为boss直线向右移动（for简单移动状态机）
x=x+4
return x..","..y
elseif mode==13 then--坦克向右，沿着地图逝去
x=x+1
y=y+3
return x..","..y
elseif mode==14 then--坦克向左，沿着地图逝去
x=x-1
y=y+3
return x..","..y
elseif mode==99 then--不动，沿着地图逝去
x=x
y=y+3
return x..","..y
--正弦波不用去实现了，用状态机判断就可以实现！
else
return x..","..y
end
end;
