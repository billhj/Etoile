disp('rebuild');
framesize = size(model.q, 2);
dof = size(model.q, 1);
%model.tau = zeros(1,dof);
time = 1.0 / 120;

qout = zeros(dof, framesize);
qdout = zeros(dof, framesize);
qddout = zeros(dof, framesize);
tauout = zeros(dof, framesize);
% q = zeros(1,dof);
% qd = zeros(1,dof);
% qdd = zeros(1,dof);
% tau = zeros(1,dof);

j = 1;
for n = 1 : dof
    qout(n,j) = model.q(n,j);
    qdout(n,j) = model.qd(n,j);
    tauout(n,j) = model.tau(n,j);
end
qddout(:,j) = FDab(model, qout(:,j), qdout(:,j), tauout(:,j));

for j = 2 : 100
    if mod(j,100)==0
     disp(j)
    end
   
    for n = 1 : 3
        qout(n,j) = model.q(n,j);
        qdout(n,j) = model.qd(n,j);
        qddout(n,j) = model.qdd(n,j);
        tauout(n,j) = model.tau(n,j);
    end
    for n = 4 : dof
        qdout(n,j) = qdout(n,j-1) + qddout(n, j-1) * time; 
        qout(n,j) = qout(n,j-1) + qdout(n, j-1) * time;
        if qout(n,j) > 3.14159265
            qout(n,j) = qout(n,j) - 3.14159265 * 2;
        end
        if qout(n,j) < -3.1415
            qout(n,j) = qout(n,j) + 3.14159265 * 2;
        end
         %  x = [1, qout(n,j), qdout(n,j), tauout(n - 3, j), qout(n - 3,j), qdout(n - 3,j)];
        x = [1, qout(n,j), qdout(n,j)];
       %    para = [mdl_accurecompensePD{n}.Coefficients{1,1}, mdl_accurecompensePD{n}.Coefficients{2,1}, mdl_accurecompensePD{n}.Coefficients{3,1}, mdl_accurecompensePD{n}.Coefficients{4,1}, mdl_accurecompensePD{n}.Coefficients{4,1}, mdl_accurecompensePD{n}.Coefficients{6,1}];
    %   para = [mdl_originalPD{n}.Coefficients{1,1}, mdl_originalPD{n}.Coefficients{2,1}, mdl_originalPD{n}.Coefficients{3,1}];
    para = [mdl_originalPDrotation{n}.Coefficients{1,1}, mdl_originalPDrotation{n}.Coefficients{2,1}, -sqrt(mdl_originalPDrotation{n}.Coefficients{2,1}) * 2];
    %  para = [0, 40, -28];
        tau1 = para * x';
%         if tau1 > 1
%             tau1 = 1;
%         end
%         if tau1 < -1
%             tau1 = -1;
%         end
        tauout(n,j) = tau1;
 %        tauout(n,j) = model.tau(n,j);
    end
    
    qddoutlocal = FDab(model, qout(:,j), qdout(:,j), tauout(:,j));
    qddout(4:12,j) = qddoutlocal(4:12);
    
end
disp('end rebuild');
