clc
clear

input_dir = 'E:\2_demo_1\query_pgm';
output_dir = 'E:\2_demo_1\query_pgm_resized';

%I = imread('test.pgm');


imagefiles = dir(fullfile(input_dir,'*.pgm*'));
n=numel(imagefiles);

for i=1:n
    image_name = imagefiles(i).name;
    im=imread(fullfile(input_dir,image_name));
    J = imresize(im, 0.5);
    output = fullfile(output_dir, image_name); 
    imwrite(J, output);
end