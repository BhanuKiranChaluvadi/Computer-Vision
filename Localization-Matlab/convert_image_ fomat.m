clear; close; clc;

%% Convert input ".JPG/.PNG" images to ".PGM"  files
%  Name them as 1--2--3--4--etc
%  Also output files containing all the created pgm file names
% This code is executed 1st and then change_listed_names
% Change type of input images - JPG/PNG etc..

input_dir = 'E:\2_demo_1\query_jpg_renamed';
output_dir = 'E:\2_demo_1\query_pgm';

output_file_name = 'pgm_list.txt';
output_file_path = fullfile(output_dir, output_file_name);
fileID = fopen(output_file_path,'wt');

imagefiles = dir(fullfile(input_dir,'*.jpg*'));
n=numel(imagefiles);

for i=1:n
    image_name = imagefiles(i).name;
    im=imread(fullfile(input_dir,image_name));
    %s = num2str(i-1);
    %s = strcat('q', s);
    s = image_name;
    baseFileName = strrep(image_name,'.jpg','.pgm');
    %baseFileName = strcat(s,'.jpg');
    %
    output = fullfile(output_dir, baseFileName); 
    imwrite(im, output);
    

    %%
    %
    baseFileName_with_newLine = strcat(baseFileName,'\n');
    fprintf(fileID, '%s\n', baseFileName);
end
fids=fopen('all');
fclose(fids);
