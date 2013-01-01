function GetNewPos (x,y,mode)
if mode==0 then--模式0为直线向下移动
x=x
y=y+1
return x..","..y
elseif mode==1 then--模式1为斜线向左下运动
if y <=20 and x>=110 then
x=x+1
y=y+4
elseif y>20 and y<100 and x>=110 then
x=x-1
y=y+1
elseif y>=100 and y<150 and x>=140 then
x=x+1
y=y+1
elseif y>=150 and x>110 then
x=x-1
elseif y>0 and x<=110 then
y=y-1
elseif y==0 and x<400 then
x=x+1
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
elseif mode==4 then--模式4为斜线向左下抛物线运动
--[[if y>480 then
x=x+1
y=y-1
else
x=x+y*0.01
y=y+1
return x..","..y
elseif mode==5 then--模式5为向上运动
x=x
y=y-1]]--
return x..","..y
elseif mode==5 then--模式5为斜线向左上抛物线运动
x=x-1
y=y-1
return x..","..y
--正弦波不用去实现了，用状态机判断就可以实现！
else
return x..","..y
end
end;
