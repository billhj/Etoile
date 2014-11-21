function [torque] = computeArmTorque(skeleton)
disp('simplecomputeTorque');
for joint = skeleton
    if(strcmp(joint.name,'RightShoulder'))
        RightShoulderJoint = joint;
    elseif (strcmp(joint.name,'RightElbow'))
        RightElbowJoint = joint;
    elseif (strcmp(joint.name,'RightWrist'))
        RightWristJoint = joint;
    end
end

torque.upperarmPos = (RightShoulderJoint.Dxyz + RightElbowJoint.Dxyz) * 0.5;
torque.lowerarmPos = (RightElbowJoint.Dxyz + RightWristJoint.Dxyz) * 0.5;
torque.com = (torque.upperarmPos * 8 + torque.lowerarmPos * 5) / 13.0;
torque.v = zeros(size(torque.com));
torque.a = zeros(size(torque.com));
torque.upv = zeros(size(torque.com));
torque.upa = zeros(size(torque.com));
torque.lowv = zeros(size(torque.com));
torque.lowa = zeros(size(torque.com));
for i = 1 : size(RightShoulderJoint.Dxyz,2) - 1
    torque.v(:,i) = torque.com(:,i + 1) - torque.com(:,i);
    torque.upv(:,i) = torque.upperarmPos(:,i + 1) - torque.upperarmPos(:,i);
    torque.lowv(:,i) = torque.lowerarmPos(:,i+1) - torque.lowerarmPos(:,i);
end
torque.v(:,size(RightShoulderJoint.Dxyz,2)) = torque.v(:,size(RightShoulderJoint.Dxyz,2)-1);
torque.v = torque.v * 120; 
torque.upv(:,size(RightShoulderJoint.Dxyz,2)) = torque.upv(:,size(RightShoulderJoint.Dxyz,2)-1);
torque.upv = torque.upv * 120; 
torque.lowv(:,size(RightShoulderJoint.Dxyz,2)) = torque.lowv(:,size(RightShoulderJoint.Dxyz,2)-1);
torque.lowv = torque.lowv * 120; 

for i = 1 : size(RightShoulderJoint.Dxyz,2) - 1
    torque.a(:,i) = torque.v(:,i + 1) - torque.v(:,i);
    torque.upa(:,i) = torque.upv(:,i + 1) - torque.upv(:,i);
    torque.lowa(:,i) = torque.lowv(:,i + 1) - torque.lowv(:,i);
end
torque.a(:,size(RightShoulderJoint.Dxyz,2)) = torque.a(:,size(RightShoulderJoint.Dxyz,2)-1);
torque.a = torque.a * 120;
torque.upa(:,size(RightShoulderJoint.Dxyz,2)) = torque.upa(:,size(RightShoulderJoint.Dxyz,2)-1);
torque.upa = torque.upa * 120;
torque.lowa(:,size(RightShoulderJoint.Dxyz,2)) = torque.lowa(:,size(RightShoulderJoint.Dxyz,2)-1);
torque.lowa = torque.lowa * 120;

torque.r = torque.com - RightShoulderJoint.Dxyz;
torque.rup = torque.upperarmPos - RightShoulderJoint.Dxyz;
torque.rlow = torque.lowerarmPos - RightShoulderJoint.Dxyz;
torque.a_gravity = repmat([0;-9.8;0],1,size(torque.a,2));
torque.gravity = cross(torque.r, torque.a_gravity) * 13;
torque.storque = cross(torque.r, torque.a) * 13 + torque.gravity;
torque.storqueS = cross(torque.r, torque.a) * 13*3 + torque.gravity;
torque.storquenograivity = cross(torque.r, torque.a) * 13;
torque.ssepratetorque = cross(torque.rup,  torque.upa) * 8 + cross(torque.rlow,  torque.lowa) * 5;

disp('end simplecomputeTorque');
end