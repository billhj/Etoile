disp('rebuild');
framesize = size(model.q{1},2);
dof = size(model.q, 2);
%model.tau = zeros(1,dof);
time = 1.0 / 30;
model.NB = 12;
for n = 1 : dof
    model.tau{1, n} = zeros(1,framesize);
end

qout = zeros{dof, framesize};
qdout = zeros{dof, framesize};
qddout = zeros{dof, framesize};
q = zeros(1,dof);
qd = zeros(1,dof);
qdd = zeros(1,dof);

for j = 1 : framesize
    
    if(j == 1)
        for n = 1 : dof
            q(n) = model.q{1,n}(1,j);
            qd(n) = model.qd{1,n}(1,j);
            tau(n) = ;
        end
    end
    qdd = FDab(model, q, qd, tau, f_ext );
    qout(:,j) = q;
    qdout(:,j) = qd;
    qddout(:,j) = qdd;
    
    for n = 1 : 3
        q(n) = model.q{1,n}(1,j);
        qd(n) = model.qd{1,n}(1,j);
        tau(n) = ;
    end
    for n = 4 : dof
        qd(n) = qd(n) + qdd(n) * time; 
        q(n) = q(n) + qd(n) * time;
        tau(n) = pd(q(n), qd(n), tau(n - 3));
    end
    
end
disp('end rebuild');

