close all;
clear;

load data_3_out.txt
% load data_2_out.txt

% plot(data_1_out(:,1),data_1_out(:,2));
hold on 
x3=data_3_out(:,1);
plot(x3,data_3_out(:,2));
hold on;

p3 = polyfit(x3,data_3_out(:,2),6);
y3_ = polyval(p3,x3,'r');
plot(x3,y3_);

fid = fopen('myx3.txt','w');
fprintf(fid,'%d,',x3);
fclose(fid);

fid = fopen('myy3.txt','w');
fprintf(fid,'%4.0f,',y3_);
fclose(fid);