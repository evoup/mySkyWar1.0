function GetNewPos (x,y,mode)
if mode==0 then--ģʽ0Ϊֱ�������ƶ�
x=x
y=y+1
return x..","..y
elseif mode==1 then--ģʽ1Ϊб���������˶�
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
elseif mode==2 then--ģʽ2Ϊб���������˶�
x=x+1
y=y+1
return x..","..y
elseif mode==3 then--ģʽ3Ϊб���������������˶�
x=x+y*0.01
y=y+1
return x..","..y
elseif mode==4 then--ģʽ4Ϊб���������������˶�
--[[if y>480 then
x=x+1
y=y-1
else
x=x+y*0.01
y=y+1
return x..","..y
elseif mode==5 then--ģʽ5Ϊ�����˶�
x=x
y=y-1]]--
return x..","..y

--���Ҳ�����ȥʵ���ˣ���״̬���жϾͿ���ʵ�֣�
else
return x..","..y
end
end;
