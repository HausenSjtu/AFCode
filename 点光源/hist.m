clear;
% close all;

name = 'n_1.jpg';
img = imread(name);
gray = rgb2gray(img);

figure;
subplot(2,1,1)
imhist(gray);
title(name)

subplot(2,1,2)
imshow(gray);

H = imhist(gray);

% % plot(H)
% AdapH = adapthisteq(gray);
% figure;imshow(AdapH);
% figure;imhist(gray);