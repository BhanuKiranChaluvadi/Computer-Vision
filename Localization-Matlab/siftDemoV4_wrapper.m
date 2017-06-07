clear all; close all; clc;

%%

input_dir = 'E:\2_demo_1\query_pgm_resized';  %Directory containing list of keys list
output_dir = 'E:\2_demo_1\query_keys';

input_file_name = 'pgm_list.txt';
input_file_path = fullfile(input_dir, input_file_name);
fid = fopen(input_file_path);

tline = fgets(fid);

while ischar(tline)
    tline = strtrim(tline);
    input_file = fullfile(input_dir,tline);
    sift(input_file);
    Output_name = strrep(tline,'.pgm','.key');
    output_file = fullfile(output_dir, Output_name);
    movefile('tmp.key',output_file);
    tline = fgets(fid);
end