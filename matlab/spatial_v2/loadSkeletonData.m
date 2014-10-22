function skeleton = loadSkeletonData(skeleton)
disp('loadSkeletonExtraData');
time = 1.0 / skeleton.frequency;
%computer first dev / speed
for j = 1:size(skeleton,2)
    if not(isspace(skeleton(1,j).name))
        skeleton(1,j).Dxyz1 =zeros(size(skeleton(1,j).Dxyz));
        skeleton(1,j).rxyz1 =zeros(size(skeleton(1,j).Dxyz));
        for i = 1:size(skeleton(1,j).Dxyz, 2)
             if (i == 1)
                 skeleton(1,j).Dxyz1(:,i) = substraction(skeleton(1,j).Dxyz(:,i+1) , skeleton(1,j).Dxyz(:,i)) / time;
                 skeleton(1,j).rxyz1(:,i) = substraction(skeleton(1,j).rxyz(:,i+1) , skeleton(1,j).rxyz(:,i)) / time;
             else
                 if i == size(skeleton(1,j).Dxyz, 2)
                    skeleton(1,j).Dxyz1(:,i) = substraction(skeleton(1,j).Dxyz(:,i) , skeleton(1,j).Dxyz(:,i - 1)) / time;
                    skeleton(1,j).rxyz1(:,i) = substraction(skeleton(1,j).rxyz(:,i) , skeleton(1,j).rxyz(:,i - 1)) / time;
                 else
                    skeleton(1,j).Dxyz1(:,i) =  substraction(skeleton(1,j).Dxyz(:,i+1) , skeleton(1,j).Dxyz(:,i - 1)) * 0.5 / time;
                    skeleton(1,j).rxyz1(:,i) =  substraction(skeleton(1,j).rxyz(:,i+1) , skeleton(1,j).rxyz(:,i - 1)) * 0.5 / time;
                 end
             end
        end
    end 
end

%computer second dev / accelaration
for j = 1:size(skeleton,2)
    if not(isspace(skeleton(1,j).name))
        skeleton(1,j).Dxyz2 =zeros(size(skeleton(1,j).Dxyz1));
        skeleton(1,j).rxyz2 =zeros(size(skeleton(1,j).Dxyz1));
        for i = 1:size(skeleton(1,j).Dxyz, 2)
             if (i == 1)
                    skeleton(1,j).Dxyz2(:,i) = substraction(skeleton(1,j).Dxyz1(:,i+1) , skeleton(1,j).Dxyz1(:,i)) / time;
                    skeleton(1,j).rxyz2(:,i) = substraction(skeleton(1,j).rxyz1(:,i+1) , skeleton(1,j).rxyz1(:,i)) / time;
             else
                 if i == size(skeleton(1,j).Dxyz, 2)
                    skeleton(1,j).Dxyz2(:,i) = substraction(skeleton(1,j).Dxyz1(:,i) , skeleton(1,j).Dxyz1(:,i - 1)) / time;
                    skeleton(1,j).rxyz2(:,i) = substraction(skeleton(1,j).rxyz1(:,i) , skeleton(1,j).rxyz1(:,i - 1)) / time;
                 else
                    skeleton(1,j).Dxyz2(:,i) =  substraction(skeleton(1,j).Dxyz1(:,i+1) , skeleton(1,j).Dxyz1(:,i - 1)) * 0.5 / time;
                    skeleton(1,j).rxyz2(:,i) =  substraction(skeleton(1,j).rxyz1(:,i+1) , skeleton(1,j).rxyz1(:,i - 1)) * 0.5 / time;
                 end
             end
        end
    end 
end

disp('end loadSkeletonExtraData');
end
%for not depassing 360 degree
function v = substraction(d1, d2)
    
        v1 = d1(1,1) - d2(1,1);
        v2 = d1(2,1) - d2(2,1);
        v3 = d1(3,1) - d2(3,1);
        if v1 > 3.1415
            v1 = v1 - 3.1415 * 2;
        end
        if v1 < -3.1415
            v1 = v1 + 3.1415 * 2;
        end
        if v2 > 3.1415
            v2 = v2 - 3.1415 * 2;
        end
        if v2 < -3.1415
            v2 = v2 + 3.1415 * 2;
        end
        if v3 > 3.1415
            v3 = v3 - 3.1415 * 2;
        end
        if v3 < -3.1415
            v3 = v3 + 3.1415 * 2;
        end
v = [v1;v2;v3;];

end