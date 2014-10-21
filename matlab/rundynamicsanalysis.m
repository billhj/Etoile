addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab' ) );
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab/spatial_v2' ) );
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab/bvh-matlab-master' ) );


[humanoid] = loadModelFromSkeleton('C:/Users/Jing/Documents/etoile_git/trunk/matlab/convertedZYX_caroline_l#Suit00131313.bvh');
[humanoid.skeleton] = loadSkeletonData(humanoid.skeleton);
[model] = buildDynamicsBodyBySkeleton(humanoid.skeleton);
% 