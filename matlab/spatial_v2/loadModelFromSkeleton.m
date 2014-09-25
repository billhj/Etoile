function [humanoid] = loadModelFromSkeleton(skeletonfile)

% load bvh file with skeleton and animation data
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab' ) );
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab/bvh-matlab-master' ) );
[skeleton,time] = loadbvh(skeletonfile);
humanoid.file = skeletonfile;
humanoid.skeleton = skeleton;
end %functionend




