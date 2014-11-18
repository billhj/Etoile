
function model = buildDynamicsBodyBySkeleton(skeleton)
disp('buildDynamicsBodyBySkeleton');
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
model.nameMVN{1} = 'Pelvis_z';
model.nameMVN{2} = 'Pelvis_y';
model.nameMVN{3} = 'Pelvis_x';
model.nameMVN{4} = 'L5_z';
model.nameMVN{5} = 'L5_y';
model.nameMVN{6} = 'L5_x';
model.nameMVN{7} = 'L3_z';
model.nameMVN{8} = 'L3_y';
model.nameMVN{9} = 'L3_x';
model.nameMVN{10} = 'T12_z';
model.nameMVN{11} = 'T12_y';
model.nameMVN{12} = 'T12_x';

model.parent = [0,1,2,3,4,5,6,7,8,9,10,11];
model.NB = 12;
model.jtype = {'Rz','Ry','Rx', 'Rz','Ry','Rx', 'Rz','Ry','Rx', 'Rz','Ry','Rx'};

model.q = zeros(model.NB, size(HipsJoint.rxyz,2));
model.Xtree{1} = xlt([0 0 0]);
model.Xtree{2} = xlt([0 0 0]);
model.Xtree{3} = xlt([0 0 0]);
model.q(1,:) = HipsJoint.rxyz(3,:);
model.q(2,:) = HipsJoint.rxyz(2,:);
model.q(3,:) = HipsJoint.rxyz(1,:);
model.qd(1,:) = HipsJoint.rxyz1(3,:);
model.qd(2,:) = HipsJoint.rxyz1(2,:);
model.qd(3,:) = HipsJoint.rxyz1(1,:);
model.qdd(1,:) = HipsJoint.rxyz2(3,:);
model.qdd(2,:) = HipsJoint.rxyz2(2,:);
model.qdd(3,:) = HipsJoint.rxyz2(1,:);

model.Xtree{4} = xlt([ChestJoint.offset]);
model.Xtree{5} = xlt([0 0 0]);
model.Xtree{6} = xlt([0 0 0]);
model.q(4,:) = ChestJoint.rxyz(3,:);
model.q(5,:) = ChestJoint.rxyz(2,:);
model.q(6,:) = ChestJoint.rxyz(1,:);
model.qd(4,:) = ChestJoint.rxyz1(3,:);
model.qd(5,:) = ChestJoint.rxyz1(2,:);
model.qd(6,:) = ChestJoint.rxyz1(1,:);
model.qdd(4,:) = ChestJoint.rxyz2(3,:);
model.qdd(5,:) = ChestJoint.rxyz2(2,:);
model.qdd(6,:) = ChestJoint.rxyz2(1,:);

model.Xtree{7} = xlt([Chest2Joint.offset]);
model.Xtree{8} = xlt([0 0 0]);
model.Xtree{9} = xlt([0 0 0]);
model.q(7,:) = Chest2Joint.rxyz(3,:);
model.q(8,:) = Chest2Joint.rxyz(2,:);
model.q(9,:) = Chest2Joint.rxyz(1,:);
model.qd(7,:) = Chest2Joint.rxyz1(3,:);
model.qd(8,:) = Chest2Joint.rxyz1(2,:);
model.qd(9,:) = Chest2Joint.rxyz1(1,:);
model.qdd(7,:) = Chest2Joint.rxyz2(3,:);
model.qdd(8,:) = Chest2Joint.rxyz2(2,:);
model.qdd(9,:) = Chest2Joint.rxyz2(1,:);

model.Xtree{10} = xlt([Chest3Joint.offset]);
model.Xtree{11} = xlt([0 0 0]);
model.Xtree{12} = xlt([0 0 0]);
model.q(10,:) = Chest3Joint.rxyz(3,:);
model.q(11,:) = Chest3Joint.rxyz(2,:);
model.q(12,:) = Chest3Joint.rxyz(1,:);
model.qd(10,:) = Chest3Joint.rxyz1(3,:);
model.qd(11,:) = Chest3Joint.rxyz1(2,:);
model.qd(12,:) = Chest3Joint.rxyz1(1,:);
model.qdd(10,:) = Chest3Joint.rxyz2(3,:);
model.qdd(11,:) = Chest3Joint.rxyz2(2,:);
model.qdd(12,:) = Chest3Joint.rxyz2(1,:);

mass = 10;
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

mass = 10;
startJ = ChestJoint.worldposition;
endJ = Chest2Joint.worldposition;
inertia = computeBoneInertiaMatrix(startJ,endJ, x,y,z,mass);
% L5
model.I{4} = mcI(0, [0,0,0], zeros(3,3));
model.I{5} = mcI(0, [0,0,0], zeros(3,3));
model.I{6} = mcI(mass, endJ - startJ, inertia);

mass = 10;
startJ = Chest2Joint.worldposition;
endJ = Chest3Joint.worldposition;
inertia = computeBoneInertiaMatrix(startJ,endJ, x,y,z,mass);
% L3
model.I{7} = mcI(0, [0,0,0], zeros(3,3));
model.I{8} = mcI(0, [0,0,0], zeros(3,3));
model.I{9} = mcI(mass, endJ - startJ, inertia);

mass = 20;
startJ = Chest3Joint.worldposition;
endJ = Chest4Joint.worldposition;
inertia = computeBoneInertiaMatrix(startJ, endJ, x,y,z,mass);
% T12
model.I{10} = mcI(0, [0,0,0], zeros(3,3));
model.I{11} = mcI(0, [0,0,0], zeros(3,3));
model.I{12} = mcI(mass, endJ - startJ, inertia);

disp('end buildDynamicsBodyBySkeleton');
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
else
    inertiaMatrix = computeUniformBox(x, y, z, mass);
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