addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab' ) );
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab/spatial_v2' ) );
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab/bvh-matlab-master' ) );


[humanoid, skeletonO] = loadSkeleton('C:/Users/Jing/Documents/etoile_git/trunk/matlab/nadine session-011_Suit 00131313_11037-14163#Suit 00131313.bvh');
[humanoid.skeleton] = loadSkeletonData(humanoid.skeleton);
[model] = buildDynamicsBodyBySkeleton(humanoid.skeleton);
[model] = computeTorque(model);
armmodel = buildarmdynamicsmodel(humanoid.skeleton);
% 