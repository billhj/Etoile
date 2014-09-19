function [model, skeleton] = loadModelFromSkeleton(skeletonfile)

% load bvh file with skeleton and animation data
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab' ) );
addpath( genpath( 'C:/Users/Jing/Documents/etoile_git/trunk/matlab/bvh-matlab-master' ) );
[skeleton,time] = loadbvh(skeletonfile);

% build a dynamics model with skeleton
model = loadHuman(skeleton);

% for joint = skeleton
%     if joint.parent ~= 0
%         parentjoint = skeleton(joint.parent);
%     else
%         disp('no parent joint')
%     end
% end

end %functionend

function skeleton = computeFirstSecondDeriv(skeleton)
for joint = skeleton
    for i = 1 : size(joint.Dxyz, 1)
        joint.Dxyz1(i) =  (joint.Dxyz(i+1) - joint.Dxyz(i - 1)) * 0.5;
        joint.rxyz1(i) =  (joint.rxyz(i+1) - joint.rxyz(i - 1)) * 0.5;
    end
end

for joint = skeleton
    for i = 1 : size(joint.Dxyz1, 1)
        joint.Dxyz2(i) =  (joint.Dxyz1(i+1) - joint.Dxyz1(i - 1)) * 0.5;
        joint.rxyz2(i) =  (joint.rxyz1(i+1) - joint.rxyz1(i - 1)) * 0.5;
    end
end

end

function model = loadHuman(skeleton)
% model.NB = 23;
% model.nameMVN{1} = 'Pelvis';
% model.nameMVN{2} = 'L5';
% model.nameMVN{3} = 'L3';
% model.nameMVN{4} = 'T12';
% model.nameMVN{5} = 'T8';
% model.nameMVN{6} = 'Neck';
% model.nameMVN{7} = 'Head';
% model.nameMVN{8} = 'RightShoulder';
% model.nameMVN{9} = 'RightUpperArm';
% model.nameMVN{10} = 'RightForeArm';
% model.nameMVN{11} = 'RightHand';
% model.nameMVN{12} = 'LeftShoulder';
% model.nameMVN{13} = 'LeftUpperArm';
% model.nameMVN{14} = 'LeftForeArm';
% model.nameMVN{15} = 'LeftHand';
% model.nameMVN{16} = 'RightUpperLeg';
% model.nameMVN{17} = 'RightLowerLeg';
% model.nameMVN{18} = 'RightFoot';
% model.nameMVN{19} = 'RightToe';
% model.nameMVN{20} = 'LeftUpperLeg';
% model.nameMVN{21} = 'LeftLowerLeg';
% model.nameMVN{22} = 'LeftFoot';
% model.nameMVN{23} = 'LeftToe';

for joint = skeleton
    if(strcmp(joint.name,'Hips'))
        HipsJoint = joint;
    elseif (strcmp(joint.name,'Chest'))
        ChestJoint = joint;
    elseif (strcmp(joint.name,'Chest2'))
        Chest2Joint = joint;
    elseif (strcmp(joint.name,'Chest3'))
        Chest3Joint = joint;
    elseif (strcmp(joint.name,'Chest4'))
        Chest4Joint = joint;
    end
end
model.gravity = [0; -9.8; 0];
model.nameMVN{1} = 'Pelvis_x';
model.nameMVN{2} = 'Pelvis_y';
model.nameMVN{3} = 'Pelvis_z';
model.nameMVN{4} = 'L5_x';
model.nameMVN{5} = 'L5_y';
model.nameMVN{6} = 'L5_z';
model.nameMVN{7} = 'L3_x';
model.nameMVN{8} = 'L3_y';
model.nameMVN{9} = 'L3_z';
model.nameMVN{10} = 'T12_x';
model.nameMVN{11} = 'T12_y';
model.nameMVN{12} = 'T12_z';

model.parent = [0,1,2,3,4,5,6,5,8,9,10,11];
model.jtype = {'Rx','Ry','Rz', 'Rx','Ry','Rz', 'Rx','Ry','Rz', 'Rx','Ry','Rz'};
model.Xtree{1} = xlt([0 0 0]);
model.Xtree{2} = xlt([0 0 0]);
model.Xtree{3} = xlt([0 0 0]);

model.Xtree{4} = xlt([0 1 0]);
model.Xtree{5} = xlt([0 0 0]);
model.Xtree{6} = xlt([0 0 0]);

model.Xtree{7} = xlt([0 1 0]);
model.Xtree{8} = xlt([0 0 0]);
model.Xtree{9} = xlt([0 0 0]);

model.Xtree{10} = xlt([0 1 0]);
model.Xtree{11} = xlt([0 0 0]);
model.Xtree{12} = xlt([0 0 0]);

mass = 20;
x = 0.5;
y= 1;
z = 0.5;
startJ = HipsJoint.worldposition;
endJ = ChestJoint.worldposition;
inertia = computeBoneInertiaMatrix(startJ, endJ, x,y,z,mass);
% Pelvis
model.I{1} = mcI(0, [0,0,0], zeros(3,3));
model.I{2} = mcI(0, [0,0,0], zeros(3,3));
model.I{3} = mcI(mass, endJ - startJ, inertia);

mass = 30;
startJ = ChestJoint.worldposition;
endJ = Chest2Joint.worldposition;
inertia = computeBoneInertiaMatrix(startJ,endJ, x,y,z,mass);
% L5
model.I{4} = mcI(0, [0,0,0], zeros(3,3));
model.I{5} = mcI(0, [0,0,0], zeros(3,3));
model.I{6} = mcI(mass, endJ - startJ, inertia);

mass = 40;
startJ = Chest2Joint.worldposition;
endJ = Chest3Joint.worldposition;
inertia = computeBoneInertiaMatrix(startJ,endJ, x,y,z,mass);
% L3
model.I{7} = mcI(0, [0,0,0], zeros(3,3));
model.I{8} = mcI(0, [0,0,0], zeros(3,3));
model.I{9} = mcI(mass, endJ - startJ, inertia);

mass = 50;
startJ = Chest3Joint.worldposition;
endJ = Chest4Joint.worldposition;
inertia = computeBoneInertiaMatrix(startJ, endJ, x,y,z,mass);
% T12
model.I{10} = mcI(0, [0,0,0], zeros(3,3));
model.I{11} = mcI(0, [0,0,0], zeros(3,3));
model.I{12} = mcI(mass, endJ - startJ, inertia);


% model.parent = [0,1,2,3,4,5,6,5,8,9,10, 5,12,13,14];%  1,16,17,18, 1,20,21,22];
% model.jtype = {};

% model.nameBAP{1} = 'Pelvis';
% model.nameBAP{2} = 'L5';
% model.nameBAP{3} = 'L3';
% model.nameBAP{4} = 'T12';
% model.nameBAP{5} = 'T8';
% model.nameBAP{6} = 'Neck';
% model.nameBAP{7} = 'Head';
% model.nameBAP{8} = 'RightShoulder';
% model.nameBAP{9} = 'RightUpperArm';
% model.nameBAP{10} = 'RightForeArm';
% model.nameBAP{11} = 'RightHand';
% model.nameBAP{12} = 'LeftShoulder';
% model.nameBAP{13} = 'LeftUpperArm';
% model.nameBAP{14} = 'LeftForeArm';
% model.nameBAP{15} = 'LeftHand';
% model.nameBAP{16} = 'RightUpperLeg';
% model.nameBAP{17} = 'RightLowerLeg';
% model.nameBAP{18} = 'RightFoot';
% model.nameBAP{19} = 'RightToe';
% model.nameBAP{20} = 'LeftUpperLeg';
% model.nameBAP{21} = 'LeftLowerLeg';
% model.nameBAP{22} = 'LeftFoot';
% model.nameBAP{23} = 'LeftToe';

end %functionend

function inertiaMatrix = computeBoneInertiaMatrix(startJ, endJ, x, y, z, mass)
direction = endJ' - startJ';
direction_normalized = direction / norm(direction);
xdir = [1,0,0];
ydir = [0,1,0];
zdir = [0,0,1];
if(dot(direction, ydir) > 0)
    ydir_new = direction_normalized;
    zdir_new = cross(xdir, ydir_new);
    xdir_new = cross(ydir_new, zdir_new);
    
    ydir_new = ydir_new / norm(ydir_new);
    zdir_new = zdir_new / norm(zdir_new);
    xdir_new = xdir_new / norm(xdir_new);
    rotationMatrix = [xdir_new', ydir_new', zdir_new'];
    inertiaMatrix = computeUniformBox(x, y, z, mass);
    inertiaMatrix = rotateInertia(inertiaMatrix, rotationMatrix);
end

end

function inertiaMatrix = computeUniformBox(x, y, z, mass)
inertiaMatrix = [mass /12.0 * (y * y + z * z), 0, 0;
    0, mass /12.0 * (x * x + z * z), 0;
    0,0,mass /12.0 * (x * x + y * y)];
end

function outputinertiaMatrix = rotateInertia(inertiaMatrix, rotationMatrix)
outputinertiaMatrix = rotationMatrix * inertiaMatrix * rotationMatrix';
end

function outputinertiaMatrix = translateInertia(inertiaMatrix, x, y, z, mass)
outputinertiaMatrix = [inertiaMatrix(0) + mass * (y * y + z * z), inertiaMatrix(1) + mass * x * y, inertiaMatrix(2) + mass * x * z;
    inertiaMatrix(3) + mass * x * y, inertiaMatrix(4) + mass * (x * x + z * z), inertiaMatrix(5) + mass * y * z;
    inertiaMatrix(6) + mass * x * z, inertiaMatrix(7) + mass * y * z, inertiaMatrix(8) + mass * (x * x + y * y)
    ];
end

