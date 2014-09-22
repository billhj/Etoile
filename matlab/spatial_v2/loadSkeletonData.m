function skeleton = loadSkeletonData(skeleton, datafile)
for j = 1:size(skeleton,2)
    joint = skeleton(1,j);
    joint.rxyz1 =zeros(size(joint.Dxyz));
    joint.Dxyz1 =zeros(size(joint.Dxyz));
    for i = [1:size(joint.Dxyz, 2)]
         if i == 1
             disp('aa')
             joint.Dxyz1(:,i) = joint.Dxyz(:,i+1) - joint.Dxyz(:,i);
             i
             size(joint.rxyz)
             joint.rxyz1(:,i) = joint.rxyz(:,i+1) - joint.rxyz(:,i);
         elseif i == size(joint.Dxyz, 2)
             disp('bb')
%             joint.Dxyz1(:,i) = joint.Dxyz(:,i) - joint.Dxyz(:,i - 1);
%             joint.rxyz1(:,i) = joint.rxyz(:,i) - joint.rxyz(:,i - 1);
         else
%             joint.Dxyz1(:,i) =  (joint.Dxyz(:,i+1) - joint.Dxyz(:,i - 1)) * 0.5;
%             joint.rxyz1(:,i) =  (joint.rxyz(:,i+1) - joint.rxyz(:,i - 1)) * 0.5;
         end
    end
end

GGG

for joint = skeleton
    for i = 1 : size(joint.Dxyz1, 2)
        if i == 1
            joint.Dxyz2(:,i) = joint.Dxyz1(:,i+1) - joint.Dxyz1(:,i);
            joint.rxyz2(:,i) = joint.rxyz1(:,i+1) - joint.rxyz1(:,i);
        elseif i == size(joint.Dxyz, 2)
            joint.Dxyz2(:,i) = joint.Dxyz1(:,i) - joint.Dxyz1(:,i - 1);
            joint.rxyz2(:,i) = joint.rxyz1(:,i) - joint.rxyz1(:,i - 1);
        else
            joint.Dxyz2(:,i) =  (joint.Dxyz1(:,i+1) - joint.Dxyz1(:,i - 1)) * 0.5;
            joint.rxyz2(:,i) =  (joint.rxyz1(:,i+1) - joint.rxyz1(:,i - 1)) * 0.5;
        end
    end
end

end