disp('rebuild');
framesize = size(model.q, 2);
dof = size(model.q, 1);
%model.tau = zeros(1,dof);
time = 1.0 / 30;

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

for j = 2 : framesize
    if mod(j,100)==0
     disp(j)
    end
   
    for n = 1 : 3
        qout(n,j) = model.q(n,j);
        qdout(n,j) = model.qd(n,j);
        tauout(n,j) = model.tau(n,j);
    end
    for n = 4 : dof
        qdout(n,j) = qdout(n,j-1) + qddout(n, j-1) * time; 
        qout(n,j) = qout(n,j-1) + qdout(n, j-1) * time;
        x = [1, qout(n,j), qdout(n,j), tauout(n - 3, j)];
        para = [mdl_recompensePD{n}.Coefficients{1,1}, mdl_recompensePD{n}.Coefficients{2,1}, mdl_recompensePD{n}.Coefficients{3,1}, mdl_recompensePD{n}.Coefficients{4,1}];
        tauout(n,j) = para * x';
    end
    
    qddout(:,j) = FDab(model, qout(:,j), qdout(:,j), tauout(:,j));
    
end
disp('end rebuild');
