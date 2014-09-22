function [skeleton] = loadModelFromSkeleton(skeletonfile)

% load bvh file with skeleton and animation data
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab' ) );
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab/bvh-matlab-master' ) );
[skeleton,time] = loadbvh(skeletonfile);
end %functionend




