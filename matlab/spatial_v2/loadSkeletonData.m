function skeleton = loadSkeletonData(skeleton)

%computer first dev / speed
for j = 1:size(skeleton,2)
    if not(isspace(skeleton(1,j).name))
        skeleton(1,j).rxyz1 =zeros(size(skeleton(1,j).Dxyz));
        skeleton(1,j).Dxyz1 =zeros(size(skeleton(1,j).Dxyz));
        for i = 1:size(skeleton(1,j).Dxyz, 2)
             if (i == 1)
                 skeleton(1,j).Dxyz1(:,i) = skeleton(1,j).Dxyz(:,i+1) - skeleton(1,j).Dxyz(:,i);
                 skeleton(1,j).rxyz1(:,i) = skeleton(1,j).rxyz(:,i+1) - skeleton(1,j).rxyz(:,i);
             else
                 if i == size(skeleton(1,j).Dxyz, 2)
                    skeleton(1,j).Dxyz1(:,i) = skeleton(1,j).Dxyz(:,i) - skeleton(1,j).Dxyz(:,i - 1);
                    skeleton(1,j).rxyz1(:,i) = skeleton(1,j).rxyz(:,i) - skeleton(1,j).rxyz(:,i - 1);
                 else
                    skeleton(1,j).Dxyz1(:,i) =  (skeleton(1,j).Dxyz(:,i+1) - skeleton(1,j).Dxyz(:,i - 1)) * 0.5;
                    skeleton(1,j).rxyz1(:,i) =  (skeleton(1,j).rxyz(:,i+1) - skeleton(1,j).rxyz(:,i - 1)) * 0.5;
                 end
             end
        end
    end 
end

%computer second dev / accelaration
for j = 1:size(skeleton,2)
    if not(isspace(skeleton(1,j).name))
        skeleton(1,j).rxyz2 =zeros(size(skeleton(1,j).Dxyz1));
        skeleton(1,j).Dxyz2 =zeros(size(skeleton(1,j).Dxyz1));
        for i = 1:size(skeleton(1,j).Dxyz, 2)
             if (i == 1)
                    skeleton(1,j).Dxyz2(:,i) = skeleton(1,j).Dxyz1(:,i+1) - skeleton(1,j).Dxyz1(:,i);
                    skeleton(1,j).rxyz2(:,i) = skeleton(1,j).rxyz1(:,i+1) - skeleton(1,j).rxyz1(:,i);
             else
                 if i == size(skeleton(1,j).Dxyz, 2)
                    skeleton(1,j).Dxyz2(:,i) = skeleton(1,j).Dxyz1(:,i) - skeleton(1,j).Dxyz1(:,i - 1);
                    skeleton(1,j).rxyz2(:,i) = skeleton(1,j).rxyz1(:,i) - skeleton(1,j).rxyz1(:,i - 1);
                 else
                    skeleton(1,j).Dxyz2(:,i) =  (skeleton(1,j).Dxyz1(:,i+1) - skeleton(1,j).Dxyz1(:,i - 1)) * 0.5;
                    skeleton(1,j).rxyz2(:,i) =  (skeleton(1,j).rxyz1(:,i+1) - skeleton(1,j).rxyz1(:,i - 1)) * 0.5;
                 end
             end
        end
    end 
end


end