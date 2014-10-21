function [humanoid] = loadModelFromSkeleton(skeletonfile)

% load bvh file with skeleton and animation data
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab' ) );
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab/bvh-matlab-master' ) );
[skeleton,time] = loadbvh(skeletonfile);

if(true)
    skeleton.offset = skeleton.offset / 100.0;
    skeleton.worldposition = skeleton.worldposition / 100.0;
    skeleton.Dxyz = skeleton.Dxyz / 100.0;
    skeleton.worldposition = skeleton.worldposition / 100.0;
end

pas = 4;
for j = 1:size(skeleton,2)
    if not(isspace(skeleton(1,j).name))
        max = size(skeleton(1,j).Dxyz, 2);
        i = 1;
        index = 1;
        nb = round(max / pas);
        Dxyz = zeros(3,nb);
        rxyz = zeros(3,nb);
        trans = zeros(4,4,nb);
        Dxyz1 = skeleton(1,j).Dxyz(1,:);
        Dxyz2 = skeleton(1,j).Dxyz(2,:);
        Dxyz3 = skeleton(1,j).Dxyz(3,:);
        rxyz1 = skeleton(1,j).rxyz(1,:);
        rxyz2 = skeleton(1,j).rxyz(2,:);
        rxyz3 = skeleton(1,j).rxyz(3,:);
        while i+pas <=max
            Dxyz(1,index) = mean(Dxyz1(i:i+pas));
            rxyz(1,index) = mean(rxyz1(i:i+pas));
            Dxyz(2,index) = mean(Dxyz2(i:i+pas));
            rxyz(2,index) = mean(rxyz2(i:i+pas));
            Dxyz(3,index) = mean(Dxyz3(i:i+pas));
            rxyz(3,index) = mean(rxyz3(i:i+pas));
            i = i + pas;
            index = index + 1;
        end
        skeleton(1,j).Dxyz = Dxyz;
        skeleton(1,j).rxyz = rxyz;
        for k = 1 : nb
            trans(:,:,k) = transformation_matrix(Dxyz(:,k), rxyz(:,k), skeleton(1,j).order);
        end
        skeleton(1,j).trans = trans;
    end
end


humanoid.file = skeletonfile;
humanoid.skeleton = skeleton;
end %functionend



function transM = transformation_matrix(displ,rxyz,order)
% Constructs the transformation for given displacement, DISPL, and
% rotations RXYZ. The vector RYXZ is of length three corresponding to
% rotations around the X, Y, Z axes.
%
% The third input, ORDER, is a vector indicating which order to apply
% the planar rotations. E.g., [3 1 2] refers applying rotations RXYZ
% around Z first, then X, then Y.
%
% Years ago we benchmarked that multiplying the separate rotation matrices
% was more efficient than pre-calculating the final rotation matrix
% symbolically, so we don't "optimise" by having a hard-coded rotation
% matrix for, say, 'ZXY' which seems more common in BVH files.
% Should revisit this assumption one day.
%
% Precalculating the cosines and sines saves around 38% in execution time.

c = cosd(rxyz);
s = sind(rxyz);

RxRyRz(:,:,1) = [1 0 0; 0 c(1) -s(1); 0 s(1) c(1)];
RxRyRz(:,:,2) = [c(2) 0 s(2); 0 1 0; -s(2) 0 c(2)];
RxRyRz(:,:,3) = [c(3) -s(3) 0; s(3) c(3) 0; 0 0 1];

rotM = RxRyRz(:,:,order(1))*RxRyRz(:,:,order(2))*RxRyRz(:,:,order(3));
transM = [rotM, displ; 0 0 0 1];
end
