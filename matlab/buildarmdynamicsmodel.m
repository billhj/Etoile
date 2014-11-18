
function model = buildarmdynamicsmodel(skeleton)
disp('buildarmdynamicsmodel');
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
    if(strcmp(joint.name,'RightShoulder'))
        RightShoulderJoint = joint;
    elseif (strcmp(joint.name,'RightElbow'))
        RightElbowJoint = joint;
    elseif (strcmp(joint.name,'RightWrist'))
        RightWristJoint = joint;
    end
end
model.gravity = [0; -9.8; 0];
model.nameMVN{1} = 'upperarm_z';
model.nameMVN{2} = 'upperarm_y';
model.nameMVN{3} = 'upperarm_x';
model.nameMVN{4} = 'lowerarm_z';
model.nameMVN{5} = 'lowerarm_y';
model.nameMVN{6} = 'lowerarm_x';

model.parent = [0,1,2,3,4,5];
model.NB = 6;
model.jtype = {'Rz','Ry','Rx', 'Rz','Ry','Rx'};

model.q = zeros(model.NB, size(RightShoulderJoint.rxyz,2));
model.Xtree{1} = xlt([0 0 0]);
model.Xtree{2} = xlt([0 0 0]);
model.Xtree{3} = xlt([0 0 0]);
model.q(1,:) = RightShoulderJoint.rxyz(3,:);
model.q(2,:) = RightShoulderJoint.rxyz(2,:);
model.q(3,:) = RightShoulderJoint.rxyz(1,:);
model.qd(1,:) = RightShoulderJoint.rxyz1(3,:);
model.qd(2,:) = RightShoulderJoint.rxyz1(2,:);
model.qd(3,:) = RightShoulderJoint.rxyz1(1,:);
model.qdd(1,:) = RightShoulderJoint.rxyz2(3,:);
model.qdd(2,:) = RightShoulderJoint.rxyz2(2,:);
model.qdd(3,:) = RightShoulderJoint.rxyz2(1,:);

model.Xtree{4} = xlt([RightElbowJoint.offset]);
model.Xtree{5} = xlt([0 0 0]);
model.Xtree{6} = xlt([0 0 0]);
model.q(4,:) = RightElbowJoint.rxyz(3,:);
model.q(5,:) = RightElbowJoint.rxyz(2,:);
model.q(6,:) = RightElbowJoint.rxyz(1,:);
model.qd(4,:) = RightElbowJoint.rxyz1(3,:);
model.qd(5,:) = RightElbowJoint.rxyz1(2,:);
model.qd(6,:) = RightElbowJoint.rxyz1(1,:);
model.qdd(4,:) = RightElbowJoint.rxyz2(3,:);
model.qdd(5,:) = RightElbowJoint.rxyz2(2,:);
model.qdd(6,:) = RightElbowJoint.rxyz2(1,:);

% model.Xtree{7} = xlt([RightWrist.offset]);
% model.Xtree{8} = xlt([0 0 0]);
% model.Xtree{9} = xlt([0 0 0]);
% model.q(7,:) = Chest2Joint.rxyz(3,:);
% model.q(8,:) = Chest2Joint.rxyz(2,:);
% model.q(9,:) = Chest2Joint.rxyz(1,:);
% model.qd(7,:) = Chest2Joint.rxyz1(3,:);
% model.qd(8,:) = Chest2Joint.rxyz1(2,:);
% model.qd(9,:) = Chest2Joint.rxyz1(1,:);
% model.qdd(7,:) = Chest2Joint.rxyz2(3,:);
% model.qdd(8,:) = Chest2Joint.rxyz2(2,:);
% model.qdd(9,:) = Chest2Joint.rxyz2(1,:);


mass = 8;
x = 0.5;
y= 1;
z = 0.5;
startJ = RightShoulderJoint.worldposition;
endJ = RightElbowJoint.worldposition;
inertia = computeBoneInertiaMatrix(startJ, endJ, x,y,z,mass);
% Pelvis
model.I{1} = mcI(0, [0,0,0], zeros(3,3));
model.I{2} = mcI(0, [0,0,0], zeros(3,3));
model.I{3} = mcI(mass, endJ - startJ, inertia);

mass = 5;
startJ = RightElbowJoint.worldposition;
endJ = RightWristJoint.worldposition;
inertia = computeBoneInertiaMatrix(startJ,endJ, x,y,z,mass);
% L5
model.I{4} = mcI(0, [0,0,0], zeros(3,3));
model.I{5} = mcI(0, [0,0,0], zeros(3,3));
model.I{6} = mcI(mass, endJ - startJ, inertia);


disp('end buildarmdynamicsmodel');
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