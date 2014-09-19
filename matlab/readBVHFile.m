function [skel, channels, channelsName, frameLength] = readBVHFile(filename)
    addpath('C:/Users/Jing/Documents/matlabmocap/MOCAP0p136/MOCAP0p136');
    addpath('C:/Users/Jing/Documents/matlabmocap/NDLUTIL0p161/NDLUTIL0p161');
    [skel, channels, frameLength] = bvhReadFile(filename);
    tree = skel.tree;
    idx = 1;
    i = 1;
    treesize = size(tree,2);
    channelsName = cell(1, size(channels, 2));
    while(i <= treesize)
        channelsOne = tree(i).channels;
        name = tree(i).name;
        j = 1;
        channelsOneSize = size(channelsOne,2);
        while(j <= channelsOneSize)
            channel = channelsOne(j);
            channelsName(1,idx) = strcat(name,'=',  channel);
            disp(idx); 
            j = j + 1; 
            idx = idx + 1;
        end
        i = i + 1;
    end
end