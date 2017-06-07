clear; close; clc;
%%

input_dir = 'E:\2_demo_3\visualSFM_stuff\images_2';
output_dir = 'E:\2_demo_3\visualSFM_stuff\images_2';

output_file_name = 'pgm_list.txt';
output_file_path = fullfile(output_dir, output_file_name);
fileID = fopen(output_file_path,'wt');

imagefiles = dir(fullfile(input_dir,'*.jpg*'));
n=numel(imagefiles);

for i=1:n
    image_name = imagefiles(i).name;
    fprintf(fileID, '%s\n', image_name);
end