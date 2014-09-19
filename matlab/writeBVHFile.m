function writeBVHFile(filename, skel, channels, frameLength)
    addpath('C:/Users/Jing/Documents/matlabmocap/MOCAP0p136/MOCAP0p136');
    addpath('C:/Users/Jing/Documents/matlabmocap/NDLUTIL0p161/NDLUTIL0p161');
    bvhWriteFile(filename, skel, channels, frameLength);
end