clear; close; clc

%% Read each line from a input file and convert into desired string
% Used to to convert 1.pgm --> 1.key file name
% This codce is execurted 2nd.
input_dir = 'E:\demo_4\data_pgm';
output_dir = 'E:\demo_4\data_pgm';

input_file_name = 'pgm_list.txt';
input_file_path = fullfile(input_dir, input_file_name);
fid = fopen(input_file_path);

output_file_name = 'keys_list.txt';
output_file_path = fullfile(output_dir, output_file_name);
fileID = fopen(output_file_path,'wt');

tline = fgets(fid);
while ischar(tline)
    newName = strrep(tline,'.pgm','.key');
    fprintf(fileID, '%s', newName);
    tline = fgets(fid);
end