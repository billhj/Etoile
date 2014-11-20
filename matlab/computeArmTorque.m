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
for i = 1 : size(RightShoulderJoint.Dxyz,2) - 1
    torque.v(:,i) = torque.com(:,i+1) - torque.com(:,i);
end
torque.v(:,size(RightShoulderJoint.Dxyz,2)) = torque.v(:,size(RightShoulderJoint.Dxyz,2) - 1);
torque.v = torque.v * 120; 

for i = 1 : size(RightShoulderJoint.Dxyz,2) - 1
    torque.a(:,i) = torque.v(:,i+1) - torque.v(:,i);
end
torque.a(:,size(RightShoulderJoint.Dxyz,2)) = torque.a(:,size(RightShoulderJoint.Dxyz,2) - 1);
torque.a = torque.a * 120;

torque.r = torque.com - RightShoulderJoint.Dxyz;

torque.storque = cross(torque.r,  torque.a) * 13;

disp('end simplecomputeTorque');
end