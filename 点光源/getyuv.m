% RGB2YUV
% Y = 0.299R + 0.587G + 0.114B 
% U = -0.147R - 0.289G + 0.436B 
% V = 0.615R - 0.515G - 0.100B  
% 
% YUV2RGB
% R = Y + 1.14V 
% G = Y - 0.39U - 0.58V 
% B = Y + 2.03U


close all;
clear all;
clc;

yMax = [];

for i=1:6
    name = ['l_',num2str(i),'.jpg'];
    RGB = imread(name);
    figure;
    subplot(2,1,1);
    imshow(RGB);
    title(name);
    % RGB = mat2gray(RGB);
    R = RGB(:,:,1);
    G = RGB(:,:,2);
    B = RGB(:,:,3);


    % RGB2YUV
    Y = 0.299*R + 0.587*G + 0.114*B;
    U = -0.147*R- 0.289*G + 0.436*B;
    V = 0.615*R - 0.515*G - 0.100*B;
    YUV = cat(3, Y, U, V);
    subplot(2,1,2);
    H = imhist(Y);
    bar(H);
    title('Y');
    yMax = [yMax sum(H(250:256))];
end

figure;
plot(yMax);


% YUV2RGB
% RGB1 = zeros(size(RGB));
% RGB1(:,:,1) = Y + 1.14 * V;
% RGB1(:,:,2) = Y - 0.39 * U - 0.58 * V;
% RGB1(:,:,3) = Y + 2.03 * U;
% figure; imshow(RGB1)

% After YUV to RGB, The Image should same with original image.
